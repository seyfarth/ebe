#include <stdio.h>
#include <stdlib.h>
//#include <sys/times.h>

void do_inst();
double corr ( double *a, double *b, long n );

double time()
{
   // struct tms t;
   // times(&t);
   // return (t.tms_utime+t.tms_stime)/100.0;
   return 0;
}

int main ( int argc, char **argv )
{
    int size, count;
    int i;
    double start, end;
   // double instrs, instrs_per_sec;
    double *a, *b, c;

    size = argc > 1 ? atoi(argv[1]) : 100000;
    count = argc > 2 ? atoi(argv[2]) : 10000;
    //size = argc > 1 ? atoi(argv[1]) : 8;
    count = argc > 2 ? atoi(argv[2]) : 1;

    a = malloc(size*sizeof(double));
    b = malloc(size*sizeof(double));
    for ( i = 0; i < size; i++ ) {
        a[i] = i+rand()%1000;
        b[i] = i+rand()%1000;
    }
    start = time();
    for ( i = 0; i < count; i++ ) {
        c = corr(a,b,size);
        if ( i == 0 ) printf("c = %10.6f\n",c);
    }
    end = time();
    printf("used %10.4f\n",end-start);
    //instrs = count * size;
    return 0;
}
