#include <stdio.h>
#include "vector.h"

int main()
{
    int i, n;
    Vector *v;

    v = vector_create(sizeof(int));
    n = 10;
    for ( i = 0; i < n; i++ ) {
        vector_pushback(v,&i);
    }
    while ( vector_popback(v,&i) == 0 ) {
        printf("%d ",i);
    }
    printf("\n");
    return 0;
}
