#include <cstdio>

class __UnBuffer
{
public:
    __UnBuffer();
};

__UnBuffer::__UnBuffer()
{
    setbuf(stdout,NULL);
}

__UnBuffer _unBuffer;
