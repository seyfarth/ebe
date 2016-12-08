/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
    INT = 258,
    FLOAT = 259,
    BOOL = 260,
    STRING = 261,
    ID = 262,
    DB = 263,
    DW = 264,
    DD = 265,
    DQ = 266,
    DT = 267,
    DDQ = 268,
    DO = 269,
    DY = 270,
    RESB = 271,
    RESW = 272,
    RESD = 273,
    RESQ = 274,
    REST = 275,
    RESDQ = 276,
    RESO = 277,
    RESY = 278,
    EQU = 279,
    INCBIN = 280,
    TIMES = 281,
    BYTE = 282,
    WORD = 283,
    DWORD = 284,
    QWORD = 285,
    TWORD = 286,
    BITS = 287,
    USE16 = 288,
    USE32 = 289,
    USE64 = 290,
    DEFAULT = 291,
    REL = 292,
    ABS = 293,
    SEGMENT = 294,
    TEXT = 295,
    DATA = 296,
    BSS = 297,
    SECT = 298,
    ABSOLUTE = 299,
    EXTERN = 300,
    GLOBAL = 301,
    COMMON = 302,
    STRUC = 303,
    ENDSTRUC = 304,
    TRUE = 305,
    FALSE = 306,
    INST = 307,
    REG = 308,
    FPREG = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 34 "parse.y" /* yacc.c:1909  */

    QVariant *QVariant_t;

#line 113 "parse.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
