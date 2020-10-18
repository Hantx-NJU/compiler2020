#include "tree.h"
extern int yylineno;

Node* NewNode(char* name, char* text){
    Node* p =  (Node*)malloc(sizeof(Node));
    p->lineno = yylineno;
    strcpy(p->name, name);
    strcpy(p->text, text);
    p->childSum = 0;
    for(int i = 0; i < MAX_CHILD; ++i)
    {
        p->children[i] = NULL;
    }
    return p;
}  

void AddChild(Node* parent,Node* child){
    if(parent->childSum == MAX_CHILD)   return;
    if(parent->childSum == 0)   parent->lineno = child->lineno;
    parent->children[parent->childSum] = child;
    ++parent->childSum;
}


void PreOrder(Node* node, int num){
    if(node == NULL) return;
    for(int i = 0; i < num; ++ i)
        printf(" ");
    if(node->childSum > 0)
    {
        printf("%s (%d)\n", node->name, node->lineno);
        for(int i = 0; i < node->childSum; ++i)
            PreOrder(node->children[i], num + 2);
    }
    else{
        printf("%s",node->name);
        if(strcmp(node->name, "ID") == 0 || strcmp(node->name, "TYPE") == 0)
        {
            printf(": %s", node->text);
        }
        else if(strcmp(node->name, "INT") == 0)
        {
            printf(": %d", atoi(node->text));
        }
        else if(strcmp(node->name, "FLOAT") == 0)
        {
            printf(": %f", atof(node->text));
        }
        printf("\n");
    }
}

