# include "intercode.h"
# include "semantic.h"

void translate(Node* root) {
    init();
    if(root != NULL) {
        translate_Program(root);
        ShowAllInterCodes();
    }
}

void translate_Program(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 1);
    translate_ExtDefList(node->children[0]);    // Program -> ExtDefList
}

void translate_ExtDefList(Node* node) {
    if(node == NULL) return;
    assert(node->childSum == 2);
    translate_ExtDef(node->children[0]);        // ExtDefList -> ExtDef ExtDefList
    translate_ExtDefList(node->children[1]);
}

void translate_ExtDef(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 2 || node->childSum == 3);
    if(strcmp(node->children[1]->name, "ExtDecList") == 0) {    // ExtDef -> Specifier ExtDecList SEMI
        // Do Nothing
        // Specifier 中调用 StructSpecifier 虽然会遇到 Exp, 但在语义分析阶段将其定义为错误类型 15，故这里不再作处理，下同
        // 其实只需要考虑由函数生成的中间代码就可以了
    }
    else if(strcmp(node->children[1]->name, "SEMI") == 0) {     // ExtDef -> Specifier SEMI
        // Do Nothing
    }
    else if(strcmp(node->children[1]->name, "FunDec") == 0) {   // ExtDef -> Specifier FunDec CompSt
        pInterCodes p1 = translate_FunDec(node->children[1]);
        pInterCodes p2 = translate_CompSt(node->children[2]);
        // 连接 p1，p2 到 intercodeslist 上面
        assert(p1 != NULL);
        concat(intercodeslist, p1);
        if(p2 != NULL) concat(intercodeslist, p2);
    }
}

pInterCodes translate_FunDec(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 3 || node->childSum == 4);
    pInterCodes p = new_pInterCodes();
    p->code.kind = CDFUNCTION;
    p->code.u.singleOP.op = new_pOperand();
    p->code.u.singleOP.op->kind = OPFUNCTION;
    int index = lookup(node->children[0]->text);
    assert(hashtable[index]->type->kind == FUNCTION);
    strcpy(p->code.u.singleOP.op->u.name, hashtable[index]->name);
    if(node->childSum == 4) {       // FunDec -> ID LP VarList RP
        pFieldList iter = hashtable[index]->type->u.function.argv;
        while(iter != NULL) {
            pInterCodes np = new_pInterCodes();
            np->code.kind = PARAM;
            np->code.u.singleOP.op = new_pOperand();
            assert(iter->type->kind == BASIC || iter->type->kind == STRUCTURE); // 假设2 & 选做1
            if(iter->type->kind == BASIC) np->code.u.singleOP.op->kind = VARIABLE;
            else if(iter->type->kind == STRUCTURE) np->code.u.singleOP.op->kind = OPSTRUCTURE;
            strcpy(np->code.u.singleOP.op->u.name, iter->name);
            concat(p, np);      
            iter = iter->tail;
        }
    }
    return p;
}

pInterCodes translate_CompSt(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 4);    // Compst -> DefList StmtList RC
    pInterCodes p1 = translate_DefList(node->children[1]);
    pInterCodes p2 = translate_StmtList(node->children[2]);
    if(p2!= NULL) concat(p1, p2);
    return p1;
}

pInterCodes translate_StmtList(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 2);    // StmtList -> Stmt StmtList
    pInterCodes p1 = translate_Stmt(node->children[0]);
    pInterCodes p2 = translate_StmtList(node->children[1]);
    if(p2 != NULL) concat(p1, p2);
    return p1;
}

pInterCodes translate_Stmt(Node* node) {
    // TODO
}

pInterCodes translate_DefList(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 2);    // DefList -> Def DefList
    pInterCodes p1 = translate_Def(node->children[0]);
    pInterCodes p2 = translate_DefList(node->children[1]);
    if(p2 != NULL) concat(p1, p2);
    return p1;
}

pInterCodes translate_Def(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 3);    // Def -> Specifier DecList SEMI
    return translate_DecList(node->children[1]);
}

pInterCodes translate_DecList(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) {      // DecList -> Dec
        return translate_Dec(node->children[0]);
    }
    else {      // DecList -> Dec COMMA DecList
        pInterCodes p1 = translate_Dec(node->children[0]);
        pInterCodes p2 = translate_DecList(node->children[2]);
        if(p2 != NULL) concat(p1, p2);
        return p1;
    }
}

pInterCodes translate_Dec(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) { // Dec -> VarDec
        return NULL;
    }
    else { // Dec -> VarDec ASSIGNOP Exp
        assert(node->children[0]->childSum == 1);    // Exp 不会是 ARRAY，参考错误类型 5，VarDec 只能是 BASIC
        int index = lookup(node->children[0]->children[0]);
        assert(hashtable[index]->type->kind == BASIC);  // Exp 只能是 BASIC，参考错误类型 5，VarDec 只能是 BASIC
        pOperand p = new_pOperand();    // VarDec
        p->kind = VARIABLE;
        strcpy(p->u.name, hashtable[index]->name);
        return translate_Exp(node->children[2], p);
    }
}

pInterCodes translate_Exp(Node* node, pOperand place) {
    // TODO
    return NULL;
}

pInterCodes translate_Args(Node* node, pArgList arg_list) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) {   // Args -> Exp
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0], t1);
        InsertArg(arg_list, t1);
        return code1;
    }
    else {  // Args -> Exp COMMA Args
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0], t1);
        InsertArg(arg_list, t1);
        pInterCodes code2 = translate_Args(node->children[2], arg_list);
        concat(code1, code2);
        return code1;
    }
}

pInterCodes translate_Cond(Node* node, pOperand label_true, pOperand label_false) {
    // TODO
    return NULL;
}

pOperand new_temp() {
    pOperand p = new_pOperand();
    p->kind = TEMP;
    p->u.no = global_temp_no++;
    return p;
}

pOperand new_label() {
    pOperand p = new_pOperand();
    p->kind = OPLABEL;
    p->u.no = global_label_no++;
    return p;
}

void init() {
    global_temp_no = 0;
    global_label_no = 0;
    intercodeslist = new_pInterCodes();
    intercodeslist->next = intercodeslist;      // 虚假的头
    intercodeslist->prev = intercodeslist;
}

void ShowAllInterCodes() {
    pInterCodes iter = intercodeslist->next;
    while(iter != intercodeslist) {
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
    else if(p->code.kind == CDFUNCTION) {
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
    else if(p->code.kind == CDMUL) {
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" * ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == CDDIV) {
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
        printf("*");
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
    else if(p->code.kind == CDRETURN) {
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
    if(p->kind == VARIABLE) printf("%s", p->u.name);
    else if(p->kind == CONSTANT) printf("#%d", p->u.val);
    else if(p->kind == TEMP) printf("t%d", p->u.no);
    else if(p->kind == OPLABEL) printf("label%d", p->u.no);
    else if(p->kind == OPFUNCTION) printf("%s", p->u.name);
    else if(p->kind == OPSTRUCTURE) printf("%s", p->u.name);
}

void concat(pInterCodes p1, pInterCodes p2) {
    assert(p1 != NULL && p2 != NULL);
    p1->prev->next = p2;
    p2->prev->next = p1;
    pInterCodes tmp = p2->prev;
    p2->prev = p1->prev;
    p1->prev = tmp;
}

void InsertArg(pArgList arg_list, pOperand t) {
    if(arg_list == NULL) {
        arg_list = (pArgList)calloc(1, sizeof(ArgList));
        arg_list->arg = t;
        arg_list->next = NULL;      // 其实用 calloc 不需要这句
    }
    else{
        pArgList p = (pArgList)calloc(1, sizeof(ArgList));
        p->next = arg_list;
        arg_list =p;
    }
}

pInterCodes new_pInterCodes() {
    pInterCodes p = (pInterCodes)calloc(1, sizeof(InterCodes));
    p->next = p;
    p->prev = p;
    return p;
}

pOperand new_pOperand() {
    pOperand p = (pOperand)calloc(1, sizeof(Operand));
    return p;
}

