%{
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include "symbols.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);
char *latest_label=0;
string format = "hex1";
int incr = 1;
int size;
int count=1;
int data_loc=0;
int bss_loc=0;
int text_loc=0;
int *loc = &data_loc;
int *save_loc;
extern struc *current_struc;
int times=1;
string name;
%}

%union {
    int ival;
    double fval;
    char *sval;
}

%token <ival> INST
%token <ival> FPREG
%token <ival> REG
%token <ival> INT
%token <fval> FLOAT
%token <ival> BOOL
%token <sval> STRING
%token <sval> ID

%token DB
%token DW
%token DD
%token DQ
%token DT
%token DDQ
%token DO
%token DY
%token RESB
%token RESW
%token RESD
%token RESQ
%token REST
%token RESDQ
%token RESO
%token RESY
%token EQU
%token INCBIN
%token TIMES

%token BYTE
%token WORD
%token DWORD
%token QWORD
%token TWORD

%token BITS
%token USE16
%token USE32
%token USE64

%token DEFAULT
%token REL
%token ABS

%token SEGMENT
%token TEXT
%token DATA
%token BSS
%token SECT

%token ABSOLUTE
%token EXTERN
%token GLOBAL
%token COMMON

%token STRUC
%token ENDSTRUC

%token ALIGN

%token TRUE
%token FALSE

%token DOLLAR
%token DDOLLAR

%type <ival> start
%type <ival> file
%type <ival> command
%type <ival> instruction
%type <ival> values
%type <ival> expr
%type <ival> operand

%left '|'
%left '^'
%left '&'
%left '>' '<'
%left '+' '-'
%left '*' '/' '%'
%precedence NEG

%%

start: file

file: file command
    | command
    ;

command: instruction    { times = 1; }
    | segment_selection { times = 1; }
    | data_definition   { times = 1; }
    | start_structure   { times = 1; }
    | end_structure     { times = 1; }
    | data_reservation  { times = 1; }
    | label             { times = 1; }
    | TIMES expr        { times = $2; }
    | ALIGN expr        { *loc = (*loc+$2-1)/$2*$2; }
    | label EQU expr    {
        times = 1;
        set_location(latest_label,$3);
      }
    | '\n'              { }
    | error             { times = 1; }
    ;
    
start_structure:
    STRUC ID  {
        save_loc = loc;
        current_struc = start_struc ( $2 );
        loc = &(current_struc->loc);
    };

end_structure:
    ENDSTRUC  {
        name = current_struc->name + "_size";
        current_struc = 0;
        insert_label ( (char *)name.c_str(), *loc );
        loc = save_loc;
    };
    
segment_selection:
      SEGMENT TEXT      { 
        latest_label=(char *)"zzzzzzzzz";
        loc = &text_loc; 
      }
    | SEGMENT DATA      { 
        latest_label=(char *)"zzzzzzzzz";
        loc = &data_loc; 
      }
    | SEGMENT BSS       { 
        latest_label=(char *)"zzzzzzzzz";
        loc = &bss_loc; 
      }
    ;

data_reservation:
      RESB expr    {
        size = $2*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex1");
        *loc += size;
      }
    | RESW expr    {
        size = $2*2*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex2");
        *loc += size;
      }
    | RESD expr    {
        size = $2*4*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex4");
        *loc += size;
      }
    | RESQ expr    {
        size = $2*8*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex8");
        *loc += size;
      }
    | REST expr    {
        size = $2*10*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex2");
        *loc += size;
      }
    | RESO expr    {
        size = $2*16*times;
        set_size(latest_label,size);
        set_format(latest_label,"double");
        *loc += size;
      }
    | RESDQ expr   {
        size = $2*16*times;
        set_size(latest_label,size);
        set_format(latest_label,"double");
        *loc += size;
      }
    | RESY expr    {
        size = $2*32*times;
        set_size(latest_label,size);
        set_format(latest_label,"double");
        *loc += size;
      }
    ;

data_definition:
      DB  { size=0; count=1; incr=1; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec1";
        } else if ( format == "float" ) {
            format = "hex1";
        } else if ( format == "hex1" ) {
            format = "hex1";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    | DW  { size=0; count=1; incr=2; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec2";
        } else if ( format == "float" ) {
            format = "hex2";
        } else if ( format == "hex1" ) {
            format = "hex2";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    | DD  { size=0; count=1; incr=4; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec4";
        } else if ( format == "hex1" ) {
            format = "hex4";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    | DQ  { size=0; count=1; incr=8; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec8";
        } else if ( format == "float" ) {
            format = "double";
        } else if ( format == "hex1" ) {
            format = "hex8";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    | DT  { size=0; count=1; incr=10; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec2";
        } else if ( format == "float" ) {
            format = "hex2";
        } else if ( format == "hex1" ) {
            format = "hex2";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    | DO  { size=0; count=1; incr=16; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec8";
        } else if ( format == "float" ) {
            format = "double";
        } else if ( format == "hex1" ) {
            format = "hex8";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    | DDQ { size=0; count=1; incr=16; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec8";
        } else if ( format == "float" ) {
            format = "double";
        } else if ( format == "hex1" ) {
            format = "hex8";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    | DY  { size=0; count=1; incr=32; } values {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec8";
        } else if ( format == "float" ) {
            format = "double";
        } else if ( format == "hex1" ) {
            format = "hex8";
        }
        set_format(latest_label,format);
        *loc += size;
        }
    ;

values:
      values ',' expr   {
          size += count*incr*times; count=1;
      }
    | expr              {
          size +=  count*incr*times; count=1;
      }
    ;

expr:
      INT               { if ( format != "string" ) format="dec"; }
    | FLOAT             { if ( format != "string" ) format="float";}
    | STRING            { format="string"; count=(strlen($1)+incr-1)/incr; }
    | ID                { $$ = get_location($1); }
    | DOLLAR            { $$ = *loc; }
    | DDOLLAR           { $$ = 0; }
    | '(' expr ')'      { $$ = $2; }
    | '-' expr %prec NEG{ $$ = -$2; }
    | '~' expr          { $$ = ~$2; }
    | expr '+' expr     { $$ = $1 + $3; }
    | expr '-' expr     { $$ = $1 - $3; }
    | expr '*' expr     { $$ = $1 * $3; }
    | expr '/' expr     { $$ = $1 / $3; }
    | expr '/' '/' expr { $$ = $1 / $4; }
    | expr '%' expr     { $$ = $1 % $3; }
    | expr '%' '%' expr { $$ = $1 % $4; }
    | expr '|' expr     { $$ = $1 | $3; }
    | expr '^' expr     { $$ = $1 ^ $3; }
    | expr '&' expr     { $$ = $1 & $3; }
    | expr '>' '>' expr { $$ = $1 >> $4; }
    | expr '<' '<' expr { $$ = $1 << $4; }
    | REG               { $$ = 0; }
    ;

label:
      ID ':'            {
          if ( $1[0] == '.' ) {
              name = latest_label;
              name += $1;
              latest_label = (char *)name.c_str();
          } else {
              latest_label = $1;
          }
          insert_label(latest_label,*loc);
      }
    | ID                {
          if ( $1[0] == '.' ) {
              name = latest_label;
              name += $1;
              latest_label = (char *)name.c_str();
          } else {
              latest_label = $1;
          }
          insert_label(latest_label,*loc);
      }
    ;

instruction: INST '\n'      { }
    | INST operand '\n'
    | INST operand ',' operand '\n'
    | INST operand ',' operand ',' operand '\n'
    ;

operand:
      FPREG
    | expr 
    | '[' expr ']'      { $$ = $2; }
    ;

%%

void yyerror(const char *s)
{
}
