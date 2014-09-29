#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

void do_inst();

void sobel ( unsigned char *data, float *results, long rows, long cols );

double time()
{
    struct tms t;
    times(&t);
    return (t.tms_utime+t.tms_stime)/100.0;
}

int main ( int argc, char **argv )
{
    long rows, cols;
    long count, images;
    int r, c, i;
    double start, end;
    unsigned char **data;
    float *result;

    rows = argc > 1 ? atol(argv[1]) : 1024;
    cols = argc > 2 ? atol(argv[2]) : 1024;
    count = argc > 3 ? atoi(argv[3]) : 1000;
    images = argc > 4 ? atoi(argv[4]) : 10;

    data = (unsigned char **) malloc ( images * sizeof(unsigned char *) );
    for ( i = 0; i < images; i++ ) {
        data[i] = (unsigned char *) malloc ( rows * cols );
        for ( r = 0; r < rows; r++ ) {
            for ( c  = 0; c < cols; c++ ) {
                data[i][r*cols+c]=random() & 0x0f;
            }
        }
    }
    result = (float *)malloc(rows*cols*sizeof(float));

    start = time();
    for ( i = 0; i < count; i++ ) {
        sobel ( data[i%images], result, rows, cols );
    }
    end = time();
    for ( r = 0; r < 10; r++ ) {
        for ( c  = 0; c < 10; c++ ) {
            printf( "%7d ",data[0][r*cols+c]);
        }
        printf("\n");
    }
    printf("\n");
    for ( r = 0; r < 10; r++ ) {
        for ( c  = 0; c < 10; c++ ) {
            printf( "%7.2f ",result[r*cols+c]);
        }
        printf("\n");
    }
    
    printf("used: %10.4f\n",end-start);
    printf("FPS: %10.4f\n",count/(end-start));
    return 0;
}
