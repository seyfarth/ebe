#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

void roll_dice ( int count[7][7], int n );
void print_count ( int count[7][7] );
void print_seven_pct ( int count[7][7], int n );

void roll_dice ( int c[7][7], int n )
{
    int i, a, b;

    for ( i = 0; i < n; i++ ) {
        a = (rand() >> 5) % 6 + 1;
        b = (rand() >> 5) % 6 + 1;
        assert ( a > 0 && a <= 6 );
        assert ( b > 0 && b <= 6 );
        c[a][b]++;
    }
}

void print_count ( int count[7][7] )
{
    int r, c;

    for ( r = 1; r <= 6; r++ ) {
        for ( c = 1; c <= 6; c++ ) {
            printf ( "%7d", count[r][c] );
        }
        printf ( "\n" );
    }
}

void print_seven_pct ( int count[7][7], int n )
{
    int r, c, sum = 0;

    for ( r = 1; r <= 6; r++ ) {
        c = 7 - r;
        assert ( r > 0 && r <= 6 );
        assert ( c > 0 && c <= 6 );
        sum += count[r][c];
    }

    printf ( "Pct = %10.2f\n", (double)sum/n*100.0 );
}

int main ( int argc, char *argv[] )
{
    int count[7][7] = { { 0 } };
    int n;

    srand ( time(NULL) );

    n = ( argc > 1 ) ? atoi(argv[1]) : 100;

    roll_dice ( count, n );
    print_count ( count );

    print_seven_pct ( count, n );

    return 0;
}
