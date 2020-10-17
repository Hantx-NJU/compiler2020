#ifndef __TREE_H__
#define __TREE_H__
typedef struct node{
    int lineno;
    char* name;
    int type;
    int num_of_child;
    struct node* children[20];
    union{
        int _int;
        float _float;
        char*_string;
    }val;
} Node;

typedef struct Node* pNode;

pNode root;

pNode NewNode(int lineno, char *name, int type,void * val);
void AddChild(pNode parent, pNode child);
void PreOrder(pNode node);

#endif