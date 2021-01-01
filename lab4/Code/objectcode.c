# include "objectcode.h"

void assemble(pInterCodes list) {
    // init registers
    initregs();
    /* print public codes */
    formatting(list);
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

void clearregs(){
    for(int i = 8; i <= 25; ++i) {
        regs[i].state = FREE;
        regs[i].content = NULL;
    }
}

void initframe() {
    // DONE 
    // 重置 vartable, framesize
    // 保存旧 fp
    // 移动 fp 到 sp
    clearregs();
    vartab = NULL;
    framesize = 0;
    fprintf(fout, "  addi $sp, $sp, -4\n");
    fprintf(fout, "  sw $fp, ($sp)\n");
    fprintf(fout, "  move $fp, $sp\n");
    // 可能有误，你可以参考计算机系统基础的教材
    // assert(0);
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

int GetRegNo(pOperand op) {
    if(op->kind == OPCONSTANT) {   // var 可能是立即数, 需要先加载到寄存器中 
        int no = AllocateReg(op,NULL);
        fprintf(fout,"  li $%s, %d\n",regs[no].name,op->u.val);
        return no;
    }
    if(op->kind == OPTEMP) sprintf(op->u.name,"t%d",op->u.no);
    for(pTempVar iter = vartab; iter != NULL; iter = iter->next) {
        if(strcmp(op->u.name, iter->name) == 0) {
            if(iter->regno == -1) iter->regno = AllocateReg(op,iter);
            return iter->regno;
        }
    }
    // 不在变量表中
    pTempVar p = newpTempVar(op->u.name);
    // 插入变量表
    insertTempVar(p);
    // 分配寄存器
    p->regno = AllocateReg(op,p);
    return p->regno;
}

int GetAddr(pOperand op) {
    // 对于数组，在DEC中已经将 op 加入 vartab, 这里直接查找并返回基地址在栈帧中的偏移
    for(pTempVar iter = vartab; iter != NULL; iter = iter->next) {
        if(strcmp(op->u.name,iter->name) == 0) {
            return iter->offset;
        }
    }
    // 除此之外，这个函数应该不会调用，如果调用，请在下面填写
    assert(0);
}

int AllocateReg(pOperand op, pTempVar var) {
    /* 如果有空闲直接分配 */
    for(int i = 8; i <= 25; ++i) {  // t0-t7, s0-s7, t8-t9
        if(regs[i].state == FREE) {
            regs[i].state = BUSY;
            regs[i].content = var;
            return i;
        }
    }
    /* 没有空闲, 进行调度 */
    // 优先换出立即数
    for(int i = 8; i <= 25; ++i) {      
        if(regs[i].content == NULL) {
            regs[i].content = var;
            return i;
        }
    }
    // 选择换入换出寄存器
    int change = -1;
    for(int i = 8; i <= 25; ++i) {
        if(regs->content->name[0] == 't'){   // 优先换出 OPTEAMP
            change = i;
            break;
        }
    }
    if(change == -1) change = rand() % 18 + 8;    // 随机选出一个替换
    // 换出(溢出到内存)
    if(var->offset == -1) { // 没有在栈帧中则分配位置, 并溢出
        fprintf(fout,"  addi $sp, $sp, -4\n"); 
        fprintf(fout,"  sw $%s, 0($sp)\n",regs[change].name);     // 存到栈顶
        var->offset = framesize;
        framesize += 4;
    } 
    else fprintf(fout,"  sw $%s, -%d($fp)\n",regs[change].name,var->offset); // 已经在栈帧中分配了位置
    // 换入
    regs[change].content = var;
    return change;
}

pTempVar newpTempVar(char* name) {
    pTempVar p = (pTempVar)calloc(1, sizeof(TempVar));
    strcpy(p->name,name);
    p->next= NULL;
    p->offset = -1;
    p->regno = -1;
}

void insertTempVar(pTempVar ptv) {
    if(vartab == NULL) vartab = ptv;
    else {
        ptv->next = vartab;
        vartab = ptv;
    }
}

void trans2objcode(pInterCodes list) {
    if(list->code.kind == CDLABEL) {
        fprintf(fout,"label%d:\n",list->code.u.singleOP.op->u.no);
    } else if(list->code.kind == CDFUNCTION) {
        fprintf(fout,"\n%s:\n",list->code.u.singleOP.op->u.name);
        initframe();    // 为这个函数初始化栈帧
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
        // TODO
        // 恢复寄存器,恢复fp,sp,取出返回地址等, 你也可以自己设计或参考ICS
        transRETURN(list);
    } else if(list->code.kind == CDDEC) {
        transDEC(list);
    } else if(list->code.kind == CDARG) {
        // DONE
        transARG(list);
        // 传参，参考手册 109 页
        // 这里建议在第一次遇到ARG时，就顺着list把所有参数传入了，这样可能更方便判断哪些放到栈中，哪些放到寄存器中 
        // assert(0);
    } else if(list->code.kind == CDCALL) {
        // DONE
        transCALL(list);
        // assert(0);
    } else if(list->code.kind == CDPARAM) {
        // DONE
        transPARAM(list);
        // assert(0);
    } else if(list->code.kind == CDREAD) {
        transREAD(list);
    } else if(list->code.kind == CDWRITE) {
        transWRITE(list);
    }
    else assert(0);
}

void transRETURN(pInterCodes list){
    int reg_x = GetRegNo(list->code.u.singleOP.op);
    fprintf(fout, "  addi $sp, $sp, %d\n", framesize);
    fprintf(fout, "  lw $fp, 0($sp)\n");
    fprintf(fout, "  lw $ra, 4($sp)\n");
    fprintf(fout, "  addi $sp, $sp, 8\n");
    fprintf(fout, "  move $v0, $%s\n",regs[reg_x].name);
    fprintf(fout, "  jr $ra\n");
}

void transPARAM(pInterCodes list){
    if(list->prev->code.kind == CDPARAM)    return;
    int count = 0;
    while(list->code.kind == CDPARAM){
        pTempVar p = newpTempVar(list->code.u.singleOP.op->u.name);
        insertTempVar(p);
        p->regno = AllocateReg(list->code.u.singleOP.op, p);
        int n = 8 + 4 * count;
        fprintf(fout, "  lw $%s, %d($fp)\n", regs[p->regno].name, n);
        ++count;
        list = list->next;
    }
}

void transCALL(pInterCodes list){
    fprintf(fout, "  addi $sp, $sp, -4\n");
    fprintf(fout, "  sw $ra, 0($sp)\n");
    fprintf(fout, "  jal %s\n", list->code.u.doubleOP.right->u.name);
    fprintf(fout, "  lw $ra, 0($sp)\n");
    fprintf(fout, "  addi $sp, $sp, 4\n");
    int n = GetRegNo(list->code.u.doubleOP.left);
    fprintf(fout, "  move $%s, $v0\n", regs[n].name);
}

void transARG(pInterCodes list){
    if(list->prev->code.kind == CDARG)  return;
    //all store in stack
    while(list->code.kind == CDARG){
        pTempVar p = vartab;
        for(; p; p = p->next)
            if(p->regno == -1)  continue;
            if(p->offset == -1){
                fprintf(fout,"  addi $sp, $sp, -4\n"); 
                fprintf(fout,"  sw $%s, 0($sp)\n",regs[p->regno].name);     // 存到栈顶
                p->offset = framesize;
                framesize += 4;
            }
            else fprintf(fout,"  sw $%s, -%d($fp)\n",regs[p->regno].name,p->offset);
    }
        clearregs();
        fprintf(fout, "  addi $sp, $sp, -4\n");
        framesize += 4;
        int no = GetRegNo(list->code.u.singleOP.op);
        fprintf(fout, "  sw $%s, ($sp)\n", regs[no].name);
        list = list->next;
}

void transASSIGN(pInterCodes list) {
    if(list->code.u.doubleOP.right->kind == OPCONSTANT) {
        int reg_x = GetRegNo(list->code.u.doubleOP.left);
        fprintf(fout,"  li $%s, %d\n",regs[reg_x].name,list->code.u.doubleOP.right->u.val);
    }
    else {
        int reg_x = GetRegNo(list->code.u.doubleOP.left);
        int reg_y = GetRegNo(list->code.u.doubleOP.right);
        fprintf(fout,"  move $%s, $%s\n",regs[reg_x].name,regs[reg_y].name);
    }
}

void transADD_SUB(pInterCodes list) {
    if(list->code.u.tripleOP.op2->kind != OPCONSTANT) {
        // x := y + z or x := y - z
        int reg_x = GetRegNo(list->code.u.tripleOP.result);
        int reg_y = GetRegNo(list->code.u.tripleOP.op1);
        int reg_z = GetRegNo(list->code.u.tripleOP.op2);
        if(list->code.kind == CDADD)
            fprintf(fout,"  add $%s, $%s, $%s\n",regs[reg_x].name,regs[reg_y].name,regs[reg_z].name);
        else  fprintf(fout,"  sub $%s, $%s, $%s\n",regs[reg_x].name,regs[reg_y].name,regs[reg_z].name);
    }
    else {
        // x := y + #k or x := y - #k
        int reg_x = GetRegNo(list->code.u.tripleOP.result);
        int reg_y = GetRegNo(list->code.u.tripleOP.op1);
        if(list->code.kind == CDADD)
            fprintf(fout,"  addi $%s, $%s, %d\n",regs[reg_x].name,regs[reg_y].name,list->code.u.tripleOP.op2->u.val);
        else  fprintf(fout,"  addi $%s, $%s, %d\n",regs[reg_x].name,regs[reg_y].name,-(list->code.u.tripleOP.op2->u.val));
    }
}

void transMUL_DIV(pInterCodes list) {
    int reg_x = GetRegNo(list->code.u.tripleOP.result);
    int reg_y = GetRegNo(list->code.u.tripleOP.op1);
    int reg_z = GetRegNo(list->code.u.tripleOP.op2);
    if(list->code.kind == CDMUL)
        fprintf(fout,"  mul $%s, $%s, $%s\n",regs[reg_x].name,regs[reg_y].name,regs[reg_z].name);
    else {
        fprintf(fout,"  div $%s, $%s\n",regs[reg_y].name,regs[reg_z].name);
        fprintf(fout,"  mflo $%s\n",regs[reg_x].name);
    }
}

void transGET_ADDR(pInterCodes list) {
    // x = &y
    int reg_x = GetRegNo(list->code.u.doubleOP.left);
    int addr = GetAddr(list->code.u.doubleOP.right);
    fprintf(fout,"  addi $%s, $fp, %d\n",regs[reg_x].name,addr);     //  li or la ?
}

void transREAD_ADDR(pInterCodes list) {
    int reg_x = GetRegNo(list->code.u.doubleOP.left);
    int reg_y = GetRegNo(list->code.u.doubleOP.right);
    fprintf(fout,"  lw $%s, 0($%s)\n",regs[reg_x].name,regs[reg_y].name);
}

void transSTORE_ADDR(pInterCodes list) {
    int reg_x = GetRegNo(list->code.u.doubleOP.left);
    int reg_y = GetRegNo(list->code.u.doubleOP.right);
    fprintf(fout,"  sw $%s, 0($%s)\n",regs[reg_y].name,regs[reg_x].name);
}

void transIF_GOTO(pInterCodes list) {
    // IF x [relop] y GOTO z
    int reg_x = GetRegNo(list->code.u.if_gotoOP.op1);
    int reg_y = GetRegNo(list->code.u.if_gotoOP.op2);
    if(strcmp(list->code.u.if_gotoOP.relop,"==") == 0) {
        fprintf(fout,"  beq $%s, $%s, label%d\n",regs[reg_x].name,regs[reg_y].name,list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,"!=") == 0) {
        fprintf(fout,"  bne $%s, $%s, label%d\n",regs[reg_x].name,regs[reg_y].name,list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,">") == 0) {
        fprintf(fout,"  bgt $%s, $%s, label%d\n",regs[reg_x].name,regs[reg_y].name,list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,"<") == 0) {
        fprintf(fout,"  blt $%s, $%s, label%d\n",regs[reg_x].name,regs[reg_y].name,list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,">=") == 0) {
        fprintf(fout,"  bge $%s, $%s, label%d\n",regs[reg_x].name,regs[reg_y].name,list->code.u.if_gotoOP.label->u.no);
    } else if(strcmp(list->code.u.if_gotoOP.relop,"<=") == 0) {
        fprintf(fout,"  ble $%s, $%s, label%d\n",regs[reg_x].name,regs[reg_y].name,list->code.u.if_gotoOP.label->u.no);
    }
}


void transDEC(pInterCodes list) {
    // 将数组插入临时变量表
    pTempVar p = newpTempVar(list->code.u.decOP.op->u.name);
    p->offset = framesize;
    insertTempVar(p);
    // 在栈帧中分配空间
    fprintf(fout,"  addi $sp, $sp, -%d\n",list->code.u.decOP.size);
    framesize += list->code.u.decOP.size;
}

void transREAD(pInterCodes list) {
    /* reference to hand book*/
    fprintf(fout,"  addi $sp, $sp, -4\n");
    fprintf(fout,"  sw $ra, 0($sp)\n");
    fprintf(fout,"  jal read\n");
    fprintf(fout,"  lw $ra, 0($sp)\n");
    fprintf(fout,"  addi $sp, $sp, 4\n");
    int reg_x = GetRegNo(list->code.u.singleOP.op);
    fprintf(fout,"  move $%s, $v0\n",regs[reg_x].name);
}

void transWRITE(pInterCodes list) {
    /* reference to hand book*/
    int reg_x = GetRegNo(list->code.u.singleOP.op);
    fprintf(fout,"  move $a0, $%s\n",regs[reg_x].name);
    fprintf(fout,"  addi $sp, $sp, -4\n");
    fprintf(fout,"  sw $ra, 0($sp)\n");
    fprintf(fout,"  jal write\n");
    fprintf(fout,"  lw $ra, 0($sp)\n");
    fprintf(fout,"  addi $sp, $sp, 4\n");
}