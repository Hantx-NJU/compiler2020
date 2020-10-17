%{
#include <stdio.h>
#include "lex.yy.c"
extern int yylex (void);
int yyerror(char* msg);
%}

/*use location information*/
%locations

/*declared types*/
%union  {
    int type_int;
    float type_float;
}

/* declared tokens */
%token <type_int> INT
%token <type_float> FLOAT
%token ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

/* declared non-terminals */
//%type Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier
//%type OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt
//%type DefList Def DecList Dec Exp

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT 
%left LP RP LB RB DOT

%%
/* High-level Definitions */
Program     : ExtDefList
            ;
ExtDefList  : ExtDef ExtDefList
            | /* empty */
            ;
ExtDef      : Specifier ExtDecList  SEMI
            | Specifier SEMI
            | Specifier FunDec CompSt
            ;
ExtDecList  : VarDec
            | VarDec  COMMA ExtDefList
            ;

/* Specifiers */
Specifier       : TYPE
                | StructSpecifier
                ;
StructSpecifier : STRUCT OptTag LC DefList RC
                | STRUCT Tag
                ;
OptTag          : ID
                | /* empty */
                ;
Tag             : ID
                ;

/* Declarators */
VarDec      : ID
            | VarDec LB INT RB
            ;
FunDec      : ID LP VarList RP
            | ID LP RP
            ;
VarList     : ParamDec COMMA VarList
            | ParamDec
            ;
ParamDec    : Specifier VarDec
            ;

/* Statements */
CompSt      : LC DefList StmtList RC
            | error RC
            ;
StmtList    : Stmt StmtList
            | /* empty */
            ;
Stmt        : Exp SEMI 
            | CompSt
            | RETURN Exp SEMI
            | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
            | IF LP Exp RP Stmt ELSE Stmt
            | WHILE LP Exp RP Stmt
            | error SEMI
            ;

/* Local Definitions */
DefList     : Def DefList
            | /* empty */
            ;
Def         : Specifier DecList SEMI
            ;
DecList     : Dec 
            | Dec COMMA DecList
            ;
Dec         : VarDec
            | VarDec ASSIGNOP Exp
            ;

/* Expressions */
Exp     : Exp ASSIGNOP Exp
        | Exp AND Exp
        | Exp OR Exp
        | Exp RELOP Exp
        | Exp PLUS Exp
        | Exp MINUS Exp
        | Exp STAR Exp
        | Exp DIV Exp
        | LP Exp RP 
        | MINUS Exp 
        | NOT Exp 
        | ID LP Args RP 
        | ID LP RP
        | Exp LB Exp RB 
        | Exp DOT ID
        | ID
        | INT
        | FLOAT
        | error RP
        ;
Args    : Exp COMMA Args
        | Exp
        ;

%%
int yyerror(char* msg) {
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno,msg);
}
