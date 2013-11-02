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

#define NICKNAME_BY_DEFAULT "nickname"    // Кличка питомца по-умолчанию
#define TYPE_BY_DEFAULT     "type"        // Тип питомца по-умолчанию
#define COLOR_BY_DEFAULT    "color"       // Цвет питомца по-умолчанию
#define BUF_SIZE            10            // Размер буффера
#define EXIT_ERROR          -1            // Код ошибки

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct pet
{
    char* nickname;
    char* type;
    char* color;
};

void inputByDefault (struct pet* );           // Ввод значений по-умолчанию
void outputPet ( const struct pet* );         // Вывод всех "питомцев"
char* getLine( char* );                       // Правильный динам. ввод строки
void inputPet(struct pet*, const size_t );    // Ввод параметров питомца
void swap (struct pet* , struct pet* );       // Swap
void quick_sort ( struct pet* , size_t );     // Быстрая Сортировка
void clearPet (struct pet* , size_t );        // Очистка памяти

int main(void)
{
    size_t i = 0;

    if (scanf("%zd\n", &i) != 1 )
    {
        printf("Ошибка ввода!\n");
        exit (EXIT_ERROR);
    }

    struct pet* in = (struct pet* ) malloc( i * sizeof( struct pet ) );
    if ( !in )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }

    size_t j;
    for ( j = 0; j < i; j++ )
    {
        inputByDefault( &in[j] );
        inputPet( in, j );
    }

    quick_sort(in, i);

    printf("\n");
    for ( j = 0; j < i; j++ )
    {
        outputPet( &in[j] );
    }

    clearPet( in, i );
    free( in );
    return 0;
}

void inputByDefault ( struct pet* in )
{
    if ( in == NULL )
    {
        printf("Ошибка! \n");
        exit (EXIT_ERROR);
    }
    in->nickname = (char *) malloc (strlen(NICKNAME_BY_DEFAULT)+1);
    if ( !in->nickname )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }
    strcpy(in->nickname, NICKNAME_BY_DEFAULT );

    in->type = (char *) malloc (strlen(TYPE_BY_DEFAULT)+1);
    if ( !in->type )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }
    strcpy(in->type, TYPE_BY_DEFAULT);

    in->color = (char *) malloc (strlen(COLOR_BY_DEFAULT)+1);
    if ( !in->color )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }
    strcpy(in->color, COLOR_BY_DEFAULT);
}

void outputPet ( const struct pet* in )
{
    if ( in == NULL )
    {
        printf("Ошибка! \n");
        exit (EXIT_ERROR);
    }
    printf("Nickname: %s\n", in->nickname);
    printf("Type: %s\n", in->type);
    printf("Color: %s\n\n", in->color);
}

char* getLine( char* in )
{
    if ( in == NULL )
    {
        printf("Ошибка! \n");
        exit (EXIT_ERROR);
    }
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
    in[i] = buf;
    ++i;

    while ( ( buf = getchar() ) != '\n' )
    {
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

        in[i] = buf;
        ++i;
    }

    in[i]='\0';

    return in;
}

void inputPet( struct pet* in, const size_t num )
{
    if ( in == NULL )
    {
        printf("Ошибка! \n");
        exit (EXIT_ERROR);
    }
    in[num].nickname = getLine(in[num].nickname);
    in[num].type = getLine(in[num].type);
    in[num].color = getLine(in[num].color);
}

void swap (struct pet* one, struct pet* two )
{
    if ( one == NULL || two == NULL )
    {
        printf("Ошибка! \n");
        exit (EXIT_ERROR);
    }

    char* buf = ( char* ) malloc ( 100 * sizeof(char) );
    if ( !buf )
    {
        printf("Ошибка выделения памяти! \n");
        exit (EXIT_ERROR);
    }

    strcpy(buf,one->nickname);
    strcpy(one->nickname,two->nickname);
    strcpy(two->nickname,buf);

    strcpy(buf,one->color);
    strcpy(one->color,two->color);
    strcpy(two->color,buf);

    strcpy(buf,one->type);
    strcpy(one->type,two->type);
    strcpy(two->type,buf);
}

void quick_sort ( struct pet* a, size_t n )
{
    if ( a == NULL )
    {
        printf("Ошибка! \n");
        exit (EXIT_ERROR);
    }

    size_t i = 0;
    size_t j = n - 1;
    size_t side = 0;
    while ( i != j )
    {
        if ( strcmp( a[i].type , a[j].type) > 0 )
        {
            swap ( &a[i], &a[j] );
            side = !side;
        }
        if ( side )
        {
            ++i;
        }
        else
        {
            --j;
        }
    }
    if ( i > 1 )
        quick_sort ( a, i );
    if ( n > i + 1 )
        quick_sort ( a + (i + 1), n - (i + 1) );
}

void clearPet (struct pet* in, size_t num)
{
    if ( in == NULL )
    {
        printf("Ошибка! \n");
        exit (EXIT_ERROR);
    }
    size_t i;
    for ( i = 0; i < num; i++ )
    {
        free ( in[i].nickname );
        free ( in[i].type );
        free ( in[i].color );
    }
}
