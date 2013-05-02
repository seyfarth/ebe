#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

long popcnt_array ( long *a, int size );

double time()
{
    struct tms t;
    times(&t);
    return (t.tms_utime+t.tms_stime)/100.0;
}

int main ( int argc, char **argv )
{
    long n;
    int size, count;
    int i;
    double start, end;
    long *a;


    size = argc > 1 ? atoi(argv[1]) : 100000;
    count = argc > 2 ? atoi(argv[2]) : 1000;

    a = (long *) malloc ( size * sizeof(long) );
    //srandom(time(NULL));
    srandom(1);
    for ( i = 0; i < size; i++ ) {
        a[i] = random()<<32 | random();
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
