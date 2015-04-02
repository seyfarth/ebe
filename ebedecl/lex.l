%{
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;
#define YY_DECL extern "C" int yylex()
// Also include the Bison generated header where the tokens are defined.
#include "parse.h"
#include "tokenmap.h"
int len;
%}

%%

[0-9]+ {
    yylval.ival = atoi(yytext);
    return INT;
}

0[xX][0-9a-fA-F]+ {
    yylval.ival = atoi(yytext);
    return INT;
}

([0-9]+\.[0-9]+|\.[0-9]+|[0-9]+\.) {
    yylval.fval = atof(yytext);
    return FLOAT;
}

[\.a-zA-Z_][a-zA-Z_0-9]*  {
    yylval.sval = strdup(yytext);
    return string_to_token(yytext);
}

\"([^"\\]*|\\.)*\"|'([^'\\]*|\\.)*' {
    len = strlen(yytext);
    yytext[len-1] = 0;
    yylval.sval = strdup(yytext+1);
    return STRING;
}

\$\$    { return DDOLLAR; }

\$      { return DOLLAR; }

[ \t\r]  {} 

\;[^\n]*\n {}

\n   { return yytext[0]; }

.    { return yytext[0]; }

%%
