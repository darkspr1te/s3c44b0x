/* A Bison parser, made from grammar.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse pcap_parse
#define yylex pcap_lex
#define yyerror pcap_error
#define yylval pcap_lval
#define yychar pcap_char
#define yydebug pcap_debug
#define yynerrs pcap_nerrs
# define	DST	257
# define	SRC	258
# define	HOST	259
# define	GATEWAY	260
# define	NET	261
# define	MASK	262
# define	PORT	263
# define	LESS	264
# define	GREATER	265
# define	PROTO	266
# define	BYTE	267
# define	ARP	268
# define	RARP	269
# define	IP	270
# define	TCP	271
# define	UDP	272
# define	ICMP	273
# define	IGMP	274
# define	IGRP	275
# define	ATALK	276
# define	DECNET	277
# define	LAT	278
# define	SCA	279
# define	MOPRC	280
# define	MOPDL	281
# define	TK_BROADCAST	282
# define	TK_MULTICAST	283
# define	NUM	284
# define	INBOUND	285
# define	OUTBOUND	286
# define	LINK	287
# define	GEQ	288
# define	LEQ	289
# define	NEQ	290
# define	ID	291
# define	EID	292
# define	HID	293
# define	LSH	294
# define	RSH	295
# define	LEN	296
# define	OR	297
# define	AND	298
# define	UMINUS	299

#line 1 "grammar.y"

/*
 * Copyright (c) 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#ifndef lint
static const char rcsid[] =
    "@(#) $Header: /cvs/sw/new-wave/lib/libpcap/grammar.y,v 1.2 2003/09/12 01:52:37 pdh Exp $ (LBL)";
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <linux/autoconf.h>

#if __STDC__
struct mbuf;
struct rtentry;
#endif

#include <net/if.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <stdio.h>

#include "pcap-int.h"

#include "gencode.h"
#include <pcap-namedb.h>

#include "gnuc.h"
#ifdef HAVE_OS_PROTO_H
#include "os-proto.h"
#endif

#define QSET(q, p, d, a) (q).proto = (p),\
			 (q).dir = (d),\
			 (q).addr = (a)

int n_errors = 0;

static struct qual qerr = { Q_UNDEF, Q_UNDEF, Q_UNDEF
#if !CONFIG_SH_SECUREEDGE5410
, Q_UNDEF 
#endif
};

static void
yyerror(char *msg)
{
	++n_errors;
	bpf_error("%s", msg);
	/* NOTREACHED */
}

#ifndef YYBISON
int yyparse(void);

int
pcap_parse()
{
	return (yyparse());
}
#endif


#line 87 "grammar.y"
#ifndef YYSTYPE
typedef union {
	int i;
	bpf_u_int32 h;
	u_char *e;
	char *s;
	struct stmt *stmt;
	struct arth *a;
	struct {
		struct qual q;
		struct block *b;
	} blk;
	struct block *rblk;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		151
#define	YYFLAG		-32768
#define	YYNTBASE	61

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 299 ? yytranslate[x] : 87)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    45,     2,     2,     2,     2,    47,     2,
      54,    53,    50,    48,     2,    49,     2,    51,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    60,     2,
      57,    56,    55,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    58,     2,    59,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    46,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    52
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    12,    16,    20,    24,
      26,    28,    30,    32,    36,    38,    42,    46,    48,    50,
      53,    55,    57,    59,    63,    67,    69,    71,    73,    76,
      80,    83,    86,    89,    92,    95,    99,   101,   105,   109,
     111,   113,   114,   116,   118,   122,   126,   130,   134,   136,
     138,   140,   142,   144,   146,   148,   150,   152,   154,   156,
     158,   160,   162,   164,   166,   168,   170,   172,   175,   178,
     181,   184,   189,   191,   193,   195,   197,   199,   201,   203,
     205,   207,   209,   214,   221,   225,   229,   233,   237,   241,
     245,   249,   253,   256,   260,   262,   264,   266,   268,   270,
     272,   274
};
static const short yyrhs[] =
{
      62,    63,     0,    62,     0,     0,    72,     0,    63,    64,
      72,     0,    63,    64,    66,     0,    63,    65,    72,     0,
      63,    65,    66,     0,    44,     0,    43,     0,    67,     0,
      86,     0,    69,    70,    53,     0,    37,     0,    39,    51,
      30,     0,    39,     8,    39,     0,    39,     0,    38,     0,
      68,    66,     0,    45,     0,    54,     0,    67,     0,    71,
      64,    66,     0,    71,    65,    66,     0,    86,     0,    70,
       0,    74,     0,    68,    72,     0,    75,    76,    77,     0,
      75,    76,     0,    75,    77,     0,    75,    12,     0,    75,
      78,     0,    73,    66,     0,    69,    63,    53,     0,    79,
       0,    83,    81,    83,     0,    83,    82,    83,     0,    80,
       0,    79,     0,     0,     4,     0,     3,     0,     4,    43,
       3,     0,     3,    43,     4,     0,     4,    44,     3,     0,
       3,    44,     4,     0,     5,     0,     7,     0,     9,     0,
       6,     0,    33,     0,    16,     0,    14,     0,    15,     0,
      17,     0,    18,     0,    19,     0,    20,     0,    21,     0,
      22,     0,    23,     0,    24,     0,    25,     0,    27,     0,
      26,     0,    75,    28,     0,    75,    29,     0,    10,    30,
       0,    11,    30,     0,    13,    30,    85,    30,     0,    31,
       0,    32,     0,    55,     0,    34,     0,    56,     0,    35,
       0,    57,     0,    36,     0,    86,     0,    84,     0,    79,
      58,    83,    59,     0,    79,    58,    83,    60,    30,    59,
       0,    83,    48,    83,     0,    83,    49,    83,     0,    83,
      50,    83,     0,    83,    51,    83,     0,    83,    47,    83,
       0,    83,    46,    83,     0,    83,    40,    83,     0,    83,
      41,    83,     0,    49,    83,     0,    69,    84,    53,     0,
      42,     0,    47,     0,    46,     0,    57,     0,    55,     0,
      56,     0,    30,     0,    69,    86,    53,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   135,   139,   141,   143,   144,   145,   146,   147,   149,
     151,   153,   154,   156,   158,   159,   161,   163,   175,   176,
     178,   180,   182,   183,   184,   186,   188,   190,   191,   193,
     194,   195,   196,   197,   199,   200,   201,   202,   204,   206,
     209,   210,   213,   214,   215,   216,   217,   218,   221,   222,
     223,   226,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   244,   245,   246,
     247,   248,   249,   250,   252,   253,   254,   256,   257,   258,
     260,   261,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   277,   278,   279,   280,   281,
     283,   284
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "DST", "SRC", "HOST", "GATEWAY", "NET", 
  "MASK", "PORT", "LESS", "GREATER", "PROTO", "BYTE", "ARP", "RARP", "IP", 
  "TCP", "UDP", "ICMP", "IGMP", "IGRP", "ATALK", "DECNET", "LAT", "SCA", 
  "MOPRC", "MOPDL", "TK_BROADCAST", "TK_MULTICAST", "NUM", "INBOUND", 
  "OUTBOUND", "LINK", "GEQ", "LEQ", "NEQ", "ID", "EID", "HID", "LSH", 
  "RSH", "LEN", "OR", "AND", "'!'", "'|'", "'&'", "'+'", "'-'", "'*'", 
  "'/'", "UMINUS", "')'", "'('", "'>'", "'='", "'<'", "'['", "']'", "':'", 
  "prog", "null", "expr", "and", "or", "id", "nid", "not", "paren", "pid", 
  "qid", "term", "head", "rterm", "pqual", "dqual", "aqual", "ndaqual", 
  "pname", "other", "relop", "irelop", "arth", "narth", "byteop", "pnum", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    61,    61,    62,    63,    63,    63,    63,    63,    64,
      65,    66,    66,    66,    67,    67,    67,    67,    67,    67,
      68,    69,    70,    70,    70,    71,    71,    72,    72,    73,
      73,    73,    73,    73,    74,    74,    74,    74,    74,    74,
      75,    75,    76,    76,    76,    76,    76,    76,    77,    77,
      77,    78,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    80,    80,    80,
      80,    80,    80,    80,    81,    81,    81,    82,    82,    82,
      83,    83,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    85,    85,    85,    85,    85,
      86,    86
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     2,     1,     0,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     3,     1,     3,     3,     1,     1,     2,
       1,     1,     1,     3,     3,     1,     1,     1,     2,     3,
       2,     2,     2,     2,     2,     3,     1,     3,     3,     1,
       1,     0,     1,     1,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     6,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     3
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       3,    41,     0,     0,     0,    54,    55,    53,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    66,    65,   100,
      72,    73,    52,    94,    20,     0,    21,     1,    41,    41,
       4,     0,    27,     0,    40,    39,     0,    81,    80,    69,
      70,     0,     0,     0,    92,    10,     9,    41,    41,    28,
       0,    81,    80,    14,    18,    17,    34,    11,     0,     0,
      12,    43,    42,    48,    51,    49,    50,    32,    67,    68,
      30,    31,    33,     0,    75,    77,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    74,    76,    78,     0,     0,
      96,    95,    98,    99,    97,     0,     0,     6,    41,    41,
       5,    80,     8,     7,    35,    93,   101,     0,     0,    19,
      22,     0,    26,     0,    25,     0,     0,     0,     0,    29,
       0,    90,    91,    89,    88,    84,    85,    86,    87,    37,
      38,    71,    80,    16,    15,     0,    13,     0,     0,    45,
      47,    44,    46,    82,     0,    23,    24,     0,    83,     0,
       0,     0
};

static const short yydefgoto[] =
{
     149,     1,    50,    47,    48,   109,    57,    58,    42,   112,
     113,    30,    31,    32,    33,    70,    71,    72,    43,    35,
      88,    89,    36,    37,    95,    38
};

static const short yypact[] =
{
  -32768,   131,   -10,    19,    30,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   256,-32768,   -29,   215,   215,
  -32768,   213,-32768,    87,    31,-32768,   266,-32768,-32768,-32768,
  -32768,   287,   256,   -50,-32768,-32768,-32768,   173,   173,-32768,
     -34,   -24,     9,-32768,-32768,    -6,-32768,-32768,   213,   213,
  -32768,    23,    34,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
      54,-32768,-32768,   256,-32768,-32768,-32768,   256,   256,   256,
     256,   256,   256,   256,   256,-32768,-32768,-32768,   256,   256,
  -32768,-32768,-32768,-32768,-32768,    38,   244,-32768,   173,   173,
  -32768,    12,-32768,-32768,-32768,-32768,-32768,    -3,    52,-32768,
  -32768,   -17,    11,   -29,     9,    48,    79,    83,    84,-32768,
     119,    -8,    -8,   278,   290,    29,    29,-32768,-32768,   244,
     244,-32768,    -5,-32768,-32768,     9,-32768,   213,   213,-32768,
  -32768,-32768,-32768,-32768,    65,-32768,-32768,    41,-32768,   101,
     102,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,   105,    -4,     0,   -30,   -55,     6,    -1,-32768,
  -32768,   -22,-32768,-32768,-32768,-32768,    33,-32768,    22,-32768,
  -32768,-32768,    46,   -18,-32768,   -26
};


#define	YYLAST		344


static const short yytable[] =
{
      29,    56,   107,    52,   110,    60,    49,    28,    73,    45,
      46,    51,   -12,    19,    45,    46,    52,    97,   102,   104,
      39,   101,   101,    34,    51,   100,   103,    29,    29,   105,
      59,   -36,    60,   114,    28,    28,   133,    26,   -25,   -25,
      81,    82,    83,    84,   110,   108,    99,    99,   106,    40,
      34,    34,   139,    98,    98,   -12,   -12,    59,   111,    63,
      41,    65,   106,    66,   136,   -12,   115,   116,   131,    34,
      34,    44,   101,   132,   -36,   -36,    49,   117,   118,    83,
      84,    51,   134,   140,   -36,   135,   141,   142,    96,    73,
      61,    62,    63,    64,    65,   147,    66,    99,    29,    67,
     148,   150,   151,   119,    98,    98,    27,   145,   146,   137,
     111,    60,    60,   138,     0,    68,    69,     0,     0,   120,
      34,    34,     0,   121,   122,   123,   124,   125,   126,   127,
     128,    -2,     0,     0,   129,   130,    59,    59,     0,     0,
       0,     2,     3,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    77,
      78,    19,    20,    21,    22,    79,    80,    81,    82,    83,
      84,     0,     0,    23,     0,     0,    24,     0,   143,   144,
      25,     0,     0,     2,     3,    26,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,     0,     0,    19,    20,    21,    22,     0,     0,     0,
      53,    54,    55,     0,     0,    23,     0,     0,    24,     0,
       0,     0,    25,     0,     0,     2,     3,    26,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     0,    19,    20,    21,    22,     0,
      53,    54,    55,     0,     0,     0,     0,    23,    24,     0,
      24,     0,     0,     0,    25,     0,     0,    26,     0,    26,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    77,    78,    19,     0,     0,    22,
      79,    80,    81,    82,    83,    84,     0,     0,    23,     0,
      74,    75,    76,     0,     0,    25,    77,    78,     0,     0,
      26,     0,    79,    80,    81,    82,    83,    84,    77,    78,
       0,    85,    86,    87,     0,    80,    81,    82,    83,    84,
      77,    78,     0,    90,    91,     0,     0,     0,    81,    82,
      83,    84,    92,    93,    94
};

static const short yycheck[] =
{
       1,    31,     8,    29,    59,    31,    28,     1,    58,    43,
      44,    29,     0,    30,    43,    44,    42,    47,    48,    53,
      30,    47,    48,     1,    42,    47,    48,    28,    29,    53,
      31,     0,    58,    59,    28,    29,    39,    54,    43,    44,
      48,    49,    50,    51,    99,    51,    47,    48,    53,    30,
      28,    29,     4,    47,    48,    43,    44,    58,    59,     5,
      30,     7,    53,     9,    53,    53,    43,    44,    30,    47,
      48,    25,    98,    99,    43,    44,    98,    43,    44,    50,
      51,    99,    30,     4,    53,   111,     3,     3,    42,    58,
       3,     4,     5,     6,     7,    30,     9,    98,    99,    12,
      59,     0,     0,    70,    98,    99,     1,   137,   138,   113,
     111,   137,   138,   113,    -1,    28,    29,    -1,    -1,    73,
      98,    99,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,     0,    -1,    -1,    88,    89,   137,   138,    -1,    -1,
      -1,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    40,
      41,    30,    31,    32,    33,    46,    47,    48,    49,    50,
      51,    -1,    -1,    42,    -1,    -1,    45,    -1,    59,    60,
      49,    -1,    -1,    10,    11,    54,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    -1,    -1,    -1,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    10,    11,    54,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    30,    -1,    30,    31,    32,    33,    -1,
      37,    38,    39,    -1,    -1,    -1,    -1,    42,    45,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    54,    -1,    54,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    40,    41,    30,    -1,    -1,    33,
      46,    47,    48,    49,    50,    51,    -1,    -1,    42,    -1,
      34,    35,    36,    -1,    -1,    49,    40,    41,    -1,    -1,
      54,    -1,    46,    47,    48,    49,    50,    51,    40,    41,
      -1,    55,    56,    57,    -1,    47,    48,    49,    50,    51,
      40,    41,    -1,    46,    47,    -1,    -1,    -1,    48,    49,
      50,    51,    55,    56,    57
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 1:
#line 136 "grammar.y"
{
	finish_parse(yyvsp[0].blk.b);
}
    break;
case 3:
#line 141 "grammar.y"
{ yyval.blk.q = qerr; }
    break;
case 5:
#line 144 "grammar.y"
{ gen_and(yyvsp[-2].blk.b, yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 6:
#line 145 "grammar.y"
{ gen_and(yyvsp[-2].blk.b, yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 7:
#line 146 "grammar.y"
{ gen_or(yyvsp[-2].blk.b, yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 8:
#line 147 "grammar.y"
{ gen_or(yyvsp[-2].blk.b, yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 9:
#line 149 "grammar.y"
{ yyval.blk = yyvsp[-1].blk; }
    break;
case 10:
#line 151 "grammar.y"
{ yyval.blk = yyvsp[-1].blk; }
    break;
case 12:
#line 154 "grammar.y"
{ yyval.blk.b = gen_ncode(NULL, (bpf_u_int32)yyvsp[0].i,
						   yyval.blk.q = yyvsp[-1].blk.q); }
    break;
case 13:
#line 156 "grammar.y"
{ yyval.blk = yyvsp[-1].blk; }
    break;
case 14:
#line 158 "grammar.y"
{ yyval.blk.b = gen_scode(yyvsp[0].s, yyval.blk.q = yyvsp[-1].blk.q); }
    break;
case 15:
#line 159 "grammar.y"
{ yyval.blk.b = gen_mcode(yyvsp[-2].s, NULL, yyvsp[0].i,
				    yyval.blk.q = yyvsp[-3].blk.q); }
    break;
case 16:
#line 161 "grammar.y"
{ yyval.blk.b = gen_mcode(yyvsp[-2].s, yyvsp[0].s, 0,
				    yyval.blk.q = yyvsp[-3].blk.q); }
    break;
case 17:
#line 163 "grammar.y"
{
				  /* Decide how to parse HID based on proto */
				  yyval.blk.q = yyvsp[-1].blk.q;
				  switch (yyval.blk.q.proto) {
				  case Q_DECNET:
					yyval.blk.b = gen_ncode(yyvsp[0].s, 0, yyval.blk.q);
					break;
				  default:
					yyval.blk.b = gen_ncode(yyvsp[0].s, 0, yyval.blk.q);
					break;
				  }
				}
    break;
case 18:
#line 175 "grammar.y"
{ yyval.blk.b = gen_ecode(yyvsp[0].e, yyval.blk.q = yyvsp[-1].blk.q); }
    break;
case 19:
#line 176 "grammar.y"
{ gen_not(yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 20:
#line 178 "grammar.y"
{ yyval.blk = yyvsp[-1].blk; }
    break;
case 21:
#line 180 "grammar.y"
{ yyval.blk = yyvsp[-1].blk; }
    break;
case 23:
#line 183 "grammar.y"
{ gen_and(yyvsp[-2].blk.b, yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 24:
#line 184 "grammar.y"
{ gen_or(yyvsp[-2].blk.b, yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 25:
#line 186 "grammar.y"
{ yyval.blk.b = gen_ncode(NULL, (bpf_u_int32)yyvsp[0].i,
						   yyval.blk.q = yyvsp[-1].blk.q); }
    break;
case 28:
#line 191 "grammar.y"
{ gen_not(yyvsp[0].blk.b); yyval.blk = yyvsp[0].blk; }
    break;
case 29:
#line 193 "grammar.y"
{ QSET(yyval.blk.q, yyvsp[-2].i, yyvsp[-1].i, yyvsp[0].i); }
    break;
case 30:
#line 194 "grammar.y"
{ QSET(yyval.blk.q, yyvsp[-1].i, yyvsp[0].i, Q_DEFAULT); }
    break;
case 31:
#line 195 "grammar.y"
{ QSET(yyval.blk.q, yyvsp[-1].i, Q_DEFAULT, yyvsp[0].i); }
    break;
case 32:
#line 196 "grammar.y"
{ QSET(yyval.blk.q, yyvsp[-1].i, Q_DEFAULT, Q_PROTO); }
    break;
case 33:
#line 197 "grammar.y"
{ QSET(yyval.blk.q, yyvsp[-1].i, Q_DEFAULT, yyvsp[0].i); }
    break;
case 34:
#line 199 "grammar.y"
{ yyval.blk = yyvsp[0].blk; }
    break;
case 35:
#line 200 "grammar.y"
{ yyval.blk.b = yyvsp[-1].blk.b; yyval.blk.q = yyvsp[-2].blk.q; }
    break;
case 36:
#line 201 "grammar.y"
{ yyval.blk.b = gen_proto_abbrev(yyvsp[0].i); yyval.blk.q = qerr; }
    break;
case 37:
#line 202 "grammar.y"
{ yyval.blk.b = gen_relation(yyvsp[-1].i, yyvsp[-2].a, yyvsp[0].a, 0);
				  yyval.blk.q = qerr; }
    break;
case 38:
#line 204 "grammar.y"
{ yyval.blk.b = gen_relation(yyvsp[-1].i, yyvsp[-2].a, yyvsp[0].a, 1);
				  yyval.blk.q = qerr; }
    break;
case 39:
#line 206 "grammar.y"
{ yyval.blk.b = yyvsp[0].rblk; yyval.blk.q = qerr; }
    break;
case 41:
#line 210 "grammar.y"
{ yyval.i = Q_DEFAULT; }
    break;
case 42:
#line 213 "grammar.y"
{ yyval.i = Q_SRC; }
    break;
case 43:
#line 214 "grammar.y"
{ yyval.i = Q_DST; }
    break;
case 44:
#line 215 "grammar.y"
{ yyval.i = Q_OR; }
    break;
case 45:
#line 216 "grammar.y"
{ yyval.i = Q_OR; }
    break;
case 46:
#line 217 "grammar.y"
{ yyval.i = Q_AND; }
    break;
case 47:
#line 218 "grammar.y"
{ yyval.i = Q_AND; }
    break;
case 48:
#line 221 "grammar.y"
{ yyval.i = Q_HOST; }
    break;
case 49:
#line 222 "grammar.y"
{ yyval.i = Q_NET; }
    break;
case 50:
#line 223 "grammar.y"
{ yyval.i = Q_PORT; }
    break;
case 51:
#line 226 "grammar.y"
{ yyval.i = Q_GATEWAY; }
    break;
case 52:
#line 228 "grammar.y"
{ yyval.i = Q_LINK; }
    break;
case 53:
#line 229 "grammar.y"
{ yyval.i = Q_IP; }
    break;
case 54:
#line 230 "grammar.y"
{ yyval.i = Q_ARP; }
    break;
case 55:
#line 231 "grammar.y"
{ yyval.i = Q_RARP; }
    break;
case 56:
#line 232 "grammar.y"
{ yyval.i = Q_TCP; }
    break;
case 57:
#line 233 "grammar.y"
{ yyval.i = Q_UDP; }
    break;
case 58:
#line 234 "grammar.y"
{ yyval.i = Q_ICMP; }
    break;
case 59:
#line 235 "grammar.y"
{ yyval.i = Q_IGMP; }
    break;
case 60:
#line 236 "grammar.y"
{ yyval.i = Q_IGRP; }
    break;
case 61:
#line 237 "grammar.y"
{ yyval.i = Q_ATALK; }
    break;
case 62:
#line 238 "grammar.y"
{ yyval.i = Q_DECNET; }
    break;
case 63:
#line 239 "grammar.y"
{ yyval.i = Q_LAT; }
    break;
case 64:
#line 240 "grammar.y"
{ yyval.i = Q_SCA; }
    break;
case 65:
#line 241 "grammar.y"
{ yyval.i = Q_MOPDL; }
    break;
case 66:
#line 242 "grammar.y"
{ yyval.i = Q_MOPRC; }
    break;
case 67:
#line 244 "grammar.y"
{ yyval.rblk = gen_broadcast(yyvsp[-1].i); }
    break;
case 68:
#line 245 "grammar.y"
{ yyval.rblk = gen_multicast(yyvsp[-1].i); }
    break;
case 69:
#line 246 "grammar.y"
{ yyval.rblk = gen_less(yyvsp[0].i); }
    break;
case 70:
#line 247 "grammar.y"
{ yyval.rblk = gen_greater(yyvsp[0].i); }
    break;
case 71:
#line 248 "grammar.y"
{ yyval.rblk = gen_byteop(yyvsp[-1].i, yyvsp[-2].i, yyvsp[0].i); }
    break;
case 72:
#line 249 "grammar.y"
{ yyval.rblk = gen_inbound(0); }
    break;
case 73:
#line 250 "grammar.y"
{ yyval.rblk = gen_inbound(1); }
    break;
case 74:
#line 252 "grammar.y"
{ yyval.i = BPF_JGT; }
    break;
case 75:
#line 253 "grammar.y"
{ yyval.i = BPF_JGE; }
    break;
case 76:
#line 254 "grammar.y"
{ yyval.i = BPF_JEQ; }
    break;
case 77:
#line 256 "grammar.y"
{ yyval.i = BPF_JGT; }
    break;
case 78:
#line 257 "grammar.y"
{ yyval.i = BPF_JGE; }
    break;
case 79:
#line 258 "grammar.y"
{ yyval.i = BPF_JEQ; }
    break;
case 80:
#line 260 "grammar.y"
{ yyval.a = gen_loadi(yyvsp[0].i); }
    break;
case 82:
#line 263 "grammar.y"
{ yyval.a = gen_load(yyvsp[-3].i, yyvsp[-1].a, 1); }
    break;
case 83:
#line 264 "grammar.y"
{ yyval.a = gen_load(yyvsp[-5].i, yyvsp[-3].a, yyvsp[-1].i); }
    break;
case 84:
#line 265 "grammar.y"
{ yyval.a = gen_arth(BPF_ADD, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 85:
#line 266 "grammar.y"
{ yyval.a = gen_arth(BPF_SUB, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 86:
#line 267 "grammar.y"
{ yyval.a = gen_arth(BPF_MUL, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 87:
#line 268 "grammar.y"
{ yyval.a = gen_arth(BPF_DIV, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 88:
#line 269 "grammar.y"
{ yyval.a = gen_arth(BPF_AND, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 89:
#line 270 "grammar.y"
{ yyval.a = gen_arth(BPF_OR, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 90:
#line 271 "grammar.y"
{ yyval.a = gen_arth(BPF_LSH, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 91:
#line 272 "grammar.y"
{ yyval.a = gen_arth(BPF_RSH, yyvsp[-2].a, yyvsp[0].a); }
    break;
case 92:
#line 273 "grammar.y"
{ yyval.a = gen_neg(yyvsp[0].a); }
    break;
case 93:
#line 274 "grammar.y"
{ yyval.a = yyvsp[-1].a; }
    break;
case 94:
#line 275 "grammar.y"
{ yyval.a = gen_loadlen(); }
    break;
case 95:
#line 277 "grammar.y"
{ yyval.i = '&'; }
    break;
case 96:
#line 278 "grammar.y"
{ yyval.i = '|'; }
    break;
case 97:
#line 279 "grammar.y"
{ yyval.i = '<'; }
    break;
case 98:
#line 280 "grammar.y"
{ yyval.i = '>'; }
    break;
case 99:
#line 281 "grammar.y"
{ yyval.i = '='; }
    break;
case 101:
#line 284 "grammar.y"
{ yyval.i = yyvsp[-1].i; }
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 286 "grammar.y"

