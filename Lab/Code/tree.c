#include "syntax.tab.h"
# include<stdio.h>
# include<string.h>
# include<stdlib.h>

extern enum yytokentype;

typedef struct Node{
    int lineno;
    char* name;
    enum yytokentype type;
    int num_of_child;
    struct Node* children[20];
    union{
        int _int;
        float _float;
        char*_string;
    }val;
} Node;

struct Node* root;

Node *NewNode(int lineno, char *name, enum yytokentype type,void * val){
    Node* p =  malloc(sizeof(Node));
    p->lineno = lineno;
    p->name = malloc(strlen(name) + 1);
    strcpy(p->name, name);
    p->num_of_child = 0;
    p->type = 0;
    if(type == ID){
        p->val._string = malloc(strlen((char *)val)+1);
        strcpy(p->val._string, (char *)val);
        p->type = ID;
    }
    else if(type == INT){
        p->val._int = *(int *)val;
        p->type = INT;
    }
    else if(type == FLOAT){
        p->val._float= *(float *)val;
        p->type = FLOAT;
    }
}  

void Add(Node* parent, Node* child){
   parent->children[parent->num_of_child++] = child;
}

void PreOrder(Node* node){
    if(node == NULL) return;
    if(node->type == ID){
        printf("%s: %s\n", node->name, node->val);
    }
    else if(node->type == INT){
        printf("%s: %d\n", node->name, node->val);
    }
    else if(node->type == FLOAT){
        printf("%s: %f\n", node->name, node->val);
    }
    else{
        printf("%s (%d)\n", node->name, node->lineno);
    }
    for(int i = 0; i < node->num_of_child; ++i){
        PreOrder(node->children[i]);
    }
}
