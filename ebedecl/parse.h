/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSE_H_INCLUDED
# define YY_YY_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INST = 258,
    FPREG = 259,
    REG = 260,
    INT = 261,
    FLOAT = 262,
    BOOL = 263,
    STRING = 264,
    ID = 265,
    DB = 266,
    DW = 267,
    DD = 268,
    DQ = 269,
    DT = 270,
    DDQ = 271,
    DO = 272,
    DY = 273,
    RESB = 274,
    RESW = 275,
    RESD = 276,
    RESQ = 277,
    REST = 278,
    RESDQ = 279,
    RESO = 280,
    RESY = 281,
    EQU = 282,
    INCBIN = 283,
    TIMES = 284,
    BYTE = 285,
    WORD = 286,
    DWORD = 287,
    QWORD = 288,
    TWORD = 289,
    BITS = 290,
    USE16 = 291,
    USE32 = 292,
    USE64 = 293,
    DEFAULT = 294,
    REL = 295,
    ABS = 296,
    SEGMENT = 297,
    TEXT = 298,
    DATA = 299,
    BSS = 300,
    SECT = 301,
    ABSOLUTE = 302,
    EXTERN = 303,
    GLOBAL = 304,
    COMMON = 305,
    STRUC = 306,
    ENDSTRUC = 307,
    ALIGN = 308,
    TRUE = 309,
    FALSE = 310,
    DOLLAR = 311,
    DDOLLAR = 312,
    NEG = 313
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 29 "parse.y" /* yacc.c:1909  */

    int ival;
    double fval;
    char *sval;

#line 119 "parse.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
