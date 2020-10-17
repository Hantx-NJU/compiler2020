#include <stdio.h>

extern FILE* yyin;
extern int yylineno;
extern int yylex (void);
extern void yyrestart ( FILE *input_file  );
extern int yyparse();

int LexCurrentLineno = 0;
int SynCurrentLineno = 0;
void lexerror(char* msg)
{
    if(yylineno != LexCurrentLineno)
    {
        printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, msg);
        LexCurrentLineno = yylineno;
    }
}
void synerror(char* msg) {
    if(yylineno != SynCurrentLineno)
    {
        printf("Error type B at Line %d: \'%s\'\n",yylineno, msg);
        SynCurrentLineno = yylineno;
    }
}

int main(int argc, char** argv) {
    if(argc <= 1)   return 1;
    FILE* f =fopen(argv[1], "r");
    if(!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    return 0;
}