# ifndef     __INTERCODE_H__
# define     __INTERCODE_H__

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include "tree.h"

typedef struct Operand* pOperand;
typedef struct Operand{
    enum {VARIABLE, CONSTANT, TEMP, OPLABEL, OPFUNCTION, OPSTRUCTURE} kind;
    union{
        int no;          // TEMP, OPLABEL
        int val;         // CONSTANT
        char name[32];   // FUNCTION, VARIABLE, STRUCTURE (由于不重名，直接用符号表中的名字)
    } u;
} Operand;

typedef struct InterCode* pInterCode;
typedef struct InterCode {
    enum {LABEL, CDFUNCTION, ASSIGN, ADD, SUB, CDMUL, CDDIV, 
            GET_ADDR, GO_ADDR, STORE_ADDR, GOTO, IF_GOTO,
          CDRETURN, DEC, ARG, CALL, PARAM, READ, WRITE} kind;
    union{
        // LABEL x : || FUNCTION f : || GOTO x || RETURN x || ARG x || PARAM x || READ x || WRITE x
        struct {
            pOperand op;
        }singleOP;
        // x := y || x := &y || x := *y || *x := y || x := CALL f
        struct{
            pOperand left, right;
        }doubleOP;
        // x := y + z || x := y - z || x := y * z || x := y / z
        struct{
            pOperand result, op1, op2;
        }tripleOP;
        // IF x [relop] y GOTO z
        struct{
            pOperand op1, op2, label;
            char relop[32];
        }if_gotoOP;
        // DEC x [size]
        struct{
            pOperand op;
            int size;
        }decOP;
    } u;
} InterCode;

// 双向链表存储中间代码
typedef struct InterCodes* pInterCodes;
typedef struct InterCodes {
    InterCode code;
    pInterCodes prev;
    pInterCodes next;
} InterCodes;

pInterCodes intercodeslist;

// ArgList (单向链表)
typedef struct ArgList* pArgList;
typedef struct ArgList {
    pOperand arg;
    pArgList next;
} ArgList;

// 遍历语法树，完成翻译
void translate(Node* root);

// 产生式的翻译函数
void translate_Program(Node* node);
void translate_ExtDefList(Node* node);
void translate_ExtDef(Node* node);          // 所有中间代码以函数自然分块，故在 ExtDef 中就可以链接到 intercodeslist 里面了
pInterCodes translate_FunDec(Node* node);   // 传回的 pInterCodes 指向第一行中间代码(没有假头节点)，注意和 intercodeslist 区分(有)
pInterCodes translate_CompSt(Node* node);
pInterCodes translate_StmtList(Node* node);
pInterCodes translate_Stmt(Node* node);
pInterCodes translate_DefList(Node* node);
pInterCodes translate_Def(Node* node);
pInterCodes translate_DecList(Node* node);
pInterCodes translate_Dec(Node* node);
pInterCodes translate_Exp(Node* node, pOperand place);
pInterCodes translate_Args(Node* node, pArgList arg_list);

pInterCodes translate_Cond(Node* node, pOperand label_true, pOperand label_false);

// 其他翻译函数
int global_temp_no;
int global_label_no;
pOperand new_temp();
pOperand new_label();
pInterCodes Exp_to_Cond(Node* node, pOperand place);

// 其他功能函数
void init();
void ShowAllInterCodes();   // 显示全部
void ShowInterCode(pInterCodes p);  // 显示某行代码
void ShowOperand(pOperand p); // 显示某个操作数
void concat(pInterCodes p1, pInterCodes p2); // 将 p2 连接到 p1 后面
void InsertArg(pArgList arg_list, pOperand t); // arg_list = t1 + arg_list（注意顺序）
pInterCodes new_pInterCodes();      // 分配内存并清0
pOperand new_pOperand(); 

#endif