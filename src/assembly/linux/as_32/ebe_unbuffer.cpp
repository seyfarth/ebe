#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;

#ifdef __linux__
ifstream __ebeNull("/dev/null");
FILE *__ebe_stdin;
#elif __APPLE__
ifstream __ebeNull("/dev/null");
FILE *__ebe_stdin;
#else
ifstream __ebeNull("nul");
FILE __ebe_stdin;
#endif
streambuf *__ebe_cin;
FILE *__ebe_NULL_fp;

void __ebeSetNULL()
{
    __ebe_cin = cin.rdbuf();
    cin.rdbuf(__ebeNull.rdbuf());
#ifdef __linux__
    __ebe_stdin = stdin;
    stdin = __ebe_NULL_fp;
#elif __APPLE__
    __ebe_stdin = stdin;
    stdin = __ebe_NULL_fp;
#else
    __ebe_stdin = *stdin;
    *stdin = *__ebe_NULL_fp;
#endif
}

void __ebeSetNormal()
{
    cin.rdbuf(__ebe_cin);
#ifdef __linux__
    stdin = __ebe_stdin;
#elif __APPLE__
    stdin = __ebe_stdin;
#else
    *stdin = __ebe_stdin;
#endif
}

class __EbeUnBuffer
{
public:
    __EbeUnBuffer();
};

__EbeUnBuffer::__EbeUnBuffer()
{
    setbuf(stdout,NULL);
    setbuf(stderr,NULL);
#ifdef __linux__
    __ebe_NULL_fp = fopen("/dev/null","r");
#elif __APPLE__
    __ebe_NULL_fp = fopen("/dev/null","r");
#else
    __ebe_NULL_fp = fopen("nul","r");
#endif
}

__EbeUnBuffer __unEbeBuffer;

