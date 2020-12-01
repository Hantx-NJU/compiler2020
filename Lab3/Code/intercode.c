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
    
}
