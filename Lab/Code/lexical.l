%option yylineno
%{
#include "syntax.tab.h"
int yycolumn = 1;
#define YY_USER_ACTION  yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}
digit   [0-9]
digits [0-9]+
letter  [a-zA-z]
space   " "|\t|\r
%%
([1-9]+{digit}*)|0[0-7]*|0[Xx][a-fA-F]+    {printf("INT, %s\n", yytext);yylval.type_int = atoi(yytext);    return INT;}
({digits}\.{digits})|(({digit}*\.?{digits})|{digits}\.)[Ee][+-]?{digits}    {printf("FLOAT, %s\n", yytext);yylval.type_float = atof(yytext);   return FLOAT;}
";"  {printf("SEMI\n");}
","   {printf("COMMA\n");}
"=" {printf("ASSIGNOP\n");}
">"|"<"|">="|"<="|"=="|"!=" {printf("RELOP\n");}
"+" {printf("PLUS\n");return ADD;}
"-" {printf("MINUS\n");return SUB;}
"*" {printf("STAR\n");return MUL;}
"/" {printf("DIV\n");return DIV;}
"&&"    {printf("AND\n");}
"||"   {printf("OR\n");}
"." {printf("DOT\n");}
"!" {printf("NOT\n");}
"int"|"float"   {printf("TYPE\n");}
"(" {printf("LP\n");}
")" {printf("RP\n");}
"[" {printf("LB\n");}
"]" {printf("RB\n");}
"{" {printf("LC\n");}
"}" {printf("RC\n");}
"struct"    {printf("STRUCT\n");}
"return"    {printf("RETURN\n");}
"if"    {printf("IF\n");}
"else"  {printf("ELSE\n");}
"while" {printf("WHILE\n");}
[_a-zA-Z][_a-zA-Z0-9]* {printf("ID, %s\n",yytext);}
{space} {}
\n {yycolumn = 1;}
.   {printf("Error type A at Line %d : Mysterious characters  \'%s\'\n", yylineno, yytext);}
%%
