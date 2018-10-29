%{	
	#define YYERROR_VERBOSE
	#include "../ast/ast.h"

	NBlock* root;
	extern int yylex();
	extern void yyerror(const char* x);
	extern char *yytext;
%}

%union {
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
}

%type <block> document stmts block_stmt fn_block
%type <stmt> stmt assignment
%type <expr> expr lit
%type <identif> ident fn_source
%type <func> item_fn
%type <identsVec> fn_sources_params fn_sources maybe_fn_sources
%type <varvec> fn_decl params maybe_params fn_params
%type <statements> maybe_stmts
%type <vardec> param
%type <type> type
%type <typevec> maybe_type_clause fn_type_clause types_list;

%token SHL
%token SHR
%token LE
%token EQEQ
%token NE
%token GE
%token ANDAND
%token OROR
%token SHLEQ
%token SHREQ
%token MINUSEQ
%token ANDEQ
%token OREQ
%token PLUSEQ
%token STAREQ
%token SLASHEQ
%token CARETEQ
%token PERCENTEQ
%token DOTDOT
%token DOTDOTDOT
%token MOD_SEP
%token RARROW
%token LARROW
%token FAT_ARROW
%token LIT_BYTE
%token LIT_CHAR
%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_STR
%token LIT_STR_RAW
%token LIT_BYTE_STR
%token LIT_BYTE_STR_RAW
%token IDENT
%token UNDERSCORE
%token LIFETIME
%token INNER_DOC_COMMENT
%token OUTER_DOC_COMMENT

// keywords
%token DO
%token ELSE
%token IF
%token FOR
%token FALSE
%token TRUE
%token PIPE
%token PUSH
%token TYPE
%token WHILE
%token CONTINUE
%token FILTER
%token MODULE

%precedence IDENT
%precedence PUSH

%right '=' SHLEQ SHREQ MINUSEQ ANDEQ OREQ PLUSEQ STAREQ SLASHEQ CARETEQ PERCENTEQ
%right LARROW
%left OROR
%left ANDAND
%left EQEQ NE
%left '<' '>' LE GE
%left '|'
%left '^'
%left '&'
%left SHL SHR
%left '+' '-'
//%precedence AS
%left '*' '/' '%'
%precedence '!'

%precedence '{' '[' '(' '.' ':'

%start document

%%

document: 
%empty { root = new NBlock();}
| stmts { root = $1;}
;

stmts: 
stmt			{ $$ = new NBlock();  $$->statements.push_back($1); }
| stmt ';'		{ $$ = new NBlock();  $$->statements.push_back($1); }
| stmts ';' stmt ';' { $$->statements.push_back($3); }
;

stmt:
block_stmt		{ $$ = $1; }
| assignment 	{ $$ = $1; }
;

assignment: 
ident '=' expr			{$$ = new NAssignment($1, $3); printf("Create assignment\n");} // x = 5
| type ident '=' expr  { $$ = new NVariableDeclaration($1, $2, $4); } // int x = 3
| type ident			{ $$ = new NVariableDeclaration($1, $2); } // int x;
;

expr:
expr '+' expr 			{ $$ = new NBinaryOp($1, OP_ADD, $3);}
| expr '-' expr 			{ $$ = new NBinaryOp($1, OP_SUB, $3);}
| expr '*' expr 			{ $$ = new NBinaryOp($1, OP_MULT, $3);}
| expr '/' expr 			{ $$ = new NBinaryOp($1, OP_DIV, $3);}
| '(' expr ')'			{ $$ = $2;}
| lit					{ $$ = $1; }
;

lit: 
LIT_INTEGER 			{ $$ = new NInteger(atol(yytext)); printf("READ INT: %s\n",yytext); }
| LIT_FLOAT				{ $$ = new NFloat(atof(yytext)); }
| "true" 				{ $$ = new NBool(true); }
| "false"				{ $$ = new NBool(false); }


block_stmt:
item_fn			{ $$ = $1;}
;


item_fn: 
PIPE fn_sources_params ident fn_decl maybe_type_clause fn_block
{
	$$ = new NFunction($2, $3, $4, $5, $6);
}
;

maybe_type_clause:
':' fn_type_clause 	{$$ =$2;}
| %empty				{$$ = new std::vector<NType*>();}
;

fn_type_clause:
type				{ $$ = new std::vector<NType*>(); $$->push_back($1); } 
| '(' types_list ')' 	{ $$ = $2; }
;

types_list:
type		{$$ = new std::vector<NType*>(); $$->push_back($1); }
| types_list ',' type { $$->push_back($3);  }
;

type: ident { $$ = new NType($1->name); }

fn_block:
'{' maybe_stmts '}' { $$ = new NBlock(); $$->statements = *$2;}
;

fn_sources_params:
fn_source				   { $$ = new std::vector<NIdentifier*>(); $$->push_back($1); }
| '(' maybe_fn_sources ')' { $$ = $2; }
;

maybe_fn_sources:
fn_sources	{ $$ = $1; }
| %empty	{$$ = new std::vector<NIdentifier*>(); }
;

fn_sources:
fn_source { $$ = new std::vector<NIdentifier*>(); $$->push_back($1); }
| fn_sources ',' fn_source { $$->push_back($3); }
;

fn_source:
ident		{ $$ = $1; }
;

fn_decl:
fn_params { $$ = $1; }
;

fn_params:
'(' maybe_params ')' { $$ = $2;}
;

maybe_params:
params	 { $$ = $1; }
| %empty { $$ = new std::vector<NVariableDeclaration*>(); }
;

params: 
param				{ $$ = new std::vector<NVariableDeclaration*>(); $$->push_back($1); }
| params ',' param   { $$->push_back($3); }
;

param:
ident ident { $$ = new NVariableDeclaration(new NType($1->name), $2); }
;

ident:
IDENT		{ $$ = new NIdentifier(yytext); }
;

maybe_stmts: 
%empty  {$$ = new std::vector<NStatement*>(); }
| stmts { $$ = &($1->statements); }
;

%%

//TODO maybe_stmts