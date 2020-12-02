# ifndef     __INTERCODE_H__
# define     __INTERCODE_H__

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include "tree.h"

typedef struct Operand* pOperand;
typedef struct Operand{
    enum {VARIABLE, CONSTANT, ADDRESS, OPLABEL, FUNCTION, STRUCTURE} kind;
    union{
        int no;          // VARIABLE，ADDRESS, OPLABEL, STRUCTURE
        int val;         // CONSTANT
        char* name;      // FUNCTION
    } u;
} Operand;

typedef struct InterCode* pInterCode;
typedef struct InterCode {
    enum {LABEL, FUNCTION, ASSIGN, ADD, SUB, MUL, DIV, GET_ADDR, GO_ADDR, STORE_ADDR, GOTO, IF_GOTO,
          RETURN, DEC, ARG, CALL, PARAM, READ, WRITE} kind;
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
typedef struct ArgList {
    pOperand arg;
    ArgList* next;
} ArgList;

// 遍历语法树，完成翻译
void translate(Node* root);

// 对每个产生式的翻译函数
void translate_Program(Node* node);
void translate_ExtDefList(Node* node);
void translate_ExtDef(Node* node);
void translate_ExtDecList(Node* node);
void translate_Specifier(Node* node);
void translate_StructSpecifier(Node* node);
void translate_OptTag(Node* node);
void translate_Tag(Node* node);
void translate_VarDec(Node* node);
void translate_FunDec(Node* node);
void translate_VarList(Node* node);
void translate_ParamDec(Node* node);
void translate_CompSt(Node* node);
void translate_StmtList(Node* node);
pInterCodes translate_Stmt(Node* node);
void translate_DefList(Node* node);
void translate_Def(Node* node);
void translate_DecList(Node* node);
void translate_Dec(Node* node);
pInterCodes translate_Exp(Node* node, pOperand place);
pInterCodes translate_Args(Node* node, ArgList arg_list);

pInterCodes translate_Cond(Node* node, pOperand label_true, pOperand label_false);

// 其他翻译函数
int global_temp_no;
int global_label_no;
pOperand new_temp();
pOperand new_label();

// 其他功能函数
void init();
void ShowAllInterCodes();   // 显示全部
void ShowInterCode(pInterCodes p);  // 显示某行代码
void ShowOperand(pOperand p); // 显示某个操作数

#endif