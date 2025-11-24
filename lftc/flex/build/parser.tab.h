/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BUILD_PARSER_TAB_H_INCLUDED
# define YY_YY_BUILD_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    OPERATOR = 259,                /* OPERATOR  */
    BOOL_OPERATOR = 260,           /* BOOL_OPERATOR  */
    IDENTIFIER = 261,              /* IDENTIFIER  */
    INCLUDE = 262,                 /* INCLUDE  */
    USING_NAMESPACE = 263,         /* USING_NAMESPACE  */
    GREATER_THAN = 264,            /* GREATER_THAN  */
    SMALLER_THAN = 265,            /* SMALLER_THAN  */
    TYPE = 266,                    /* TYPE  */
    L_PAR = 267,                   /* L_PAR  */
    R_PAR = 268,                   /* R_PAR  */
    COMMA = 269,                   /* COMMA  */
    L_BRACE = 270,                 /* L_BRACE  */
    R_BRACE = 271,                 /* R_BRACE  */
    EQUAL = 272,                   /* EQUAL  */
    SEMI_COLON = 273,              /* SEMI_COLON  */
    RETURN = 274,                  /* RETURN  */
    IF = 275,                      /* IF  */
    ELSE = 276,                    /* ELSE  */
    WHILE = 277,                   /* WHILE  */
    READ = 278,                    /* READ  */
    WRITE = 279,                   /* WRITE  */
    BOOLEAN_LITERAL = 280,         /* BOOLEAN_LITERAL  */
    SHIFT_LEFT = 281,              /* SHIFT_LEFT  */
    SHIFT_RIGHT = 282              /* SHIFT_RIGHT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BUILD_PARSER_TAB_H_INCLUDED  */
