/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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

#ifndef YY_YY_GRAMMAR_TAB_HPP_INCLUDED
# define YY_YY_GRAMMAR_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SHL = 258,
    SHR = 259,
    LE = 260,
    EQEQ = 261,
    NE = 262,
    GE = 263,
    ANDAND = 264,
    OROR = 265,
    SHLEQ = 266,
    SHREQ = 267,
    MINUSEQ = 268,
    ANDEQ = 269,
    OREQ = 270,
    PLUSEQ = 271,
    STAREQ = 272,
    SLASHEQ = 273,
    CARETEQ = 274,
    PERCENTEQ = 275,
    DOTDOT = 276,
    DOTDOTDOT = 277,
    MOD_SEP = 278,
    RARROW = 279,
    LARROW = 280,
    FAT_ARROW = 281,
    LIT_BYTE = 282,
    LIT_CHAR = 283,
    LIT_INTEGER = 284,
    LIT_FLOAT = 285,
    LIT_STR = 286,
    LIT_STR_RAW = 287,
    LIT_BYTE_STR = 288,
    LIT_BYTE_STR_RAW = 289,
    IDENT = 290,
    UNDERSCORE = 291,
    LIFETIME = 292,
    INNER_DOC_COMMENT = 293,
    OUTER_DOC_COMMENT = 294,
    DO = 295,
    ELSE = 296,
    IF = 297,
    FOR = 298,
    FALSE = 299,
    TRUE = 300,
    PIPE = 301,
    PUSH = 302,
    TYPE = 303,
    WHILE = 304,
    CONTINUE = 305,
    FILTER = 306,
    MODULE = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 11 "grammar.y" /* yacc.c:1916  */

	NBlock* block;	
	NAssignment* assign;
	NVariableDeclaration* vardec;
	NBinaryOp* binop;
	NIdentifier* identif;
	NStatement *stmt;
	NExpression *expr;
	NFunction *func;	
	NType* type;
	std::vector<NType*>* typevec;
	std::vector<NIdentifier*> *identsVec;
	std::vector<NExpression*> *exprvec;
	std::vector<NVariableDeclaration*> *varvec;	
	std::vector<NStatement*>* statements;
	std::string *string;	

#line 125 "grammar.tab.hpp" /* yacc.c:1916  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_TAB_HPP_INCLUDED  */
