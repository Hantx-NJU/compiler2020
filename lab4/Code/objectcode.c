# include "objectcode.h"

void assemble(pInterCodes list) {
    // init registers
    initregs();
    /* print public codes */
    formatting(list);
    argnum = 0;
    paramnum = 0;
    hasArg = 0;
    lastregno = 0;
    /* translate to object code */
    pInterCodes iter = list->next;
    while(iter != list) {
        trans2objcode(iter);
        iter = iter->next;
    }
}

void initregs() {
    const char* names[MAX_REG_NUM] = {"zero","at","v0","v1","a0","a1","a2","a3",
                                      "t0","t1","t2","t3","t4","t5","t6","t7",
                                      "s0","s1","s2","s3","s4","s5","s6","s7",
                                      "t8","t9","k0","k1","gp","sp","fp","ra"};         // 注意，我们使用帧指针 fp
    for(int i = 0; i < MAX_REG_NUM; ++i) {
        strcpy(regs[i].name,names[i]);
        regs[i].state = FREE;
        regs[i].content = NULL;
    }
}

void formatting(pInterCodes list) {
    // head
    fprintf(fout,".data\n");
    fprintf(fout,"_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(fout,"_ret: .asciiz \"\\n\"\n");
    fprintf(fout,".globl main\n");
    
    // read
    fprintf(fout,".text\n");
    fprintf(fout,"read:\n");
    fprintf(fout,"  li $v0, 4\n");
    fprintf(fout,"  la $a0, _prompt\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  li $v0, 5\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  jr $ra\n");
    fprintf(fout,"\n");

    // write
    fprintf(fout,"write:\n");
    fprintf(fout,"  li $v0, 1\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  li $v0, 4\n");
    fprintf(fout,"  la $a0, _ret\n");
    fprintf(fout,"  syscall\n");
    fprintf(fout,"  move $v0, $0\n");
    fprintf(fout,"  jr $ra\n");
}

void check(pOperand op){
    fprintf(fout, "%d  %d\n", op->kind, op->num);
    if(op->kind == OPADDRESS || op->kind == OPVARIABLE || op->kind == OPARRAY){
        fprintf(fout, "%s\n", op->u.name);
    }
    else if(op->kind == OPTEMP){
        fprintf(fout, "%d\n", op->u.no);
    }
}

void GetReg(pOperand op, char* reg_name, int kind) {
    if(kind == 0){
        //Address
        // check(op);
        fprintf(fout, "  addi $%s, $fp, %d\n", reg_name, -4 * op->num);
        return;
    }
    if(kind == 1){
        //Pointer
        fprintf(fout, "  lw $%s, %d($fp)\n", reg_name, -4 * op->num);
        fprintf(fout, "  lw $%s, 0($%s)\n", reg_name, reg_name);
        return;
    }
    if(op->kind == OPVARIABLE || op->kind == OPTEMP || op->kind == OPADDRESS || op->kind == OPARRAY){
        fprintf(fout, "  lw $%s, %d($fp)\n", reg_name, -4 * op->num);
        return;
    }
    if(op->kind == OPCONSTANT) {   // var 可能是立即数, 需要先加载到寄存器中 
        fprintf(fout,"  li $%s, %d\n",reg_name,op->u.val);
        return;
    }
    // if(op->kind == OPTEMP) sprintf(op->u.name,"t%d",op->u.no);
    return;
}

void StoreReg(pOperand op, char* reg_name1, char* reg_name2, int kind) {
    // fprintf(fout, "%d  %d\n", op->kind, op->num);
    // if(op->kind == OPADDRESS || op->kind == OPVARIABLE){
    //     fprintf(fout, "%s\n", op->u.name);
    // }
    // else if(op->kind == OPTEMP){
    //     fprintf(fout, "%d\n", op->u.no);
    // }
    if(kind == 1){
        //Pointer
        fprintf(fout, "  lw $%s, %d($fp)\n", reg_name1, -4 * op->num);
        fprintf(fout, "  sw $%s, 0($%s)\n", reg_name2, reg_name1);
    }
    // else if(op->kind == OPVARIABLE || op->kind == OPTEMP){
    else{
        // check(op);
        fprintf(fout, "  sw $%s, %d($fp)\n", reg_name2, -4 * op->num);
    }
    return;
}

void trans2objcode(pInterCodes list) {
    if(list->code.kind != CDPARAM)  paramnum = 0;
    if(list->code.kind == CDLABEL) {
        fprintf(fout,"label%d:\n",list->code.u.singleOP.op->u.no);
    } else if(list->code.kind == CDFUNCTION) {
        fprintf(fout,"\n%s:\n",list->code.u.singleOP.op->u.name);
        fprintf(fout, "  move $fp, $sp\n");
    } else if(list->code.kind == CDASSIGN) {
        transASSIGN(list);
    } else if(list->code.kind == CDADD || list->code.kind == CDSUB) {
        transADD_SUB(list);
    } else if(list->code.kind == CDMUL || list->code.kind == CDDIV) {
        transMUL_DIV(list);
    } else if(list->code.kind == CDGET_ADDR) {
        transGET_ADDR(list);
    } else if(list->code.kind == CDREAD_ADDR) {
        transREAD_ADDR(list);
    } else if(list->code.kind == CDSTORE_ADDR) {
        transSTORE_ADDR(list);
    } else if(list->code.kind == CDGOTO) {
        fprintf(fout,"  j label%d\n",list->code.u.singleOP.op->u.no);
    } else if(list->code.kind == CDIF_GOTO) {
        transIF_GOTO(list);
    } else if(list->code.kind == CDRETURN) {
        transRETURN(list);
    } else if(list->code.kind == CDDEC) {
        transDEC(list);
    } else if(list->code.kind == CDARG) {
        transARG(list);
        // 传参，参考手册 109 页
        // 这里建议在第一次遇到ARG时，就顺着list把所有参数传入了，这样可能更方便判断哪些放到栈中，哪些放到寄存器中 
    } else if(list->code.kind == CDCALL) {
        transCALL(list);
    } else if(list->code.kind == CDPARAM) {
        transPARAM(list);
    } else if(list->code.kind == CDREAD) {
        transREAD(list);
    } else if(list->code.kind == CDWRITE) {
        transWRITE(list);
    }
    else assert(0);
}

void transRETURN(pInterCodes list){
    GetReg(list->code.u.singleOP.op, "v0", -1);
    fprintf(fout, "  jr $ra\n");
}

void transPARAM(pInterCodes list){
    fprintf(fout, "  lw $t0, %d($fp)\n", 8 + 4 * paramnum);
    ++paramnum;
    fprintf(fout, "  sw $t0, %d($fp)\n", -4 * list->code.u.singleOP.op->num);
}

void transCALL(pInterCodes list){
    fprintf(fout, "  addi $sp, $sp, -8\n");
    fprintf(fout, "  sw $ra, 4($sp)\n");
    fprintf(fout, "  sw $fp, 0($sp)\n");

    fprintf(fout, "  jal %s\n", list->code.u.doubleOP.right->u.name);
    
    fprintf(fout, "  move $sp, $fp\n");
    fprintf(fout, "  lw $fp, 0($sp)\n");
    fprintf(fout, "  lw $ra, 4($sp)\n");

    fprintf(fout, "  addi $sp, $sp, %d\n", 8 + 4 * argnum);
    argnum=0;
    
    StoreReg(list->code.u.doubleOP.left, "t0", "v0", -1);
}

void transARG(pInterCodes list){
    paramnum = 0;
    ++argnum;
    GetReg(list->code.u.singleOP.op, "t0", -1);
    fprintf(fout, "  addi $sp, $sp, -4\n");
    fprintf(fout, "  sw $t0, 0($sp)\n");
}

void transASSIGN(pInterCodes list) {
    GetReg(list->code.u.doubleOP.right, "t0", -1);
    StoreReg(list->code.u.doubleOP.left, "t1", "t0", -1);
}

void transADD_SUB(pInterCodes list) {
    GetReg(list->code.u.tripleOP.op1, "t0", -1);
    GetReg(list->code.u.tripleOP.op2, "t1", -1);
    if(list->code.kind == CDADD){
        fprintf(fout, "  add $t2, $t0, $t1\n");
    }
    else{
        fprintf(fout, "  sub $t2, $t0, $t1\n");
    }
    StoreReg(list->code.u.tripleOP.result, "t3", "t2", -1);
}

void transMUL_DIV(pInterCodes list) {
    GetReg(list->code.u.tripleOP.op1, "t0", -1);
    GetReg(list->code.u.tripleOP.op2, "t1", -1);
    if(list->code.kind == CDMUL){
        fprintf(fout, "  mul $t2, $t0, $t1\n");
    }
    else{
        fprintf(fout, "  div $t0, $t1\n");
        fprintf(fout, "  mflo $t2\n");
    }
    StoreReg(list->code.u.tripleOP.result, "t3", "t2", -1);
}

void transGET_ADDR(pInterCodes list) {
    // x = &y
    GetReg(list->code.u.doubleOP.right, "t0", 0);
    StoreReg(list->code.u.doubleOP.left, "t1", "t0", -1);
}

void transREAD_ADDR(pInterCodes list) {
    GetReg(list->code.u.doubleOP.right, "t0", 1);
    StoreReg(list->code.u.doubleOP.left, "t1", "t0", -1);
}

void transSTORE_ADDR(pInterCodes list) {
    GetReg(list->code.u.doubleOP.right, "t0", -1);
    StoreReg(list->code.u.doubleOP.left, "t1", "t0", 1);
}

void transIF_GOTO(pInterCodes list) {
    // IF x [relop] y GOTO z
    GetReg(list->code.u.if_gotoOP.op1, "t0", -1);
    GetReg(list->code.u.if_gotoOP.op2, "t1", -1);
    if(strcmp(list->code.u.if_gotoOP.relop,"==") == 0) {
        fprintf(fout,"  beq $t0, $t1, label%d\n", list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,"!=") == 0) {
        fprintf(fout,"  bne $t0, $t1, label%d\n", list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,">") == 0) {
        fprintf(fout,"  bgt $t0, $t1, label%d\n", list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,"<") == 0) {
        fprintf(fout,"  blt $t0, $t1, label%d\n", list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,">=") == 0) {
        fprintf(fout,"  bge $t0, $t1, label%d\n", list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,"<=") == 0) {
        fprintf(fout,"  ble $t0, $t1, label%d\n", list->code.u.if_gotoOP.label->u.no);
    }
}


void transDEC(pInterCodes list) {
    if(list->code.u.decOP.size > 0){
        fprintf(fout, "  addi $sp, $sp, %d\n", -list->code.u.decOP.size);
    }
}

void transREAD(pInterCodes list) {
    /* reference to hand book*/
    fprintf(fout,"  addi $sp, $sp, -4\n");
    fprintf(fout,"  sw $ra, 0($sp)\n");
    fprintf(fout,"  jal read\n");
    fprintf(fout,"  lw $ra, 0($sp)\n");
    fprintf(fout,"  addi $sp, $sp, 4\n");
    StoreReg(list->code.u.singleOP.op, "t0", "v0", -1);
}

void transWRITE(pInterCodes list) {
    /* reference to hand book*/
    GetReg(list->code.u.singleOP.op, "a0", -1);
    fprintf(fout,"  addi $sp, $sp, -4\n");
    fprintf(fout,"  sw $ra, 0($sp)\n");
    fprintf(fout,"  jal write\n");
    fprintf(fout,"  lw $ra, 0($sp)\n");
    fprintf(fout,"  addi $sp, $sp, 4\n");
}

void pushall(){
    fprintf(fout, "  addi $sp, $sp, -72\n");
    for(int i = 8; i <= 25; ++i){
        fprintf(fout, "  sw %s, %d(sp)\n", regs[i].name, (i - 8) * 4);
    }
}

void popall(){
    for(int i = 8; i <= 25; ++i){
        fprintf(fout, "  lw %s, %d($sp)\n", regs[i].name, (i - 8) * 4);
    }
    fprintf(fout, "  addi $sp, $sp, 72\n");
}