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


Node* root;

extern Node* NewNode(int lineno, char *name, int type,void * val);
extern void AddChild(Node* parent, Node* child);
extern void PreOrder(Node* node, int num_of_space);

#endif