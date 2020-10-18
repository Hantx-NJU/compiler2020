%option yylineno
%{
    #include "tree.h"
    void lexerror(char* msg);
    int yycolumn = 1;
    #define YY_USER_ACTION  yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

digit [0-9]
digits [0-9]+
letter [_a-zA-Z]

INT ([1-9][0-9]*)|0 
FLOAT ({digits}\.{digits})|(({digit}*\.?{digits})|{digits}\.)[Ee][+-]?{digits}
ID [_a-zA-Z][_a-zA-Z0-9]*
SPACE " "|\n|\t|\r
SEMI ";"
COMMA ","
ASSIGNOP "="
RELOP ">"|"<"|">="|"<="|"=="|"!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT "."
NOT "!"
TYPE "int"|"float"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT "struct"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"

%%
\n          {yycolumn = 1;}
{SPACE}     {}
{SEMI}      {yylval.node = NewNode("SEMI",yytext);return SEMI;}
{COMMA}     {yylval.node = NewNode("COMMA",yytext);return COMMA;}
{ASSIGNOP}  {yylval.node = NewNode("ASSIGNOP",yytext);return ASSIGNOP;}
{RELOP}     {yylval.node = NewNode("RELOP",yytext);return RELOP;}
{PLUS}      {yylval.node = NewNode("PLUS",yytext);return PLUS;}
{MINUS}     {yylval.node = NewNode("MINUS",yytext);return MINUS;}
{STAR}      {yylval.node = NewNode("STAR",yytext);return STAR;}
{DIV}       {yylval.node = NewNode("DIV",yytext);return DIV;}
{AND}       {yylval.node = NewNode("AND",yytext);return AND;}
{OR}        {yylval.node = NewNode("OR",yytext);return OR;}
{DOT}       {yylval.node = NewNode("DOT",yytext);return DOT;}
{NOT}       {yylval.node = NewNode("NOT",yytext);return NOT;}
{TYPE}      {yylval.node = NewNode("TYPE",yytext);return TYPE;}
{LP}        {yylval.node = NewNode("LP",yytext);return LP;}
{RP}        {yylval.node = NewNode("RP",yytext);return RP;}
{LB}        {yylval.node = NewNode("LB",yytext);return LB;}
{RB}        {yylval.node = NewNode("RB",yytext);return RB;}
{LC}        {yylval.node = NewNode("LC",yytext);return LC;}
{RC}        {yylval.node = NewNode("RC",yytext);return RC;}
{STRUCT}    {yylval.node = NewNode("STRUCT",yytext);return STRUCT;}
{RETURN}    {yylval.node = NewNode("RETURN",yytext);return RETURN;}
{IF}        {yylval.node = NewNode("IF",yytext);return IF;}
{ELSE}      {yylval.node = NewNode("ELSE",yytext);return ELSE;}
{WHILE}     {yylval.node = NewNode("WHILE",yytext);return WHILE;}   
{INT}   {yylval.node = NewNode("INT",yytext);return INT;}
{FLOAT}   {yylval.node = NewNode("FLOAT",yytext);return FLOAT;}
{ID}   {yylval.node = NewNode("ID",yytext);return ID;}
.   {lexerror(yytext);}


%%