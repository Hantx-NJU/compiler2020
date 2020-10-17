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
([1-9]+{digit}*)|0[0-7]*|0[Xx][a-fA-F]+    {printf("INT, %s\n", yytext); yylval.type_int = atoi(yytext);    return INT;}
({digits}\.{digits})|(({digit}*\.?{digits})|{digits}\.)[Ee][+-]?{digits}    {printf("FLOAT, %s\n", yytext); yylval.type_float = atof(yytext);   return FLOAT;}
";"  {return SEMI;}
","   {return COMMA;}
"=" {return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" {return RELOP;}
"+" {return PLUS;}
"-" {return MINUS;}
"*" {return STAR;}
"/" {return DIV;}
"&&"    {return AND;}
"||"   {return OR;}
"." {return DOT;}
"!" {return NOT;}
"int"|"float"   {return TYPE;}
"(" {return LP;}
")" {return RP;}
"[" {return LB;}
"]" {return RB;}
"{" {return LC;}
"}" {return RC;}
"struct"    {return STRUCT;}
"return"    {return RETURN;}
"if"    {return IF;}
"else"  {return ELSE;}
"while" {return WHILE;}
[_a-zA-Z][_a-zA-Z0-9]* {printf("ID, %s\n",yytext); return ID;}
{space} {}
\n {yycolumn = 1;}
.   {printf("Error type A at Line %d : Mysterious characters  \'%s\'\n", yylineno, yytext);}
%%
