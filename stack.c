#include <stdio.h>
#include <stdlib.h>

int global_j = 0;

int main ()
{

    char *h = malloc(10);
    int j = 0;

    printf ("Globals are : %p, text is %p, stack is %p, heap is %p\n",
            &global_j, main, &j, h);
    return 0;
}
