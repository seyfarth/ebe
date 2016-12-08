#include <iostream>

using namespace std;

int fact(int);

int main(int argc, char **argv)
{
    cout << fact(5) << endl;
    return 0;
}

int fact(int n)
{
    if ( n < 2 ) return 1;
    return n * fact(n-1);
}
