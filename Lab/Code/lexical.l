%option yylineno

digit [0-9]
letter [_a-zA-Z]

INT (-?[1-9][0-9]*)|0 
FLOAT -?[0-9]+.[0-9]+
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
TYPE int|float
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while

%%
{SPACE}  {}
{SEMI}  {printf("SEMI\n");}
{COMMA} {printf("COMMA\n");}
{ASSIGNOP}  {printf("ASSIGNOP\n");}
{RELOP} {printf("RELPO:%s\n", yytext);}
{PLUS}  {return ADD;}
{MINUS} {return SUB;}
{STAR}  {return MUL;}
{DIV}   {return DIV;}
{AND}   {printf("AND\n");}
{OR}    {printf("OR\n");}
{DOT}   {printf("DOT\n");}
{NOT}   {printf("NOT\n");}
{TYPE}  {printf("TYPE: %s\n", yytext);}
{LP}    {printf("LP\n");}
{RP}    {printf("RP\n");}
{LB}    {printf("LB\n");}
{RB}    {printf("RB\n");}
{LC}    {printf("LC\n");}
{RC}    {printf("RC\n");}
{INT}   {yylval.type_int = atoi(yytext);    return INT;}
{FLOAT}   {yylval.type_float = atof(yytext);   return FLOAT;}
{ID}   {printf("ID: %s\n", yytext);}
.   {printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}


%%
