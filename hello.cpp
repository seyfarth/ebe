#include <iostream>

using namespace std;

int f(int);

int main ( int argc, char *argv[] )
{
    int n=0;
    for ( int i = 0; i < 10; i++ ) {
        n += f(i);
    }
    return 0;
}
int f ( int x )
{
    return x*x;
}

