#include <cstdio>
#include <cmath>

void print_type(double x)
{
    printf("double\n");
}

void print_type(float x)
{
    printf("float\n");
}

void print_type(unsigned long x)
{
    printf("unsigned long\n");
}

void print_type(long x)
{
    printf("long\n");
}

void print_type(unsigned int x)
{
    printf("unsigned int\n");
}

void print_type(int x)
{
    printf("int\n");
}

void print_type(unsigned short x)
{
    printf("unsigned short\n");
}

void print_type(short x)
{
    printf("short\n");
}

void print_type(signed char x)
{
    printf("signed char\n");
}

void print_type(unsigned char x)
{
    printf("unsigned char\n");
}

void print_type(char x)
{
    printf("char\n");
}

void print_type(bool x)
{
    printf("bool\n");
}

template <class T>
void dump ( T v )
{
    T x = v;
    int i;
    print_type(x);
    for ( i = 0; i < sizeof(T); i++ ) {
        printf("%02x ",((unsigned char *)&x)[i]);
    }
    printf("\n");
}

int main()
{
    int a = 100;
    int b = 6;
    int negb = -7;
    dump(a / negb);
    printf("%d\n",a);
    printf("%d\n",b);
    printf("%d\n",negb);
    return 0;
}
