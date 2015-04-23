#include <iostream>
#include <cstdio>
#include "symbols.h"

extern "C" int yyparse();
extern void build_token_map();
extern FILE *yyin;

int main(int argc, char **argv)
{
    if ( argc > 1 ) {
        yyin = fopen ( argv[1], "r" );
        if ( yyin == 0 ) {
            fprintf(stderr,"Could not open %s\n",argv[1]);
            return 1;
        }
    }
    build_token_map();
    yyparse();
    dump_labels();
    dump_strucs();
    return 0;
}
