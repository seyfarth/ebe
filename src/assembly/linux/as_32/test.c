#include <stdio.h>

double sum ( double a, double b )
{
    return a+b;
}


int main ( int argc, char **argv )
{
    int n;

    scanf ( "%d", &n );
    printf ( "The sum of %d and %d is %g\n", n, argc, sum(n,argc) );
    return 0;
}
