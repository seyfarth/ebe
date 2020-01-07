/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
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
/* Tokens.  */
#define INST 258
#define FPREG 259
#define REG 260
#define INT 261
#define FLOAT 262
#define BOOL 263
#define STRING 264
#define ID 265
#define DB 266
#define DW 267
#define DD 268
#define DQ 269
#define DT 270
#define DDQ 271
#define DO 272
#define DY 273
#define RESB 274
#define RESW 275
#define RESD 276
#define RESQ 277
#define REST 278
#define RESDQ 279
#define RESO 280
#define RESY 281
#define EQU 282
#define INCBIN 283
#define TIMES 284
#define BYTE 285
#define WORD 286
#define DWORD 287
#define QWORD 288
#define TWORD 289
#define BITS 290
#define USE16 291
#define USE32 292
#define USE64 293
#define DEFAULT 294
#define REL 295
#define ABS 296
#define SEGMENT 297
#define TEXT 298
#define DATA 299
#define BSS 300
#define SECT 301
#define ABSOLUTE 302
#define EXTERN 303
#define GLOBAL 304
#define COMMON 305
#define STRUC 306
#define ENDSTRUC 307
#define ALIGN 308
#define TRUE 309
#define FALSE 310
#define DOLLAR 311
#define DDOLLAR 312
#define NEG 313




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 29 "parse.y"
{
    int ival;
    double fval;
    char *sval;
}
/* Line 1529 of yacc.c.  */
#line 171 "parse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

