/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parse.y" /* yacc.c:339  */

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

#line 94 "parse.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parse.h".  */
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
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 29 "parse.y" /* yacc.c:355  */

    int ival;
    double fval;
    char *sval;

#line 199 "parse.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 214 "parse.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  74
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   311

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  128

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   313

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      69,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    67,    60,     2,
      71,    72,    65,    63,    70,    64,     2,    66,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    74,     2,
      62,     2,    61,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    75,     2,    76,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    58,     2,    73,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    68
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   119,   119,   121,   122,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   138,   139,   143,   150,   158,
     162,   166,   173,   179,   185,   191,   197,   203,   209,   215,
     224,   224,   236,   236,   248,   248,   258,   258,   270,   270,
     282,   282,   294,   294,   306,   306,   321,   324,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     355,   365,   377,   378,   379,   380,   384,   385,   386
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INST", "FPREG", "REG", "INT", "FLOAT",
  "BOOL", "STRING", "ID", "DB", "DW", "DD", "DQ", "DT", "DDQ", "DO", "DY",
  "RESB", "RESW", "RESD", "RESQ", "REST", "RESDQ", "RESO", "RESY", "EQU",
  "INCBIN", "TIMES", "BYTE", "WORD", "DWORD", "QWORD", "TWORD", "BITS",
  "USE16", "USE32", "USE64", "DEFAULT", "REL", "ABS", "SEGMENT", "TEXT",
  "DATA", "BSS", "SECT", "ABSOLUTE", "EXTERN", "GLOBAL", "COMMON", "STRUC",
  "ENDSTRUC", "ALIGN", "TRUE", "FALSE", "DOLLAR", "DDOLLAR", "'|'", "'^'",
  "'&'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "NEG", "'\\n'",
  "','", "'('", "')'", "'~'", "':'", "'['", "']'", "$accept", "start",
  "file", "command", "start_structure", "end_structure",
  "segment_selection", "data_reservation", "data_definition", "$@1", "$@2",
  "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "values", "expr", "label",
  "instruction", "operand", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   124,    94,
      38,    62,    60,    43,    45,    42,    47,    37,   313,    10,
      44,    40,    41,   126,    58,    91,    93
};
# endif

#define YYPACT_NINF -88

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-88)))

#define YYTABLE_NINF -3

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     180,   -88,    -3,   -38,   -88,   -88,   -88,   -88,   -88,   -88,
     -88,   -88,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   -24,    30,   -88,   205,   -88,    43,   120,   -88,   -88,
     -88,   -88,   -88,   -88,    25,   -88,   -88,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   205,   -88,   205,   205,   205,   221,
     -61,   -88,   205,   205,   205,   205,   205,   205,   205,   205,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   -88,
     -88,   -88,   -88,   221,   -88,   -88,   205,   -88,   192,   221,
     -35,   205,   205,   205,    -6,     2,   205,   205,   205,    94,
     170,   -88,    53,   -14,   221,   -14,   -14,   -14,   -14,   -14,
     -14,   -14,   221,   -88,   -88,   230,    31,   237,   205,   205,
     -32,   -32,   -88,   205,   -88,   205,   -88,   -31,   205,   -16,
     -16,   -88,   -88,   -88,    53,   221,    -4,   -88
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    16,     0,    71,    30,    32,    34,    36,    38,    42,
      40,    44,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,     0,    15,     0,     0,     4,     8,
       9,     6,    10,     7,    11,     5,    76,    69,    48,    49,
      50,    51,    52,    53,     0,    72,     0,     0,     0,    77,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      22,    23,    24,    25,    26,    28,    27,    29,    12,    19,
      20,    21,    17,    13,     1,     3,     0,    55,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    31,    47,    33,    35,    37,    39,    43,
      41,    45,    14,    54,    78,    64,    65,    66,     0,     0,
      57,    58,    59,     0,    60,     0,    62,     0,     0,    67,
      68,    61,    63,    74,     0,    46,     0,    75
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -88,   -88,   -88,    40,   -88,   -88,   -88,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   -88,   -88,   -88,   252,    -2,   -88,
     -88,   -87
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    26,    27,    28,    29,    30,    31,    32,    33,    52,
      53,    54,    55,    56,    58,    57,    59,    93,    94,    34,
      35,    50
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      49,    36,    37,    38,    39,   117,    40,    41,    91,    92,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    73,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    88,    89,    90,    51,   126,   123,   124,
      72,   104,    77,    74,    78,    79,    80,    86,    87,    88,
      89,    90,    76,    42,    43,   108,   118,    36,    37,    38,
      39,    44,    40,    41,   109,   127,    45,    75,    46,     0,
      47,     0,    48,     0,   102,     0,     0,     0,     0,   105,
     106,   107,     0,     0,   110,   111,   112,   114,   116,     0,
      49,    83,    84,    85,    86,    87,    88,    89,    90,    37,
      38,    39,     0,    40,    41,     0,   119,   120,     0,    42,
      43,   121,     0,   122,     0,     0,   125,    44,     0,     0,
      -2,     1,    49,     2,    46,     0,    47,     0,    48,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    20,
      42,    43,     0,     0,     0,     0,     0,     0,    44,     0,
     113,     0,    21,     0,     0,    46,     0,    47,     0,     0,
       0,    22,    23,    24,     0,    37,    38,    39,     0,    40,
      41,     1,     0,     2,     0,     0,     0,     0,     0,    25,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    20,
      37,    38,    39,     0,    40,    41,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,    42,    43,     0,     0,
       0,    22,    23,    24,    44,     0,     0,   115,     0,     0,
       0,    46,     0,    47,     0,     0,     0,     0,     0,    25,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
       0,    42,    43,     0,   103,     0,     0,     0,     0,    44,
       0,     0,     0,     0,     0,     0,    46,     0,    47,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    84,    85,
      86,    87,    88,    89,    90,    95,    96,    97,    98,    99,
     100,   101
};

static const yytype_int8 yycheck[] =
{
       2,     4,     5,     6,     7,    92,     9,    10,    69,    70,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    43,
      44,    45,    24,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    65,    66,    67,    74,   124,    69,    70,
      10,    76,    44,     0,    46,    47,    48,    63,    64,    65,
      66,    67,    27,    56,    57,    61,    70,     4,     5,     6,
       7,    64,     9,    10,    62,    69,    69,    27,    71,    -1,
      73,    -1,    75,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    90,    -1,
      92,    60,    61,    62,    63,    64,    65,    66,    67,     5,
       6,     7,    -1,     9,    10,    -1,   108,   109,    -1,    56,
      57,   113,    -1,   115,    -1,    -1,   118,    64,    -1,    -1,
       0,     1,   124,     3,    71,    -1,    73,    -1,    75,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      66,    -1,    42,    -1,    -1,    71,    -1,    73,    -1,    -1,
      -1,    51,    52,    53,    -1,     5,     6,     7,    -1,     9,
      10,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    69,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
       5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    51,    52,    53,    64,    -1,    -1,    67,    -1,    -1,
      -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,    69,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      -1,    56,    57,    -1,    72,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    61,    62,
      63,    64,    65,    66,    67,    53,    54,    55,    56,    57,
      58,    59
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      29,    42,    51,    52,    53,    69,    78,    79,    80,    81,
      82,    83,    84,    85,    96,    97,     4,     5,     6,     7,
       9,    10,    56,    57,    64,    69,    71,    73,    75,    95,
      98,    74,    86,    87,    88,    89,    90,    92,    91,    93,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    43,
      44,    45,    10,    95,     0,    80,    27,    95,    95,    95,
      95,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    69,    70,    94,    95,    94,    94,    94,    94,    94,
      94,    94,    95,    72,    76,    95,    95,    95,    61,    62,
      95,    95,    95,    66,    95,    67,    95,    98,    70,    95,
      95,    95,    95,    69,    70,    95,    98,    69
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    77,    78,    79,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    81,    82,    83,
      83,    83,    84,    84,    84,    84,    84,    84,    84,    84,
      86,    85,    87,    85,    88,    85,    89,    85,    90,    85,
      91,    85,    92,    85,    93,    85,    94,    94,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      96,    96,    97,    97,    97,    97,    98,    98,    98
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     1,     1,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     2,     3,     3,     3,
       3,     4,     3,     4,     3,     3,     3,     4,     4,     1,
       2,     1,     2,     3,     5,     7,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 125 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1426 "parse.c" /* yacc.c:1646  */
    break;

  case 6:
#line 126 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1432 "parse.c" /* yacc.c:1646  */
    break;

  case 7:
#line 127 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1438 "parse.c" /* yacc.c:1646  */
    break;

  case 8:
#line 128 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1444 "parse.c" /* yacc.c:1646  */
    break;

  case 9:
#line 129 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1450 "parse.c" /* yacc.c:1646  */
    break;

  case 10:
#line 130 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1456 "parse.c" /* yacc.c:1646  */
    break;

  case 11:
#line 131 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1462 "parse.c" /* yacc.c:1646  */
    break;

  case 12:
#line 132 "parse.y" /* yacc.c:1646  */
    { times = (yyvsp[0].ival); }
#line 1468 "parse.c" /* yacc.c:1646  */
    break;

  case 13:
#line 133 "parse.y" /* yacc.c:1646  */
    { *loc = (*loc+(yyvsp[0].ival)-1)/(yyvsp[0].ival)*(yyvsp[0].ival); }
#line 1474 "parse.c" /* yacc.c:1646  */
    break;

  case 14:
#line 134 "parse.y" /* yacc.c:1646  */
    {
        times = 1;
        set_location(latest_label,(yyvsp[0].ival));
      }
#line 1483 "parse.c" /* yacc.c:1646  */
    break;

  case 15:
#line 138 "parse.y" /* yacc.c:1646  */
    { }
#line 1489 "parse.c" /* yacc.c:1646  */
    break;

  case 16:
#line 139 "parse.y" /* yacc.c:1646  */
    { times = 1; }
#line 1495 "parse.c" /* yacc.c:1646  */
    break;

  case 17:
#line 143 "parse.y" /* yacc.c:1646  */
    {
        save_loc = loc;
        current_struc = start_struc ( (yyvsp[0].sval) );
        loc = &(current_struc->loc);
    }
#line 1505 "parse.c" /* yacc.c:1646  */
    break;

  case 18:
#line 150 "parse.y" /* yacc.c:1646  */
    {
        name = current_struc->name + "_size";
        current_struc = 0;
        insert_label ( (char *)name.c_str(), *loc );
        loc = save_loc;
    }
#line 1516 "parse.c" /* yacc.c:1646  */
    break;

  case 19:
#line 158 "parse.y" /* yacc.c:1646  */
    { 
        latest_label=(char *)"zzzzzzzzz";
        loc = &text_loc; 
      }
#line 1525 "parse.c" /* yacc.c:1646  */
    break;

  case 20:
#line 162 "parse.y" /* yacc.c:1646  */
    { 
        latest_label=(char *)"zzzzzzzzz";
        loc = &data_loc; 
      }
#line 1534 "parse.c" /* yacc.c:1646  */
    break;

  case 21:
#line 166 "parse.y" /* yacc.c:1646  */
    { 
        latest_label=(char *)"zzzzzzzzz";
        loc = &bss_loc; 
      }
#line 1543 "parse.c" /* yacc.c:1646  */
    break;

  case 22:
#line 173 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex1");
        *loc += size;
      }
#line 1554 "parse.c" /* yacc.c:1646  */
    break;

  case 23:
#line 179 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*2*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex2");
        *loc += size;
      }
#line 1565 "parse.c" /* yacc.c:1646  */
    break;

  case 24:
#line 185 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*4*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex4");
        *loc += size;
      }
#line 1576 "parse.c" /* yacc.c:1646  */
    break;

  case 25:
#line 191 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*8*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex8");
        *loc += size;
      }
#line 1587 "parse.c" /* yacc.c:1646  */
    break;

  case 26:
#line 197 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*10*times;
        set_size(latest_label,size);
        set_format(latest_label,"hex2");
        *loc += size;
      }
#line 1598 "parse.c" /* yacc.c:1646  */
    break;

  case 27:
#line 203 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*16*times;
        set_size(latest_label,size);
        set_format(latest_label,"double");
        *loc += size;
      }
#line 1609 "parse.c" /* yacc.c:1646  */
    break;

  case 28:
#line 209 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*16*times;
        set_size(latest_label,size);
        set_format(latest_label,"double");
        *loc += size;
      }
#line 1620 "parse.c" /* yacc.c:1646  */
    break;

  case 29:
#line 215 "parse.y" /* yacc.c:1646  */
    {
        size = (yyvsp[0].ival)*32*times;
        set_size(latest_label,size);
        set_format(latest_label,"double");
        *loc += size;
      }
#line 1631 "parse.c" /* yacc.c:1646  */
    break;

  case 30:
#line 224 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=1; }
#line 1637 "parse.c" /* yacc.c:1646  */
    break;

  case 31:
#line 224 "parse.y" /* yacc.c:1646  */
    {
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
#line 1654 "parse.c" /* yacc.c:1646  */
    break;

  case 32:
#line 236 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=2; }
#line 1660 "parse.c" /* yacc.c:1646  */
    break;

  case 33:
#line 236 "parse.y" /* yacc.c:1646  */
    {
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
#line 1677 "parse.c" /* yacc.c:1646  */
    break;

  case 34:
#line 248 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=4; }
#line 1683 "parse.c" /* yacc.c:1646  */
    break;

  case 35:
#line 248 "parse.y" /* yacc.c:1646  */
    {
        set_size(latest_label,size);
        if ( format == "dec" ) {
            format = "dec4";
        } else if ( format == "hex1" ) {
            format = "hex4";
        }
        set_format(latest_label,format);
        *loc += size;
        }
#line 1698 "parse.c" /* yacc.c:1646  */
    break;

  case 36:
#line 258 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=8; }
#line 1704 "parse.c" /* yacc.c:1646  */
    break;

  case 37:
#line 258 "parse.y" /* yacc.c:1646  */
    {
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
#line 1721 "parse.c" /* yacc.c:1646  */
    break;

  case 38:
#line 270 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=10; }
#line 1727 "parse.c" /* yacc.c:1646  */
    break;

  case 39:
#line 270 "parse.y" /* yacc.c:1646  */
    {
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
#line 1744 "parse.c" /* yacc.c:1646  */
    break;

  case 40:
#line 282 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=16; }
#line 1750 "parse.c" /* yacc.c:1646  */
    break;

  case 41:
#line 282 "parse.y" /* yacc.c:1646  */
    {
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
#line 1767 "parse.c" /* yacc.c:1646  */
    break;

  case 42:
#line 294 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=16; }
#line 1773 "parse.c" /* yacc.c:1646  */
    break;

  case 43:
#line 294 "parse.y" /* yacc.c:1646  */
    {
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
#line 1790 "parse.c" /* yacc.c:1646  */
    break;

  case 44:
#line 306 "parse.y" /* yacc.c:1646  */
    { size=0; count=1; incr=32; }
#line 1796 "parse.c" /* yacc.c:1646  */
    break;

  case 45:
#line 306 "parse.y" /* yacc.c:1646  */
    {
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
#line 1813 "parse.c" /* yacc.c:1646  */
    break;

  case 46:
#line 321 "parse.y" /* yacc.c:1646  */
    {
          size += count*incr*times; count=1;
      }
#line 1821 "parse.c" /* yacc.c:1646  */
    break;

  case 47:
#line 324 "parse.y" /* yacc.c:1646  */
    {
          size +=  count*incr*times; count=1;
      }
#line 1829 "parse.c" /* yacc.c:1646  */
    break;

  case 48:
#line 330 "parse.y" /* yacc.c:1646  */
    { if ( format != "string" ) format="dec"; }
#line 1835 "parse.c" /* yacc.c:1646  */
    break;

  case 49:
#line 331 "parse.y" /* yacc.c:1646  */
    { if ( format != "string" ) format="float";}
#line 1841 "parse.c" /* yacc.c:1646  */
    break;

  case 50:
#line 332 "parse.y" /* yacc.c:1646  */
    { format="string"; count=(strlen((yyvsp[0].sval))+incr-1)/incr; }
#line 1847 "parse.c" /* yacc.c:1646  */
    break;

  case 51:
#line 333 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = get_location((yyvsp[0].sval)); }
#line 1853 "parse.c" /* yacc.c:1646  */
    break;

  case 52:
#line 334 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = *loc; }
#line 1859 "parse.c" /* yacc.c:1646  */
    break;

  case 53:
#line 335 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 1865 "parse.c" /* yacc.c:1646  */
    break;

  case 54:
#line 336 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-1].ival); }
#line 1871 "parse.c" /* yacc.c:1646  */
    break;

  case 55:
#line 337 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = -(yyvsp[0].ival); }
#line 1877 "parse.c" /* yacc.c:1646  */
    break;

  case 56:
#line 338 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = ~(yyvsp[0].ival); }
#line 1883 "parse.c" /* yacc.c:1646  */
    break;

  case 57:
#line 339 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) + (yyvsp[0].ival); }
#line 1889 "parse.c" /* yacc.c:1646  */
    break;

  case 58:
#line 340 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) - (yyvsp[0].ival); }
#line 1895 "parse.c" /* yacc.c:1646  */
    break;

  case 59:
#line 341 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) * (yyvsp[0].ival); }
#line 1901 "parse.c" /* yacc.c:1646  */
    break;

  case 60:
#line 342 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) / (yyvsp[0].ival); }
#line 1907 "parse.c" /* yacc.c:1646  */
    break;

  case 61:
#line 343 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-3].ival) / (yyvsp[0].ival); }
#line 1913 "parse.c" /* yacc.c:1646  */
    break;

  case 62:
#line 344 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) % (yyvsp[0].ival); }
#line 1919 "parse.c" /* yacc.c:1646  */
    break;

  case 63:
#line 345 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-3].ival) % (yyvsp[0].ival); }
#line 1925 "parse.c" /* yacc.c:1646  */
    break;

  case 64:
#line 346 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) | (yyvsp[0].ival); }
#line 1931 "parse.c" /* yacc.c:1646  */
    break;

  case 65:
#line 347 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) ^ (yyvsp[0].ival); }
#line 1937 "parse.c" /* yacc.c:1646  */
    break;

  case 66:
#line 348 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-2].ival) & (yyvsp[0].ival); }
#line 1943 "parse.c" /* yacc.c:1646  */
    break;

  case 67:
#line 349 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-3].ival) >> (yyvsp[0].ival); }
#line 1949 "parse.c" /* yacc.c:1646  */
    break;

  case 68:
#line 350 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-3].ival) << (yyvsp[0].ival); }
#line 1955 "parse.c" /* yacc.c:1646  */
    break;

  case 69:
#line 351 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 1961 "parse.c" /* yacc.c:1646  */
    break;

  case 70:
#line 355 "parse.y" /* yacc.c:1646  */
    {
          if ( (yyvsp[-1].sval)[0] == '.' ) {
              name = latest_label;
              name += (yyvsp[-1].sval);
              latest_label = (char *)name.c_str();
          } else {
              latest_label = (yyvsp[-1].sval);
          }
          insert_label(latest_label,*loc);
      }
#line 1976 "parse.c" /* yacc.c:1646  */
    break;

  case 71:
#line 365 "parse.y" /* yacc.c:1646  */
    {
          if ( (yyvsp[0].sval)[0] == '.' ) {
              name = latest_label;
              name += (yyvsp[0].sval);
              latest_label = (char *)name.c_str();
          } else {
              latest_label = (yyvsp[0].sval);
          }
          insert_label(latest_label,*loc);
      }
#line 1991 "parse.c" /* yacc.c:1646  */
    break;

  case 72:
#line 377 "parse.y" /* yacc.c:1646  */
    { }
#line 1997 "parse.c" /* yacc.c:1646  */
    break;

  case 78:
#line 386 "parse.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-1].ival); }
#line 2003 "parse.c" /* yacc.c:1646  */
    break;


#line 2007 "parse.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 389 "parse.y" /* yacc.c:1906  */


void yyerror(const char *s)
{
}
