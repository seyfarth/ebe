#include <stdio.h>

float z;
long data[10];

int main ( int argc, char **argv )
{
    int x=0x12345678;
    int i;
    double sum=x+12;
    signed long y = 0x1234567890abcdef;
    z = 10;
    x = 2 * z;
    argc = x;
    for ( i = 0; i < 10; i++ ) {
        data[i] = i*i;
    }
    printf ( "Hello Worl=d!\n" );
    printf ( "x = %ld, %lf\n", x+y+(int)z, sum );
    return 0;
}
