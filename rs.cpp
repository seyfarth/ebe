/*
 *  radix_sort.c    A test program for radix_sort
 *  author:  Benjamin R. Seyfarth
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/times.h>
/*
 *  Function prototypes
 */
long UserTime( void );
void Randomize ( int *a, int n );
void Sort (int  a[], int  size );
inline void CountSort ( int t[], int a[], int c[], int n, int bits );
int Sorted ( int a[], int n );
int CheckSum ( int a[], int n );

long OldTime, NewTime, Time;

/*
 *  Main routine for the Mergesort test program.
 */
int main ( int argc, char *argv[] )
{
    int *a;
    int n;
    int CorrectSum;

/*
 *  Process the command line parameters.
 */
    if ( argc < 2 ) {
        //fprintf(stderr,"usage: %s array_size\n", argv[0]);
        //exit(1);
        n = 10;
    } else {
        n = atoi(argv[1]);
    }

/*
 *  Prepare the array of random integers to sort.
 */
    a = (int *) malloc ( n * sizeof(int) );
    Randomize ( a, n );
    CorrectSum = CheckSum ( a, n );

/*
 *  Time the sorting process.
 */
    OldTime = UserTime();
    Sort ( a, n );
    NewTime = UserTime();
    Time = NewTime - OldTime;
    printf("Time used %ld.%02ld seconds\n",Time/100,Time%100);

/*
 *  Make sure it really sorted the values.
 */
    if ( Sorted(a,n) ) printf("Sorted array\n");
    if ( CheckSum(a,n) != CorrectSum ) printf("Check sum failed\n");
    return 0;
}


/*
 *  This is a function to get the user cpu time for the process.
 */
long UserTime( void )
{
    struct tms t;

    times(&t);
    return t.tms_utime;
}

/*
 *  This is a function to fill an array with random integers.
 */
void Randomize ( int *a, int n )
{
    int i;

    for ( i = 0; i < n; i++ ) a[i] = rand();
}

/*
 *
 */
void Sort (int  a[], int  size )
{
    int *t;
    int c[256];

    t = (int *)malloc ( size * sizeof(int) );
    if ( t == NULL ) {
        fprintf(stderr,"Could not calloc t with %d ints\n",
            size );
        exit(1);
    }

    CountSort ( a, t, c, size, 0 );
    CountSort ( t, a, c, size, 8 );
    CountSort ( a, t, c, size, 16 );
    CountSort ( t, a, c, size, 24 );
    free ( t );
}
            
inline void CountSort ( int a[], int b[], int c[], int n, int bits )
{
    int i;

    for ( i = 0; i < 256; i++ ) c[i] = 0;

    for ( i = 0; i < n; i++ ) c[(a[i]>>bits)&0xff]++;
    
    for ( i = 1; i < 256; i++ ) c[i] = c[i]+c[i-1];

    for ( i = n-1; i >= 0; i-- ) b[--c[(a[i]>>bits)&0xff]] = a[i];
}

/*
 *  This is a function to verify that the sorting works.
 */
int Sorted ( int a[], int n )
{
    int i;

    for ( i = 0; i < n-1; i++ ) {
        if ( a[i] > a[i+1] ) {
            printf("Error at location %d\n",i);
            return 0;
        }
    }
    return 1;
}

/*
 *  Function to compute exclusive or over an array.
 */
int CheckSum ( int a[], int n )
{
    int i, s;

    s = a[0];
    for ( i = 1; i < n; i++ ) s ^= a[i];
    return s;
}


