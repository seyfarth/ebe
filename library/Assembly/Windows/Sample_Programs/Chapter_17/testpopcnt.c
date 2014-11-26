#include <stdio.h>
#include <stdlib.h>
//#include <sys/times.h>
#include <Windows.h>
#include <WinBase.h>

long popcnt_array ( long long *a, int size );

LARGE_INTEGER frequency, clicks;

double time()
{
    QueryPerformanceCounter(&clicks);

    return (double)clicks.QuadPart/frequency.QuadPart;
}

int main ( int argc, char **argv )
{
    long n;
    int size, count;
    int i;
    double start, end;
    long long *a;

    QueryPerformanceFrequency(&frequency);

    size = argc > 1 ? atoi(argv[1]) : 100000;
    count = argc > 2 ? atoi(argv[2]) : 1000;

    a = (long long *) malloc ( size * sizeof(long long) );
    //srandom(time(NULL));
    srand(1);
    for ( i = 0; i < size; i++ ) {
        a[i] = rand();
        a[i] = a[i] << 16 | rand();
        a[i] = a[i] << 16 | rand();
        a[i] = a[i] << 16 | rand();
    }
    start = time();
    for ( i = 0; i < count; i++ ) {
        n = popcnt_array ( a, size );
    }
    end = time();
    printf("count %ld\n",n);
    printf("elapsed %10.4f\n",end-start);
    return 0;
}
