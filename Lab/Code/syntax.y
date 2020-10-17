%{
#include <stdio.h>
#include "syntax.tab.h"
#include "lex.yy.c"
#include "tree.c"
extern int yylex (void);
void synerror(char* msg);
int yyerror(char* msg);
%}

/*  use location information */
%locations

/* declared types */
%union {
    Node *node;
}

/* declared tokens */
%token <node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token <node> AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

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
Program     : ExtDefList {
    $$ = NewNode(@$.first_line,  "Program", 0, NULL);
    Add($$, $1);
    root = $$;
}
            ;
ExtDefList  : ExtDef ExtDefList{
    
}
            | /* empty */
            ;
ExtDef      : Specifier ExtDecList SEMI
            | Specifier SEMI
            | Specifier FunDec CompSt
            | Specifier error SEMI                {synerror("syntax error, the global variable cannot be initialized in the definition.");}
            ;
ExtDecList  : VarDec
            | VarDec COMMA ExtDefList
            ;

/* Specifiers */
Specifier       : TYPE
                | StructSpecifier
                ;
StructSpecifier : STRUCT OptTag LC DefList RC
                | STRUCT Tag
                | STRUCT OptTag LC DefList error RC     {synerror("syntax error, near 'RC'");}
                ;
OptTag          : ID
                | /* empty */
                ;
Tag             : ID
                ;

/* Declarators */
VarDec      : ID
            | VarDec LB INT RB
            | VarDec LB INT error RB            {synerror("syntax error, near 'RB'");}
            ;
FunDec      : ID LP VarList RP
            | ID LP RP
            | error RP                  {synerror("syntax error, in function definition");}
            ;
VarList     : ParamDec COMMA VarList
            | ParamDec
            | ParamDec error COMMA VarList      {synerror("syntax error, near ','");}  
            ;
ParamDec    : Specifier VarDec
            ;

/* Statements */
CompSt      : LC DefList StmtList RC
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
            | Exp error SEMI                            {synerror("syntax error, near ';'");}  
            | IF LP Exp error RP Stmt ELSE Stmt         {synerror("syntax error, near 'RP'");}  
            ;

/* Local Definitions */
DefList     : Def DefList
            | /* empty */
            ;
Def         : Specifier DecList SEMI
            | error SEMI                                {synerror("syntax error, near ';'");}  
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
        | error                         {synerror("syntax error, about Exp");}  
        ;
Args    : Exp COMMA Args
        | Exp   
        ;

%%

int yyerror(char* msg){
     fprintf(stderr, "yyerror Error type B at Line %d: \'%s\'\n",yylineno, msg);   
}
