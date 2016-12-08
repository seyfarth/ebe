#include <stdio.h>
#include <stdlib.h>

int main ( int argc, char **argv )
{
    int n, i;

    n = argc > 1 ? atoi(argv[1]) : 1000;
    
    i = 1;
    while ( n != 1 ) {
        printf("%4d %10d\n",i,n);
  //      scanf("%d",&x);
  //      fflush(stdout);
        if ( n & 1 ) {
            n = 3 * n + 1;
        } else {
            n = n / 2;
        }
        i++;
    }

    return 0;
}
