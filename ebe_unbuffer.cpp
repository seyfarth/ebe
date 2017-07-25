#include <cstdio>
#include <iostream>
#include <fstream>
#ifndef Q_OS_WIN32
#include <signal.h>
#endif

using namespace std;

class __EbeUnBuffer
{
public:
    __EbeUnBuffer();
};

#ifndef Q_OS_WIN32
void __ebeHandler ( int sig )
{
    signal ( SIGUSR1, __ebeHandler );
}
    
#endif

__EbeUnBuffer::__EbeUnBuffer()
{
    setbuf(stdout,NULL);
    setbuf(stderr,NULL);
#ifndef Q_OS_WIN32
    signal ( SIGUSR1, __ebeHandler );
#endif
}

__EbeUnBuffer __unEbeBuffer;

