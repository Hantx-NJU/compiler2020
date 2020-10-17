#include "syntax.tab.h"
#include "tree.h"
# include<stdio.h>
# include<string.h>
# include<stdlib.h>

pNode NewNode(int lineno, char *name, int type,void * val){
    pNode p =  malloc(sizeof(Node));
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

void AddChild(pNode parent,pNode child){
   parent->children[parent->num_of_child++] = child;
}

void PreOrder(pNode node){
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

