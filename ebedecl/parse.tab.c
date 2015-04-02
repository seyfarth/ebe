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
#line 22 "parse.y" /* yacc.c:339  */

#include <QtGui>
#include <QHash>

QHash<QString,int> token_map;

extern int yylex(void);
void yyerror(const char *s);

#line 76 "parse.tab.c" /* yacc.c:339  */

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
    RESB = 270,
    RESW = 271,
    RESD = 272,
    RESQ = 273,
    REST = 274,
    RESDQ = 275,
    RESO = 276,
    EQU = 277,
    INCBIN = 278,
    TIMES = 279,
    BYTE = 280,
    WORD = 281,
    DWORD = 282,
    QWORD = 283,
    TWORD = 284,
    BITS = 285,
    USE16 = 286,
    USE32 = 287,
    USE64 = 288,
    DEFAULT = 289,
    REL = 290,
    ABS = 291,
    SEGMENT = 292,
    TEXT = 293,
    DATA = 294,
    BSS = 295,
    SECT = 296,
    ABSOLUTE = 297,
    EXTERN = 298,
    GLOBAL = 299,
    COMMON = 300,
    STRUC = 301,
    ENDSTRUC = 302,
    TRUE = 303,
    FALSE = 304,
    INST = 305,
    REG = 306,
    FPREG = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 34 "parse.y" /* yacc.c:355  */

    QVariant *QVariant_t;

#line 170 "parse.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 185 "parse.tab.c" /* yacc.c:358  */

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
#define YYFINAL  35
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   65

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  29
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  39

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      53,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    54,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   111,   111,   113,   114,   115,   118,   119,   120,   121,
     122,   126,   127,   128,   132,   133,   134,   135,   136,   137,
     138,   142,   143,   147,   148,   149,   153,   154,   155,   158
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "BOOL", "STRING", "ID",
  "DB", "DW", "DD", "DQ", "DT", "DDQ", "DO", "RESB", "RESW", "RESD",
  "RESQ", "REST", "RESDQ", "RESO", "EQU", "INCBIN", "TIMES", "BYTE",
  "WORD", "DWORD", "QWORD", "TWORD", "BITS", "USE16", "USE32", "USE64",
  "DEFAULT", "REL", "ABS", "SEGMENT", "TEXT", "DATA", "BSS", "SECT",
  "ABSOLUTE", "EXTERN", "GLOBAL", "COMMON", "STRUC", "ENDSTRUC", "TRUE",
  "FALSE", "INST", "REG", "FPREG", "'\\n'", "','", "':'", "$accept",
  "start", "file", "command", "segment_selection", "data_definition",
  "values", "value", "label", "instruction", YY_NULLPTR
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
     305,   306,   307,    10,    44,    58
};
# endif

#define YYPACT_NINF -52

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-52)))

#define YYTABLE_NINF -29

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       0,   -52,   -51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     -21,   -52,   -52,     6,    15,   -52,   -52,   -52,   -52,   -52,
     -52,   -52,   -52,   -52,   -34,   -52,   -34,   -34,   -34,   -34,
     -34,   -34,   -52,   -52,   -52,   -52,   -52,    -1,   -52
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    10,    27,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     4,     0,     0,     5,     7,     8,     9,     6,
      26,    23,    24,    25,    14,    22,    15,    16,    17,    18,
      20,    19,    11,    12,    13,     1,     3,     0,    21
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -52,   -52,   -52,     7,   -52,   -52,    26,     1,   -52,   -52
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    24,    25,    18,    19
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
     -28,     1,    21,    22,    20,    23,    35,     2,     3,     4,
       5,     6,     7,     8,     9,    -2,     1,    32,    33,    34,
      37,    36,     2,     3,     4,     5,     6,     7,     8,     9,
      26,    27,    28,    29,    30,    31,     0,    10,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,     0,    10,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    11
};

static const yytype_int8 yycheck[] =
{
       0,     1,     3,     4,    55,     6,     0,     7,     8,     9,
      10,    11,    12,    13,    14,     0,     1,    38,    39,    40,
      54,    14,     7,     8,     9,    10,    11,    12,    13,    14,
       4,     5,     6,     7,     8,     9,    -1,    37,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    37,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     7,     8,     9,    10,    11,    12,    13,    14,
      37,    50,    53,    57,    58,    59,    60,    61,    64,    65,
      55,     3,     4,     6,    62,    63,    62,    62,    62,    62,
      62,    62,    38,    39,    40,     0,    59,    54,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    58,    59,    59,    59,    59,
      59,    60,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    62,    62,    63,    63,    63,    64,    64,    64,    65
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     1,     1,     1,     1,     2,     1,     0,     1
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
        case 2:
#line 111 "parse.y" /* yacc.c:1646  */
    {qDebug() << "done";}
#line 1294 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 113 "parse.y" /* yacc.c:1646  */
    { qDebug() << "file command"; }
#line 1300 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 114 "parse.y" /* yacc.c:1646  */
    { qDebug() << "file empty"; }
#line 1306 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 126 "parse.y" /* yacc.c:1646  */
    { qDebug() << "text segment"; }
#line 1312 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 127 "parse.y" /* yacc.c:1646  */
    { qDebug() << "data segment"; }
#line 1318 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 128 "parse.y" /* yacc.c:1646  */
    { qDebug() << "bss segment"; }
#line 1324 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 147 "parse.y" /* yacc.c:1646  */
    { qDebug() << "INT value"; }
#line 1330 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 153 "parse.y" /* yacc.c:1646  */
    { qDebug() << "label" << *(yyvsp[-1].QVariant_t); }
#line 1336 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 154 "parse.y" /* yacc.c:1646  */
    { qDebug() << "label" << *(yyvsp[0].QVariant_t); }
#line 1342 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 155 "parse.y" /* yacc.c:1646  */
    { qDebug() << "empty label"; }
#line 1348 "parse.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 158 "parse.y" /* yacc.c:1646  */
    { qDebug() << "inst"; }
#line 1354 "parse.tab.c" /* yacc.c:1646  */
    break;


#line 1358 "parse.tab.c" /* yacc.c:1646  */
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
#line 162 "parse.y" /* yacc.c:1906  */


void yyerror(const char *s)
{
    qDebug() << "error:" << s;
}

void build_token_map()
{
    token_map["true"] = TRUE;
    token_map["false"] = FALSE;
    token_map["db"] = DB;
    token_map["dw"] = DW;
    token_map["dd"] = DD;
    token_map["dq"] = DQ;
    token_map["dt"] = DT;
    token_map["ddq"] = DDQ;
    token_map["do"] = DO;
    token_map["resb"] = RESB;
    token_map["resw"] = RESW;
    token_map["resd"] = RESD;
    token_map["resq"] = RESQ;
    token_map["rest"] = REST;
    token_map["resdq"] = RESDQ;
    token_map["reso"] = RESO;
    token_map["equ"] = EQU;
    token_map["incbin"] = INCBIN;
    token_map["times"] = TIMES;

    token_map["byte"] = BYTE;
    token_map["word"] = WORD;
    token_map["dword"] = DWORD;
    token_map["qword"] = QWORD;
    token_map["tword"] = TWORD;

    token_map["bits"] = BITS;
    token_map["use16"] = USE16;
    token_map["use32"] = USE32;
    token_map["use64"] = USE64;

    token_map["default"] = DEFAULT;
    token_map["rel"] = REL;
    token_map["abs"] = ABS;

    token_map["segment"] = SEGMENT;
    token_map["section"] = SEGMENT;
    token_map[".text"] = TEXT;
    token_map[".data"] = DATA;
    token_map[".bss"] = BSS;
    token_map["__SECT__"] = SECT;

    token_map["absolute"] = ABSOLUTE;
    token_map["extern"] = EXTERN;
    token_map["global"] = GLOBAL;
    token_map["common"] = COMMON;

    token_map["struc"] = STRUC;
    token_map["endstruc"] = ENDSTRUC;


    token_map["aad"] = INST;
    token_map["aam"] = INST;
    token_map["aas"] = INST;
    token_map["adc"] = INST;
    token_map["adcx"] = INST;
    token_map["add"] = INST;
    token_map["addsd"] = INST;
    token_map["addss"] = INST;
    token_map["addsubps"] = INST;
    token_map["adox"] = INST;
    token_map["aesdec"] = INST;
    token_map["aesdeclast"] = INST;
    token_map["aesenclast"] = INST;
    token_map["aesimc"] = INST;
    token_map["aeskeygenassist"] = INST;
    token_map["and"] = INST;
    token_map["andn"] = INST;
    token_map["andnpd"] = INST;
    token_map["andnps"] = INST;
    token_map["andpd"] = INST;
    token_map["andps"] = INST;
    token_map["arpl"] = INST;
    token_map["bb0_reset"] = INST;
    token_map["bb1_reset"] = INST;
    token_map["bextr"] = INST;
    token_map["blendps"] = INST;
    token_map["blendvpd"] = INST;
    token_map["blendvps"] = INST;
    token_map["blsi"] = INST;
    token_map["blsmsk"] = INST;
    token_map["blsr"] = INST;
    token_map["bound"] = INST;
    token_map["bsf"] = INST;
    token_map["bsr"] = INST;
    token_map["bswap"] = INST;
    token_map["bt"] = INST;
    token_map["btc"] = INST;
    token_map["btr"] = INST;
    token_map["bts"] = INST;
    token_map["bzhi"] = INST;
    token_map["call"] = INST;
    token_map["cbw"] = INST;
    token_map["cdq"] = INST;
    token_map["cdqe"] = INST;
    token_map["clac"] = INST;
    token_map["clc"] = INST;
    token_map["cld"] = INST;
    token_map["clflush"] = INST;
    token_map["clgi"] = INST;
    token_map["cli"] = INST;
    token_map["clts"] = INST;
    token_map["cmc"] = INST;
    token_map["cmova"] = INST;
    token_map["cmovae"] = INST;
    token_map["cmovb"] = INST;
    token_map["cmovbe"] = INST;
    token_map["cmovc"] = INST;
    token_map["cmove"] = INST;
    token_map["cmovg"] = INST;
    token_map["cmovge"] = INST;
    token_map["cmovl"] = INST;
    token_map["cmovle"] = INST;
    token_map["cmovna"] = INST;
    token_map["cmovnae"] = INST;
    token_map["cmovnb"] = INST;
    token_map["cmovnbe"] = INST;
    token_map["cmovnc"] = INST;
    token_map["cmovne"] = INST;
    token_map["cmovng"] = INST;
    token_map["cmovnge"] = INST;
    token_map["cmovnl"] = INST;
    token_map["cmovnle"] = INST;
    token_map["cmovno"] = INST;
    token_map["cmovnp"] = INST;
    token_map["cmovns"] = INST;
    token_map["cmovnz"] = INST;
    token_map["cmovo"] = INST;
    token_map["cmovp"] = INST;
    token_map["cmovpe"] = INST;
    token_map["cmovpo"] = INST;
    token_map["cmovs"] = INST;
    token_map["cmovz"] = INST;
    token_map["cmp"] = INST;
    token_map["cmpeqpd"] = INST;
    token_map["cmpeqps"] = INST;
    token_map["cmpeqsd"] = INST;
    token_map["cmpeqss"] = INST;
    token_map["cmplepd"] = INST;
    token_map["cmpleps"] = INST;
    token_map["cmplesd"] = INST;
    token_map["cmpless"] = INST;
    token_map["cmpltpd"] = INST;
    token_map["cmpltps"] = INST;
    token_map["cmpltsd"] = INST;
    token_map["cmpltss"] = INST;
    token_map["cmpneqpd"] = INST;
    token_map["cmpneqps"] = INST;
    token_map["cmpneqsd"] = INST;
    token_map["cmpneqss"] = INST;
    token_map["cmpnlepd"] = INST;
    token_map["cmpnleps"] = INST;
    token_map["cmpnlesd"] = INST;
    token_map["cmpnless"] = INST;
    token_map["cmpnltpd"] = INST;
    token_map["cmpnltps"] = INST;
    token_map["cmpnltsd"] = INST;
    token_map["cmpnltss"] = INST;
    token_map["cmpordpd"] = INST;
    token_map["cmpordps"] = INST;
    token_map["cmpordsd"] = INST;
    token_map["cmpordss"] = INST;
    token_map["cmppd"] = INST;
    token_map["cmpps"] = INST;
    token_map["cmpsb"] = INST;
    token_map["cmpsd"] = INST;
    token_map["cmpsq"] = INST;
    token_map["cmpss"] = INST;
    token_map["cmpsw"] = INST;
    token_map["cmpunordpd"] = INST;
    token_map["cmpunordps"] = INST;
    token_map["cmpunordsd"] = INST;
    token_map["cmpunordss"] = INST;
    token_map["cmpxchg"] = INST;
    token_map["cmpxchg16b"] = INST;
    token_map["cmpxchg486"] = INST;
    token_map["cmpxchg8b"] = INST;
    token_map["comisd"] = INST;
    token_map["comiss"] = INST;
    token_map["cpuid"] = INST;
    token_map["cpu_read"] = INST;
    token_map["cpu_write"] = INST;
    token_map["cqo"] = INST;
    token_map["crc32"] = INST;
    token_map["cvtdq2pd"] = INST;
    token_map["cvtdq2ps"] = INST;
    token_map["cvtpd2dq"] = INST;
    token_map["cvtpd2pi"] = INST;
    token_map["cvtpd2ps"] = INST;
    token_map["cvtpi2pd"] = INST;
    token_map["cvtpi2ps"] = INST;
    token_map["cvtps2dq"] = INST;
    token_map["cvtps2pd"] = INST;
    token_map["cvtps2pi"] = INST;
    token_map["cvtsd2si"] = INST;
    token_map["cvtsd2ss"] = INST;
    token_map["cvtsi2sd"] = INST;
    token_map["cvtsi2ss"] = INST;
    token_map["cvtss2sd"] = INST;
    token_map["cvtss2si"] = INST;
    token_map["cvttpd2dq"] = INST;
    token_map["cvttpd2pi"] = INST;
    token_map["cvttps2dq"] = INST;
    token_map["cvttps2pi"] = INST;
    token_map["cvttsd2si"] = INST;
    token_map["cvttss2si"] = INST;
    token_map["cwd"] = INST;
    token_map["cwde"] = INST;
    token_map["daa"] = INST;
    token_map["das"] = INST;
    token_map["db"] = INST;
    token_map["dd"] = INST;
    token_map["ddq"] = INST;
    token_map["dec"] = INST;
    token_map["div"] = INST;
    token_map["divpd"] = INST;
    token_map["divps"] = INST;
    token_map["divsd"] = INST;
    token_map["divss"] = INST;
    token_map["dmint"] = INST;
    token_map["do"] = INST;
    token_map["dppd"] = INST;
    token_map["dpps"] = INST;
    token_map["dq"] = INST;
    token_map["dt"] = INST;
    token_map["dw"] = INST;
    token_map["dy"] = INST;
    token_map["emms"] = INST;
    token_map["enter"] = INST;
    token_map["equ"] = INST;
    token_map["extern"] = INST;
    token_map["extractps"] = INST;
    token_map["extrq"] = INST;
    token_map["f2xm1"] = INST;
    token_map["fabs"] = INST;
    token_map["fadd"] = INST;
    token_map["faddp"] = INST;
    token_map["fbld"] = INST;
    token_map["fbstp"] = INST;
    token_map["fchs"] = INST;
    token_map["fclex"] = INST;
    token_map["fcmovb"] = INST;
    token_map["fcmovbe"] = INST;
    token_map["fcmove"] = INST;
    token_map["fcmovnb"] = INST;
    token_map["fcmovnbe"] = INST;
    token_map["fcmovne"] = INST;
    token_map["fcmovnu"] = INST;
    token_map["fcmovu"] = INST;
    token_map["fcom"] = INST;
    token_map["fcomi"] = INST;
    token_map["fcomip"] = INST;
    token_map["fcomp"] = INST;
    token_map["fcompp"] = INST;
    token_map["fcos"] = INST;
    token_map["fdecstp"] = INST;
    token_map["fdisi"] = INST;
    token_map["fdiv"] = INST;
    token_map["fdivp"] = INST;
    token_map["fdivr"] = INST;
    token_map["fdivrp"] = INST;
    token_map["femms"] = INST;
    token_map["feni"] = INST;
    token_map["ffree"] = INST;
    token_map["ffreep"] = INST;
    token_map["fiadd"] = INST;
    token_map["ficom"] = INST;
    token_map["ficomp"] = INST;
    token_map["fidiv"] = INST;
    token_map["fidivr"] = INST;
    token_map["fild"] = INST;
    token_map["fimul"] = INST;
    token_map["fincstp"] = INST;
    token_map["finit"] = INST;
    token_map["fist"] = INST;
    token_map["fistp"] = INST;
    token_map["fisttp"] = INST;
    token_map["fisub"] = INST;
    token_map["fisubr"] = INST;
    token_map["fld"] = INST;
    token_map["fld1"] = INST;
    token_map["fldcw"] = INST;
    token_map["fldenv"] = INST;
    token_map["fldl2e"] = INST;
    token_map["fldl2t"] = INST;
    token_map["fldlg2"] = INST;
    token_map["fldln2"] = INST;
    token_map["fldpi"] = INST;
    token_map["fldz"] = INST;
    token_map["fmul"] = INST;
    token_map["fmulp"] = INST;
    token_map["fnclex"] = INST;
    token_map["fndisi"] = INST;
    token_map["fneni"] = INST;
    token_map["fninit"] = INST;
    token_map["fnop"] = INST;
    token_map["fnsave"] = INST;
    token_map["fnstcw"] = INST;
    token_map["fnstenv"] = INST;
    token_map["fnstsw"] = INST;
    token_map["fpatan"] = INST;
    token_map["fprem"] = INST;
    token_map["fprem1"] = INST;
    token_map["fptan"] = INST;
    token_map["frndint"] = INST;
    token_map["frstor"] = INST;
    token_map["fsave"] = INST;
    token_map["fscale"] = INST;
    token_map["fsetpm"] = INST;
    token_map["fsin"] = INST;
    token_map["fsincos"] = INST;
    token_map["fsqrt"] = INST;
    token_map["fst"] = INST;
    token_map["fstcw"] = INST;
    token_map["fstenv"] = INST;
    token_map["fstp"] = INST;
    token_map["fstsw"] = INST;
    token_map["fsub"] = INST;
    token_map["fsubp"] = INST;
    token_map["fsubr"] = INST;
    token_map["fsubrp"] = INST;
    token_map["ftst"] = INST;
    token_map["fucom"] = INST;
    token_map["fucomi"] = INST;
    token_map["fucomip"] = INST;
    token_map["fucomp"] = INST;
    token_map["fucompp"] = INST;
    token_map["fwait"] = INST;
    token_map["fxam"] = INST;
    token_map["fxch"] = INST;
    token_map["fxrstor64"] = INST;
    token_map["fxsave"] = INST;
    token_map["fxsave64"] = INST;
    token_map["fxtract"] = INST;
    token_map["fyl2x"] = INST;
    token_map["fyl2xp1"] = INST;
    token_map["global"] = INST;
    token_map["haddpd"] = INST;
    token_map["haddps"] = INST;
    token_map["hint_nop0"] = INST;
    token_map["hint_nop1"] = INST;
    token_map["hint_nop10"] = INST;
    token_map["hint_nop11"] = INST;
    token_map["hint_nop12"] = INST;
    token_map["hint_nop13"] = INST;
    token_map["hint_nop14"] = INST;
    token_map["hint_nop15"] = INST;
    token_map["hint_nop16"] = INST;
    token_map["hint_nop17"] = INST;
    token_map["hint_nop18"] = INST;
    token_map["hint_nop19"] = INST;
    token_map["hint_nop2"] = INST;
    token_map["hint_nop20"] = INST;
    token_map["hint_nop21"] = INST;
    token_map["hint_nop22"] = INST;
    token_map["hint_nop23"] = INST;
    token_map["hint_nop24"] = INST;
    token_map["hint_nop25"] = INST;
    token_map["hint_nop26"] = INST;
    token_map["hint_nop27"] = INST;
    token_map["hint_nop28"] = INST;
    token_map["hint_nop29"] = INST;
    token_map["hint_nop3"] = INST;
    token_map["hint_nop30"] = INST;
    token_map["hint_nop31"] = INST;
    token_map["hint_nop32"] = INST;
    token_map["hint_nop33"] = INST;
    token_map["hint_nop34"] = INST;
    token_map["hint_nop35"] = INST;
    token_map["hint_nop36"] = INST;
    token_map["hint_nop37"] = INST;
    token_map["hint_nop38"] = INST;
    token_map["hint_nop39"] = INST;
    token_map["hint_nop4"] = INST;
    token_map["hint_nop40"] = INST;
    token_map["hint_nop41"] = INST;
    token_map["hint_nop42"] = INST;
    token_map["hint_nop43"] = INST;
    token_map["hint_nop44"] = INST;
    token_map["hint_nop45"] = INST;
    token_map["hint_nop46"] = INST;
    token_map["hint_nop47"] = INST;
    token_map["hint_nop48"] = INST;
    token_map["hint_nop49"] = INST;
    token_map["hint_nop5"] = INST;
    token_map["hint_nop50"] = INST;
    token_map["hint_nop51"] = INST;
    token_map["hint_nop52"] = INST;
    token_map["hint_nop53"] = INST;
    token_map["hint_nop54"] = INST;
    token_map["hint_nop55"] = INST;
    token_map["hint_nop56"] = INST;
    token_map["hint_nop57"] = INST;
    token_map["hint_nop58"] = INST;
    token_map["hint_nop59"] = INST;
    token_map["hint_nop6"] = INST;
    token_map["hint_nop60"] = INST;
    token_map["hint_nop61"] = INST;
    token_map["hint_nop62"] = INST;
    token_map["hint_nop63"] = INST;
    token_map["hint_nop7"] = INST;
    token_map["hint_nop8"] = INST;
    token_map["hint_nop9"] = INST;
    token_map["hlt"] = INST;
    token_map["hsubpd"] = INST;
    token_map["hsubps"] = INST;
    token_map["ibts"] = INST;
    token_map["icebp"] = INST;
    token_map["idiv"] = INST;
    token_map["imul"] = INST;
    token_map["in"] = INST;
    token_map["inc"] = INST;
    token_map["incbin"] = INST;
    token_map["insb"] = INST;
    token_map["insd"] = INST;
    token_map["insertps"] = INST;
    token_map["insertq"] = INST;
    token_map["insw"] = INST;
    token_map["int"] = INST;
    token_map["int01"] = INST;
    token_map["int03"] = INST;
    token_map["int1"] = INST;
    token_map["int3"] = INST;
    token_map["into"] = INST;
    token_map["invd"] = INST;
    token_map["invept"] = INST;
    token_map["invlpg"] = INST;
    token_map["invlpga"] = INST;
    token_map["invpcid"] = INST;
    token_map["invvpid"] = INST;
    token_map["iret"] = INST;
    token_map["iretd"] = INST;
    token_map["iretq"] = INST;
    token_map["iretw"] = INST;
    token_map["ja"] = INST;
    token_map["jae"] = INST;
    token_map["jb"] = INST;
    token_map["jbe"] = INST;
    token_map["jc"] = INST;
    token_map["jcxz"] = INST;
    token_map["je"] = INST;
    token_map["jecxz"] = INST;
    token_map["jg"] = INST;
    token_map["jge"] = INST;
    token_map["jl"] = INST;
    token_map["jle"] = INST;
    token_map["jmp"] = INST;
    token_map["jmpe"] = INST;
    token_map["jna"] = INST;
    token_map["jnae"] = INST;
    token_map["jnb"] = INST;
    token_map["jnbe"] = INST;
    token_map["jnc"] = INST;
    token_map["jne"] = INST;
    token_map["jng"] = INST;
    token_map["jnge"] = INST;
    token_map["jnl"] = INST;
    token_map["jnle"] = INST;
    token_map["jno"] = INST;
    token_map["jnp"] = INST;
    token_map["jns"] = INST;
    token_map["jnz"] = INST;
    token_map["jo"] = INST;
    token_map["jp"] = INST;
    token_map["jpe"] = INST;
    token_map["jpo"] = INST;
    token_map["jrcxz"] = INST;
    token_map["js"] = INST;
    token_map["jz"] = INST;
    token_map["lahf"] = INST;
    token_map["lar"] = INST;
    token_map["lddqu"] = INST;
    token_map["ldmxcsr"] = INST;
    token_map["lds"] = INST;
    token_map["lea"] = INST;
    token_map["leave"] = INST;
    token_map["les"] = INST;
    token_map["lfence"] = INST;
    token_map["lfs"] = INST;
    token_map["lgdt"] = INST;
    token_map["lgs"] = INST;
    token_map["lidt"] = INST;
    token_map["lldt"] = INST;
    token_map["llwpcb"] = INST;
    token_map["lmsw"] = INST;
    token_map["loadall"] = INST;
    token_map["loadall286"] = INST;
    token_map["lodsb"] = INST;
    token_map["lodsd"] = INST;
    token_map["lodsq"] = INST;
    token_map["lodsw"] = INST;
    token_map["loop"] = INST;
    token_map["loope"] = INST;
    token_map["loopne"] = INST;
    token_map["loopnz"] = INST;
    token_map["loopz"] = INST;
    token_map["lsl"] = INST;
    token_map["lss"] = INST;
    token_map["ltr"] = INST;
    token_map["lwpins"] = INST;
    token_map["lwpval"] = INST;
    token_map["lzcnt"] = INST;
    token_map["maxpd"] = INST;
    token_map["maxps"] = INST;
    token_map["maxsd"] = INST;
    token_map["maxss"] = INST;
    token_map["mfence"] = INST;
    token_map["minpd"] = INST;
    token_map["minps"] = INST;
    token_map["minsd"] = INST;
    token_map["minss"] = INST;
    token_map["monitor"] = INST;
    token_map["montmul"] = INST;
    token_map["mov"] = INST;
    token_map["movapd"] = INST;
    token_map["movaps"] = INST;
    token_map["movbe"] = INST;
    token_map["movd"] = INST;
    token_map["movddup"] = INST;
    token_map["movdq2q"] = INST;
    token_map["movdqa"] = INST;
    token_map["movdqu"] = INST;
    token_map["movhlps"] = INST;
    token_map["movhpd"] = INST;
    token_map["movhps"] = INST;
    token_map["movlhps"] = INST;
    token_map["movlpd"] = INST;
    token_map["movlps"] = INST;
    token_map["movmskpd"] = INST;
    token_map["movmskps"] = INST;
    token_map["movntdq"] = INST;
    token_map["movntdqa"] = INST;
    token_map["movnti"] = INST;
    token_map["movntpd"] = INST;
    token_map["movntps"] = INST;
    token_map["movntq"] = INST;
    token_map["movntsd"] = INST;
    token_map["movntss"] = INST;
    token_map["movq"] = INST;
    token_map["movq2dq"] = INST;
    token_map["movsb"] = INST;
    token_map["movsd"] = INST;
    token_map["movshdup"] = INST;
    token_map["movsldup"] = INST;
    token_map["movsq"] = INST;
    token_map["movss"] = INST;
    token_map["movsw"] = INST;
    token_map["movsx"] = INST;
    token_map["movsxd"] = INST;
    token_map["movupd"] = INST;
    token_map["movups"] = INST;
    token_map["movzx"] = INST;
    token_map["mpsadbw"] = INST;
    token_map["mul"] = INST;
    token_map["mulpd"] = INST;
    token_map["mulps"] = INST;
    token_map["mulsd"] = INST;
    token_map["mulss"] = INST;
    token_map["mulx"] = INST;
    token_map["mwait"] = INST;
    token_map["neg"] = INST;
    token_map["nop"] = INST;
    token_map["not"] = INST;
    token_map["or"] = INST;
    token_map["orpd"] = INST;
    token_map["orps"] = INST;
    token_map["out"] = INST;
    token_map["outsb"] = INST;
    token_map["outsd"] = INST;
    token_map["outsw"] = INST;
    token_map["pabsb"] = INST;
    token_map["pabsd"] = INST;
    token_map["pabsw"] = INST;
    token_map["packssdw"] = INST;
    token_map["packsswb"] = INST;
    token_map["packusdw"] = INST;
    token_map["packuswb"] = INST;
    token_map["paddb"] = INST;
    token_map["paddd"] = INST;
    token_map["paddq"] = INST;
    token_map["paddsb"] = INST;
    token_map["paddsiw"] = INST;
    token_map["paddsw"] = INST;
    token_map["paddusb"] = INST;
    token_map["paddusw"] = INST;
    token_map["paddw"] = INST;
    token_map["palignr"] = INST;
    token_map["pand"] = INST;
    token_map["pandn"] = INST;
    token_map["pause"] = INST;
    token_map["paveb"] = INST;
    token_map["pavgb"] = INST;
    token_map["pavgusb"] = INST;
    token_map["pavgw"] = INST;
    token_map["pblendvb"] = INST;
    token_map["pblendw"] = INST;
    token_map["pclmulhqhqdq"] = INST;
    token_map["pclmulhqlqdq"] = INST;
    token_map["pclmullqhqdq"] = INST;
    token_map["pclmulqdq"] = INST;
    token_map["pcmpeqb"] = INST;
    token_map["pcmpeqd"] = INST;
    token_map["pcmpeqq"] = INST;
    token_map["pcmpeqw"] = INST;
    token_map["pcmpestri"] = INST;
    token_map["pcmpestrm"] = INST;
    token_map["pcmpgtb"] = INST;
    token_map["pcmpgtd"] = INST;
    token_map["pcmpgtq"] = INST;
    token_map["pcmpgtw"] = INST;
    token_map["pcmpistri"] = INST;
    token_map["pcmpistrm"] = INST;
    token_map["pdep"] = INST;
    token_map["pdistib"] = INST;
    token_map["pext"] = INST;
    token_map["pextrb"] = INST;
    token_map["pextrd"] = INST;
    token_map["pextrq"] = INST;
    token_map["pextrw"] = INST;
    token_map["pf2id"] = INST;
    token_map["pfacc"] = INST;
    token_map["pfadd"] = INST;
    token_map["pfcmpeq"] = INST;
    token_map["pfcmpge"] = INST;
    token_map["pfcmpgt"] = INST;
    token_map["pfmax"] = INST;
    token_map["pfmin"] = INST;
    token_map["pfmul"] = INST;
    token_map["pfnacc"] = INST;
    token_map["pfpnacc"] = INST;
    token_map["pfrcp"] = INST;
    token_map["pfrcpit1"] = INST;
    token_map["pfrcpit2"] = INST;
    token_map["pfrsqit1"] = INST;
    token_map["pfrsqrt"] = INST;
    token_map["pfrsqrtv"] = INST;
    token_map["pfsub"] = INST;
    token_map["pfsubr"] = INST;
    token_map["phaddd"] = INST;
    token_map["phaddsw"] = INST;
    token_map["phaddw"] = INST;
    token_map["phminposuw"] = INST;
    token_map["phsubd"] = INST;
    token_map["phsubsw"] = INST;
    token_map["phsubw"] = INST;
    token_map["pi2fd"] = INST;
    token_map["pi2fw"] = INST;
    token_map["pinsrb"] = INST;
    token_map["pinsrd"] = INST;
    token_map["pinsrq"] = INST;
    token_map["pinsrw"] = INST;
    token_map["pmachriw"] = INST;
    token_map["pmaddubsw"] = INST;
    token_map["pmaddwd"] = INST;
    token_map["pmagw"] = INST;
    token_map["pmaxsb"] = INST;
    token_map["pmaxsd"] = INST;
    token_map["pmaxsw"] = INST;
    token_map["pmaxub"] = INST;
    token_map["pmaxud"] = INST;
    token_map["pmaxuw"] = INST;
    token_map["pminsb"] = INST;
    token_map["pminsd"] = INST;
    token_map["pminsw"] = INST;
    token_map["pminub"] = INST;
    token_map["pminud"] = INST;
    token_map["pminuw"] = INST;
    token_map["pmovmskb"] = INST;
    token_map["pmovsxbd"] = INST;
    token_map["pmovsxbq"] = INST;
    token_map["pmovsxbw"] = INST;
    token_map["pmovsxdq"] = INST;
    token_map["pmovsxwd"] = INST;
    token_map["pmovsxwq"] = INST;
    token_map["pmovzxbd"] = INST;
    token_map["pmovzxbq"] = INST;
    token_map["pmovzxbw"] = INST;
    token_map["pmovzxdq"] = INST;
    token_map["pmovzxwd"] = INST;
    token_map["pmovzxwq"] = INST;
    token_map["pmuldq"] = INST;
    token_map["pmulhriw"] = INST;
    token_map["pmulhrsw"] = INST;
    token_map["pmulhrwa"] = INST;
    token_map["pmulhrwc"] = INST;
    token_map["pmulhuw"] = INST;
    token_map["pmulhw"] = INST;
    token_map["pmulld"] = INST;
    token_map["pmullw"] = INST;
    token_map["pmuludq"] = INST;
    token_map["pmvgezb"] = INST;
    token_map["pmvlzb"] = INST;
    token_map["pmvnzb"] = INST;
    token_map["pmvzb"] = INST;
    token_map["pop"] = INST;
    token_map["popa"] = INST;
    token_map["popad"] = INST;
    token_map["popaw"] = INST;
    token_map["popcnt"] = INST;
    token_map["popf"] = INST;
    token_map["popfd"] = INST;
    token_map["popfq"] = INST;
    token_map["popfw"] = INST;
    token_map["por"] = INST;
    token_map["prefetch"] = INST;
    token_map["prefetcht0"] = INST;
    token_map["prefetcht1"] = INST;
    token_map["prefetcht2"] = INST;
    token_map["prefetchw"] = INST;
    token_map["psadbw"] = INST;
    token_map["pshufb"] = INST;
    token_map["pshufd"] = INST;
    token_map["pshufhw"] = INST;
    token_map["pshuflw"] = INST;
    token_map["pshufw"] = INST;
    token_map["psignb"] = INST;
    token_map["psignd"] = INST;
    token_map["psignw"] = INST;
    token_map["pslld"] = INST;
    token_map["pslldq"] = INST;
    token_map["psllq"] = INST;
    token_map["psllw"] = INST;
    token_map["psrad"] = INST;
    token_map["psraw"] = INST;
    token_map["psrld"] = INST;
    token_map["psrldq"] = INST;
    token_map["psrlq"] = INST;
    token_map["psrlw"] = INST;
    token_map["psubb"] = INST;
    token_map["psubd"] = INST;
    token_map["psubq"] = INST;
    token_map["psubsb"] = INST;
    token_map["psubsiw"] = INST;
    token_map["psubsw"] = INST;
    token_map["psubusb"] = INST;
    token_map["psubusw"] = INST;
    token_map["psubw"] = INST;
    token_map["pswapd"] = INST;
    token_map["ptest"] = INST;
    token_map["punpckhbw"] = INST;
    token_map["punpckhdq"] = INST;
    token_map["punpckhqdq"] = INST;
    token_map["punpckhwd"] = INST;
    token_map["punpcklbw"] = INST;
    token_map["punpckldq"] = INST;
    token_map["punpcklqdq"] = INST;
    token_map["punpcklwd"] = INST;
    token_map["push"] = INST;
    token_map["pusha"] = INST;
    token_map["pushad"] = INST;
    token_map["pushaw"] = INST;
    token_map["pushf"] = INST;
    token_map["pushfd"] = INST;
    token_map["pushfq"] = INST;
    token_map["pushfw"] = INST;
    token_map["pxor"] = INST;
    token_map["rcl"] = INST;
    token_map["rcpps"] = INST;
    token_map["rcpss"] = INST;
    token_map["rcr"] = INST;
    token_map["rdfsbase"] = INST;
    token_map["rdgsbase"] = INST;
    token_map["rdm"] = INST;
    token_map["rdmsr"] = INST;
    token_map["rdpmc"] = INST;
    token_map["rdrand"] = INST;
    token_map["rdseed"] = INST;
    token_map["rdshr"] = INST;
    token_map["rdtsc"] = INST;
    token_map["rdtscp"] = INST;
    token_map["resb"] = INST;
    token_map["resd"] = INST;
    token_map["resdq"] = INST;
    token_map["reso"] = INST;
    token_map["resq"] = INST;
    token_map["rest"] = INST;
    token_map["resw"] = INST;
    token_map["resy"] = INST;
    token_map["ret"] = INST;
    token_map["retf"] = INST;
    token_map["retn"] = INST;
    token_map["rol"] = INST;
    token_map["ror"] = INST;
    token_map["rorx"] = INST;
    token_map["roundpd"] = INST;
    token_map["roundps"] = INST;
    token_map["roundsd"] = INST;
    token_map["roundss"] = INST;
    token_map["rsdc"] = INST;
    token_map["rsldt"] = INST;
    token_map["rsm"] = INST;
    token_map["rsqrtps"] = INST;
    token_map["rsqrtss"] = INST;
    token_map["rsts"] = INST;
    token_map["sahf"] = INST;
    token_map["sal"] = INST;
    token_map["salc"] = INST;
    token_map["sar"] = INST;
    token_map["sarx"] = INST;
    token_map["sbb"] = INST;
    token_map["scasb"] = INST;
    token_map["scasd"] = INST;
    token_map["scasq"] = INST;
    token_map["scasw"] = INST;
    token_map["seta"] = INST;
    token_map["setae"] = INST;
    token_map["setb"] = INST;
    token_map["setbe"] = INST;
    token_map["setc"] = INST;
    token_map["sete"] = INST;
    token_map["setg"] = INST;
    token_map["setge"] = INST;
    token_map["setl"] = INST;
    token_map["setle"] = INST;
    token_map["setna"] = INST;
    token_map["setnae"] = INST;
    token_map["setnb"] = INST;
    token_map["setnbe"] = INST;
    token_map["setnc"] = INST;
    token_map["setne"] = INST;
    token_map["setng"] = INST;
    token_map["setnge"] = INST;
    token_map["setnl"] = INST;
    token_map["setnle"] = INST;
    token_map["setno"] = INST;
    token_map["setnp"] = INST;
    token_map["setns"] = INST;
    token_map["setnz"] = INST;
    token_map["seto"] = INST;
    token_map["setp"] = INST;
    token_map["setpe"] = INST;
    token_map["setpo"] = INST;
    token_map["sets"] = INST;
    token_map["setz"] = INST;
    token_map["sfence"] = INST;
    token_map["sgdt"] = INST;
    token_map["shl"] = INST;
    token_map["shld"] = INST;
    token_map["shlx"] = INST;
    token_map["shr"] = INST;
    token_map["shrd"] = INST;
    token_map["shrx"] = INST;
    token_map["shufpd"] = INST;
    token_map["shufps"] = INST;
    token_map["sidt"] = INST;
    token_map["skinit"] = INST;
    token_map["sldt"] = INST;
    token_map["slwpcb"] = INST;
    token_map["smi"] = INST;
    token_map["smint"] = INST;
    token_map["smintold"] = INST;
    token_map["smsw"] = INST;
    token_map["sqrtpd"] = INST;
    token_map["sqrtps"] = INST;
    token_map["sqrtsd"] = INST;
    token_map["sqrtss"] = INST;
    token_map["stac"] = INST;
    token_map["stc"] = INST;
    token_map["std"] = INST;
    token_map["stgi"] = INST;
    token_map["sti"] = INST;
    token_map["stmxcsr"] = INST;
    token_map["stosb"] = INST;
    token_map["stosd"] = INST;
    token_map["stosq"] = INST;
    token_map["stosw"] = INST;
    token_map["str"] = INST;
    token_map["sub"] = INST;
    token_map["subpd"] = INST;
    token_map["subps"] = INST;
    token_map["subsd"] = INST;
    token_map["subss"] = INST;
    token_map["svdc"] = INST;
    token_map["svldt"] = INST;
    token_map["svts"] = INST;
    token_map["swapgs"] = INST;
    token_map["syscall"] = INST;
    token_map["sysenter"] = INST;
    token_map["sysexit"] = INST;
    token_map["sysret"] = INST;
    token_map["test"] = INST;
    token_map["times"] = INST;
    token_map["tzcnt"] = INST;
    token_map["ucomisd"] = INST;
    token_map["ucomiss"] = INST;
    token_map["ud0"] = INST;
    token_map["ud1"] = INST;
    token_map["ud2"] = INST;
    token_map["ud2a"] = INST;
    token_map["ud2b"] = INST;
    token_map["umov"] = INST;
    token_map["unpckhpd"] = INST;
    token_map["unpckhps"] = INST;
    token_map["unpcklpd"] = INST;
    token_map["unpcklps"] = INST;
    token_map["vaddpd"] = INST;
    token_map["vaddps"] = INST;
    token_map["vaddsd"] = INST;
    token_map["vaddss"] = INST;
    token_map["vaddsubpd"] = INST;
    token_map["vaddsubps"] = INST;
    token_map["vaesdec"] = INST;
    token_map["vaesdeclast"] = INST;
    token_map["vaesenclast"] = INST;
    token_map["vaesimc"] = INST;
    token_map["vaeskeygenassist"] = INST;
    token_map["vandnpd"] = INST;
    token_map["vandnps"] = INST;
    token_map["vandpd"] = INST;
    token_map["vandps"] = INST;
    token_map["vblendpd"] = INST;
    token_map["vblendps"] = INST;
    token_map["vblendvpd"] = INST;
    token_map["vblendvps"] = INST;
    token_map["vbroadcastf128"] = INST;
    token_map["vbroadcasti128"] = INST;
    token_map["vbroadcastsd"] = INST;
    token_map["vbroadcastss"] = INST;
    token_map["vcmpeq_ospd"] = INST;
    token_map["vcmpeq_osps"] = INST;
    token_map["vcmpeq_ossd"] = INST;
    token_map["vcmpeq_osss"] = INST;
    token_map["vcmpeqpd"] = INST;
    token_map["vcmpeqps"] = INST;
    token_map["vcmpeqsd"] = INST;
    token_map["vcmpeqss"] = INST;
    token_map["vcmpeq_uqpd"] = INST;
    token_map["vcmpeq_uqps"] = INST;
    token_map["vcmpeq_uqsd"] = INST;
    token_map["vcmpeq_uqss"] = INST;
    token_map["vcmpeq_uspd"] = INST;
    token_map["vcmpeq_usps"] = INST;
    token_map["vcmpeq_ussd"] = INST;
    token_map["vcmpeq_usss"] = INST;
    token_map["vcmpfalse_oqpd"] = INST;
    token_map["vcmpfalse_oqps"] = INST;
    token_map["vcmpfalse_oqsd"] = INST;
    token_map["vcmpfalse_oqss"] = INST;
    token_map["vcmpfalse_ospd"] = INST;
    token_map["vcmpfalse_osps"] = INST;
    token_map["vcmpfalse_ossd"] = INST;
    token_map["vcmpfalse_osss"] = INST;
    token_map["vcmpfalsepd"] = INST;
    token_map["vcmpfalseps"] = INST;
    token_map["vcmpfalsesd"] = INST;
    token_map["vcmpfalsess"] = INST;
    token_map["vcmpge_oqpd"] = INST;
    token_map["vcmpge_oqps"] = INST;
    token_map["vcmpge_oqsd"] = INST;
    token_map["vcmpge_oqss"] = INST;
    token_map["vcmpge_ospd"] = INST;
    token_map["vcmpge_osps"] = INST;
    token_map["vcmpge_ossd"] = INST;
    token_map["vcmpge_osss"] = INST;
    token_map["vcmpgepd"] = INST;
    token_map["vcmpgeps"] = INST;
    token_map["vcmpgesd"] = INST;
    token_map["vcmpgess"] = INST;
    token_map["vcmpgt_oqpd"] = INST;
    token_map["vcmpgt_oqps"] = INST;
    token_map["vcmpgt_oqsd"] = INST;
    token_map["vcmpgt_oqss"] = INST;
    token_map["vcmpgt_ospd"] = INST;
    token_map["vcmpgt_osps"] = INST;
    token_map["vcmpgt_ossd"] = INST;
    token_map["vcmpgt_osss"] = INST;
    token_map["vcmpgtpd"] = INST;
    token_map["vcmpgtps"] = INST;
    token_map["vcmpgtsd"] = INST;
    token_map["vcmpgtss"] = INST;
    token_map["vcmple_oqpd"] = INST;
    token_map["vcmple_oqps"] = INST;
    token_map["vcmple_oqsd"] = INST;
    token_map["vcmple_oqss"] = INST;
    token_map["vcmple_ospd"] = INST;
    token_map["vcmple_osps"] = INST;
    token_map["vcmple_ossd"] = INST;
    token_map["vcmple_osss"] = INST;
    token_map["vcmplepd"] = INST;
    token_map["vcmpleps"] = INST;
    token_map["vcmplesd"] = INST;
    token_map["vcmpless"] = INST;
    token_map["vcmplt_oqpd"] = INST;
    token_map["vcmplt_oqps"] = INST;
    token_map["vcmplt_oqsd"] = INST;
    token_map["vcmplt_oqss"] = INST;
    token_map["vcmplt_ospd"] = INST;
    token_map["vcmplt_osps"] = INST;
    token_map["vcmplt_ossd"] = INST;
    token_map["vcmplt_osss"] = INST;
    token_map["vcmpltpd"] = INST;
    token_map["vcmpltps"] = INST;
    token_map["vcmpltsd"] = INST;
    token_map["vcmpltss"] = INST;
    token_map["vcmpneq_oqpd"] = INST;
    token_map["vcmpneq_oqps"] = INST;
    token_map["vcmpneq_oqsd"] = INST;
    token_map["vcmpneq_oqss"] = INST;
    token_map["vcmpneq_ospd"] = INST;
    token_map["vcmpneq_osps"] = INST;
    token_map["vcmpneq_ossd"] = INST;
    token_map["vcmpneq_osss"] = INST;
    token_map["vcmpneqpd"] = INST;
    token_map["vcmpneqps"] = INST;
    token_map["vcmpneqsd"] = INST;
    token_map["vcmpneqss"] = INST;
    token_map["vcmpneq_uqpd"] = INST;
    token_map["vcmpneq_uqps"] = INST;
    token_map["vcmpneq_uqsd"] = INST;
    token_map["vcmpneq_uqss"] = INST;
    token_map["vcmpneq_uspd"] = INST;
    token_map["vcmpneq_usps"] = INST;
    token_map["vcmpneq_ussd"] = INST;
    token_map["vcmpneq_usss"] = INST;
    token_map["vcmpngepd"] = INST;
    token_map["vcmpngeps"] = INST;
    token_map["vcmpngesd"] = INST;
    token_map["vcmpngess"] = INST;
    token_map["vcmpnge_uqpd"] = INST;
    token_map["vcmpnge_uqps"] = INST;
    token_map["vcmpnge_uqsd"] = INST;
    token_map["vcmpnge_uqss"] = INST;
    token_map["vcmpnge_uspd"] = INST;
    token_map["vcmpnge_usps"] = INST;
    token_map["vcmpnge_ussd"] = INST;
    token_map["vcmpnge_usss"] = INST;
    token_map["vcmpngtpd"] = INST;
    token_map["vcmpngtps"] = INST;
    token_map["vcmpngtsd"] = INST;
    token_map["vcmpngtss"] = INST;
    token_map["vcmpngt_uqpd"] = INST;
    token_map["vcmpngt_uqps"] = INST;
    token_map["vcmpngt_uqsd"] = INST;
    token_map["vcmpngt_uqss"] = INST;
    token_map["vcmpngt_uspd"] = INST;
    token_map["vcmpngt_usps"] = INST;
    token_map["vcmpngt_ussd"] = INST;
    token_map["vcmpngt_usss"] = INST;
    token_map["vcmpnlepd"] = INST;
    token_map["vcmpnleps"] = INST;
    token_map["vcmpnlesd"] = INST;
    token_map["vcmpnless"] = INST;
    token_map["vcmpnle_uqpd"] = INST;
    token_map["vcmpnle_uqps"] = INST;
    token_map["vcmpnle_uqsd"] = INST;
    token_map["vcmpnle_uqss"] = INST;
    token_map["vcmpnle_uspd"] = INST;
    token_map["vcmpnle_usps"] = INST;
    token_map["vcmpnle_ussd"] = INST;
    token_map["vcmpnle_usss"] = INST;
    token_map["vcmpnltpd"] = INST;
    token_map["vcmpnltps"] = INST;
    token_map["vcmpnltsd"] = INST;
    token_map["vcmpnltss"] = INST;
    token_map["vcmpnlt_uqpd"] = INST;
    token_map["vcmpnlt_uqps"] = INST;
    token_map["vcmpnlt_uqsd"] = INST;
    token_map["vcmpnlt_uqss"] = INST;
    token_map["vcmpnlt_uspd"] = INST;
    token_map["vcmpnlt_usps"] = INST;
    token_map["vcmpnlt_ussd"] = INST;
    token_map["vcmpnlt_usss"] = INST;
    token_map["vcmpordpd"] = INST;
    token_map["vcmpordps"] = INST;
    token_map["vcmpord_qpd"] = INST;
    token_map["vcmpord_qps"] = INST;
    token_map["vcmpord_qsd"] = INST;
    token_map["vcmpord_qss"] = INST;
    token_map["vcmpordsd"] = INST;
    token_map["vcmpord_spd"] = INST;
    token_map["vcmpord_sps"] = INST;
    token_map["vcmpordss"] = INST;
    token_map["vcmpord_ssd"] = INST;
    token_map["vcmpord_sss"] = INST;
    token_map["vcmppd"] = INST;
    token_map["vcmpps"] = INST;
    token_map["vcmpsd"] = INST;
    token_map["vcmpss"] = INST;
    token_map["vcmptruepd"] = INST;
    token_map["vcmptrueps"] = INST;
    token_map["vcmptruesd"] = INST;
    token_map["vcmptruess"] = INST;
    token_map["vcmptrue_uqpd"] = INST;
    token_map["vcmptrue_uqps"] = INST;
    token_map["vcmptrue_uqsd"] = INST;
    token_map["vcmptrue_uqss"] = INST;
    token_map["vcmptrue_uspd"] = INST;
    token_map["vcmptrue_usps"] = INST;
    token_map["vcmptrue_ussd"] = INST;
    token_map["vcmptrue_usss"] = INST;
    token_map["vcmpunordpd"] = INST;
    token_map["vcmpunordps"] = INST;
    token_map["vcmpunord_qpd"] = INST;
    token_map["vcmpunord_qps"] = INST;
    token_map["vcmpunord_qsd"] = INST;
    token_map["vcmpunord_qss"] = INST;
    token_map["vcmpunordsd"] = INST;
    token_map["vcmpunord_spd"] = INST;
    token_map["vcmpunord_sps"] = INST;
    token_map["vcmpunordss"] = INST;
    token_map["vcmpunord_ssd"] = INST;
    token_map["vcmpunord_sss"] = INST;
    token_map["vcomisd"] = INST;
    token_map["vcomiss"] = INST;
    token_map["vcvtdq2pd"] = INST;
    token_map["vcvtdq2ps"] = INST;
    token_map["vcvtpd2dq"] = INST;
    token_map["vcvtpd2ps"] = INST;
    token_map["vcvtph2ps"] = INST;
    token_map["vcvtps2dq"] = INST;
    token_map["vcvtps2pd"] = INST;
    token_map["vcvtps2ph"] = INST;
    token_map["vcvtsd2si"] = INST;
    token_map["vcvtsd2ss"] = INST;
    token_map["vcvtsi2sd"] = INST;
    token_map["vcvtsi2ss"] = INST;
    token_map["vcvtss2sd"] = INST;
    token_map["vcvtss2si"] = INST;
    token_map["vcvttpd2dq"] = INST;
    token_map["vcvttps2dq"] = INST;
    token_map["vcvttsd2si"] = INST;
    token_map["vcvttss2si"] = INST;
    token_map["vdivpd"] = INST;
    token_map["vdivps"] = INST;
    token_map["vdivsd"] = INST;
    token_map["vdivss"] = INST;
    token_map["vdppd"] = INST;
    token_map["vdpps"] = INST;
    token_map["verr"] = INST;
    token_map["verw"] = INST;
    token_map["vextractf128"] = INST;
    token_map["vextracti128"] = INST;
    token_map["vextractps"] = INST;
    token_map["vfmadd123pd"] = INST;
    token_map["vfmadd123ps"] = INST;
    token_map["vfmadd123sd"] = INST;
    token_map["vfmadd123ss"] = INST;
    token_map["vfmadd132pd"] = INST;
    token_map["vfmadd132ps"] = INST;
    token_map["vfmadd132sd"] = INST;
    token_map["vfmadd132ss"] = INST;
    token_map["vfmadd213pd"] = INST;
    token_map["vfmadd213ps"] = INST;
    token_map["vfmadd213sd"] = INST;
    token_map["vfmadd213ss"] = INST;
    token_map["vfmadd231pd"] = INST;
    token_map["vfmadd231ps"] = INST;
    token_map["vfmadd231sd"] = INST;
    token_map["vfmadd231ss"] = INST;
    token_map["vfmadd312pd"] = INST;
    token_map["vfmadd312ps"] = INST;
    token_map["vfmadd312sd"] = INST;
    token_map["vfmadd312ss"] = INST;
    token_map["vfmadd321pd"] = INST;
    token_map["vfmadd321ps"] = INST;
    token_map["vfmadd321sd"] = INST;
    token_map["vfmadd321ss"] = INST;
    token_map["vfmaddpd"] = INST;
    token_map["vfmaddps"] = INST;
    token_map["vfmaddsd"] = INST;
    token_map["vfmaddss"] = INST;
    token_map["vfmaddsub123pd"] = INST;
    token_map["vfmaddsub123ps"] = INST;
    token_map["vfmaddsub132pd"] = INST;
    token_map["vfmaddsub132ps"] = INST;
    token_map["vfmaddsub213pd"] = INST;
    token_map["vfmaddsub213ps"] = INST;
    token_map["vfmaddsub231pd"] = INST;
    token_map["vfmaddsub231ps"] = INST;
    token_map["vfmaddsub312pd"] = INST;
    token_map["vfmaddsub312ps"] = INST;
    token_map["vfmaddsub321pd"] = INST;
    token_map["vfmaddsub321ps"] = INST;
    token_map["vfmaddsubpd"] = INST;
    token_map["vfmaddsubps"] = INST;
    token_map["vfmsub123pd"] = INST;
    token_map["vfmsub123ps"] = INST;
    token_map["vfmsub123sd"] = INST;
    token_map["vfmsub123ss"] = INST;
    token_map["vfmsub132pd"] = INST;
    token_map["vfmsub132ps"] = INST;
    token_map["vfmsub132sd"] = INST;
    token_map["vfmsub132ss"] = INST;
    token_map["vfmsub213pd"] = INST;
    token_map["vfmsub213ps"] = INST;
    token_map["vfmsub213sd"] = INST;
    token_map["vfmsub213ss"] = INST;
    token_map["vfmsub231pd"] = INST;
    token_map["vfmsub231ps"] = INST;
    token_map["vfmsub231sd"] = INST;
    token_map["vfmsub231ss"] = INST;
    token_map["vfmsub312pd"] = INST;
    token_map["vfmsub312ps"] = INST;
    token_map["vfmsub312sd"] = INST;
    token_map["vfmsub312ss"] = INST;
    token_map["vfmsub321pd"] = INST;
    token_map["vfmsub321ps"] = INST;
    token_map["vfmsub321sd"] = INST;
    token_map["vfmsub321ss"] = INST;
    token_map["vfmsubadd123pd"] = INST;
    token_map["vfmsubadd123ps"] = INST;
    token_map["vfmsubadd132pd"] = INST;
    token_map["vfmsubadd132ps"] = INST;
    token_map["vfmsubadd213pd"] = INST;
    token_map["vfmsubadd213ps"] = INST;
    token_map["vfmsubadd231pd"] = INST;
    token_map["vfmsubadd231ps"] = INST;
    token_map["vfmsubadd312pd"] = INST;
    token_map["vfmsubadd312ps"] = INST;
    token_map["vfmsubadd321pd"] = INST;
    token_map["vfmsubadd321ps"] = INST;
    token_map["vfmsubaddpd"] = INST;
    token_map["vfmsubaddps"] = INST;
    token_map["vfmsubpd"] = INST;
    token_map["vfmsubps"] = INST;
    token_map["vfmsubsd"] = INST;
    token_map["vfmsubss"] = INST;
    token_map["vfnmadd123pd"] = INST;
    token_map["vfnmadd123ps"] = INST;
    token_map["vfnmadd123sd"] = INST;
    token_map["vfnmadd123ss"] = INST;
    token_map["vfnmadd132pd"] = INST;
    token_map["vfnmadd132ps"] = INST;
    token_map["vfnmadd132sd"] = INST;
    token_map["vfnmadd132ss"] = INST;
    token_map["vfnmadd213pd"] = INST;
    token_map["vfnmadd213ps"] = INST;
    token_map["vfnmadd213sd"] = INST;
    token_map["vfnmadd213ss"] = INST;
    token_map["vfnmadd231pd"] = INST;
    token_map["vfnmadd231ps"] = INST;
    token_map["vfnmadd231sd"] = INST;
    token_map["vfnmadd231ss"] = INST;
    token_map["vfnmadd312pd"] = INST;
    token_map["vfnmadd312ps"] = INST;
    token_map["vfnmadd312sd"] = INST;
    token_map["vfnmadd312ss"] = INST;
    token_map["vfnmadd321pd"] = INST;
    token_map["vfnmadd321ps"] = INST;
    token_map["vfnmadd321sd"] = INST;
    token_map["vfnmadd321ss"] = INST;
    token_map["vfnmaddpd"] = INST;
    token_map["vfnmaddps"] = INST;
    token_map["vfnmaddsd"] = INST;
    token_map["vfnmaddss"] = INST;
    token_map["vfnmsub123pd"] = INST;
    token_map["vfnmsub123ps"] = INST;
    token_map["vfnmsub123sd"] = INST;
    token_map["vfnmsub123ss"] = INST;
    token_map["vfnmsub132pd"] = INST;
    token_map["vfnmsub132ps"] = INST;
    token_map["vfnmsub132sd"] = INST;
    token_map["vfnmsub132ss"] = INST;
    token_map["vfnmsub213pd"] = INST;
    token_map["vfnmsub213ps"] = INST;
    token_map["vfnmsub213sd"] = INST;
    token_map["vfnmsub213ss"] = INST;
    token_map["vfnmsub231pd"] = INST;
    token_map["vfnmsub231ps"] = INST;
    token_map["vfnmsub231sd"] = INST;
    token_map["vfnmsub231ss"] = INST;
    token_map["vfnmsub312pd"] = INST;
    token_map["vfnmsub312ps"] = INST;
    token_map["vfnmsub312sd"] = INST;
    token_map["vfnmsub312ss"] = INST;
    token_map["vfnmsub321pd"] = INST;
    token_map["vfnmsub321ps"] = INST;
    token_map["vfnmsub321sd"] = INST;
    token_map["vfnmsub321ss"] = INST;
    token_map["vfnmsubpd"] = INST;
    token_map["vfnmsubps"] = INST;
    token_map["vfnmsubsd"] = INST;
    token_map["vfnmsubss"] = INST;
    token_map["vfrczpd"] = INST;
    token_map["vfrczps"] = INST;
    token_map["vfrczsd"] = INST;
    token_map["vfrczss"] = INST;
    token_map["vgatherdpd"] = INST;
    token_map["vgatherdps"] = INST;
    token_map["vgatherqpd"] = INST;
    token_map["vgatherqps"] = INST;
    token_map["vhaddpd"] = INST;
    token_map["vhaddps"] = INST;
    token_map["vhsubpd"] = INST;
    token_map["vhsubps"] = INST;
    token_map["vinsertf128"] = INST;
    token_map["vinserti128"] = INST;
    token_map["vinsertps"] = INST;
    token_map["vlddqu"] = INST;
    token_map["vldmxcsr"] = INST;
    token_map["vldqqu"] = INST;
    token_map["vmaskmovdqu"] = INST;
    token_map["vmaskmovpd"] = INST;
    token_map["vmaskmovps"] = INST;
    token_map["vmaxpd"] = INST;
    token_map["vmaxps"] = INST;
    token_map["vmaxsd"] = INST;
    token_map["vmaxss"] = INST;
    token_map["vmclear"] = INST;
    token_map["vmfunc"] = INST;
    token_map["vminpd"] = INST;
    token_map["vminps"] = INST;
    token_map["vminsd"] = INST;
    token_map["vminss"] = INST;
    token_map["vmlaunch"] = INST;
    token_map["vmload"] = INST;
    token_map["vmmcall"] = INST;
    token_map["vmovapd"] = INST;
    token_map["vmovaps"] = INST;
    token_map["vmovd"] = INST;
    token_map["vmovddup"] = INST;
    token_map["vmovdqa"] = INST;
    token_map["vmovdqu"] = INST;
    token_map["vmovhlps"] = INST;
    token_map["vmovhpd"] = INST;
    token_map["vmovhps"] = INST;
    token_map["vmovlhps"] = INST;
    token_map["vmovlpd"] = INST;
    token_map["vmovlps"] = INST;
    token_map["vmovmskpd"] = INST;
    token_map["vmovmskps"] = INST;
    token_map["vmovntdq"] = INST;
    token_map["vmovntdqa"] = INST;
    token_map["vmovntpd"] = INST;
    token_map["vmovntps"] = INST;
    token_map["vmovntqq"] = INST;
    token_map["vmovq"] = INST;
    token_map["vmovqqa"] = INST;
    token_map["vmovqqu"] = INST;
    token_map["vmovsd"] = INST;
    token_map["vmovshdup"] = INST;
    token_map["vmovsldup"] = INST;
    token_map["vmovss"] = INST;
    token_map["vmovupd"] = INST;
    token_map["vmovups"] = INST;
    token_map["vmpsadbw"] = INST;
    token_map["vmptrld"] = INST;
    token_map["vmptrst"] = INST;
    token_map["vmread"] = INST;
    token_map["vmresume"] = INST;
    token_map["vmrun"] = INST;
    token_map["vmsave"] = INST;
    token_map["vmulpd"] = INST;
    token_map["vmulps"] = INST;
    token_map["vmulsd"] = INST;
    token_map["vmulss"] = INST;
    token_map["vmwrite"] = INST;
    token_map["vmxoff"] = INST;
    token_map["vmxon"] = INST;
    token_map["vorpd"] = INST;
    token_map["vorps"] = INST;
    token_map["vpabsb"] = INST;
    token_map["vpabsd"] = INST;
    token_map["vpabsw"] = INST;
    token_map["vpackssdw"] = INST;
    token_map["vpacksswb"] = INST;
    token_map["vpackusdw"] = INST;
    token_map["vpackuswb"] = INST;
    token_map["vpaddb"] = INST;
    token_map["vpaddd"] = INST;
    token_map["vpaddq"] = INST;
    token_map["vpaddsb"] = INST;
    token_map["vpaddsw"] = INST;
    token_map["vpaddusb"] = INST;
    token_map["vpaddusw"] = INST;
    token_map["vpaddw"] = INST;
    token_map["vpalignr"] = INST;
    token_map["vpand"] = INST;
    token_map["vpandn"] = INST;
    token_map["vpavgb"] = INST;
    token_map["vpavgw"] = INST;
    token_map["vpblendd"] = INST;
    token_map["vpblendvb"] = INST;
    token_map["vpblendw"] = INST;
    token_map["vpbroadcastb"] = INST;
    token_map["vpbroadcastd"] = INST;
    token_map["vpbroadcastq"] = INST;
    token_map["vpbroadcastw"] = INST;
    token_map["vpclmulhqhqdq"] = INST;
    token_map["vpclmulhqlqdq"] = INST;
    token_map["vpclmullqhqdq"] = INST;
    token_map["vpclmulqdq"] = INST;
    token_map["vpcmov"] = INST;
    token_map["vpcmpeqb"] = INST;
    token_map["vpcmpeqd"] = INST;
    token_map["vpcmpeqq"] = INST;
    token_map["vpcmpeqw"] = INST;
    token_map["vpcmpestri"] = INST;
    token_map["vpcmpestrm"] = INST;
    token_map["vpcmpgtb"] = INST;
    token_map["vpcmpgtd"] = INST;
    token_map["vpcmpgtq"] = INST;
    token_map["vpcmpgtw"] = INST;
    token_map["vpcmpistri"] = INST;
    token_map["vpcmpistrm"] = INST;
    token_map["vpcomb"] = INST;
    token_map["vpcomd"] = INST;
    token_map["vpcomq"] = INST;
    token_map["vpcomub"] = INST;
    token_map["vpcomud"] = INST;
    token_map["vpcomuq"] = INST;
    token_map["vpcomuw"] = INST;
    token_map["vpcomw"] = INST;
    token_map["vperm2f128"] = INST;
    token_map["vperm2i128"] = INST;
    token_map["vpermd"] = INST;
    token_map["vpermilpd"] = INST;
    token_map["vpermilps"] = INST;
    token_map["vpermpd"] = INST;
    token_map["vpermps"] = INST;
    token_map["vpermq"] = INST;
    token_map["vpextrb"] = INST;
    token_map["vpextrd"] = INST;
    token_map["vpextrq"] = INST;
    token_map["vpextrw"] = INST;
    token_map["vpgatherdd"] = INST;
    token_map["vpgatherdq"] = INST;
    token_map["vpgatherqd"] = INST;
    token_map["vpgatherqq"] = INST;
    token_map["vphaddbd"] = INST;
    token_map["vphaddbq"] = INST;
    token_map["vphaddbw"] = INST;
    token_map["vphaddd"] = INST;
    token_map["vphadddq"] = INST;
    token_map["vphaddsw"] = INST;
    token_map["vphaddubd"] = INST;
    token_map["vphaddubq"] = INST;
    token_map["vphaddubw"] = INST;
    token_map["vphaddudq"] = INST;
    token_map["vphadduwd"] = INST;
    token_map["vphadduwq"] = INST;
    token_map["vphaddw"] = INST;
    token_map["vphaddwd"] = INST;
    token_map["vphaddwq"] = INST;
    token_map["vphminposuw"] = INST;
    token_map["vphsubbw"] = INST;
    token_map["vphsubd"] = INST;
    token_map["vphsubdq"] = INST;
    token_map["vphsubsw"] = INST;
    token_map["vphsubw"] = INST;
    token_map["vphsubwd"] = INST;
    token_map["vpinsrb"] = INST;
    token_map["vpinsrd"] = INST;
    token_map["vpinsrq"] = INST;
    token_map["vpinsrw"] = INST;
    token_map["vpmacsdd"] = INST;
    token_map["vpmacsdqh"] = INST;
    token_map["vpmacsdql"] = INST;
    token_map["vpmacssdd"] = INST;
    token_map["vpmacssdqh"] = INST;
    token_map["vpmacssdql"] = INST;
    token_map["vpmacsswd"] = INST;
    token_map["vpmacssww"] = INST;
    token_map["vpmacswd"] = INST;
    token_map["vpmacsww"] = INST;
    token_map["vpmadcsswd"] = INST;
    token_map["vpmadcswd"] = INST;
    token_map["vpmaddubsw"] = INST;
    token_map["vpmaddwd"] = INST;
    token_map["vpmaskmovd"] = INST;
    token_map["vpmaskmovq"] = INST;
    token_map["vpmaxsb"] = INST;
    token_map["vpmaxsd"] = INST;
    token_map["vpmaxsw"] = INST;
    token_map["vpmaxub"] = INST;
    token_map["vpmaxud"] = INST;
    token_map["vpmaxuw"] = INST;
    token_map["vpminsb"] = INST;
    token_map["vpminsd"] = INST;
    token_map["vpminsw"] = INST;
    token_map["vpminub"] = INST;
    token_map["vpminud"] = INST;
    token_map["vpminuw"] = INST;
    token_map["vpmovmskb"] = INST;
    token_map["vpmovsxbd"] = INST;
    token_map["vpmovsxbq"] = INST;
    token_map["vpmovsxbw"] = INST;
    token_map["vpmovsxdq"] = INST;
    token_map["vpmovsxwd"] = INST;
    token_map["vpmovsxwq"] = INST;
    token_map["vpmovzxbd"] = INST;
    token_map["vpmovzxbq"] = INST;
    token_map["vpmovzxbw"] = INST;
    token_map["vpmovzxdq"] = INST;
    token_map["vpmovzxwd"] = INST;
    token_map["vpmovzxwq"] = INST;
    token_map["vpmuldq"] = INST;
    token_map["vpmulhrsw"] = INST;
    token_map["vpmulhuw"] = INST;
    token_map["vpmulhw"] = INST;
    token_map["vpmulld"] = INST;
    token_map["vpmullw"] = INST;
    token_map["vpmuludq"] = INST;
    token_map["vpor"] = INST;
    token_map["vpperm"] = INST;
    token_map["vprotb"] = INST;
    token_map["vprotd"] = INST;
    token_map["vprotq"] = INST;
    token_map["vprotw"] = INST;
    token_map["vpsadbw"] = INST;
    token_map["vpshab"] = INST;
    token_map["vpshad"] = INST;
    token_map["vpshaq"] = INST;
    token_map["vpshaw"] = INST;
    token_map["vpshlb"] = INST;
    token_map["vpshld"] = INST;
    token_map["vpshlq"] = INST;
    token_map["vpshlw"] = INST;
    token_map["vpshufb"] = INST;
    token_map["vpshufd"] = INST;
    token_map["vpshufhw"] = INST;
    token_map["vpshuflw"] = INST;
    token_map["vpsignb"] = INST;
    token_map["vpsignd"] = INST;
    token_map["vpsignw"] = INST;
    token_map["vpslld"] = INST;
    token_map["vpslldq"] = INST;
    token_map["vpsllq"] = INST;
    token_map["vpsllvd"] = INST;
    token_map["vpsllvq"] = INST;
    token_map["vpsllw"] = INST;
    token_map["vpsrad"] = INST;
    token_map["vpsravd"] = INST;
    token_map["vpsraw"] = INST;
    token_map["vpsrld"] = INST;
    token_map["vpsrldq"] = INST;
    token_map["vpsrlq"] = INST;
    token_map["vpsrlvd"] = INST;
    token_map["vpsrlvq"] = INST;
    token_map["vpsrlw"] = INST;
    token_map["vpsubb"] = INST;
    token_map["vpsubd"] = INST;
    token_map["vpsubq"] = INST;
    token_map["vpsubsb"] = INST;
    token_map["vpsubsw"] = INST;
    token_map["vpsubusb"] = INST;
    token_map["vpsubusw"] = INST;
    token_map["vpsubw"] = INST;
    token_map["vptest"] = INST;
    token_map["vpunpckhbw"] = INST;
    token_map["vpunpckhdq"] = INST;
    token_map["vpunpckhqdq"] = INST;
    token_map["vpunpckhwd"] = INST;
    token_map["vpunpcklbw"] = INST;
    token_map["vpunpckldq"] = INST;
    token_map["vpunpcklqdq"] = INST;
    token_map["vpunpcklwd"] = INST;
    token_map["vpxor"] = INST;
    token_map["vrcpps"] = INST;
    token_map["vrcpss"] = INST;
    token_map["vroundpd"] = INST;
    token_map["vroundps"] = INST;
    token_map["vroundsd"] = INST;
    token_map["vroundss"] = INST;
    token_map["vrsqrtps"] = INST;
    token_map["vrsqrtss"] = INST;
    token_map["vshufpd"] = INST;
    token_map["vshufps"] = INST;
    token_map["vsqrtpd"] = INST;
    token_map["vsqrtps"] = INST;
    token_map["vsqrtsd"] = INST;
    token_map["vsqrtss"] = INST;
    token_map["vstmxcsr"] = INST;
    token_map["vsubpd"] = INST;
    token_map["vsubps"] = INST;
    token_map["vsubsd"] = INST;
    token_map["vsubss"] = INST;
    token_map["vtestpd"] = INST;
    token_map["vtestps"] = INST;
    token_map["vucomisd"] = INST;
    token_map["vucomiss"] = INST;
    token_map["vunpckhpd"] = INST;
    token_map["vunpckhps"] = INST;
    token_map["vunpcklpd"] = INST;
    token_map["vunpcklps"] = INST;
    token_map["vxorpd"] = INST;
    token_map["vxorps"] = INST;
    token_map["vzeroall"] = INST;
    token_map["vzeroupper"] = INST;
    token_map["wbinvd"] = INST;
    token_map["wrfsbase"] = INST;
    token_map["wrgsbase"] = INST;
    token_map["wrmsr"] = INST;
    token_map["wrshr"] = INST;
    token_map["xabort"] = INST;
    token_map["xadd"] = INST;
    token_map["xbegin"] = INST;
    token_map["xbts"] = INST;
    token_map["xchg"] = INST;
    token_map["xcryptcbc"] = INST;
    token_map["xcryptcfb"] = INST;
    token_map["xcryptctr"] = INST;
    token_map["xcryptecb"] = INST;
    token_map["xcryptofb"] = INST;
    token_map["xend"] = INST;
    token_map["xlat"] = INST;
    token_map["xlatb"] = INST;
    token_map["xor"] = INST;
    token_map["xorpd"] = INST;
    token_map["xorps"] = INST;
    token_map["xrstor"] = INST;
    token_map["xrstor64"] = INST;
    token_map["xsave"] = INST;
    token_map["xsave64"] = INST;
    token_map["xsaveopt"] = INST;
    token_map["xsaveopt64"] = INST;
    token_map["xsetbv"] = INST;
    token_map["xsha1"] = INST;
    token_map["xsha256"] = INST;
    token_map["xtest"] = INST;

    token_map["rax"] = REG;
    token_map["rbx"] = REG;
    token_map["rcx"] = REG;
    token_map["rdx"] = REG;
    token_map["rsi"] = REG;
    token_map["rdi"] = REG;
    token_map["rbp"] = REG;
    token_map["rsp"] = REG;
    token_map["r8"] = REG;
    token_map["r9"] = REG;
    token_map["r10"] = REG;
    token_map["r11"] = REG;
    token_map["r12"] = REG;
    token_map["r13"] = REG;
    token_map["r14"] = REG;
    token_map["r15"] = REG;
    token_map["eax"] = REG;
    token_map["ebx"] = REG;
    token_map["ecx"] = REG;
    token_map["edx"] = REG;
    token_map["esi"] = REG;
    token_map["edi"] = REG;
    token_map["ebp"] = REG;
    token_map["esp"] = REG;
    token_map["r8d"] = REG;
    token_map["r9d"] = REG;
    token_map["r10d"] = REG;
    token_map["r11d"] = REG;
    token_map["r12d"] = REG;
    token_map["r13d"] = REG;
    token_map["r14d"] = REG;
    token_map["r15d"] = REG;
    token_map["ax"] = REG;
    token_map["bx"] = REG;
    token_map["cx"] = REG;
    token_map["dx"] = REG;
    token_map["si"] = REG;
    token_map["di"] = REG;
    token_map["bp"] = REG;
    token_map["sp"] = REG;
    token_map["al"] = REG;
    token_map["ah"] = REG;
    token_map["bl"] = REG;
    token_map["bh"] = REG;
    token_map["cl"] = REG;
    token_map["ch"] = REG;
    token_map["dl"] = REG;
    token_map["dh"] = REG;
    token_map["sil"] = REG;
    token_map["dil"] = REG;
    token_map["bpl"] = REG;
    token_map["spl"] = REG;
    token_map["r8b"] = REG;
    token_map["r9b"] = REG;
    token_map["r10b"] = REG;
    token_map["r11b"] = REG;
    token_map["r12b"] = REG;
    token_map["r13b"] = REG;
    token_map["r14b"] = REG;
    token_map["r15b"] = REG;
    token_map["xmm0"] = FPREG;
    token_map["xmm1"] = FPREG;
    token_map["xmm2"] = FPREG;
    token_map["xmm3"] = FPREG;
    token_map["xmm4"] = FPREG;
    token_map["xmm5"] = FPREG;
    token_map["xmm6"] = FPREG;
    token_map["xmm7"] = FPREG;
    token_map["xmm8"] = FPREG;
    token_map["xmm9"] = FPREG;
    token_map["xmm10"] = FPREG;
    token_map["xmm11"] = FPREG;
    token_map["xmm12"] = FPREG;
    token_map["xmm13"] = FPREG;
    token_map["xmm14"] = FPREG;
    token_map["xmm15"] = FPREG;
    token_map["ymm0"] = FPREG;
    token_map["ymm1"] = FPREG;
    token_map["ymm2"] = FPREG;
    token_map["ymm3"] = FPREG;
    token_map["ymm4"] = FPREG;
    token_map["ymm5"] = FPREG;
    token_map["ymm6"] = FPREG;
    token_map["ymm7"] = FPREG;
    token_map["ymm8"] = FPREG;
    token_map["ymm9"] = FPREG;
    token_map["ymm10"] = FPREG;
    token_map["ymm11"] = FPREG;
    token_map["ymm12"] = FPREG;
    token_map["ymm13"] = FPREG;
    token_map["ymm14"] = FPREG;
    token_map["ymm15"] = FPREG;
}
