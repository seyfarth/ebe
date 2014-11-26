#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>

int main()
{
	printf("O_RDONLY = %x\n", O_RDONLY );
	printf("O_WRONLY = %x\n", O_WRONLY );
	printf("O_RDWR = %x\n", O_RDWR );
	printf("O_CREAT = %x\n", O_CREAT );
	printf("O_BINARY = %x\n", O_BINARY );
	return 0;
}
