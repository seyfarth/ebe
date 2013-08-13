#include <cstdio>

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
    double y = 1.75;
    float x = 1.5;
    unsigned long ul = 125;
    long l = 125;
    unsigned int ui = 125;
    int i = 125;
    unsigned short us = 125;
    short s = 125;
    signed char sc = 125;
    unsigned char uc = 125;
    char c = 125;
    bool b = true;

    dump(y);
    dump(x);
    dump(ul);
    dump(l);
    dump(ui);
    dump(i);
    dump(us);
    dump(s);
    dump(sc);
    dump(uc);
    dump(c);
    dump(b);
    
    dump((ul << 3) + 1);

    return 0;
}
