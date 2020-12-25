#ifndef __TREE_H__
#define __TREE_H__
#include "syntax.tab.h"
# include<stdio.h>
# include<string.h>
# include<stdlib.h>

#define MAX_LENGTH 33
#define MAX_CHILD 10
typedef struct Node{
    char name[MAX_LENGTH];
    char text[MAX_LENGTH];
    int lineno;
    int childSum;
    struct Node* children[MAX_CHILD];
} Node;


Node* root;

Node* NewNode(char* name, char* text);
void AddChild(Node* parent, Node* child);
void PreOrder(Node* node, int num);

#endif