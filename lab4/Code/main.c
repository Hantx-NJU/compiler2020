# include <stdio.h>
# include "tree.h"
# include "semantic.h"
# include "intercode.h"

extern FILE* yyin;
extern FILE* fout;
extern int yylineno;
extern Node* root;
extern int yylex (void);
extern void yyrestart ( FILE *input_file  );
extern int yyparse();
int ErrorNum = 0;
int LexCurrentLineno = 0;
int SynCurrentLineno = 0;
void lexerror(char* msg)
{
    if(yylineno != LexCurrentLineno)
    {
        ++ErrorNum;
        printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, msg);
        LexCurrentLineno = yylineno;
    }
}
void synerror(char* msg) {
    if(yylineno != SynCurrentLineno)
    {
        ++ErrorNum;
        printf("Error type B at Line %d: \'%s\'\n",yylineno, msg);
        SynCurrentLineno = yylineno;
    }
}

int main(int argc, char** argv) {
    root = NULL;
    if(argc <= 1)   return 1;
    FILE* f =fopen(argv[1], "r");
    if(!f)
    {
        perror(argv[1]);
        return 1;
    }
    fout = fopen(argv[2],"w");
    if(!fout) {
        perror(argv[2]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    if(!ErrorNum && root)
    {
        //PreOrder(root, 0);    //We don't need to print parser tree in lab2
        //printf("\n");
        // 语义分析
        initHashtable();
        traverseTree(root);
        // 中间代码生成
        translate(root);
        fclose(fout);
    }
    return 0;
}