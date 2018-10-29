%{
#include <stdio.h>
#include "../ast/ast.h"
#include "grammar.tab.hpp"
// "pipe".+([a-zA-Z]+[0-9a-zA-Z]*) printf("Hello");
%}

%option yylineno
%option noyywrap
%option stack


%x str
%x byte
%x linecomment
%x blockcomment
%x suffix
%x doc_line

ident [a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]*


%%
<suffix>{ident}		{ BEGIN(INITIAL); }
<suffix>(.|\n)		{ yyless(0); BEGIN(INITIAL); }

[ \n\t\r] 			{ }

\xef\xbb\xbf {
  // UTF-8 byte order mark (BOM), ignore if in line 1, error otherwise
  if (yyget_lineno() != 1) {
    return -1;
  }
}

\/\/(\/|\!)		{ BEGIN(doc_line); yymore(); }
<doc_line>\n	{ BEGIN(INITIAL);
				yyleng--;
				yytext[yyleng] = 0;
				return ((yytext[2] == '!') ? INNER_DOC_COMMENT : OUTER_DOC_COMMENT);
				}
<doc_line>[^\n]*      { yymore(); }

\/\/|\/\/\/\/         { BEGIN(linecomment); }
<linecomment>\n       { BEGIN(INITIAL); }
<linecomment>[^\n]*   { }

\/\*                  { yy_push_state(blockcomment); }
<blockcomment>\/\*    { yy_push_state(blockcomment); }
<blockcomment>\*\/    { yy_pop_state(); }
<blockcomment>(.|\n)   { }

_	{return UNDERSCORE;}
pipe {return PIPE;}
pump {return PUSH;}
filt {return FILTER;}
module {return MODULE;}

-?[0-9]+ 		{ return LIT_INTEGER; }
[0-9][0-9_]*\.(\.[a-zA-Z])	{ yyless(yyleng - 2); return LIT_INTEGER; }

[0-9][0-9_]*\.[0-9_]*([eE][-\+]?[0-9_]+)?	{ BEGIN(suffix); return LIT_FLOAT;}
[0-9][0-9_]*(\.[0-9_]*)?[eE][-\+]?[0-9_]+	{ BEGIN(suffix); return LIT_FLOAT;}

; { return ';';}
, {return ',';}
\.     { return '.'; }
\(     { return '('; }
\)     { return ')'; }
\{     { return '{'; }
\}     { return '}'; }
\[     { return '['; }
\]     { return ']'; }

\~     { return '~'; }
::     { return MOD_SEP; }
:      { printf("FOUND COLON\n");return ':'; }
\$     { return '$'; }
\?     { return '?'; }

==    { return EQEQ; }
=>    { return FAT_ARROW; }
=     { return '='; }
\!=   { return NE; }
\!    { return '!'; }
\<=   { return LE; }
\<\<  { return SHL; }
\<\<= { return SHLEQ; }
\<    { return '<'; }
\>=   { return GE; }
\>\>  { return SHR; }
\>\>= { return SHREQ; }
\>    { return '>'; }

{ident}  { return IDENT; }

\x27.\x27		{ return LIT_CHAR; }

\x22	{ BEGIN(str); yymore();}
<str>\x22 { BEGIN(suffix); return LIT_STR; }

<str><<EOF>>                     { return -1; }
<str>\\[n\nr\rt\\\x27\x220]      { yymore(); }
<str>\\x[0-9a-fA-F]{2}           { yymore(); }
<str>\\u\{([0-9a-fA-F]_*){1,6}\} { yymore(); }
<str>\\[^n\nrt\\\x27\x220]       { return -1; }
<str>(.|\n)                      { yymore(); }

\<-  { return LARROW; }
-\>  { return RARROW; }
-    { return '-'; }
-=   { return MINUSEQ; }
&&   { return ANDAND; }
&    { return '&'; }
&=   { return ANDEQ; }
\|\| { return OROR; }
\|   { return '|'; }
\|=  { return OREQ; }
\+   { return '+'; }
\+=  { return PLUSEQ; }
\*   { return '*'; }
\*=  { return STAREQ; }
\/   { return '/'; }
\/=  { return SLASHEQ; }
\^   { return '^'; }
\^=  { return CARETEQ; }
%    { return '%'; }
%=   { return PERCENTEQ; }

[\n\t] {/* ignore */}

<<EOF>> { yyterminate(); return 0; }

%%