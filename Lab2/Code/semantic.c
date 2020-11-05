# include "semantic.h"

void traverseTree(Node* root) {
    if(root != NULL) Program(root);
    show(); // for debug only
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
        hashtable[i] = NULL；
    }
}

// 闭散列， 线性探测
void insert(pFieldList pf) { 
    if(pf == NULL) return;
    int index = hash(pf->name);
    for(int i = index; i != (index - 1) % HASHTABLE_SIZE; i = (i + 1) % HASHTABLE_SIZE) {
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

void Program(Node* node) {
    if(node == NULL) return;
    assert(node->childSum == 1);
    ExtDefList(node->children[0]);  // Program -> ExtDefList
}

void ExtDefList(Node* node) {

}

void show() {
    for(int i = 0; i < HASHTABLE_SIZE; ++i) {
        if(hashtable[i] != NULL) {
            printf("index: %d name: %s ", i, hashtable[i]->name);
            showtype(hashtable[i]->type);
            printf("\n");
        }
    }
}

void showtype(pType pt) {
    if(pt->kind == BASIC) { // BASIC
        if(pt->u.basic == BASIC_INT) printf("type: int");
        else if(pt->u.basic == BASIC_FLOAT) printf("type :float");
    }
    else if(pt->kind == ARRAY) { // ARRAY
        printf("type: [%d] ", pt->u.array.size);
        pType tmp = pt->u.array.elem;
        while(tmp->kind != BASIC) {
            printf("[%d] ", tmp->u.array.size);
            tmp = tmp->u.array.elem;
        }
        if(tmp->u.basic == BASIC_INT) printf("int");
        else if(tmp->u.basic == BASIC_FLOAT) printf("float");
    }
    else if(pt->kind == STRUCTURE) { // STRUCTURE
        printf("type: structure ");
        printf("tag: %s(%d)\n", pt->u.structure->name, lookup(pt->u.structure->name));
    }
    else if(pt->kind == STRUCT_TAG) { // STRUCT_TAG
        printf("type: struct_tag member: ");
        pFieldList tmp = pt->u.member;
        while(tmp != NULL) {
            printf("%s(%d) ", tmp->name, lookup(tmp->name));
            tmp = tmp->tail;
        }
    }
    else if(pt->kind == FUNCTION) { // FUNCTION
        printf("argc: %d ret: ", pt->u.function.argc);
        showtype(pt->u.function.ret);
        printf("argv: ");
        pFieldList tmp = pt->u.function.argv;
        while(tmp != NULL) {
            printf("%s(%d) ", tmp->name, lookup(tmp->name));
            tmp = tmp->tail;
        }
    }
    else assert(0);
}