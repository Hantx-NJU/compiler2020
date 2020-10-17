#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM 33
#define MAX_CHILD 10

extern int yylineno;

typedef struct TreeNode
{
    char name[MAX_NUM];
    char text[MAX_NUM];
    int lineno;
    struct TreeNode* child[MAX_CHILD];
    int childNum;
}TreeNode;

TreeNode* CreateNode(char *name, char* text);

void AddChild(TreeNode* child, int no);

void PrintTree(TreeNode* root);