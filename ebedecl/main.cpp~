#include <iostream>
#include <cstdio>
#include "symbols.h"

extern "C" int yyparse();
extern void build_token_map();

int main()
{
    build_token_map();
    yyparse();
    dump_labels();
    dump_strucs();
    return 0;
}
