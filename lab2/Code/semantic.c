# include "semantic.h"

void traverseTree(Node* root) {
    if(root != NULL) Program(root);
    //show(); // for debug only
}

unsigned int hash(char* name) {
    unsigned int val = 0;
    unsigned int i;
    for(; *name; ++name){
        val = (val << 2) + *name;
        if(i = val & ~HASHTABLE_SIZE) val = (val ^ (i >> 12)) & HASHTABLE_SIZE;
    }
    assert(val >= 0 && val < HASHTABLE_SIZE);
    return val;
}

void initHashtable() {
    for(int i = 0; i < HASHTABLE_SIZE; ++i) {
        hashtable[i] = NULL;
    }
}

// 闭散列， 线性探测
void insert(pFieldList pf) { 
    if(pf == NULL) return;
    int index = hash(pf->name);
    for(int i = index; i != (index - 1) % HASHTABLE_SIZE; i = (i + 1) % HASHTABLE_SIZE) {   //warning: if index = 0, what about (index - 1) % HASHTABLE_SIZE
        if(hashtable[i] == NULL){
            hashtable[i] = pf;
            return;
        }
    }
    assert(0);      // Hash 表满
}

int lookup(char* name) { // 返回下标， 不在表中返回 -1
    int index = hash(name);
    for(int i = index; i != (index - 1) % HASHTABLE_SIZE && hashtable[i] != NULL; i = (i + 1) % HASHTABLE_SIZE){
        if(strcmp(name, hashtable[i]->name) == 0) {
            return i;
        }
    }
    return -1;
}

int isEqual(pType a, pType b){
    // equal is 0, otherwise is 1
    if(a->kind != b->kind)
        return 1;
    pFieldList apf, bpf;
    switch(a->kind){
        case BASIC: 
            if(a->u.basic != b->u.basic)    return 1;
            break;
        case ARRAY: 
            while(1){
                if(a->kind != b->kind)  return 1;
                if(a->kind != ARRAY)    return isEqual(a, b);
                a = a->u.array.elem;
                b = b->u.array.elem;
            }
            break;
        case STRUCTURE:
            return isEqual(a->u.structure->type, b->u.structure->type);            
            break;
        case STRUCT_TAG:    
            apf = a->u.member;
            bpf = b->u.member;
            while(apf){
                if(bpf == NULL) return 1;
                if(isEqual(apf->type, bpf->type) == 1)  return 1;
                apf = apf->tail;
                bpf = bpf->tail;
            }
            if(bpf != NULL) return 1;
            break;
        case FUNCTION:
            //no compairison between two functions 
            break;
    }
    return 0;
}

void Program(Node* node) {
    if(node == NULL) return;
    assert(node->childSum == 1);
    ExtDefList(node->children[0]);  // Program -> ExtDefList
}

void ExtDefList(Node* node) {
    if(node == NULL) return;
    assert(node->childSum == 2);
    ExtDef(node->children[0]);      // ExtDefList -> ExtDef ExtDefList
    ExtDefList(node->children[1]);
}

void ExtDef(Node* node) {
    if(node == NULL) return;
    assert(node->childSum == 2 || node->childSum == 3);
    pType pt = Specifier(node->children[0]);
    if(pt == NULL) return;         // 针对 Tag 未定义的情况
    if(strcmp(node->children[1]->name, "ExtDecList") == 0) {    // ExtDef -> Specifier ExtDecList SEMI
        ExtDecList(node->children[1], pt);
    }
    else if(strcmp(node->children[1]->name, "SEMI") == 0) {     // ExtDef -> Specifier SEMI
        // Do Nothing
    }
    else if(strcmp(node->children[1]->name, "FunDec") == 0) {   // ExtDef -> Specifier FunDec CompSt
        FunDec(node->children[1], pt);
        CompSt(node->children[2], pt);
    }
}

void ExtDecList(Node* node, pType pt) {
    if(node == NULL) return;
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) { // ExtDecList -> VarDec
        VarDec(node->children[0], pt, NULL);
    }
    else if(node->childSum == 3) { // ExtDecList -> VarDec COMMA ExtDecList
        VarDec(node->children[0], pt, NULL);
        ExtDecList(node->children[2], pt);
    }
}

pType Specifier(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 1);
    pType pt = (pType)calloc(1, sizeof(Type_));
    if(strcmp(node->children[0]->name, "TYPE") == 0) { // Specifier -> TYPE
        pt->kind = BASIC;
        if(strcmp(node->children[0]->text, "int") == 0) pt->u.basic = BASIC_INT;
        else if(strcmp(node->children[0]->text, "float") == 0) pt->u.basic = BASIC_FLOAT;
        return pt;
    }
    else if(strcmp(node->children[0]->name, "StructSpecifier") == 0) { // Specifier -> StructSpecifier
        return StructSpecifier(node->children[0]);
    }
}

pType StructSpecifier(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 5 || node->childSum == 2);
    if(node->childSum == 5) { // StructSpecifier -> STRUCT OptTag LC DefList RC
        char* opttag = OptTag(node->children[1]);
        if(opttag != NULL) {
            int index = lookup(opttag);
            if(index != -1) { // OptTag 已被定义
                printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", node->children[1]->lineno, opttag);
                return NULL; 
            }
        }
        pType pt = (pType)calloc(1, sizeof(Type_));
        pType npt = (pType)calloc(1, sizeof(Type_));
        pFieldList pf = (pFieldList)calloc(1, sizeof(FieldList_));
        pt->kind = STRUCTURE;
        pt->u.structure = pf;
        pf->name = opttag;
        pf->type = npt;
        npt->kind = STRUCT_TAG;
        if(opttag != NULL) insert(pf);
        DefList(node->children[3], pf);
        return pt; 
    }
    else if(node->childSum == 2) { // StructSpecifier -> STRUCT Tag
        char* tag = Tag(node->children[1]);
        int index = lookup(tag);
        if(index == -1) { // 没有定义 tag
            printf("Error type 17 at Line %d: Undefined structure \"%s\".\n", node->children[1]->lineno, tag);
            return NULL;
        }
        pType pt = (pType)calloc(1, sizeof(Type_));
        pt->kind = STRUCTURE;
        pt->u.structure = hashtable[index];
        return pt;
    }
}

char* OptTag(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 1);
    return node->children[0]->text;
}

char* Tag(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 1);
    return node->children[0]->text;
}

void VarDec(Node* node, pType pt, pFieldList pf) {
    if(node == NULL) return;
    assert(node->childSum == 1 || node->childSum == 4);
    if(node->childSum == 1) {   // VarDec -> ID
        if(lookup(node->children[0]->text) != -1) { // ID已经在hash中
            if(pf != NULL){
                if(pf->type->kind == STRUCT_TAG){
                    printf("Error type 15 at Line %d: Redefined field \"%s\".\n", node->children[0]->lineno, node->children[0]->text);
                    return;
                }
            }
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", node->children[0]->lineno, node->children[0]->text);
            return;
        }
        pFieldList npf = (pFieldList)calloc(1, sizeof(FieldList_));
        npf->name = node->children[0]->text;
        npf->type = pt;
        insert(npf);
        if(pf != NULL) { // 这个 Var 是 pf 的形参或者成员
            assert(pf->type->kind == FUNCTION || pf->type->kind == STRUCT_TAG);
            if(pf->type->kind == FUNCTION) {
                if(pf->type->u.function.argc == 0) { // 插入第一个形参
                    pf->type->u.function.argv= npf;
                }
                else {
                    pFieldList tmp = pf->type->u.function.argv;
                    while(tmp->tail != NULL) tmp = tmp->tail;
                    tmp->tail = npf;
                }
                pf->type->u.function.argc++;
            }
            else if(pf->type->kind == STRUCT_TAG) {
                if(pf->type->u.member == NULL) { // 插入第一个成员
                    pf->type->u.member = npf;
                }
                else {
                    pFieldList tmp = pf->type->u.member;
                    while(tmp->tail != NULL) tmp = tmp->tail;
                    tmp->tail = npf;
                }
            }
        }
    }
    else if(node->childSum == 4) { // VarDec -> VarDec LB INT RB
        pType npt = (pType)calloc(1, sizeof(Type_));
        npt->kind = ARRAY;
        npt->u.array.size = atoi(node->children[2]->text);
        npt->u.array.elem = pt;
        VarDec(node->children[0], npt, pf);
    }
}

void FunDec(Node* node, pType pt) {
    if(node == NULL) return;
    assert(node->childSum == 4 || node->childSum == 3);
    // for both production
    if(lookup(node->children[0]->text) != -1) { // ID 已经在hash中
        printf("Error type 4 at Line %d: Redefined function \"%s\".\n", node->children[0]->lineno, node->children[0]->text);
        //return;
        if(node->childSum == 4){    //FunDec -> ID LP VarList RP
            return VarList(node->children[2], NULL);
        }
    }
    pFieldList pf = (pFieldList)calloc(1, sizeof(FieldList_));
    pType npt = (pType)calloc(1, sizeof(Type_));
    pf->name = node->children[0]->text;
    pf->type = npt;
    npt->kind = FUNCTION;
    npt->u.function.ret = pt;
    npt->u.function.argc = 0;
    npt->u.function.argv = NULL;
    insert(pf);
    if(node->childSum == 4) { // FunDec -> ID LP VarList RP
        VarList(node->children[2], pf);
    }
}

void VarList(Node* node, pFieldList pf) {
    if(node == NULL) return;
    assert(node->childSum == 3 || node->childSum == 1);
    if(node->childSum == 3) { // VarList -> ParamDec COMMA VarList
        ParamDec(node->children[0], pf);
        VarList(node->children[2], pf);
    }
    else if(node->childSum == 1) { // VarList -> ParamDec
        ParamDec(node->children[0], pf);
    }
}

void ParamDec(Node* node, pFieldList pf) {
    if(node == NULL) return;
    assert(node->childSum == 2);
    pType pt = Specifier(node->children[0]); // ParamDec -> Specifier VarDec
    if(pt == NULL) return; 
    VarDec(node->children[1], pt, pf);
}

void CompSt(Node* node, pType pt) {
    if(node == NULL) return;
    assert(node->childSum == 4);
    DefList(node->children[1], NULL); // CompSt -> LC DefList StmtList RC
    StmtList(node->children[2], pt);
}

void StmtList(Node* node, pType pt) { // pt 为返回值类型, 用于检查错误类型 8
    if(node == NULL) return;
    assert(node->childSum == 2);
    Stmt(node->children[0], pt);    // StmtList -> Stmt StmtList
    StmtList(node->children[1], pt);
}

void Stmt(Node* node, pType pt) {
    // TODO
    if(node == NULL) return;
    assert(node->childSum == 1 || node->childSum == 2 || node->childSum == 3 
            || node->childSum == 5 || node->childSum == 7);
    if(node->childSum == 1){    //Stmt -> CompSt
        CompSt(node->children[0], pt);
    }
    else if(node->childSum == 2){   //Stmt ->Exp SEMI
        Exp(node->children[0]);
    }
    else if(node->childSum == 3){   //Stmt -> RETURN Exp SEMI
        pType pret = Exp(node->children[1]);
        if(!pret)   return;
        if(isEqual(pt, pret) == 1)
            printf("Error type 8 at Line %d: Type mismatched for return\n", node->children[0]->lineno);
    }
    else if(node->childSum == 5){   //Stmt -> IF LP Exp RP Stmt | WHILE LP Exp RP Stmt
        Exp(node->children[2]);
        Stmt(node->children[4], pt);
    }
    else if(node->childSum == 7){   //Stmt -> IF LP Exp RP Stmt ELSE Stmt
        Exp(node->children[2]);
        Stmt(node->children[4], pt);
        Stmt(node->children[6], pt);
    }
}

void DefList(Node* node, pFieldList pf) {
    if(node == NULL) return;
    assert(node->childSum == 2);
    Def(node->children[0], pf); // DefList -> Def DefList
    DefList(node->children[1], pf);
}

void Def(Node* node, pFieldList pf) {
    if(node == NULL) return;
    assert(node->childSum == 3);
    pType pt = Specifier(node->children[0]); // Def -> Specifier DecList SEMI
    if(pt == NULL) return;  // 针对 Tag 为空的情况
    DecList(node->children[1], pt, pf);
}

void DecList(Node* node, pType pt, pFieldList pf) {
    if(node == NULL) return;
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) { // DecList -> Dec
        Dec(node->children[0], pt, pf);
    }
    else if(node->childSum == 3) { // DecList -> Dec COMMA DecList
        Dec(node->children[0], pt, pf);
        DecList(node->children[2], pt, pf);
    }
}

void Dec(Node* node, pType pt, pFieldList pf) {
    if(node == NULL) return;
    assert(node->childSum == 1 || node->childSum == 3);
    if(node->childSum == 1) { // Dec -> VarDec
        VarDec(node->children[0], pt, pf);
    }
    else if(node->childSum == 3) { // Dec -> VarDec ASSIGNOP Exp
        if(pf != NULL){
            if(pf->type->kind == STRUCT_TAG) {
                printf("Error type 15 at Line %d: Initialized field while defining.\n", node->children[0]->lineno);
                return;
            }
        }
        VarDec(node->children[0], pt, pf);
        // TODO
        Exp(node->children[2]);
    }
}

pType Exp(Node* node) {
    if(node == NULL) return NULL;
    assert(node->childSum == 1 || node->childSum == 2 || node->childSum == 3 || node->childSum == 4);
    //printf("%s\n", node->children[1]->name);
    pType pt = NULL;
    if(node->childSum == 1){
        if(strcmp(node->children[0]->name, "ID") == 0)
        {
            if(lookup(node->children[0]->text) == -1)
                printf("Error type 1 at Line %d: Undefined variable \"%s \"\n", node->children[0]->lineno, node->children[0]->text);
            else{
                int i = lookup(node->children[0]->text);
                pt = hashtable[i]->type;
            }
        }
        else{
            pt = (pType)calloc(1, sizeof(Type_));
            pt->kind = BASIC;
            if(strcmp(node->children[0]->name, "INT") == 0){
                pt->u.basic = 0;
            }
            else{
                pt->u.basic = 1;
            }
        }
        // no error when INT or FLOAT
    }
    else if(node->childSum == 2){   //Exp -> MINUS Exp | NOT Exp
        pt = Exp(node->children[1]);
        if(!pt) return NULL;
    }
    else if(node->childSum == 3)
    {
        if(strcmp(node->children[0]->name, "LP") == 0){ //Exp -> LP Exp RP
            pt = Exp(node->children[1]);
            if(!pt) return NULL;
        }
        else if(strcmp(node->children[0]->name, "ID") == 0){    //Exp -> ID LP RP
            if(lookup(node->children[0]->text) == -1)
                printf("Error type 2 at Line %d: Undefined function \"%s \"\n", node->children[0]->lineno, node->children[0]->text);
            else{
                int i = lookup(node->children[0]->text);
                if(hashtable[i]->type->kind != FUNCTION)
                    printf("Error type 11 at Line %d: \"%s\" is not a function\n", node->children[0]->lineno, node->children[0]->text);
                else 
                    pt = hashtable[i]->type;
            }
        }
        else if(strcmp(node->children[2]->name, "ID") == 0){    //Exp -> Exp DOT ID
            //printf("%s\n",node->children[2]->text);
            pt = Exp(node->children[0]);
            //printf("after %s\n",node->children[2]->text);
            if(!pt) return NULL;
            //printf("point\n");
            if(pt->kind != STRUCTURE)
                printf("Error type 13 at Line %d: Illegal use of \".\"\n", node->children[0]->lineno);
            else{
                int isMember = -1;
                pFieldList f = pt->u.structure->type->u.member;
                while(f){
                    if(strcmp(node->children[2]->text, f->name) == 0){
                        isMember = 1;
                        pt = f->type;
                        break;
                    }
                    f = f->tail;
                }
                if(isMember == -1)
                    printf("Error type 14 at Line %d: Non-existent field \"%s\"\n", node->children[0]->lineno, node->children[2]->text);
            }
        }
        else{   //Exp -> Exp (ASSIGNOP | AND | OR | RELOP | PLUS | MINUS | STAR | DIV) Exp
            pt = Exp(node->children[0]);
            if(!pt) return NULL;
            pType p = Exp(node->children[2]);
            if(!p)  return NULL;
            if(strcmp(node->children[1]->name, "ASSIGNOP") == 0){   //Exp -> Exp ASSIGNOP Exp
                if(!( (strcmp(node->children[0]->children[0]->name, "ID") == 0 && node->children[0]->childSum == 1)
                        || (strcmp(node->children[0]->children[0]->name, "Exp") == 0 && node->children[0]->childSum == 4)
                        || (node->children[0]->childSum == 3 && strcmp(node->children[0]->children[1]->name, "DOT") == 0))){
                            printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable\n", node->children[0]->lineno);
                        }
                else if(isEqual(pt, p) == 1){
                    printf("Error type 5 at Line %d: Type mismatched for assignment\n", node->children[0]->lineno);
                }
            }
            else if(isEqual(pt, p) == 1 || pt->kind != BASIC){
                printf("Error type 7 at Line %d: Type mismatched for operand\n", node->children[0]->lineno);
            }
        }
    }
    else{
        if(strcmp(node->children[0]->name, "ID") == 0){ //Exp -> ID LP Args RP
            if(lookup(node->children[0]->text) == -1)
                printf("Error type 2 at Line %d: Undefined function \"%s \"\n", node->children[0]->lineno, node->children[0]->text);
            else{
                int i = lookup(node->children[0]->text);
                if(hashtable[i]->type->kind != FUNCTION)
                    printf("Error type 11 at Line %d: \"%s\" is not a function\n", node->children[0]->lineno, node->children[0]->text);
                else{
                    pt = hashtable[i]->type->u.function.ret;
                    pFieldList pf = Args(node->children[2]);
                    pFieldList p = hashtable[i]->type->u.function.argv;
                    //int argc = hashtable[i]->type->u.function.argc;
                    //int count = 0;
                    while(p || pf){
                        if(p == NULL || pf == NULL){
                            printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments\n", node->children[0]->lineno, node->children[0]->text);
                            break;
                        }
                        if(isEqual(p->type, pf->type) == 1){
                            printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments\n", node->children[0]->lineno, node->children[0]->text);
                            break;
                        }
                        p = p->tail;
                        pf = pf->tail;
                    }
                }
            }
        }
        else if(strcmp(node->children[0]->name, "Exp") == 0){ //Exp -> Exp LB Exp RB
            pType p1 = Exp(node->children[0]);
            pType p2 = Exp(node->children[2]);
            if(!p1) return NULL;
            if(p1->kind != ARRAY){
                printf("Error type 10 at Line %d: Not an array\n", node->children[0]->lineno);
            }
            else{
                while(p1->kind == ARRAY)
                    p1 = p1->u.array.elem;
                pt = p1;
            }
            if(!p2) return NULL;
            if(p2->kind != BASIC || p2->u.basic != 0){
                printf("Error type 12 at Line %d: Not a integer\n", node->children[0]->lineno);
            }
        }
    }
    return pt;
}

pFieldList Args(Node* node){
    if(node == NULL) return NULL;
    assert(node->childSum == 1 || node->childSum == 3);
    //pType pt = (pType)calloc(1, sizeof(Type_));
    pFieldList pf = (pFieldList)calloc(1, sizeof(FieldList_));
    pType pt = Exp(node->children[0]);
    if(!pt) return NULL;
    pf->type = pt;
    pf->tail = NULL;    //Args -> Exp
    if(node->childSum == 3){    //Args -> Exp COMMA Args
        pf->tail = Args(node->children[2]);
    }
    return pf;
}

void show() {
    printf("\nSHOW BEGIN\n");
    for(int i = 0; i < HASHTABLE_SIZE; ++i) {
        if(hashtable[i] != NULL) {
            printf("index: %d\t\tname: %10s\ttype: ", i, hashtable[i]->name);
            showtype(hashtable[i]->type);
            printf("\n");
        }
    }
}

void showtype(pType pt) {
    if(pt->kind == BASIC) { // BASIC
        if(pt->u.basic == BASIC_INT) printf("int ");
        else if(pt->u.basic == BASIC_FLOAT) printf("float ");
    }
    else if(pt->kind == ARRAY) { // ARRAY
        printf("[%d] ", pt->u.array.size);
        pType tmp = pt->u.array.elem;
        while(tmp->kind != BASIC) {
            printf("[%d] ", tmp->u.array.size);
            tmp = tmp->u.array.elem;
        }
        if(tmp->u.basic == BASIC_INT) printf("int");
        else if(tmp->u.basic == BASIC_FLOAT) printf("float");
    }
    else if(pt->kind == STRUCTURE) { // STRUCTURE
        printf("structure\t\t");
        if(pt->u.structure->name != NULL) printf("tag: %s(%d)\t\t", pt->u.structure->name, lookup(pt->u.structure->name));
        else printf("tag: None\t\t");
        printf("member: ");
        pFieldList tmp = pt->u.structure->type->u.member;
        while(tmp != NULL){
            printf("%s(%d) ", tmp->name, lookup(tmp->name));
            tmp = tmp->tail;
        }
    }
    else if(pt->kind == STRUCT_TAG) { // STRUCT_TAG
        printf("struct_tag\tmember: ");
        pFieldList tmp = pt->u.member;
        while(tmp != NULL) {
            printf("%s(%d) ", tmp->name, lookup(tmp->name));
            tmp = tmp->tail;
        }
    }
    else if(pt->kind == FUNCTION) { // FUNCTION
        printf("ret: ");
        showtype(pt->u.function.ret);
        printf("\t\targc: %d\t\t\t", pt->u.function.argc);
        printf("argv: ");
        pFieldList tmp = pt->u.function.argv;
        while(tmp != NULL) {
            printf("%s(%d) ", tmp->name, lookup(tmp->name));
            tmp = tmp->tail;
        }
    }
    else assert(0);
}