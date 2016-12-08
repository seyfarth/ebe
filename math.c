#include <stdio.h>

int main()
{
    int i;
    float a, b, c, d;
    float sum;
    
    for ( i = 0; i < 32; i++ ) {
        a = b = c = d = i / 32.0;
        sum += a+b+c+d;
    }
    printf("Sum = %f\n",sum);
    return 0;
}
