#include "objectcode.h"

void compiler(pInterCodes list){
    InitRegs();
    PreWork();
    pInterCodes iter = list->next;
    while(iter != list) {
        ShowInterCode(iter);
        iter = iter->next;
    }
}

int GetReg(char* var){
    for(int i = 8; i <= 15; ++i){
        if(strcmp(regs[i].var, var) == 0){
            return i;
        }
    }       
    int varoffset = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    pTempVar p = pTempVarList;
    while(p){
        if(strcmp(p->var, var) == 0){
            varoffset = p->offset;
            break;
        }
    }
    p = pTempVarList->next;

}

void StoreReg(int n){
    pTempVar p = pTempVarList;
    while(p){
        if(strcmp(p->var, regs[n].var) == 0){
            fprintf(fout, "sw %s, -%d(%%fp)", regs[n].name, p->offset);
            return;
        }
        p = p->next;
    }
    FirstStore(regs[n].var);
}

void FirstStore(char* var, int size){
    pTempVar p = pTempVarList;
    int offset = 0;
    if(!p){
        pTempVarList = (pTempVar)calloc(1, sizeof(TempVar));
        pTempVarList->offset = 12;
    }
    else{
        while(p->next)  p = p->next;
        offset = p->offset;
    }
}
void InitRegs(){
    // pTempVarList = (pTempVar)calloc(1, sizeof(TempVar));
    // pTempVarList->next = NULL;
    pTempVarList = NULL;
    spoffset = 0;
    strcpy(regs[0].name, "$zero");
    strcpy(regs[1].name, "$at");
    strcpy(regs[2].name, "$v0");
    strcpy(regs[3].name, "$v1");
    strcpy(regs[4].name, "$a0");
    strcpy(regs[5].name, "$a1");
    strcpy(regs[6].name, "$a2");
    strcpy(regs[7].name, "$a3");
    strcpy(regs[8].name, "$t0");
    strcpy(regs[9].name, "$t1");
    strcpy(regs[10].name, "$t2");
    strcpy(regs[11].name, "$t3");
    strcpy(regs[12].name, "$t4");
    strcpy(regs[13].name, "$t5");
    strcpy(regs[14].name, "$t6");
    strcpy(regs[15].name, "$t7");
    strcpy(regs[16].name, "$s0");
    strcpy(regs[17].name, "$s1");
    strcpy(regs[18].name, "$s2");
    strcpy(regs[19].name, "$s3");
    strcpy(regs[20].name, "$s4");
    strcpy(regs[21].name, "$s5");
    strcpy(regs[22].name, "$s6");
    strcpy(regs[23].name, "$s7");
    strcpy(regs[24].name, "$t8");
    strcpy(regs[25].name, "$t9");
    strcpy(regs[26].name, "$k0");
    strcpy(regs[27].name, "$k1");
    strcpy(regs[28].name, "$gp");
    strcpy(regs[29].name, "$sp");
    strcpy(regs[30].name, "$fp");
    strcpy(regs[31].name, "$ra");
}

void PreWork(){
    fprintf(fout,".data\n");
    fprintf(fout,"_prompt: .asciiz \"Enter integer(s):\"\n");
    fprintf(fout,"_ret: .asciiz \"\\n\"\n");
    fprintf(fout,".globl main\n");
    fprintf(fout,".text\n");
    fprintf(fout,"read:\n");
    fprintf(fout,"  li $v0, 4\n");
    fprintf(fout,"  la $a0, _prompt\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  li $v0, 5\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  jr $ra\n");
    fprintf(fout,"\n");
    fprintf(fout,"write:\n");
    fprintf(fout,"  li $v0, 1\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  li $v0, 4\n");
    fprintf(fout,"  la $a0, _ret\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  move $v0, $0\n");
    fprintf(fout,"  jr $ra\n");
}