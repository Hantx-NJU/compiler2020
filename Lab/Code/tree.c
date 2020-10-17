#include "tree.h"

TreeNode* CreateNode(char *name, char* text)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(node->name, name);
    strcpy(node->text, text);
    node->lineno = yylineno;
    for(int i = 0; i < MAX_CHILD; ++i)
        node->child[i] = NULL;
}

void AddChild(TreeNode* child, int no);

void PrintTree(TreeNode* root);