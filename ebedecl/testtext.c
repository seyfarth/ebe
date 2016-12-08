#include <stdio.h>
#include "text.h"

int main()
{
    Text t, s;

    t = text_create("Hello ");
    s = text_create("there ");

    printf("%s\n",text_string(t));
    
    t = text_cat(t,s);
    printf("%s\n",text_string(t));

    t = text_cats(t,"world!");
    printf("%s\n",text_string(t));

    return 0;
}

