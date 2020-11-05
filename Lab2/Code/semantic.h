# ifndef     __SEMANTIC_H__
# define    __SEMANTIC_H__

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include "tree.h"

# define HASHTABLE_SIZE  0x3fff
# define BASIC_INT 0
# define BASIC_FLOAT 1

typedef struct Type_* pType;
typedef struct FieldList_* pFieldList;

enum Kind_ {BASIC, ARRAY, STRUCTURE, STRUCT_TAG, FUNCTION};
struct Type_{
    enum Kind_ kind;
    union{
        // BASIC 
        int basic;          // int-> 0, float->1
        // ARRAY
        struct{pType elem; int size;} array;
        // STRUCTURE
        pFieldList structure;
        // STRUCT_TAG
        pFieldList member;
        // FUNCTION
        struct{
            int argc;       // number of parameters
            pFieldList argv;      // parameters
            pType ret;      // return value
        }function;
    }   u;
};

struct FieldList_   {
    char* name;
    pType type;
    pFieldList tail;    
};

// Hash Table 存储所有定义内容：结构体定义 和 变量定义
pFieldList hashtable[HASHTABLE_SIZE];

// 遍历语法树语义分析
void traverseTree(Node* root);

// 与 Hash 相关的函数
unsigned int hash(char* name);  // hash 函数
void initHashtable();           // 初始化 hashtable
void insert(pFieldList pf);    // 插入 hash
int lookup(char* name);  // 查询 hash

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

// DEBUG 函数
void show();    // 展示hashtable中的所有项
void showtype(pType pt); // 展示类型

# endif