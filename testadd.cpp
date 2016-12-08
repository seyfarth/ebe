#include <cstdio>
 
int add ( int a, int b );

int main ( int argc, char **argv )
{
    int a, b, sum;

    printf("Enter 2 integers: ");
    scanf ( "%d %d", &a, &b );
    sum = add(a,b);
    printf("%d + %d = %d\n", a, b, sum );
     
    return 0;
}
