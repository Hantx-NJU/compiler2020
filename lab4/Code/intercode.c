# include "intercode.h"

void translate(Node* root) {
    init();
    if(root != NULL) {
        translate_Program(root);
        /* we need not to show intercodes in lab4*/
        // ShowAllInterCodes(intercodeslist);
        // fprintf(fout,"\n\n");    
    }
}

void translate_Program(Node* node) {
   if(node == NULL) return;
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
        global_vt_no = 0;
        pInterCodes p2 = translate_CompSt(node->children[2]);
        pInterCodes p1 = translate_FunDec(node->children[1]);
        
        // 连接 p1，p2 到 intercodeslist 上面
        intercodeslist = AddInterCodesList(intercodeslist,p1);
        intercodeslist = AddInterCodesList(intercodeslist,p2);
    }
}

pInterCodes translate_FunDec(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 3 || node->childSum == 4);
    int index = lookup(node->children[0]->text);
    assert(hashtable[index]->type->kind == FUNCTION);
    pOperand op1 = new_pOperand(OPFUNCTION,-1,-1,hashtable[index]->name);
    pInterCodes p = new_pInterCodes(CDFUNCTION,op1,NULL,NULL,NULL,-1);
    pOperand op = new_pOperand(OPARRAY,-1,-1,"tttfunc");
    pInterCodes p1 = new_pInterCodes(CDDEC,op,NULL,NULL,NULL,global_vt_no * 4);
    p = AddInterCodesList(p, p1);
    
    if(node->childSum == 4) {       // FunDec -> ID LP VarList RP
        pFieldList iter = hashtable[index]->type->u.function.argv;
        while(iter != NULL) {
            if(iter->type->kind == ARRAY) {
                printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
                exit(-1);
            }
            pOperand op1;
            if(iter->type->kind == BASIC) op1 = new_pOperand(OPVARIABLE, -1, -1, iter->name);
            else if(iter->type->kind == STRUCTURE) op1 = new_pOperand(OPADDRESS, -1, -1, iter->name);
            pInterCodes np = new_pInterCodes(CDPARAM, op1,NULL,NULL,NULL,-1);
            p = AddInterCodesList(p,np);     
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
    return AddInterCodesList(p1, p2);;
}

pInterCodes translate_StmtList(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 2);    // StmtList -> Stmt StmtList
    pInterCodes p1 = translate_Stmt(node->children[0]);
    pInterCodes p2 = translate_StmtList(node->children[1]);
    return AddInterCodesList(p1, p2);
}

pInterCodes translate_Stmt(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 2 || node->childSum == 3 || node->childSum == 5 || node->childSum == 7);
    if(node->childSum == 2)     // Stmt -> Exp SEMI
        return translate_Exp(node->children[0],new_temp());
    else if(node->childSum == 1)    // Stmt -> Compst
        return translate_CompSt(node->children[0]);
    else if(node->childSum == 3) {  // Stmt -> RETURN Exp SEMI
        pOperand op = new_temp();
        pInterCodes code1 = translate_Exp(node->children[1],op);
        if(op->kind == OPADDRESS) { // 返回值不能为结构体和数组，但可以是他们的成员
            pOperand t = new_temp();
            pInterCodes code11 = new_pInterCodes(CDREAD_ADDR,t,op,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,code11);
            op = t;
        }
        pInterCodes code2 = new_pInterCodes(CDRETURN,op,NULL,NULL,NULL,-1);
        return AddInterCodesList(code1,code2);
    }
    else if(node->childSum == 7) {  // Stmt -> IF LP Exp RP Stmt ELSE Stmt
        pOperand label1 = new_label();
        pOperand label2 = new_label();
        pOperand label3 = new_label();
        pInterCodes code1 = translate_Cond(node->children[2],label1,label2);
        pInterCodes code2 = translate_Stmt(node->children[4]);
        pInterCodes code3 = translate_Stmt(node->children[6]);
        pInterCodes codel1 = new_pInterCodes(CDLABEL,label1,NULL,NULL,NULL,-1);
        pInterCodes codel2 = new_pInterCodes(CDLABEL,label2,NULL,NULL,NULL,-1);
        pInterCodes codel3 = new_pInterCodes(CDLABEL,label3,NULL,NULL,NULL,-1);
        pInterCodes codegt = new_pInterCodes(CDGOTO,label3,NULL,NULL,NULL,-1);
        pInterCodes code;
        code = AddInterCodesList(code1,codel1);
        code = AddInterCodesList(code,code2);
        code = AddInterCodesList(code,codegt);
        code = AddInterCodesList(code,codel2);
        code = AddInterCodesList(code,code3);
        code = AddInterCodesList(code,codel3);
    }
    else if(strcmp(node->children[0]->name,"IF") == 0) {    // Stmt -> IF LP Exp RP Stmt
        pOperand label1 = new_label();
        pOperand label2 = new_label();
        pInterCodes code1 = translate_Cond(node->children[2],label1,label2);
        pInterCodes code2 = translate_Stmt(node->children[4]);
        pInterCodes codel1 = new_pInterCodes(CDLABEL,label1,NULL,NULL,NULL,-1);
        pInterCodes codel2 = new_pInterCodes(CDLABEL,label2,NULL,NULL,NULL,-1);
        code1 = AddInterCodesList(code1,codel1);
        code1 = AddInterCodesList(code1,code2);
        code1 = AddInterCodesList(code1,codel2);
        return code1;
    }
    else if(strcmp(node->children[0]->name,"WHILE") == 0) {     // Stmt -> WHILE LP Exp RP Stmt
        pOperand label1 = new_label();  
        pOperand label2 = new_label();
        pOperand label3 = new_label();
        pInterCodes code1 = translate_Cond(node->children[2],label2,label3);
        pInterCodes code2 = translate_Stmt(node->children[4]);
        pInterCodes codel1 = new_pInterCodes(CDLABEL,label1,NULL,NULL,NULL,-1);
        pInterCodes codel2 = new_pInterCodes(CDLABEL,label2,NULL,NULL,NULL,-1);
        pInterCodes codel3 = new_pInterCodes(CDLABEL,label3,NULL,NULL,NULL,-1);
        pInterCodes codegt = new_pInterCodes(CDGOTO,label1,NULL,NULL,NULL,-1);
        pInterCodes code;
        code = AddInterCodesList(codel1,code1);
        code = AddInterCodesList(code,codel2);
        code = AddInterCodesList(code,code2);
        code = AddInterCodesList(code,codegt);
        code = AddInterCodesList(code,codel3);
        return code;
    }
}

pInterCodes translate_DefList(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 2);    // DefList -> Def DefList
    pInterCodes p1 = translate_Def(node->children[0]);
    pInterCodes p2 = translate_DefList(node->children[1]);
    return AddInterCodesList(p1, p2);
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
        return AddInterCodesList(p1, p2);
    }
}

pInterCodes translate_Dec(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) { // Dec -> VarDec
        return translate_VarDec(node->children[0]);
    }
    else { // Dec -> VarDec ASSIGNOP Exp
        assert(node->children[0]->childSum == 1);    // Exp 不会是 ARRAY，参考错误类型 5，VarDec 只能是 BASIC
        int index = lookup(node->children[0]->children[0]->text);
        assert(hashtable[index]->type->kind == BASIC);  // Exp 只能是 BASIC，参考错误类型 5，VarDec 只能是 BASIC
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[2], t1);
        if(t1->kind == OPADDRESS) {
            pOperand t = new_temp();
            pInterCodes code11 = new_pInterCodes(CDREAD_ADDR,t,t1,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,code11);
            t1 = t;
        }
        pOperand var = new_pOperand(OPVARIABLE,-1,-1,hashtable[index]->name);
        pInterCodes code2 = new_pInterCodes(CDASSIGN,var,t1,NULL,NULL,-1);
        return AddInterCodesList(code1,code2);
    }
}

pInterCodes translate_VarDec(Node* node) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 4);
    if(node->childSum == 1) {   // VarDec -> ID
        int index = lookup(node->children[0]->text);
        pFieldList pf = hashtable[index];
        if(pf->type->kind == ARRAY) {
            //create array
            // fprintf(fout, "%s\n", pf->name);
            pOperand op = new_pOperand(OPARRAY,-1,-1,pf->name);
            int size = getSize(pf->type);
            size = size / 4;
            for(int i = 0; i < size; ++i){
                op->num = global_vt_no;
                ++global_vt_no;
            }
            for(int i = 0; i < namenum && i < 100; ++i){
                // fprintf(fout,"BBB%s %s %d\n",place->u.name,varlist[i].name, varlist[i].num);
                if(strcmp(varlist[i].name, pf->name) == 0){
                    // fprintf(fout,"AAA%s %d\n",place->u.name, varlist[i].num);
                    varlist[i].num = op->num;
                    break;
                }
            }
            // fprintf(fout, "%d\n", op->num);
            return new_pInterCodes(CDDEC,op,NULL,NULL,NULL,0);
        }
        else if(pf->type->kind == STRUCTURE) {
            pOperand op = new_pOperand(OPSTRUCTURE,-1,-1,pf->name);
            int size = getSize(pf->type);
            size = size / 4;
            for(int i = 0; i < size; ++i){
                ++global_vt_no;
            }
            return new_pInterCodes(CDDEC,op,NULL,NULL,NULL,0);
        }
    }
    else if(node->childSum == 4) { // VarDec -> VarDec LB INT RB
        pInterCodes code = translate_VarDec(node->children[0]);
        assert(code != NULL);
        return code;
    }
}

pInterCodes translate_Exp(Node* node, pOperand place) {
    if(strcmp(node->children[0]->name,"INT") == 0) {    // Exp -> INT
        place->kind = OPCONSTANT;
        place->u.val = atoi(node->children[0]->text);
        return NULL;
    }
    else if(strcmp(node->children[0]->name,"ID") == 0 && node->childSum == 1) {    // Exp -> ID
        int index = lookup(node->children[0]->text);
        pFieldList pf = hashtable[index];
        strcpy(place->u.name,pf->name);
        if(pf->type->kind == BASIC) {
            place->kind = OPVARIABLE;
            int flag = 0;
            for(int i = 0; i < namenum && i < 100; ++i){
                // fprintf(fout,"%s %s %d\n",place->u.name,varlist[i].name, varlist[i].num);
                if(strcmp(varlist[i].name, place->u.name) == 0){
                    // fprintf(fout,"%s %d\n",place->u.name, varlist[i].num);
                    place->num = varlist[i].num;
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                varlist[namenum].num = place->num;
                strcpy(varlist[namenum].name, place->u.name);
                ++namenum;
            }
        }
        else if(pf->type->kind == ARRAY){ 
            place->kind = OPARRAY;
            int flag = 0;
            for(int i = 0; i < namenum && i < 100; ++i){
                // fprintf(fout,"BBB%s %s %d\n",place->u.name,varlist[i].name, varlist[i].num);
                if(strcmp(varlist[i].name, place->u.name) == 0){
                    // fprintf(fout,"AAA%s %d\n",place->u.name, varlist[i].num);
                    place->num = varlist[i].num;
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                varlist[namenum].num = place->num;
                strcpy(varlist[namenum].name, place->u.name);
                ++namenum;
            }
        }
        else if(pf->type->kind == STRUCTURE) {
            if(isArg(pf)) { // isArg 问的是是否是形参(而不是实参)
                place->kind = OPADDRESS;
                int flag = 0;
                for(int i = 0; i < namenum && i < 100; ++i){
                    // fprintf(fout,"%s %s %d\n",place->u.name,varlist[i].name, varlist[i].num);
                    if(strcmp(varlist[i].name, place->u.name) == 0){
                        // fprintf(fout,"%s %d\n",place->u.name, varlist[i].num);
                        place->num = varlist[i].num;
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0){
                    varlist[namenum].num = place->num;
                    strcpy(varlist[namenum].name, place->u.name);
                    ++namenum;
                }
            } 
            else place->kind = OPSTRUCTURE;
        }
        return NULL;
    }
    else if(strcmp(node->children[1]->name,"ASSIGNOP") == 0) {   // Exp -> Exp ASSIGNOP Exp
        pOperand t1 = new_temp();
        pOperand t2 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0],t1);
        pInterCodes code2 = translate_Exp(node->children[2],t2);
        if(t1->kind != OPADDRESS && t2->kind != OPADDRESS) {
            pInterCodes code3 = new_pInterCodes(CDASSIGN,t1,t2,NULL,NULL,-1);
            code2 = AddInterCodesList(code2,code3);
        }
        else if(t1->kind == OPADDRESS && t2->kind != OPADDRESS) {
            pInterCodes code3 = new_pInterCodes(CDSTORE_ADDR,t1,t2,NULL,NULL,-1);
            code2 = AddInterCodesList(code2,code3);
        }
        else if(t1->kind != OPADDRESS && t2->kind == OPADDRESS) {
            pInterCodes code3 = new_pInterCodes(CDREAD_ADDR,t1,t2,NULL,NULL,-1);
            code2 = AddInterCodesList(code2,code3);
        }
        else {
            pOperand t = new_temp();
            pInterCodes code3 = new_pInterCodes(CDREAD_ADDR,t,t2,NULL,NULL,-1);
            pInterCodes code4 = new_pInterCodes(CDSTORE_ADDR,t1,t,NULL,NULL,-1);
            code2 = AddInterCodesList(code2,code3);
            code2 = AddInterCodesList(code2,code4);
        }
        pInterCodes codea2 = new_pInterCodes(CDASSIGN,place,t1,NULL,NULL,-1);
        code2 = AddInterCodesList(code2,codea2);
        pInterCodes code = AddInterCodesList(code1,code2);
        return code;
    }
    else if(strcmp(node->children[1]->name,"PLUS") == 0 || strcmp(node->children[1]->name,"MINUS") == 0 ||
            strcmp(node->children[1]->name,"STAR") == 0 || strcmp(node->children[1]->name,"DIV") == 0) {  // Exp -> Exp (PLUS MINUS STAR DIV) Exp
        pOperand t1 = new_temp();
        pOperand t2 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0],t1); 
        if(t1->kind == OPADDRESS) {
            pOperand t = new_temp();
            pInterCodes code11 = new_pInterCodes(CDREAD_ADDR,t,t1,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,code11);
            t1 = t;
        }
        pInterCodes code2 = translate_Exp(node->children[2],t2);
        if(t2->kind == OPADDRESS) {
            pOperand t = new_temp();
            pInterCodes code21 = new_pInterCodes(CDREAD_ADDR,t,t2,NULL,NULL,-1);
            code2 = AddInterCodesList(code2,code21);
            t2 = t;
        }
        strcmp(node->children[1]->name,"PLUS") == 0;
        pInterCodes code3 = NULL;
        strcmp(node->children[1]->name,"PLUS") == 0;
        if(strcmp(node->children[1]->name,"PLUS") == 0) code3 = new_pInterCodes(CDADD,place,t1,t2,NULL,-1);
        else if(strcmp(node->children[1]->name,"MINUS") == 0) code3 = new_pInterCodes(CDSUB,place,t1,t2,NULL,-1);
        else if(strcmp(node->children[1]->name,"STAR") == 0) code3 = new_pInterCodes(CDMUL,place,t1,t2,NULL,-1);
        else if(strcmp(node->children[1]->name,"DIV") == 0) code3 = new_pInterCodes(CDDIV,place,t1,t2,NULL,-1);
        pInterCodes code = AddInterCodesList(code1,code2);
        code = AddInterCodesList(code,code3);
        return code;
    }
    else if(strcmp(node->children[0]->name,"MINUS") == 0) { // Exp -> MINUS Exp
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[1],t1);
        if(t1->kind == OPADDRESS) {
            pOperand t = new_temp();
            pInterCodes code11 = new_pInterCodes(CDREAD_ADDR,t,t1,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,code11);
            t1 = t;
        }
        pInterCodes code2 = new_pInterCodes(CDSUB,place,new_pOperand(OPCONSTANT,-1,0,NULL),t1,NULL,-1);
        return AddInterCodesList(code1,code2);
    }
    else if(strcmp(node->children[1]->name,"RELOP") == 0 ||   // Exp -> Exp RELOP Exp
            strcmp(node->children[0]->name,"NOT") == 0 ||     // Exp -> NOT Exp
            strcmp(node->children[1]->name,"AND") == 0 ||     // Exp -> Exp AND Exp
            strcmp(node->children[1]->name,"OR") == 0) {      // Exp -> Exp OR Exp
        pOperand label1 = new_label();
        pOperand label2 = new_label();
        pInterCodes code0 = new_pInterCodes(CDASSIGN,place,new_pOperand(OPCONSTANT,-1,0,NULL),NULL,NULL,-1);
        pInterCodes code1 = translate_Cond(node,label1,label2);
        pInterCodes code21 = new_pInterCodes(CDLABEL,label1,NULL,NULL,NULL,-1);
        pInterCodes code22 = new_pInterCodes(CDASSIGN,place,new_pOperand(OPCONSTANT,-1,1,NULL),NULL,NULL,-1);
        pInterCodes code2 = AddInterCodesList(code21,code22);
        pInterCodes code3 = new_pInterCodes(CDLABEL,label2,NULL,NULL,NULL,-1);
        pInterCodes code = AddInterCodesList(code0,code1);
        code = AddInterCodesList(code,code2);
        code = AddInterCodesList(code,code3);
        return code;
    }
    else if(strcmp(node->children[0]->name,"ID") == 0 && node->childSum == 3) { // Exp -> ID LP RP
        int index = lookup(node->children[0]->text);
        if(strcmp(hashtable[index]->name,"read") == 0) 
            return new_pInterCodes(CDREAD,place,NULL,NULL,NULL,-1);
        else return new_pInterCodes(CDCALL,place,new_pOperand(OPFUNCTION,-1,-1,hashtable[index]->name),NULL,NULL,-1);
    }
    else if(strcmp(node->children[0]->name,"ID") == 0 && node->childSum == 4) { // Exp -> ID LP Args RP
        int index = lookup(node->children[0]->text);
        pArgList arg_list = NULL;
        pInterCodes code1 = translate_Args(node->children[2],&arg_list);
        if(strcmp(hashtable[index]->name,"write") == 0) {
            if(arg_list->arg->kind == OPADDRESS) {
                pOperand t = new_temp();
                pInterCodes code11 = new_pInterCodes(CDREAD_ADDR,t,arg_list->arg,NULL,NULL,-1);
                code1 = AddInterCodesList(code1,code11);
                arg_list->arg = t;
            }
            pInterCodes codew = new_pInterCodes(CDWRITE,arg_list->arg,NULL,NULL,NULL,-1);
            return AddInterCodesList(code1,codew);
        }
        pInterCodes code2 = NULL;
        for(pArgList iter = arg_list; iter != NULL; iter = iter->next) {
            if(iter->arg->kind == OPADDRESS && iter->arg->saddr != 1) {
                pOperand t = new_temp();
                pInterCodes code21 = new_pInterCodes(CDREAD_ADDR,t,iter->arg,NULL,NULL,-1);
                code2 = AddInterCodesList(code2,code21);
                iter->arg = t;
            }
            if(iter->arg->kind == OPSTRUCTURE) { // 结构体作为实参需要传址
                pOperand t = new_pOperand(OPADDRESS,-1,-1,newString(iter->arg->u.name,"_addr"));
                pInterCodes codeg = new_pInterCodes(CDGET_ADDR,t,iter->arg,NULL,NULL,-1);
                iter->arg = t;
                code2 = AddInterCodesList(code2,codeg);
            }
        }
        for(pArgList iter = arg_list; iter != NULL; iter = iter->next) {  
            pInterCodes codea = new_pInterCodes(CDARG,iter->arg,NULL,NULL,NULL,-1);
            code2 = AddInterCodesList(code2,codea);
        }
        pInterCodes codec = new_pInterCodes(CDCALL,place,new_pOperand(OPFUNCTION,-1,-1,hashtable[index]->name),NULL,NULL,-1);
        pInterCodes code = AddInterCodesList(code1,code2);
        code = AddInterCodesList(code,codec);
        return code;
    }
    else if(strcmp(node->children[0]->name,"LP") == 0) {    // Exp -> LP Exp RP
        return translate_Exp(node->children[1],place);
    }
    else if(strcmp(node->children[1]->name,"LB") == 0) {    // Exp -> Exp LB Exp RB
        if(strcmp(node->children[0]->children[0]->name,"ID") != 0) { // 高维数组
            if(strcmp(node->children[0]->children[2]->name,"ID") != 0) { // Exp DOT ID
                printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
                exit(-1);
            }
        }
        pOperand base = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0],base);
        if(base->kind != OPADDRESS) {
            assert(base->kind == OPARRAY);
            pOperand t = new_pOperand(OPADDRESS,-1,-1,newString(base->u.name,"_addr"));
            pInterCodes code11 = new_pInterCodes(CDGET_ADDR,t,base,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,code11);
            base = t;
        }
        pOperand idx = new_temp();
        pInterCodes code2 = translate_Exp(node->children[2],idx);
        // Exp1 只能是 Exp DOT ID 或 ID
        int size = 0;
        int hashindex = 0;
        if(strcmp(node->children[0]->children[0]->name,"ID") == 0) 
            hashindex = lookup(node->children[0]->children[0]->text);
        else hashindex = lookup(node->children[0]->children[2]->text);
        size = getSize(hashtable[hashindex]->type->u.array.elem);
        pOperand offset = new_temp();
        pInterCodes code3 = new_pInterCodes(CDMUL,offset,idx,new_pOperand(OPCONSTANT,-1,size,NULL),NULL,-1);

        char buffer[32];
        sprintf(buffer, "t%d", global_temp_no++);
        pOperand target = new_pOperand(OPADDRESS,-1,-1,buffer);
        pInterCodes code4 = new_pInterCodes(CDADD,target,base,offset,NULL,-1);
        place->kind = OPADDRESS;
        strcpy(place->u.name,target->u.name);
        int flag = 0;
        for(int i = 0; i < namenum && i < 100; ++i){
            // fprintf(fout,"BBB%s %s %d\n",place->u.name,varlist[i].name, varlist[i].num);
            if(strcmp(varlist[i].name, place->u.name) == 0){
                // fprintf(fout,"AAA%s %d\n",place->u.name, varlist[i].num);
                place->num = varlist[i].num;
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            varlist[namenum].num = place->num;
            strcpy(varlist[namenum].name, place->u.name);
            ++namenum;
        }
        
        if(hashtable[hashindex]->type->u.array.elem->kind == STRUCTURE) place->saddr = 1;
        pInterCodes code = AddInterCodesList(code1,code2);
        code = AddInterCodesList(code,code3);
        code = AddInterCodesList(code,code4);
        return code;   
    }
    else if(strcmp(node->children[1]->name,"DOT") == 0) {   // Exp -> Exp DOT ID
        pOperand base = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0],base);
        if(base->kind != OPADDRESS) {
            assert(base->kind == OPSTRUCTURE);
            pOperand t = new_pOperand(OPADDRESS,-1,-1,newString(base->u.name,"_addr"));
            pInterCodes codeg = new_pInterCodes(CDGET_ADDR,t,base,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,codeg);
            base = t;
        }
        int index = lookup(node->children[2]->text);
        int offset = getoffset(hashtable[index]);
        place->kind = OPADDRESS;
        int flag = 0;
        for(int i = 0; i < namenum && i < 100; ++i){
            // fprintf(fout,"%s %s %d\n",place->u.name,varlist[i].name, varlist[i].num);
            if(strcmp(varlist[i].name, place->u.name) == 0){
                // fprintf(fout,"%s %d\n",place->u.name, varlist[i].num);
                place->num = varlist[i].num;
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            varlist[namenum].num = place->num;
            strcpy(varlist[namenum].name, place->u.name);
            ++namenum;
        }
        char buffer[32];
        sprintf(buffer, "%d", place->u.no);
        strcpy(place->u.name,newString("t",buffer));
        if(hashtable[index]->type->kind == STRUCTURE) place->saddr = 1;
        pInterCodes code2 = new_pInterCodes(CDADD,place,base,new_pOperand(OPCONSTANT,-1,offset,NULL),NULL,-1);
        return AddInterCodesList(code1,code2);
    }
}

pInterCodes translate_Args(Node* node, pArgList* arg_list) {
    assert(node != NULL);
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) {   // Args -> Exp
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0], t1);
        if(t1->kind == OPARRAY) {
            printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
            exit(-1);
        }
        *arg_list = AddArgList(t1,*arg_list);
        return code1;
    }
    else {  // Args -> Exp COMMA Args
        pOperand t1 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0], t1);
        *arg_list = AddArgList(t1,*arg_list);
        pInterCodes code2 = translate_Args(node->children[2], arg_list);
        return AddInterCodesList(code1, code2);
    }
}

pInterCodes translate_Cond(Node* node, pOperand label_true, pOperand label_false) {
    assert(node != NULL);
    if(strcmp(node->children[0]->name,"NOT") == 0)            // Exp -> NOT Exp
        return translate_Cond(node->children[1],label_false,label_true);
    
    else if(node->childSum == 3 && strcmp(node->children[1]->name,"RELOP") == 0) {   // Exp -> Exp RELOP Exp
        pOperand t1 = new_temp();
        pOperand t2 = new_temp();
        pInterCodes code1 = translate_Exp(node->children[0],t1);
        if(t1->kind == OPADDRESS) {
            pOperand t = new_temp();
            pInterCodes code11 = new_pInterCodes(CDREAD_ADDR,t,t1,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,code11);
            t1 = t;
        }
        pInterCodes code2 = translate_Exp(node->children[2],t2);
        if(t2->kind == OPADDRESS) {
            pOperand t = new_temp();
            pInterCodes code21 = new_pInterCodes(CDREAD_ADDR,t,t2,NULL,NULL,-1);
            code2 = AddInterCodesList(code2,code21);
            t2 = t;
        }
        pInterCodes code3 = new_pInterCodes(CDIF_GOTO,t1,t2,label_true,node->children[1]->text,-1);
        pInterCodes codegt = new_pInterCodes(CDGOTO,label_false,NULL,NULL,NULL,-1);
        pInterCodes code;
        code = AddInterCodesList(code1,code2);
        code = AddInterCodesList(code,code3);
        code = AddInterCodesList(code,codegt);
        return code;
    }
    else if(node->childSum == 3 && strcmp(node->children[1]->name,"AND") == 0) {  // Exp -> Exp AND Exp
        pOperand label1 = new_label();
        pInterCodes code1 = translate_Cond(node->children[0],label1,label_false);
        pInterCodes code2 = translate_Cond(node->children[2],label_true,label_false);
        pInterCodes codel1 = new_pInterCodes(CDLABEL,label1,NULL,NULL,NULL,-1);
        pInterCodes code = AddInterCodesList(code1,codel1);
        code = AddInterCodesList(code,code2);
        return code;
    }
    else if(node->childSum == 3 && strcmp(node->children[1]->name,"OR") == 0) {  // Exp -> Exp OR Exp
        pOperand label1 = new_label();
        pInterCodes code1 = translate_Cond(node->children[0],label_true,label1);
        pInterCodes code2 = translate_Cond(node->children[2],label_true,label_false);
        pInterCodes codel1 = new_pInterCodes(CDLABEL,label1,NULL,NULL,NULL,-1);
        pInterCodes code = AddInterCodesList(code1,codel1);
        code = AddInterCodesList(code,code2);
        return code;
    }
    else {  
        pOperand t1 = new_temp();
        pOperand t2 = new_pOperand(OPCONSTANT,-1,0,NULL);
        pInterCodes code1 = translate_Exp(node,t1);
        if(t1->kind == OPADDRESS) {
            pOperand t3 = new_temp();
            pInterCodes tmp = new_pInterCodes(CDREAD_ADDR,t3,t1,NULL,NULL,-1);
            code1 = AddInterCodesList(code1,tmp);
            t1 = t3;
        }
        pInterCodes code2 = new_pInterCodes(CDIF_GOTO,t1,t2,label_true,"!=",-1);
        pInterCodes codegt = new_pInterCodes(CDGOTO,label_false,NULL,NULL,NULL,-1);
        pInterCodes code = AddInterCodesList(code1,code2);
        code = AddInterCodesList(code,codegt);
        return code;
    }
}

pOperand new_temp() {
    pOperand p = new_pOperand(OPTEMP,global_temp_no++,-1,NULL);
    return p;
}

pOperand new_label() {
    pOperand p = new_pOperand(OPLABEL,global_label_no++,-1,NULL);
    return p;
}

void init() {
    global_temp_no = 0;
    global_label_no = 0;
    namenum = 0;
    intercodeslist = (pInterCodes)calloc(1, sizeof(InterCodes));
    intercodeslist->next = intercodeslist;      // 虚假的头
    intercodeslist->prev = intercodeslist;
    // varnamelist = (pVarnames)calloc(1, sizeof(Varnames));
}

void ShowAllInterCodes(pInterCodes list) {
    pInterCodes iter = list->next;
    while(iter != list) {
        ShowInterCode(iter);
        iter = iter->next;
    }
}

void ShowInterCode(pInterCodes p) {
    if(p == NULL) return;
    if(p->code.kind == CDLABEL) {
        fprintf(fout,"LABEL ");
        ShowOperand(p->code.u.singleOP.op);
        fprintf(fout," :");
        // fprintf(fout,"\n %d \n", p->code.u.singleOP.op->num);
    }
    else if(p->code.kind == CDFUNCTION) {
        fprintf(fout,"FUNCTION ");
        ShowOperand(p->code.u.singleOP.op);
        fprintf(fout," :");
    }
    else if(p->code.kind == CDASSIGN) {
        ShowOperand(p->code.u.doubleOP.left);
        fprintf(fout," := ");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == CDADD) {
        ShowOperand(p->code.u.tripleOP.result);
        fprintf(fout," := ");
        ShowOperand(p->code.u.tripleOP.op1);
        fprintf(fout," + ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == CDSUB) {
        ShowOperand(p->code.u.tripleOP.result);
        fprintf(fout," := ");
        ShowOperand(p->code.u.tripleOP.op1);
        fprintf(fout," - ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == CDMUL) {
        ShowOperand(p->code.u.tripleOP.result);
        fprintf(fout," := ");
        ShowOperand(p->code.u.tripleOP.op1);
        fprintf(fout," * ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == CDDIV) {
        ShowOperand(p->code.u.tripleOP.result);
        fprintf(fout," := ");
        ShowOperand(p->code.u.tripleOP.op1);
        fprintf(fout," / ");
        ShowOperand(p->code.u.tripleOP.op2);
    }
    else if(p->code.kind == CDGET_ADDR) {
        ShowOperand(p->code.u.doubleOP.left);
        fprintf(fout," := &");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == CDREAD_ADDR) {
        ShowOperand(p->code.u.doubleOP.left);
        fprintf(fout," := *");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == CDSTORE_ADDR) {
        fprintf(fout,"*");
        ShowOperand(p->code.u.doubleOP.left);
        fprintf(fout," := ");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == CDGOTO) {
        fprintf(fout,"GOTO ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == CDIF_GOTO) {
        fprintf(fout,"IF ");
        ShowOperand(p->code.u.if_gotoOP.op1);
        fprintf(fout," %s ", p->code.u.if_gotoOP.relop);
        ShowOperand(p->code.u.if_gotoOP.op2);
        fprintf(fout," GOTO ");
        ShowOperand(p->code.u.if_gotoOP.label);
    }
    else if(p->code.kind == CDRETURN) {
        fprintf(fout,"RETURN ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == CDDEC) {
        fprintf(fout,"DEC ");
        ShowOperand(p->code.u.decOP.op);
        fprintf(fout," %d", p->code.u.decOP.size);
        // fprintf(fout,"\n %d \n", p->code.u.decOP.op->num);
    }
    else if(p->code.kind == CDARG) {
        fprintf(fout,"ARG ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == CDCALL) {
        ShowOperand(p->code.u.doubleOP.left);
        fprintf(fout," := CALL ");
        ShowOperand(p->code.u.doubleOP.right);
    }
    else if(p->code.kind == CDPARAM) {
        fprintf(fout,"PARAM ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == CDREAD) {
        fprintf(fout,"READ ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else if(p->code.kind == CDWRITE) {
        fprintf(fout,"WRITE ");
        ShowOperand(p->code.u.singleOP.op);
    }
    else assert(0);
    fprintf(fout,"\n");
}

void ShowOperand(pOperand p){
    assert(p != NULL);
    if(p->kind == OPVARIABLE) fprintf(fout,"%s", p->u.name);
    else if(p->kind == OPCONSTANT) fprintf(fout,"#%d", p->u.val);
    else if(p->kind == OPTEMP) fprintf(fout,"t%d", p->u.no);
    else if(p->kind == OPLABEL) fprintf(fout,"label%d", p->u.no);
    else if(p->kind == OPFUNCTION) fprintf(fout,"%s", p->u.name);
    else if(p->kind == OPADDRESS) fprintf(fout,"%s", p->u.name);
    else if(p->kind == OPSTRUCTURE) fprintf(fout,"%s", p->u.name);
    else if(p->kind == OPARRAY) fprintf(fout,"%s", p->u.name);
    else assert(0);
}

pInterCodes AddInterCodesList(pInterCodes p1, pInterCodes p2) {
    if(p1 == NULL) return p2;
    if(p2 == NULL) return p1;
    p1->prev->next = p2;
    p2->prev->next = p1;
    pInterCodes tmp = p2->prev;
    p2->prev = p1->prev;
    p1->prev = tmp;
    return p1;
}

pArgList AddArgList(pOperand t,pArgList arg_list) {
    if(arg_list == NULL) {
        arg_list = (pArgList)calloc(1, sizeof(ArgList));
        arg_list->arg = t;
        arg_list->next = NULL;      // 其实用 calloc 不需要这句
        return arg_list;
    }
    else{
        pArgList p = (pArgList)calloc(1, sizeof(ArgList));
        p->arg = t;
        p->next = arg_list;
        return p;
    }
}

pInterCodes new_pInterCodes(int kind, pOperand op1, pOperand op2, pOperand op3, char* relop, int size) {
    pInterCodes p = (pInterCodes)calloc(1, sizeof(InterCodes));
    p->next = p;
    p->prev = p;
    assert(kind>=0 && kind <= 18);
    p->code.kind = kind;
    switch (kind)
    {
    case CDLABEL: case CDFUNCTION: case CDGOTO: case CDRETURN: case CDARG: case CDPARAM: case CDREAD: case CDWRITE:
        p->code.u.singleOP.op = op1;
        break;
    case CDASSIGN: case CDGET_ADDR: case CDREAD_ADDR: case CDSTORE_ADDR: case CDCALL:
        p->code.u.doubleOP.left = op1;
        p->code.u.doubleOP.right = op2;
        break;
    case CDADD: case CDSUB: case CDMUL: case CDDIV:
        p->code.u.tripleOP.result = op1;
        p->code.u.tripleOP.op1 = op2;
        p->code.u.tripleOP.op2 = op3;
        break;
    case CDIF_GOTO:
        p->code.u.if_gotoOP.op1 = op1;
        p->code.u.if_gotoOP.op2 = op2;
        p->code.u.if_gotoOP.label = op3;
        strcpy(p->code.u.if_gotoOP.relop,relop);
        break;
    case CDDEC:
        p->code.u.decOP.op = op1;
        p->code.u.decOP.size = size;
        break;
    default:
        assert(0);
    }
    return p;
}

pOperand new_pOperand(int kind, int no, int val, char* name) {
    global_vt_no += 1;
    pOperand p = (pOperand)calloc(1, sizeof(Operand));
    p->kind = kind;
    p->u.no = -1;
    p->u.val = -1;
    p->num = global_vt_no;
    if(name){
        // fprintf(fout,"%s\n",name);
        int flag = 0;
        for(int i = 0; i < namenum && i < 100; ++i){
            // fprintf(fout,"%s %s %d\n",name,varlist[i].name, varlist[i].num);
            if(strcmp(varlist[i].name, name) == 0){
                // fprintf(fout,"%s %d\n",name, varlist[i].num);
                p->num = varlist[i].num;
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            varlist[namenum].num = p->num;
            strcpy(varlist[namenum].name, name);
            ++namenum;
        }
    }
    if(kind == OPTEMP || kind == OPLABEL) p->u.no = no;
    else if(kind == OPCONSTANT) p->u.val = val;
    else if(kind == OPFUNCTION || kind == OPVARIABLE || kind == OPADDRESS || kind == OPSTRUCTURE || kind == OPARRAY) strcpy(p->u.name,name);
    else assert(0);   
    return p;
}

char* newString(char* s1, char* s2) {
    char* s = (char*)malloc(sizeof(s1));
    strcpy(s,s1);
    strcat(s,s2);
    return s;
}



