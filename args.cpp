#include <iostream>

int age=11;
using namespace std;

int main ( int argc, char **argv )
{
    string s = "hello world!";
    cout << s << endl;
    cout << "argc " << argc << age << "\n";
    for ( int i = 0; i < argc; i++ ) {
        cout << "argv[" << i << "] = " << argv[i] << endl;
    }
    return 0;
}
