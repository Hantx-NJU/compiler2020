# ifndef __SEMANTIC_H__
# define __SEMANTIC_H__

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "tree.h"

# define HASHTABLE_MAX_SIZE  0x3fff

typedef struct Type_* pType;
typedef struct FieldList_* pFieldList;

enum Kind_ {BASIC, ARRAY, STRUCTURE, FUNCTION};
struct Type_{
    Kind_ kind;
    union{
        // BASIC 
        int basic;
        // ARRAY
        struct{pType elem; int size;} array;
        //  STRUCT
        pFieldList structure;
        // FUNCTION
        struct{
            int argc;       // number of parameters
            pFieldList argv;      // parameters
            pType ret;      // return value
        };
    }   u;
};

struct FieldList_   {
    char* name;
    pType type;
    pFieldList tail;    // useless in hash 
};

// 遍历语法树语义分析
void traverseTree(Node* root);

// Hash Table
pFieldList hashtable[HASHTABLE_MAX_SIZE];

// 与 Hash 相关的函数
unsigned int hash(char* name);
void initHashtable();

// 对每个产生式的处理函数
void Program(Node* node);
void ExtDefList(Node* node);
void ExtDef(Node* node);
void ExtDecList(Node* node, pType pt);  // 根据加入hashtable的位置选择是否传pt
pType Specifier(Node* node);
pType StructSpecifier(Node* node);
char* OptTag(Node* node);
char* Tag(Node* node);
pFieldList VarDec(Node* node,  pType pt);
pFieldList FunDec(Node* node, pType pt);
pFieldList VarList(Node* node, pType pt);
void CompSt(Node* node, pType pt);  // pt 为返回类型， 下同
void StmtList(Node* node, pType pt);
void Stmt(Node* node, pType pt);
void DefList(Node* node);
void Def(Node* node);
void DecList(Node* node, pType pt);
void Dec(Node* node, pType pt);
pType Exp(Node* node);
// Args建议附加到Exp中处理, 或 pFieldList Args(Node* node); 返回实参表



// 其他辅助函数
int isEqual(pType a, pType b);  // 判断两个类型是否相等
void show();    // 展示hashtable中的所有项

# endif