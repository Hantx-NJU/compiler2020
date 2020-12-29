# ifndef __OBJECTCODE_H__
# define __OBJECTCODE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "semantic.h"
#include "intercode.h" 

extern FILE* fout;

typedef struct Register {
    char name[10];
    char var[32];
} Register;

//1.栈中存储数组的的话，a[0]的地址大于a[1]的地址，即从高地址到低地址放
//2.调用函数时不额外保存寄存器，将现有寄存器存到临时变量部分统一保存
//3.维护一个TempVarList来记录当前处理的函数出现的所有临时变量
typedef struct TempVar* pTempVar;
typedef struct TempVar {
    char var[32];
    int offset;
    int size;
    pTempVar next;
}TempVar;

pTempVar pTempVarList;

Register regs[32];

int spoffset;

void compiler(pInterCodes list);

void InitRegs();
void PreWork();
void StoreReg(int n);
void FirstStore(char* var, int size);
int GetReg(char* var);




#endif
