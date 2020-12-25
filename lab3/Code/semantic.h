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
typedef struct Type_{
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
}Type_;

typedef struct FieldList_   {
    char* name;
    pType type;
    pFieldList tail;    
}FieldList_;

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
void VarDec(Node* node, pType pt, pFieldList pf); // pf 如果不为空表示这个 Var 是 pf 指向函数的形参或结构体类型的成员
void FunDec(Node* node, pType pt);
void VarList(Node* node, pFieldList pf); // 传 pf 是为了在 VarList 中改 pf 对应的 argc, argv
void ParamDec(Node* node, pFieldList pf);
void CompSt(Node* node, pType pt);  // pt 为返回类型， 下同
void StmtList(Node* node, pType pt);
void Stmt(Node* node, pType pt);
void DefList(Node* node, pFieldList pf); // pf 如果不为空表示这个 Def 是 pf 指向结构体类型的成员
void Def(Node* node, pFieldList pf);
void DecList(Node* node, pType pt, pFieldList pf);
void Dec(Node* node, pType pt, pFieldList pf);
pType Exp(Node* node);
pFieldList Args(Node* node);
// Args建议附加到Exp中处理, 或 pFieldList Args(Node* node); 返回实参表

// 其他辅助函数
int isEqual(pType a, pType b);  // 判断两个类型是否相等
int isArg(pFieldList pf);       // pf 是某个函数的参数吗
int getoffset(pFieldList pf);  // 计算成员在结构体中的偏移
int getSize(pType pt);

// DEBUG 函数
void show();    // 展示hashtable中的所有项
void showtype(pType pt); // 展示类型

# endif