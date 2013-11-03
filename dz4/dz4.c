/*
 *        Name: dz4.c
 *  Created on: Nov 1, 2013
 *      Author: roland
 *	 Copiright: 2013 All rights reserved
 */

/*
 * Каждая программа должна содержать:
 * - функцию инициализации элементов структуры (struct) значениями по умолчанию;
 * - функцию вывода элементов структуры в поток stdout в удобном виде;
 * - функцию освобождения ресурсов, занятых динамически размещенным в памяти
 *      объектом типа-структуры.
 *
 * Создать структуру для хранения информации о
 * домашнем питомце: его виде, окрасе, кличке.
 * Составить с ее использованием программу вывода
 * информации обо всех питомцах с группировкой по
 * видам (кошки, собаки, прочие).
 */

#define _GNU_SOURCE            // Макрос, чтобы не ругался -std=c99 на strdup
#define NICKNAME_BY_DEFAULT    "nickname"       // Кличка питомца по-умолчанию
#define TYPE_BY_DEFAULT        "type"           // Тип питомца по-умолчанию
#define COLOR_BY_DEFAULT       "color"          // Цвет питомца по-умолчанию
#define BUF_SIZE               10               // Размер буффера
#define EXIT_ERROR             -1               // Код ошибки

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct pet
{
    char* nickname;
    char* type;
    char* color;
};

void inputByDefault (struct pet* );            // Ввод значений по-умолчанию
void outputPet ( const struct pet* );          // Вывод всех "питомцев"
char* getLine( char* );                        // Правильный динам. ввод строки
void inputPet(struct pet*, const size_t );     // Ввод параметров питомца
int compareType ( const void*, const void* );  // Функция сравнения для qsort
void clearPet (struct pet* , const size_t );   // Очистка памяти

int main(void)
{
    size_t num;

    printf ("Введите количество питомцев : ");

    if ( scanf("%zu%*c", &num) != 1 )   // %*c, а не fflush(stdin) не помогает (
    {                                   // проблеммы из-за '\n' ...
        printf("Ошибка ввода!\n");
        return (EXIT_ERROR);
    }

    printf ("Введите имя, вид и цвет питомца.\n");
    printf ("Если хотите оставить значение по умолчанию - нажмите enter! \n");

    struct pet* in = (struct pet* ) malloc( num * sizeof( struct pet ) );
    if ( !in )
    {
        printf("Ошибка выделения памяти! \n");
        return (EXIT_ERROR);
    }

    size_t j;
    for ( j = 0; j < num; j++ )
    {
        inputByDefault( &in[j] );
        inputPet( in, j );
    }

    qsort(in, num, sizeof(struct pet), compareType);

    printf("\n");

    for ( j = 0; j < num; j++ )
    {
        outputPet( &in[j] );
    }

    clearPet( in, num );
    free( in );

    return 0;
}

void inputByDefault ( struct pet* in )
{
    assert ( in != NULL );

    in->nickname = strdup( NICKNAME_BY_DEFAULT );
    if ( !in->nickname )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }

    in->type = strdup( TYPE_BY_DEFAULT );
    if ( !in->type )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }

    in->color = strdup( COLOR_BY_DEFAULT );
    if ( !in->color )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }
}

void outputPet ( const struct pet* in )
{
    assert ( in != NULL );
    assert ( in->nickname != NULL );
    assert ( in->type != NULL );
    assert ( in->color != NULL );

    printf("Nickname: %s\n", in->nickname);
    printf("Type: %s\n", in->type);
    printf("Color: %s\n\n", in->color);
}

char* getLine( char* in )
{
    assert ( in != NULL );

    char buf ;
    if ( ( buf = getchar() ) == '\n' )
    {
        return in;
    }
    free (in);

    size_t newBufSize = BUF_SIZE;

    in = ( char* ) malloc ( newBufSize * sizeof(char) );
    if ( !in )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }

    size_t i = 0;
    do
    {
        in[i] = buf;
        ++i;

        if ( i > newBufSize - 1 )
        {
            newBufSize *= 2;
            in = ( char* ) realloc ( in, newBufSize * sizeof(char) );
            if ( !in )
            {
                printf("Ошибка выделения памяти! \n");
                exit (EXIT_ERROR);
            }
        }
    }
    while ( ( buf = getchar() ) != '\n' );

    in[i]='\0';

    return in;
}

void inputPet( struct pet* in, const size_t num )
{
    assert ( in != NULL );

    in[num].nickname = getLine(in[num].nickname);
    in[num].type = getLine(in[num].type);
    in[num].color = getLine(in[num].color);
}

int compareType ( const void* one, const void* two)
{
    const struct pet* p_one = (const struct pet*)one;
    const struct pet* p_two = (const struct pet*)two;

    return strcmp(p_one->type, p_two->type);
}

void clearPet (struct pet* in, const size_t num)
{
    assert ( in != NULL );

    if ( num == 0 )
    {
        printf ( "Не правильное количество элементов для удаления!\n" );
        return;
    }

    size_t i;
    for ( i = 0; i < num; i++ )
    {
        free ( in[i].nickname );
        free ( in[i].type );
        free ( in[i].color );
    }
}
