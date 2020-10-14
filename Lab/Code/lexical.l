%option yylineno
digit   [0-9]
digits [0-9]+
letter  [a-zA-z]
space   " "|\n|\t|\r
%%
([1-9]+{digit}*)|0[0-7]*|0[Xx][a-fA-F]+    {printf("INT\n");}
({digits}\.{digits})|(({digit}*\.?{digits})|{digits}\.)[Ee][+-]?{digits}    {printf("FLOAT\n");}
";"  {printf("SEMI\n");}
","   {printf("COMMA\n");}
"=" {printf("ASSIGNOP\n");}
">"|"<"|">="|"<="|"=="|"!=" {printf("RELOP\n");}
"+" {printf("PLUS\n");}
"-" {printf("MINUS\n");}
"*" {printf("STAR\n");}
"/" {printf("DIV\n");}
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
.   {printf("Error type A at Line %d : Mysterious characters  \'%s\'\n", yylineno, yytext);}
%%
int main(int argc, char** argv) {
    if(argc > 1) {
        if(! (yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;   
        }
    }
    yylex();
    return 0;
}