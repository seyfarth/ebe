#include <iostream>

using namespace std;

int main ( int argc, char *argv[] )
{
    string s;
    int n = 0;
    
    while ( 1 ) {
        while ( cin >> s ) {
            cout << s << endl;
        }
        n++;
        cout << "EOF " << n << endl;
    }

    return 0;
}

