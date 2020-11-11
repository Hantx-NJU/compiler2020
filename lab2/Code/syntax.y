%{
#include <stdio.h>
#include "syntax.tab.h"
#include "lex.yy.c"
#include "tree.h"
extern int yylex (void);
extern Node* root;
void synerror(char* msg);
int yyerror(char* msg);
%}

/*  use location information */
%locations

/* declared types */
%union {
    struct Node *node;
}

/* declared tokens */
%token<node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token<node> AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

/* declared non-terminals */
%type<node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier
%type<node> OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt
%type<node> DefList Def DecList Dec Exp Args

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
Program     : ExtDefList                        {$$ = NewNode("Program", ""); AddChild($$, $1); root = $$;}
            ;
ExtDefList  : ExtDef ExtDefList                 {$$ = NewNode("ExtDefList", ""); AddChild($$, $1); AddChild($$, $2);}
            | /* empty */                       {$$ = NULL;}
            ;
ExtDef      : Specifier ExtDecList SEMI         {$$ = NewNode("ExtDef", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            | Specifier SEMI                    {$$ = NewNode("ExtDef", ""); AddChild($$, $1); AddChild($$, $2);}
            | Specifier FunDec CompSt           {$$ = NewNode("ExtDef", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            | Specifier error                   {synerror("syntax error, the global variable cannot be initialized in the definition.");}
            ;
ExtDecList  : VarDec                            {$$ = NewNode("ExtDecList", ""); AddChild($$, $1);}
            | VarDec COMMA ExtDecList           {$$ = NewNode("ExtDecList", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            ;

/* Specifiers */
Specifier       : TYPE                          {$$ = NewNode("Specifier", ""); AddChild($$, $1);}
                | StructSpecifier               {$$ = NewNode("Specifier", ""); AddChild($$, $1);}
                ;
StructSpecifier : STRUCT OptTag LC DefList RC   {$$ = NewNode("StructSpecifier", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4); AddChild($$, $5);}
                | STRUCT Tag                    {$$ = NewNode("StructSpecifier", ""); AddChild($$, $1); AddChild($$, $2);}
                | STRUCT OptTag LC DefList error RC     {synerror("syntax error, near 'RC'");}
                ;
OptTag          : ID                            {$$ = NewNode("OptTag", ""); AddChild($$, $1);}
                | /* empty */                   {$$ = NULL;}
                ;
Tag             : ID                            {$$ = NewNode("Tag", ""); AddChild($$, $1);}
                ;

/* Declarators */
VarDec      : ID                                {$$ = NewNode("VarDec", ""); AddChild($$, $1);}
            | VarDec LB INT RB                  {$$ = NewNode("VarDec", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4);}
            | VarDec LB INT error RB            {synerror("syntax error, near 'RB'");}
            ;
FunDec      : ID LP VarList RP                  {$$ = NewNode("FunDec", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4);}
            | ID LP RP                          {$$ = NewNode("FunDec", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            | error RP                          {synerror("syntax error, in function definition");}
            ;
VarList     : ParamDec COMMA VarList            {$$ = NewNode("VarList", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            | ParamDec                          {$$ = NewNode("VarList", ""); AddChild($$, $1);}
            | ParamDec error COMMA VarList      {synerror("syntax error, near ','");}  
            ;
ParamDec    : Specifier VarDec                  {$$ = NewNode("ParamDec", ""); AddChild($$, $1); AddChild($$, $2);}
            ;

/* Statements */
CompSt      : LC DefList StmtList RC            {$$ = NewNode("CompSt", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4);}
            ;
StmtList    : Stmt StmtList                     {$$ = NewNode("StmtList", ""); AddChild($$, $1); AddChild($$, $2);}
            | /* empty */                       {$$ = NULL;}
            ;
Stmt        : Exp SEMI                                  {$$ = NewNode("Stmt", ""); AddChild($$, $1); AddChild($$, $2);}
            | CompSt                                    {$$ = NewNode("Stmt", ""); AddChild($$, $1);}
            | RETURN Exp SEMI                           {$$ = NewNode("Stmt", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   {$$ = NewNode("Stmt", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4); AddChild($$, $5);}
            | IF LP Exp RP Stmt ELSE Stmt               {$$ = NewNode("Stmt", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4); AddChild($$, $5);AddChild($$, $6); AddChild($$, $7);}
            | WHILE LP Exp RP Stmt                      {$$ = NewNode("Stmt", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4); AddChild($$, $5);}
            | Exp error                                 {synerror("syntax error");}  
            | IF LP Exp error RP Stmt ELSE Stmt         {synerror("syntax error, near 'RP'");}  
            ;

/* Local Definitions */
DefList     : Def DefList                       {$$ = NewNode("DefList", ""); AddChild($$, $1); AddChild($$, $2);}
            | /* empty */                       {$$ = NULL;}
            ;   
Def         : Specifier DecList SEMI            {$$ = NewNode("Def", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            | Specifier error SEMI              {synerror("syntax error, near ';'");}  
            ;
DecList     : Dec                               {$$ = NewNode("DecList", ""); AddChild($$, $1);}
            | Dec COMMA DecList                 {$$ = NewNode("DecList", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            ;
Dec         : VarDec                            {$$ = NewNode("Dec", ""); AddChild($$, $1);}
            | VarDec ASSIGNOP Exp               {$$ = NewNode("Dec", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
            ;

/* Expressions */
Exp     : Exp ASSIGNOP Exp                      {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp AND Exp                           {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp OR Exp                            {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp RELOP Exp                         {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp PLUS Exp                          {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp MINUS Exp                         {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp STAR Exp                          {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp DIV Exp                           {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | LP Exp RP                             {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | MINUS Exp                             {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2);}
        | NOT Exp                               {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2);}
        | ID LP Args RP                         {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3); AddChild($$, $4);}
        | ID LP RP                              {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp LB Exp RB                         {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);AddChild($$, $4);}
        | Exp DOT ID                            {$$ = NewNode("Exp", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | ID                                    {$$ = NewNode("Exp", ""); AddChild($$, $1);}
        | INT                                   {$$ = NewNode("Exp", ""); AddChild($$, $1);}
        | FLOAT                                 {$$ = NewNode("Exp", ""); AddChild($$, $1);}
        | ID LP error                                 {synerror("syntax error, about Exp");}
        | Exp LB error                                 {synerror("syntax error, about Exp");}  
        ;
Args    : Exp COMMA Args                        {$$ = NewNode("Args", ""); AddChild($$, $1); AddChild($$, $2); AddChild($$, $3);}
        | Exp                                   {$$ = NewNode("Args", ""); AddChild($$, $1);}
        ;

%%
int yyerror(char* msg){
     //fprintf(stderr, "yyerror Error type B at Line %d: \'%s\'\n",yylineno, msg);   
}
