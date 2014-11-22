#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

void do_inst();
double corr ( double *a, double *b, long n );

double time()
{
    struct tms t;
    times(&t);
    return (t.tms_utime+t.tms_stime)/100.0;
}

int main ( int argc, char **argv )
{
    int size, count;
    int i;
    double start, end;
    double instrs;
    double *a, *b, c;

    size = argc > 1 ? atoi(argv[1]) : 100000;
    count = argc > 2 ? atoi(argv[2]) : 10000;

    a = malloc(size*sizeof(double)+16);
    b = malloc(size*sizeof(double)+16);
    a = (double *)( (long) a & 0xffffffffffffffc0);
    b = (double *)( (long) b & 0xffffffffffffffc0);
    for ( i = 0; i < size; i++ ) {
        a[i] = i+random()%1000;
        b[i] = i+random()%1000;
    }
    start = time();
    for ( i = 0; i < count; i++ ) {
        c = corr(a,b,size);
        if ( i == 0 ) printf("c = %10.6f\n",c);
    }
    end = time();
    printf("used %10.4f\n",end-start);
    instrs = count * size;
    return 0;
}
