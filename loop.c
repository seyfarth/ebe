#include <stdio.h>

int main ( int argc, char *argv[] )
{
    char s[80];
    int n = 0;
    
    while ( 1 ) {
        while ( scanf("%79s",s) == 1 ) {
            printf("%s\n",s);
        }
        n++;
        printf("EOF %d\n",n);
    }

    return 0;
}

