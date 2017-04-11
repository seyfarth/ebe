#include <iostream>

using namespace std;

long hello = 7000;

int mat[3][3] = {
    {1, 2, 3},
    {6, 7, 8},
    {9, 10, 11}
};
// dfdfds sdfsdfdsfsdgdgdgf
/*sdfsfdsfsdf dfsdfsdf
 dsfgfdf */

char wow[] = "stuff";
double v[100] = { 1.5, 2.5, 3.5 };
bool boo = true;
float abc;

class extra
{
public:
    int x;
    int y;
};

class stuff
{
public:
    int add();
    int a;
    double b;
    unsigned char c;
    extra two;
};

int main ( int argc, char *argv[] )
{
    double a, b, c;
    double avg;
    int x[7];
    int i;
    char s[] = "hello";
    char ch = 'd';
    stuff my;
    stuff *p;
    string name="Ray";

    cout << "ready for action" << endl;
    my.a = 1;
    my.b = 2;
    my.c = 'g';
    my.two.x = 1000;
    my.two.y = 2000;
    p = &my;
    p->a = 3;

    i = 0;
    while ( cin >> a >> b >> c ) {
        cout << "a " << a << "  b " << b << "  c " << c << endl;
        avg = (a+b+c)/3.0;
        cout << "avg " << avg << endl;
        x[i%7] = avg + hello;
        i++;
    }
    cout << x[0] << s << my.c << ch << endl;
    return 0;
}
