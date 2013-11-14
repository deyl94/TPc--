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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct pet
{
    char* nickname;
    char* type;
    char* color;
};

int inputByDefault (struct pet* );             // Ввод значений по-умолчанию
int outputPet( const struct pet* );            // Вывод всех "питомцев"
int getLine(char** );                          // Динамический ввод строки
int inputPet(struct pet*, const size_t );      // Ввод параметров питомца
int compareType ( const void*, const void* );  // Функция сравнения для qsort
int clearPet (struct pet* , const size_t );    // Очистка памяти

int main(void)
{
    size_t num;

    printf ( "Введите количество питомцев : " );

    if ( scanf("%zu%*c", &num) != 1 )   // %*c, fflush(stdin) не помогает (
    {                                   // проблеммы из-за '\n' ...
        printf( "Ошибка ввода!\n" );
        return EXIT_FAILURE;
    }

    printf ("Введите имя, вид и цвет питомца.\n");
    printf ("Если хотите оставить значение по умолчанию - нажмите enter! \n");

    // Выделяем память под массив структур
    struct pet* in = (struct pet* ) malloc( num * sizeof( struct pet ) );
    if ( !in )
    {
        printf("Ошибка выделения памяти! \n");
        return EXIT_FAILURE;
    }

    // Заполняем структуру значениями. Сначала умолчательными, если будут
    // введены, то заменяеи их введёнными.
    size_t j;
    for ( j = 0; j < num; j++ )
    {
        if ( inputByDefault( &in[j] ) == EXIT_FAILURE )
        {
            printf("Ошибка выделения памяти! \n");
            // Все структуры до j чистятся
            clearPet( in, j );
            free ( in );
            return EXIT_FAILURE;
        }
        if ( inputPet( in, j ) == EXIT_FAILURE)
        {
            printf("Ошибка выделения памяти! \n");
            // Все структуры до j чистятся
            clearPet( in, j );
            free ( in );
            return EXIT_FAILURE;
        }
    }

    // Сорируем по типу питомца
    qsort(in, num, sizeof(struct pet), compareType);

    printf("\n");

    // Вывод значений полей структуры
    for ( j = 0; j < num; j++ )
    {
        if ( outputPet( &in[j] ) == EXIT_FAILURE )
        {
            clearPet( in, j );
            free ( in );
            return EXIT_FAILURE;
        }
    }

    // Очистка полей структуры
    clearPet( in, num );
    // Очистка массива структур
    free( in );

    return EXIT_SUCCESS;
}

int inputByDefault ( struct pet* in )
{
    // Функция заполняет поля структуры умолчательными значениями заданными
    // в константах NICKNAME_BY_DEFAULT, TYPE_BY_DEFAULT, COLOR_BY_DEFAULT
    // Если на каком-то этапе заполнения ошибка, все значения == NULL.

    if ( in == NULL )
        return EXIT_FAILURE;


    in->nickname = strdup( NICKNAME_BY_DEFAULT );
    if ( in->nickname == NULL )
    {
        in->color = NULL;
        in->type = NULL;
        return EXIT_FAILURE;
    }

    in->type = strdup( TYPE_BY_DEFAULT );
    if ( in->type == NULL )
    {
        free ( in->nickname );
        in->nickname = NULL;
        in->color = NULL;
        return EXIT_FAILURE;
    }

    in->color = strdup( COLOR_BY_DEFAULT );
    if ( in->color == NULL )
    {
        free( in->nickname );
        free( in->type );
        in->nickname = NULL;
        in->type = NULL;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int outputPet ( const struct pet* in )
{
    // Вывод полей структуры pet

    if ( in == NULL )
        return EXIT_FAILURE;

    if ( in->nickname == NULL )
        return EXIT_FAILURE;
    if ( in->type == NULL )
        return EXIT_FAILURE;
    if ( in->color == NULL )
        return EXIT_FAILURE;

    printf("Nickname: %s\n", in->nickname);
    printf("Type: %s\n", in->type);
    printf("Color: %s\n\n", in->color);

    return EXIT_SUCCESS;
}

int getLine( char** in )
{
    // Функция заполняет поля значениями вводимыми с клавиатуры,
    // если не вводилось ничего, то ничего не дела.
    // При ошибке возвращает EXIT_FAILURE и присваевает in = NULL

    if ( *in == NULL )
        return EXIT_FAILURE;

    // Если ничего не введено.
    char buf;
    if ( ( buf = getchar() ) == '\n' )
    {
        return EXIT_SUCCESS;
    }

    // Чистим умолчательное значение и присваиваем NULL,
    // на случай, если будут проблемы выделения памяти.
    free ( *in );
    *in = NULL;

    size_t newBufSize = BUF_SIZE;

    char* out;
    out = ( char* ) malloc ( newBufSize * sizeof(char) );
    if ( !out )
    {
        printf("Ошибка выделения памяти! \n");
        return EXIT_FAILURE;
    }

    // Выделяем кусок памяти, вводим значения с экрана, если буффер заполнен
    // увеличиваем его на BUFFER_SIZE;
    size_t i = 0;
    do
    {
        out[i] = buf;
        ++i;

        // Если буффер закончился
        if ( i > newBufSize - 1 )
        {
            newBufSize *= 2;
            out = ( char* ) realloc ( out, newBufSize * sizeof(char) );
            if ( !out )
            {
                printf("Ошибка выделения памяти! \n");
                free (out);
                return EXIT_FAILURE;
            }
        }
    }
    while ( ( buf = getchar() ) != '\n' );

    out[i]='\0';
    *in = out;

    return EXIT_SUCCESS;
}

int inputPet( struct pet* in, const size_t num )
{
    // Ввод пользователем полей структуры, проинициализированных уже
    // по-умолчанию.
    // Если какая-либо ошибка заполнения - все поля = NULL

    if ( in == NULL )
        return EXIT_FAILURE;

    if ( getLine( &in[num].nickname ) == EXIT_FAILURE)
    {
        free(in[num].color);
        free(in[num].type);
        in[num].color = NULL;
        in[num].type = NULL;
        return EXIT_FAILURE;
    }
    if ( getLine( &in[num].type ) == EXIT_FAILURE )
    {
        free(in[num].color);
        free(in[num].nickname);
        in[num].color = NULL;
        in[num].nickname = NULL;
        return EXIT_FAILURE;
    }
    if ( getLine( &in[num].color ) == EXIT_FAILURE )
    {
        free(in[num].nickname);
        free(in[num].type);
        in[num].nickname = NULL;
        in[num].type = NULL;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int compareType ( const void* one, const void* two)
{
    // Функция сравнения для поля структуры pet->type

    const struct pet* p_one = ( const struct pet* )one;
    const struct pet* p_two = ( const struct pet* )two;

    return strcmp( p_one->type, p_two->type );
}

int clearPet ( struct pet* in, const size_t num )
{
    // Очистка памяти от строк структуры

    if ( in == NULL )
        return EXIT_FAILURE;

    if ( num == 0 )
        return EXIT_FAILURE;

    size_t i;
    for ( i = 0; i < num ; i++ )
    {
        free ( in[i].nickname );
        free ( in[i].type );
        free ( in[i].color );
    }

    return EXIT_SUCCESS;
}
