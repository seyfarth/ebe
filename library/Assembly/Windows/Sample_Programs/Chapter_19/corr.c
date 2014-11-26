#include <math.h>
#include <stdio.h>
double corr ( double x[], double y[], long n )
{
    double sum_x, sum_y, sum_xx, sum_yy, sum_xy;
    long i;

    sum_x = sum_y = sum_xx = sum_yy = sum_xy = 0.0;
    for ( i = 0; i < n; i++ ) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xx += x[i]*x[i];
        sum_yy += y[i]*y[i];
        sum_xy += x[i]*y[i];
    }
    printf("sum x  %f\n", sum_x);
    printf("sum y  %f\n", sum_y);
    printf("sum xx  %f\n", sum_xx);
    printf("sum yy  %f\n", sum_yy);
    printf("sum xy  %f\n", sum_xy);
    printf("sum n*sumxy  %f\n", n*sum_xy);
    printf("sum sumx*sumx  %f\n", sum_x*sum_y);
    printf("den 1  %f\n",n*sum_xx-sum_x*sum_x);
    printf("den 2  %f\n", n*sum_yy-sum_y*sum_y);
    return (n*sum_xy-sum_x*sum_y)/
           sqrt((n*sum_xx-sum_x*sum_x)*(n*sum_yy-sum_y*sum_y));
}
