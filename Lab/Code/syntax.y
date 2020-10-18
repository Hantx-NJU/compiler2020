%{
#include <stdio.h>
#include "syntax.tab.h"
#include "lex.yy.c"
#include "tree.h"
extern int yylex (void);
void synerror(char* msg);
int yyerror(char* msg);
%}

/*  use location information */
%locations

/* declared types */
%union {
    struct Node* node;
}

/* declared tokens */
%token <node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token <node> AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

/* declared non-terminals */
%type <node> Program ExtDefList ExtDef ExtDecList   
%type <node> Specifier StructSpecifier OptTag Tag   
%type <node> VarDec FunDec VarList ParamDec         
%type <node> CompSt StmtList Stmt                   
%type <node> DefList Def Dec DecList               
%type <node> Exp Args                               

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
Program     : ExtDefList {$$ = NewNode(@$.first_line,  "Program", 0, NULL); AddChild($$, $1); root = $$;}
            ;
ExtDefList  : ExtDef ExtDefList {$$ = NewNode(@$.first_line,  "ExtDefList", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
            | /* empty */   {$$ = NULL;}
            ;
ExtDef      : Specifier ExtDecList SEMI {$$ = NewNode(@$.first_line,  "ExtDef", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            | Specifier SEMI    {$$ = NewNode(@$.first_line,  "ExtDef", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
            | Specifier FunDec CompSt   {$$ = NewNode(@$.first_line,  "ExtDef", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            | Specifier error SEMI                {synerror("syntax error, the global variable cannot be initialized in the definition.");}
            ;
ExtDecList  : VarDec    {$$ = NewNode(@$.first_line,  "ExtDecList", 0, NULL); AddChild($$, $1);}
            | VarDec COMMA ExtDefList   {$$ = NewNode(@$.first_line,  "ExtDecList", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            ;

/* Specifiers */
Specifier       : TYPE  {$$ = NewNode(@$.first_line,  "Specifier", 0, NULL); AddChild($$, $1); }
                | StructSpecifier   {$$ = NewNode(@$.first_line,  "Specifier", 0, NULL); AddChild($$, $1); }
                ;
StructSpecifier : STRUCT OptTag LC DefList RC   {$$ = NewNode(@$.first_line,  "StructSpecifier", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);AddChild($$,$5);}
                | STRUCT Tag    {$$ = NewNode(@$.first_line,  "StructSpecifier", 0, NULL); AddChild($$, $1); AddChild($$,$2);} 
                | STRUCT OptTag LC DefList error RC     {synerror("syntax error, near 'RC'");}
                ;
OptTag          : ID    {$$ = NewNode(@$.first_line,  "OptTag", 0, NULL); AddChild($$, $1); }
                | /* empty */   {$$ = NULL;}
                ;
Tag             : ID    {$$ = NewNode(@$.first_line,  "Tag", 0, NULL); AddChild($$, $1); }
                ;

/* Declarators */
VarDec      : ID    {$$ = NewNode(@$.first_line,  "VarDec", 0, NULL); AddChild($$, $1); }
            | VarDec LB INT RB  {$$ = NewNode(@$.first_line,  "VarDec", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);}
            | VarDec LB INT error RB            {synerror("syntax error, near 'RB'");}
            ;
FunDec      : ID LP VarList RP  {$$ = NewNode(@$.first_line,  "FunDec", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);}
            | ID LP RP  {$$ = NewNode(@$.first_line,  "FunDec", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            | error RP                  {synerror("syntax error, in function definition");}
            ;
VarList     : ParamDec COMMA VarList    {$$ = NewNode(@$.first_line,  "VarList", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            | ParamDec  {$$ = NewNode(@$.first_line,  "VarList", 0, NULL); AddChild($$, $1); }
            | ParamDec error COMMA VarList      {synerror("syntax error, near ','");}  
            ;
ParamDec    : Specifier VarDec  {$$ = NewNode(@$.first_line,  "ParamDec", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
            ;

/* Statements */
CompSt      : LC DefList StmtList RC    {$$ = NewNode(@$.first_line,  "CompSt", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);}
            ;
StmtList    : Stmt StmtList {$$ = NewNode(@$.first_line,  "StmtList", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
            | /* empty */   {$$ = NULL;}
            ;
Stmt        : Exp SEMI  {$$ = NewNode(@$.first_line,  "Stmt", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
            | CompSt    {$$ = NewNode(@$.first_line,  "Stmt", 0, NULL); AddChild($$, $1); }
            | RETURN Exp SEMI   {$$ = NewNode(@$.first_line,  "Stmt", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   {$$ = NewNode(@$.first_line,  "Stmt", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);AddChild($$,$5);}
            | IF LP Exp RP Stmt ELSE Stmt   {$$ = NewNode(@$.first_line,  "Stmt", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);AddChild($$,$5);AddChild($$,$6);AddChild($$,$7);}
            | WHILE LP Exp RP Stmt  {$$ = NewNode(@$.first_line,  "Stmt", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);AddChild($$,$5);}
            | Exp error SEMI                            {synerror("syntax error, near ';'");}  
            | IF LP Exp error RP Stmt ELSE Stmt         {synerror("syntax error, near 'RP'");}  
            ;

/* Local Definitions */
DefList     : Def DefList   {$$ = NewNode(@$.first_line,  "DefList", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
            | /* empty */   {$$ = NULL;}
            ;
Def         : Specifier DecList SEMI    {$$ = NewNode(@$.first_line,  "Def", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            | error SEMI                                {synerror("syntax error, near ';'");}  
            ;
DecList     : Dec   {$$ = NewNode(@$.first_line,  "DecList", 0, NULL); AddChild($$, $1);}
            | Dec COMMA DecList {$$ = NewNode(@$.first_line,  "DecList", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            ;
Dec         : VarDec    {$$ = NewNode(@$.first_line,  "Dec", 0, NULL); AddChild($$, $1); }
            | VarDec ASSIGNOP Exp   {$$ = NewNode(@$.first_line,  "Dec", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
            ;

/* Expressions */
Exp     : Exp ASSIGNOP Exp  {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp AND Exp   {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp OR Exp    {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp RELOP Exp {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp PLUS Exp  {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp MINUS Exp {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp STAR Exp  {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp DIV Exp   {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | LP Exp RP {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | MINUS Exp     {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
        | NOT Exp   {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);}
        | ID LP Args RP {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);}
        | ID LP RP  {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp LB Exp RB {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);AddChild($$,$4);}
        | Exp DOT ID    {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | ID    {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); }
        | INT   {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); }
        | FLOAT {$$ = NewNode(@$.first_line,  "Exp", 0, NULL); AddChild($$, $1); }
        | error                         {synerror("syntax error, about Exp");}  
        ;
Args    : Exp COMMA Args    {$$ = NewNode(@$.first_line,  "Args", 0, NULL); AddChild($$, $1); AddChild($$,$2);AddChild($$,$3);}
        | Exp   {$$ = NewNode(@$.first_line,  "Args", 0, NULL); AddChild($$, $1); }
        ;

%%

int yyerror(char* msg){
    
}
