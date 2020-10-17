%option yylineno
%{
#include "syntax.tab.h"
#include "tree.h"
int yycolumn = 1;
#define YY_USER_ACTION  yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}
digit   [0-9]
digits [0-9]+
letter  [a-zA-z]
space   " "|\t
%%
([1-9]+{digit}*)|0[0-7]*|0[Xx][a-fA-F]+    { int res = atoi(yytext); yylval.node = NewNode(yylineno, "INT", INT, &res);return INT;}
({digits}\.{digits})|(({digit}*\.?{digits})|{digits}\.)[Ee][+-]?{digits}    { float res = atof(yytext); yylval.node = NewNode(yylineno, "FLOAT", FLOAT,&res); return FLOAT;}
";"  {yylval.node = NewNode(yylineno, "SEMI", SEMI, NULL);return SEMI;}
","   {yylval.node = NewNode(yylineno, "COMMA", COMMA, NULL);return COMMA;}
"=" {yylval.node = NewNode(yylineno, "ASSIGNOP", ASSIGNOP, NULL);return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" {yylval.node = NewNode(yylineno, "RELOP", RELOP, NULL);return RELOP;}
"+" {yylval.node = NewNode(yylineno, "PLUS", PLUS, NULL);return PLUS;}
"-" {yylval.node = NewNode(yylineno, "MINUS", MINUS, NULL);return MINUS;}
"*" {yylval.node = NewNode(yylineno, "STAR", STAR, NULL);return STAR;}
"/" {yylval.node = NewNode(yylineno, "DIV", DIV, NULL);return DIV;}
"&&"    {yylval.node = NewNode(yylineno, "AND", AND, NULL);return AND;}
"||"   {yylval.node = NewNode(yylineno, "OR", OR, NULL);return OR;}
"." {yylval.node = NewNode(yylineno, "DOT", DOT, NULL);return DOT;}
"!" {yylval.node = NewNode(yylineno, "NOT", NOT, NULL);return NOT;}
"int"|"float"   {yylval.node = NewNode(yylineno, "TYPE", TYPE, NULL);return TYPE;}
"(" {yylval.node = NewNode(yylineno, "LP", LP, NULL);return LP;}
")" {yylval.node = NewNode(yylineno, "RP", RP, NULL);return RP;}
"[" {yylval.node = NewNode(yylineno, "LB", LB, NULL);return LB;}
"]" {yylval.node = NewNode(yylineno, "RB", RB, NULL);return RB;}
"{" {yylval.node = NewNode(yylineno, "LC", LC, NULL);return LC;}
"}" {yylval.node = NewNode(yylineno, "RC", RC, NULL);return RC;}
"struct"    {yylval.node = NewNode(yylineno, "STRUCT", STRUCT, NULL);return STRUCT;}
"return"    {yylval.node = NewNode(yylineno, "RETURN", RETURN, NULL);return RETURN;}
"if"    {yylval.node = NewNode(yylineno, "IF", IF, NULL);return IF;}
"else"  {yylval.node = NewNode(yylineno, "ELSE", ELSE, NULL);return ELSE;}
"while" {yylval.node = NewNode(yylineno, "WHILE", WHILE, NULL);return WHILE;}
[_a-zA-Z][_a-zA-Z0-9]* {char* res = malloc(strlen(yytext)+1);strcpy(res, yytext) ; yylval.node = NewNode(yylineno, "ID", ID, (void*)res);return ID;}
{space} {}
\n|\r {yycolumn = 1;}
.   {printf("Error type A at Line %d : Mysterious characters  \'%s\'\n", yylineno, yytext);}
%%
