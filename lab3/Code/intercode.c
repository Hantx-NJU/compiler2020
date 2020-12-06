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
        // translate_ExtDecList(node->children[1]);
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

// void translate_ExtDecList(Node* node){
//     assert(node != NULL);
//     assert(node->childSum == 1 || node->childSum == 3);
//     if(node->childSum == 1){    // ExtDecList -> VarDec
//         translate_VarDec(node->children[0]);
//     }
//     else{   // ExtDecList -> VarDec COMMA ExtDecList
//         translate_VarDec(node->children[0]);
//         translate_ExtDecList(node->children[2]);
//     }
// }

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
        FuncParam = iter;
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
    assert(node->childSum == 4);    // Compst -> LC DefList StmtList RC
    // assert(node->children[1] != NULL);
    pInterCodes p1 = translate_DefList(node->children[1]);
    pInterCodes p2 = translate_StmtList(node->children[2]);
    //assert(p1 != NULL);
    if(p1 == NULL)  return p2;
    else if(p2!= NULL) concat(p1, p2);
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
    if(node == NULL) return NULL;
    assert(node->childSum == 1 || node->childSum == 2 || node->childSum == 3 
        || node->childSum == 5 || node->childSum == 7);
    if(node->childSum == 2){    // Stmt -> Exp SEMI
        pInterCodes code = translate_Exp(node->children[0], NULL);
        return code;
    }
    else if(node->childSum == 1){   // Stmt -> CompSt
        pInterCodes code = translate_CompSt(node->children[0]);
        return code;
    }
    else if(node->childSum == 3){   // Stmt -> RETURN Exp SEMI
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[1], t1);
        pInterCodes code2 = new_pInterCodes();
        code2->code.kind = CDRETURN;
        code2->code.u.singleOP.op = t1;
        concat(code1, code2);
        return code1;
    }
    else if(node->childSum == 5){
        if(strcmp(node->children[0]->name, "IF") == 0){    // Stmt -> IF LP Exp RP Stmt
            pOperand label1 = new_label();
            pOperand label2 = new_label();
            pInterCodes code1 = translate_Cond(node->children[2], label1, label2);
            pInterCodes code2 = translate_Stmt(node->children[4]);
            pInterCodes code11 = new_pInterCodes();
            pInterCodes code21 = new_pInterCodes();
            code11->code.kind = LABEL;
            code11->code.u.singleOP.op = label1;
            code21->code.kind = LABEL;
            code21->code.u.singleOP.op = label2;
            concat(code1, code11);
            concat(code1, code2);
            concat(code1, code21);
            return code1;
        }
        else if(strcmp(node->children[0]->name, "WHILE") == 0){   //Stmt -> WHILE LP Exp RP Stmt
            pOperand label1 = new_label();
            pOperand label2 = new_label();
            pOperand label3 = new_label();
            pInterCodes code1 = translate_Cond(node->children[2], label2, label3);
            pInterCodes code2 = translate_Stmt(node->children[4]);
            pInterCodes code0 = new_pInterCodes();
            pInterCodes code11 = new_pInterCodes();
            pInterCodes code21 = new_pInterCodes();
            pInterCodes code22 = new_pInterCodes();
            code0->code.kind = LABEL;
            code0->code.u.singleOP.op = label1;
            code11->code.kind = LABEL;
            code11->code.u.singleOP.op = label2;
            code21->code.kind = GOTO;
            code21->code.u.singleOP.op = label1;
            code22->code.kind = LABEL;
            code22->code.u.singleOP.op = label3;
            concat(code0, code1);
            concat(code0, code11);
            concat(code0, code2);
            concat(code0, code21);
            concat(code0, code22);
            return code0;
        }
    }
    else if(node->childSum == 7){   // Stmt -> IF LP Exp RP Stmt ELSE Stmt
        pOperand label1 = new_label();
        pOperand label2 = new_label();
        pOperand label3 = new_label();
        pInterCodes code1 = translate_Cond(node->children[2], label1, label2);
        pInterCodes code2 = translate_Stmt(node->children[4]);
        pInterCodes code3 = translate_Stmt(node->children[6]);
        pInterCodes code11 = new_pInterCodes();
        pInterCodes code21 = new_pInterCodes();
        pInterCodes code22 = new_pInterCodes();
        pInterCodes code31 = new_pInterCodes();
        code11->code.kind = LABEL;
        code11->code.u.singleOP.op = label1;
        code21->code.kind = GOTO;
        code21->code.u.singleOP.op = label3;
        code22->code.kind = LABEL;
        code22->code.u.singleOP.op = label2;
        code31->code.kind = LABEL;
        code31->code.u.singleOP.op = label3;
        concat(code1, code11);
        concat(code1, code2);
        concat(code1, code21);
        concat(code1, code22);
        concat(code1, code3);
        concat(code1, code31);
        return code1;
    }
}

pInterCodes translate_DefList(Node* node) {
    //assert(node != NULL);
    if(node == NULL) return NULL;
    assert(node->childSum == 2);    // DefList -> Def DefList
    pInterCodes p1 = translate_Def(node->children[0]);
    pInterCodes p2 = translate_DefList(node->children[1]);
    if(p2 != NULL) {
        if(p1 == NULL)
            return p2;
        concat(p1, p2);
    }
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
        if(p2 != NULL) {
            if(p1 == NULL)
                return p2;
            concat(p1, p2);
        }
        return p1;
    }
}

pInterCodes translate_Dec(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) { // Dec -> VarDec
        pInterCodes code = translate_VarDec(node->children[0]);
        return code;
    }
    else { // Dec -> VarDec ASSIGNOP Exp
        assert(node->children[0]->childSum == 1);    // Exp 不会是 ARRAY，参考错误类型 5，VarDec 只能是 BASIC
        int index = lookup(node->children[0]->children[0]->text);    //fixed ,the last was node->children[0]->children[0]
        assert(hashtable[index]->type->kind == BASIC);  // Exp 只能是 BASIC，参考错误类型 5，VarDec 只能是 BASIC
        pOperand p = new_pOperand();    // VarDec
        p->kind = VARIABLE;
        strcpy(p->u.name, hashtable[index]->name);
        pInterCodes code =  translate_Exp(node->children[2], p);
        assert(code != NULL);
        return code;
    }
}

pInterCodes translate_VarDec(Node* node){
    if(node->childSum == 1){    // VarDec -> ID
        int index = lookup(node->children[0]->text);
        pFieldList pf = hashtable[index];
        if(pf->type->kind == ARRAY || pf->type->kind == STRUCTURE){
            pInterCodes code = new_pInterCodes();
            pOperand op = new_pOperand();
            if(pf->type->kind == ARRAY)
                op->kind = VARIABLE;
            else
                op->kind = OPSTRUCTURE;
            strcpy(op->u.name, pf->name);
            code->code.kind = DEC;
            code->code.u.decOP.op = op;
            code->code.u.decOP.size = GetSize(pf->type, "");
            assert(code != NULL);
            return code;
        }
    }
    else if(node->childSum == 4){   // VarDec -> VarDec LB INT RB
        pInterCodes code = translate_VarDec(node->children[0]);
        assert(code != NULL);
        return code;
    }
    return NULL;
}
pInterCodes translate_Exp(Node* node, pOperand place) {
    // TODO
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 2 || node->childSum == 3 || node->childSum == 4);
    if(node->childSum == 1){
        if(strcmp(node->children[0]->name, "INT") == 0){    // Exp -> INT
            int value = atoi(node->children[0]->text);
            pOperand p = new_pOperand();
            p->kind = CONSTANT;
            p->u.val = value;
            pInterCodes code = new_pInterCodes();
            code->code.kind = ASSIGN;
            code->code.u.doubleOP.left = place;
            code->code.u.doubleOP.right = p;
            return code;
        }
        else if(strcmp(node->children[0]->name, "ID") == 0){    // Exp -> ID
            int i = lookup(node->children[0]->text);
            pOperand p = new_pOperand();
            p->kind = VARIABLE;
            strcpy(p->u.name, hashtable[i]->name);
            pInterCodes code = new_pInterCodes();
            code->code.kind = ASSIGN;
            code->code.u.doubleOP.left = place;
            code->code.u.doubleOP.right = p;
            return code;
        }
    }
    else if(node->childSum == 2){
        if(strcmp(node->children[0]->name, "MINUS") == 0){    // Exp -> MINUS Exp
            pOperand t1 = new_temp();
            pInterCodes code1 = translate_Exp(node->children[1], t1);
            pInterCodes code2 = new_pInterCodes();
            pOperand op1 = new_pOperand();
            op1->kind = CONSTANT;
            op1->u.val = 0;
            code2->code.kind = SUB;
            code2->code.u.tripleOP.result = place;
            code2->code.u.tripleOP.op1 = op1;
            code2->code.u.tripleOP.op2 = t1;
            concat(code1, code2);
            return code1;
        }
        else if(strcmp(node->children[0]->name, "MINUS") == 0){    // Exp -> NOT Exp
            pInterCodes code = Exp_to_Cond(node, place);
            return code;
        }
    }
    else if(node->childSum == 3){
        if(strcmp(node->children[1]->name, "ASSIGNOP") == 0){    // Exp -> Exp ASSIGNOP Exp
            int i = lookup(node->children[0]->children[0]->text);
            pOperand p = new_temp();
            pOperand t1 = new_temp();
            pInterCodes code1 = translate_Exp(node->children[2], t1);
            pInterCodes code21 = new_pInterCodes();
            if(node->children[0]->childSum == 1 && strcmp(node->children[0]->children[0]->name, "ID") == 0)
            {   // Exp_1 -> ID
                p->kind = VARIABLE;
                strcpy(p->u.name, hashtable[i]->name);
                code21->code.kind = ASSIGN;
                code21->code.u.doubleOP.left = p;
                code21->code.u.doubleOP.right = t1;
                assert(p != NULL);
                assert(t1 != NULL);
            }
            else if(node->children[0]->childSum == 4 && strcmp(node->children[0]->children[1]->name, "LB") == 0)
            {   // Exp_1 -> Exp LB Exp RB
                code21 = translate_Exp(node->children[0], p);
                pInterCodes code22 = new_pInterCodes();
                code22->code.kind = STORE_ADDR;
                code22->code.u.doubleOP.left = p;
                code22->code.u.doubleOP.right = t1;
                concat(code21, code22);
                assert(p != NULL);
                assert(t1 != NULL);
            }
            else if(node->children[2]->childSum == 4 && strcmp(node->children[2]->children[1]->name, "LB") == 0)
            {   // Exp_2 -> Exp LB Exp RB
                p->kind = VARIABLE;
                strcpy(p->u.name, hashtable[i]->name);
                code21->code.kind = GO_ADDR;
                code21->code.u.doubleOP.left = p;
                code21->code.u.doubleOP.right = t1;
                assert(p != NULL);
                assert(t1 != NULL);
            }
            else if(node->children[2]->childSum == 4 && strcmp(node->children[2]->children[1]->name, "LB") == 0
                && node->children[0]->childSum == 4 && strcmp(node->children[0]->children[1]->name, "LB") == 0)
            {   // Exp_1 -> Exp LB Exp RB && Exp_2 -> Exp LB Exp RB
                p->kind = VARIABLE;
                strcpy(p->u.name, hashtable[i]->name);
                code21->code.kind = GO_ADDR;
                code21->code.u.doubleOP.left = p;
                code21->code.u.doubleOP.right = t1;
                pOperand p1 = new_temp();
                pInterCodes code22 = translate_Exp(node->children[0], p1);
                pInterCodes code23 = new_pInterCodes();
                code23->code.kind = STORE_ADDR;
                code23->code.u.doubleOP.left = p1;
                code23->code.u.doubleOP.right = p;
                concat(code21, code22);
                concat(code21, code23);
            }
            pInterCodes code22 = new_pInterCodes();
            code22->code.kind = ASSIGN;
            code22->code.u.doubleOP.left = place;
            code22->code.u.doubleOP.right = p;
            concat(code1, code21);
            concat(code1, code22);
            return code1;
        }
        else if(strcmp(node->children[1]->name, "PLUS") == 0){    // Exp -> Exp PLUS Exp
            pOperand t1 = new_temp();
            pOperand t2 = new_temp();
            pInterCodes code1 = translate_Exp(node->children[0], t1);
            pInterCodes code2 = translate_Exp(node->children[2], t2);
            pInterCodes code3 = new_pInterCodes();
            code3->code.kind = ADD;
            code3->code.u.tripleOP.result = place;
            code3->code.u.tripleOP.op1 = t1;
            code3->code.u.tripleOP.op2 = t2;
            concat(code1, code2);
            concat(code1, code3);
            return code1;
        }
        else if(strcmp(node->children[1]->name, "MINUS") == 0){    // Exp -> Exp MINUS Exp
            pOperand t1 = new_temp();
            pOperand t2 = new_temp();
            pInterCodes code1 = translate_Exp(node->children[0], t1);
            pInterCodes code2 = translate_Exp(node->children[2], t2);
            pInterCodes code3 = new_pInterCodes();
            code3->code.kind = SUB;
            code3->code.u.tripleOP.result = place;
            code3->code.u.tripleOP.op1 = t1;
            code3->code.u.tripleOP.op2 = t2;
            concat(code1, code2);
            concat(code1, code3);
            return code1;
        }
        else if(strcmp(node->children[1]->name, "STAR") == 0){    // Exp -> Exp STAR Exp
            pOperand t1 = new_temp();
            pOperand t2 = new_temp();
            pInterCodes code1 = translate_Exp(node->children[0], t1);
            pInterCodes code2 = translate_Exp(node->children[2], t2);
            pInterCodes code3 = new_pInterCodes();
            code3->code.kind = CDMUL;
            code3->code.u.tripleOP.result = place;
            code3->code.u.tripleOP.op1 = t1;
            code3->code.u.tripleOP.op2 = t2;
            concat(code1, code2);
            concat(code1, code3);
            return code1;
        }
        else if(strcmp(node->children[1]->name, "DIV") == 0){    // Exp -> Exp DIV Exp
            pOperand t1 = new_temp();
            pOperand t2 = new_temp();
            pInterCodes code1 = translate_Exp(node->children[0], t1);
            pInterCodes code2 = translate_Exp(node->children[2], t2);
            pInterCodes code3 = new_pInterCodes();
            code3->code.kind = CDDIV;
            code3->code.u.tripleOP.result = place;
            code3->code.u.tripleOP.op1 = t1;
            code3->code.u.tripleOP.op2 = t2;
            concat(code1, code2);
            concat(code1, code3);
            return code1;
        }
        else if(strcmp(node->children[0]->name, "LP") == 0){    // Exp -> LP Exp RP
            pInterCodes code = translate_Exp(node->children[1], place);
            return code;
        }
        else if(strcmp(node->children[1]->name, "LP") == 0){    // Exp -> ID LP RP
            int i = lookup(node->children[0]->text);
            pOperand p = new_pOperand();
            p->kind = OPFUNCTION;
            strcpy(p->u.name, hashtable[i]->name);
            pInterCodes code = new_pInterCodes();
            if(strcmp(p->u.name, "read") == 0){
                code->code.kind = READ;
                code->code.u.singleOP.op = place;
                return code;
            }
            assert(p!=NULL);
            code->code.kind = CALL;
            code->code.u.doubleOP.left = place;
            code->code.u.doubleOP.right = p;
            return code;
        }
        else if(strcmp(node->children[1]->name, "RELOP") == 0   // Exp -> Exp RELOP Exp
                || strcmp(node->children[1]->name, "AND") == 0  // Exp -> Exp AND Exp
                || strcmp(node->children[1]->name, "OR") == 0){    // Exp -> Exp OR Exp
            pInterCodes code = Exp_to_Cond(node, place);
            return code;
        }
        else if(strcmp(node->children[1]->name, "DOT") == 0){   // Exp -> Exp DOT ID

        }
    }
    else if(node->childSum == 4){
        if(strcmp(node->children[1]->name, "LP") == 0){    // Exp -> ID LP Args RP
            int i = lookup(node->children[0]->text);
            pOperand p = new_pOperand();
            p->kind = OPFUNCTION;
            strcpy(p->u.name, hashtable[i]->name);
            pArgList arg_list = NULL;
            pInterCodes code1 = translate_Args(node->children[2], &arg_list);
            if(strcmp(p->u.name, "write") == 0){
                pInterCodes code11 = new_pInterCodes();
                code11->code.kind = WRITE;
                code11->code.u.singleOP.op = arg_list->arg;
                concat(code1, code11);
                return code1;
            }
            assert(arg_list->arg != NULL);
            pInterCodes code2 = new_pInterCodes();
            code2->code.kind = ARG;
            code2->code.u.singleOP.op = arg_list->arg;
            assert(code2->code.u.singleOP.op!=NULL);
            arg_list = arg_list->next;
            while(arg_list != NULL){
                pInterCodes ptemp = new_pInterCodes();
                ptemp->code.kind = ARG;
                ptemp->code.u.singleOP.op = arg_list->arg;
                assert(ptemp->code.u.singleOP.op!=NULL);
                arg_list = arg_list->next;
                concat(code2, ptemp);
            }
            pInterCodes code21 = new_pInterCodes();
            code21->code.kind = CALL;
            code21->code.u.doubleOP.left = place;
            code21->code.u.doubleOP.right = p;
            concat(code1, code2);
            concat(code1, code21);
            return code1;
        }
        else if(strcmp(node->children[1]->name, "LB") == 0){    // Exp -> Exp LB Exp RB
            char name[32];
            strcpy(name, getName(node->children[0]));
            int i = lookup(name);
            pOperand p = new_pOperand();
            p->kind = VARIABLE;
            strcpy(p->u.name, hashtable[i]->name);
            pOperand t1 = new_temp();
            pInterCodes code1 = translate_Exp(node->children[2], t1); //偏移量计算
            pInterCodes code2 = new_pInterCodes();  //偏移量乘以单位size
            pOperand op1 = new_pOperand();
            op1->kind = CONSTANT;
            op1->u.val = GetSize(hashtable[i]->type->u.array.elem, "");
            pOperand offset = new_temp();
            code2->code.kind = CDMUL;
            code2->code.u.tripleOP.result = offset;
            code2->code.u.tripleOP.op1 = t1;
            code2->code.u.tripleOP.op2 = op1;
            pOperand t2 = new_temp();
            pInterCodes code3 = new_pInterCodes();  //t2 = &p
            code3->code.kind = GET_ADDR;
            code3->code.u.doubleOP.left = t2;
            code3->code.u.doubleOP.right = p;
            pOperand t3 = new_temp();
            pInterCodes code4 = new_pInterCodes();  // t3 = t2 + offset
            code4->code.kind = ADD;
            code4->code.u.tripleOP.result = t3;
            code4->code.u.tripleOP.op1 = t2;
            code4->code.u.tripleOP.op2 = offset;
            pInterCodes code5 = new_pInterCodes();  //place = t3
            place->kind = ADDR
            code5->code.kind = ASSIGN;
            code5->code.u.doubleOP.left = place;
            code5->code.u.doubleOP.right = t3;
            concat(code1, code2);
            concat(code1, code3);
            concat(code1, code4);
            concat(code1, code5);
            return code1;
        }
    }
    return NULL;
}

pInterCodes Exp_to_Cond(Node* node, pOperand place){
    pOperand label1 = new_label();
    pOperand label2 = new_label();
    pOperand op0r = new_pOperand();
    op0r->kind = CONSTANT;
    op0r->u.val = 0;
    pInterCodes code0 = new_pInterCodes();  //[place := #0]
    code0->code.kind = ASSIGN;
    code0->code.u.doubleOP.left = place;
    code0->code.u.doubleOP.right = op0r;
    pInterCodes code1 = translate_Cond(node, label1, label2);
    pOperand op21 = new_pOperand();
    op21->kind = CONSTANT;
    op21->u.val = 1;
    pInterCodes code2 = new_pInterCodes();
    code2->code.kind = LABEL;
    code2->code.u.singleOP.op = label1;
    pInterCodes code21 = new_pInterCodes();
    code21->code.kind = ASSIGN;
    code21->code.u.doubleOP.left = place;
    code21->code.u.doubleOP.right = op21;
    //concat(code2, code21);
    pInterCodes code3 = new_pInterCodes();  //[LABEL label2]
    code3->code.kind = LABEL;
    code3->code.u.singleOP.op = label2;
    concat(code0, code1);
    concat(code0, code2);
    concat(code0, code21);
    concat(code0, code3);
    return code0;
}
pInterCodes translate_Args(Node* node, ppArgList arg_list) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) {   // Args -> Exp
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0], t1);
        assert(t1!=NULL);
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
    assert(node != NULL);
    if(node->childSum == 2 && strcmp(node->children[0]->name, "NOT") == 0){    //Exp -> NOT Exp
        pInterCodes code = translate_Cond(node->children[1], label_false, label_true);
        return code;
    }
    else if(node->childSum == 3 && strcmp(node->children[1]->name, "RELOP") == 0){  // Exp -> Exp RELOP Exp
        pOperand t1 = new_temp();
        pOperand t2 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0], t1);
        pInterCodes code2 = translate_Exp(node->children[2], t2);
        pInterCodes code3 = new_pInterCodes();
        code3->code.kind = IF_GOTO;
        code3->code.u.if_gotoOP.op1 = t1;
        code3->code.u.if_gotoOP.op2 = t2;
        code3->code.u.if_gotoOP.label = label_true;
        strcpy(code3->code.u.if_gotoOP.relop, node->children[1]->text);
        pInterCodes code31 = new_pInterCodes();
        code31->code.kind = GOTO;
        code31->code.u.singleOP.op = label_false;
        concat(code1, code2);
        concat(code1, code3);
        concat(code1, code31);
        return code1;
    }
    else if(node->childSum == 3 && strcmp(node->children[1]->name, "AND") == 0){ // Exp -> Exp AND Exp
        pOperand label1 = new_label();
        pInterCodes code1 = translate_Cond(node->children[0], label1, label_false);
        pInterCodes code2 = translate_Cond(node->children[2], label_true, label_false);
        pInterCodes code11 = new_pInterCodes();
        code11->code.kind = LABEL;
        code11->code.u.singleOP.op = label1;
        concat(code1, code11);
        concat(code1, code2);
        return code1;
    }
    else if(node->childSum == 3 && strcmp(node->children[1]->name, "OR") == 0){ // Exp -> Exp OR Exp
        pOperand label1 = new_label();
        pInterCodes code1 = translate_Cond(node->children[0], label_true, label1);
        pInterCodes code2 = translate_Cond(node->children[2], label_true, label_false);
        pInterCodes code11 = new_pInterCodes();
        code11->code.kind = LABEL;
        code11->code.u.singleOP.op = label1;
        concat(code1, code11);
        concat(code1, code2);
        return code1;
    }
    else{   // other cases
        pOperand t1 = new_temp();
        pOperand op22 = new_pOperand();
        op22->kind = CONSTANT;
        op22->u.val = 0;
        pInterCodes code1 = translate_Exp(node, t1);
        pInterCodes code2 = new_pInterCodes();
        code2->code.kind = IF_GOTO;
        code2->code.u.if_gotoOP.op1 = t1;
        code2->code.u.if_gotoOP.op2 = op22;
        code2->code.u.if_gotoOP.label = label_true;
        strcpy(code2->code.u.if_gotoOP.relop, "!=");
        pInterCodes code21 = new_pInterCodes();
        code21->code.kind = GOTO;
        code21->code.u.singleOP.op = label_false;
        concat(code1, code2);
        concat(code1, code21);
        return code1; 
    }
    return NULL;
}
int GetSize(pType pt, char* name){
    if(pt->kind == BASIC){
        return 4;
    }
    else if(pt->kind == ARRAY){
        int each = GetSize(pt->u.array.elem, "");
        return each * pt->u.array.size;
    }
    else if(pt->kind == STRUCTURE){
        int ret = 0;
        pFieldList pf = pt->u.structure->type->u.member;
        while(pf != NULL && strcmp(pf->name, name) != 0){
            ret += GetSize(pf->type, "");
            pf = pf->tail;
        }
        return ret;
    }
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
        if(p->code.u.doubleOP.left == NULL) return;
        ShowOperand(p->code.u.doubleOP.left);
        printf(" := ");
        assert(p->code.u.doubleOP.right!=NULL);
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == ADD) {
        if(p->code.u.tripleOP.result == NULL) return;
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" + ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == SUB) {
        if(p->code.u.tripleOP.result == NULL) return;
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" - ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == CDMUL) {
        if(p->code.u.tripleOP.result == NULL) return;
        ShowOperand(p->code.u.tripleOP.result);
        printf(" := ");
        ShowOperand(p->code.u.tripleOP.op1);
        printf(" * ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == CDDIV) {
        if(p->code.u.tripleOP.result == NULL) return;
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
        if(p->code.u.doubleOP.left == NULL) return;
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

void InsertArg(ppArgList arg_list, pOperand t) {
    if((*arg_list) == NULL) {
        (*arg_list) = (pArgList)calloc(1, sizeof(ArgList));
        (*arg_list)->arg = t;
        assert((*arg_list)->arg != NULL);
        (*arg_list)->next = NULL;      // 其实用 calloc 不需要这句
    }
    else{
        pArgList p = (pArgList)calloc(1, sizeof(ArgList));
        p->arg = t;
        p->next = (*arg_list);
        (*arg_list) =p;
    }
}

int isParam(pFieldList pf){
    pFieldList p = FuncParam;
    while(p != NULL){
        if(p->name == pf->name){
            return GetSize(p->type, "");
        }
        p = p->tail;
    }
    return 0;
}

char* getName(Node* node){
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1){    //Exp -> ID
        return node->children[0]->text;
    }
    else{   //Exp -> Exp DOT ID
        return node->children[2]->text;
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