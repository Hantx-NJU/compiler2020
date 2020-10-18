#include "syntax.tab.h"
#include "tree.h"
# include<stdio.h>
# include<string.h>
# include<stdlib.h>

Node* NewNode(int lineno, char *name, int type,void * val){
    Node* p =  malloc(sizeof(Node));
    p->lineno = lineno;
    p->name = malloc(strlen(name) + 1);
    strcpy(p->name, name);
    p->num_of_child = 0;
    p->type = type;
    if(type == ID){
        p->val._string = malloc(strlen((char *)val)+1);
        strcpy(p->val._string, (char *)val);
        p->type = ID;
    }
    else if(type == TYPE){
        p->val._string = malloc(strlen((char *)val)+1);
        strcpy(p->val._string, (char *)val);
        p->type = TYPE;
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

void AddChild(Node* parent,Node* child){
   parent->children[parent->num_of_child++] = child;
}

void format(int n){
    for(int i = 0; i < n; ++i){
        printf(" ");
    }
}

void PreOrder(Node* node, int num_of_space){
    if(node == NULL) return;
    format(num_of_space);
    if(node->type == ID){
        printf("%s: %s\n", node->name, node->val._string);
    }
    else if(strcmp(node->name, "TYPE")==0){
        printf("%s: %s\n", node->name, node->val._string);
    }
    else if(node->type == INT){
        printf("%s: %d\n", node->name, node->val._int);
    }
    else if(node->type == FLOAT){
        printf("%s: %f\n", node->name, node->val._float);
    }
    else if(node->type != 0){
        printf("%s\n", node->name);
    }
    else{
        printf("%s (%d)\n", node->name, node->lineno);
    }
    for(int i = 0; i < node->num_of_child; ++i){
        PreOrder(node->children[i], num_of_space + 2);
    }
}

