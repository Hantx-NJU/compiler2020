# include "intercode.h"

void translate(Node* root) {
    init();
    if(root != NULL) {
        translate_Program(root);
        ShowAllInterCodes();
    }
}

void init() {
    global_temp_no = 0;
    global_label_no = 0;
    intercodeslist->next = intercodeslist;      // 虚假的头
    intercodeslist->prev = intercodeslist;
}

void ShowAllInterCodes() {
    pInterCodes iter = intercodeslist;
    while(iter->next != intercodeslist) {
        ShowInterCode(iter);
        iter = iter->next;
    }
}

void ShowInterCode(pInterCodes p) {
    assert(p != NULL);
    if(p->code.kind == LABEL) {
        printf("LABEL ");
        ShowOperand(p->code.u.singleOP.op);
        printf(" :");
    }
    else if(p->code.kind == FUNCTION) {
        printf("FUNCTION ");
        ShowOperand(p->code.u.singleOP.op);
        printf(" :");
    }
    else if(p->code.kind == ASSIGN) {
        ShowOperand(p->code.u.doubleOP.left);
        printf(" := ");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == ADD) {
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" + ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == SUB) {
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" - ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == MUL) {
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" * ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == DIV) {
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" / ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == GET_ADDR) {
        ShowOperand(p->code.u.doubleOP.left);
        printf(" := &");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == GO_ADDR) {
        ShowOperand(p->code.u.doubleOP.left);
        printf(" := *");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == STORE_ADDR) {
        printf("*")
        ShowOperand(p->code.u.doubleOP.left);
        printf(" := ");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == GOTO) {
        printf("GOTO ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == IF_GOTO) {
        printf("IF ");
        ShowOperand(p->code.u.if_gotoOP.op1);
        printf(" %s ", p->code.u.if_gotoOP.relop);
        ShowOperand(p->code.u.if_gotoOP.op2);
        printf(" GOTO ");
        ShowOperand(p->code.u.if_gotoOP.label);
    }
    else if(p->code.kind == RETURN) {
        printf("RETURN ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == DEC) {
        printf("DEC ");
        ShowOperand(p->code.u.decOP.op);
        printf(" %d", p->code.u.decOP.size);
    }
    else if(p->code.kind == ARG) {
        printf("ARG ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == CALL) {
        ShowOperand(p->code.u.doubleOP.left);
        printf(" := CALL ");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == PARAM) {
        printf("PARAM ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == READ) {
        printf("READ ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == WRITE) {
        printf("WRITE ");
        ShowOperand(p->code.u.singleOP.op);
    }
    printf("\n");
}

void ShowOperand(pOperand p){
    assert(p != NULL);
    if(p->kind == VARIABLE) printf("v%d", p->u.no);
    else if(p->kind == CONSTANT) printf("#%d", p->u.val);
    else if(p->kind == ADDRESS) printf("a%d", p->u.no);
    else if(p->kind == OPLABEL) printf("label%d", p->u.no);
    else if(p->kind == FUNCTION) printf("%s", p->u.name);
    else if(p->kind == STRUCTURE) printf("s%d", p->u.no);
}

