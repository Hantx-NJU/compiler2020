# ifndef __OBJECTCODE_H__
# define __OBJECTCODE_H__

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include "tree.h"
# include "semantic.h"
# include "intercode.h"

# define MAX_REG_NUM 32

extern FILE* fout;

typedef struct TempVar* pTempVar;
typedef struct TempVar {
    char name[32];
    int offset;     // 相对 fp 的地址，没有在内存中为 -1
    int regno;      // 存在几号寄存器中，没有在寄存器中为 -1  
    pOperand op;
    pTempVar next;  
} TempVar;

pTempVar vartab; // 当前处理的函数的变量表

typedef struct Register {
    char name[16];
    enum {FREE, BUSY} state;
    pTempVar content;           // 被替换出来时判断是否需要溢出
    pOperand op;
} Register;

struct Register regs[MAX_REG_NUM];

int framesize;  // 当前栈帧大小, 也即 sp-fp
int argnum;
int paramnum;
int hasArg;
int lastregno;
 
void assemble(pInterCodes list);        // 给 main 调用
void initregs();                        // 初始化寄存器
void initframe();                       // 初始化当前函数栈帧
void formatting(pInterCodes list);      // 打印头部格式化的汇编代码
void GetReg(pOperand op, char* reg_name, int kind);
void StoreReg(pOperand op, char* reg_name1, char* reg_name2, int kind);
void pushall();
void popall();
void check(pOperand op);


// 各种中间代码的翻译
void trans2objcode(pInterCodes list);  
void transASSIGN(pInterCodes list);
void transADD_SUB(pInterCodes list);
void transMUL_DIV(pInterCodes list);
void transGET_ADDR(pInterCodes list);
void transREAD_ADDR(pInterCodes list);
void transSTORE_ADDR(pInterCodes list);
void transIF_GOTO(pInterCodes list);
void transDEC(pInterCodes list);
void transREAD(pInterCodes list);
void transWRITE(pInterCodes list);
void transARG(pInterCodes list);
void transCALL(pInterCodes list);
void transPARAM(pInterCodes list);
void transRETURN(pInterCodes list);

# endif