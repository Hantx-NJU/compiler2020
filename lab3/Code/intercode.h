# ifndef     __INTERCODE_H__
# define     __INTERCODE_H__

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include "tree.h"

typedef struct Operand* pOperand;
struct Operand {
    enum {VARIABLE, CONSTANT, ADDRESS, LABEL, FUNCTION, STRUCTURE} kind;
    union{
        int var_no;
        char value[32];
        pOperand target;
    }
}Operand;

typedef struct InterCode* pInterCode;
struct InterCode{
    enum {LABEL, FUNCTION, ASSIGN, PLUS, MINUS, MUL, DIV, GET_ADDR, GO_ADDR, STORE_ADDR, GOTO, IF_GOTO,
            RETURN, DEC, ARG, CALL, PARAM, READ, WRITE} kind;
    union{
        // LABEL x : || FUNCTION f : || GOTO x || RETURN x || ARG x || PARAM x || READ x || WRITE x
        struct {
            pOperand op;
        }singleOP;
        // x := y || x := &y || x := *y || *x := y || x := CALL f
        struct{
            pOperand left, right;
        }doubleOP;
        // x := y + z || x := y - z || x := y * z || x := y / z
        struct{
            pOperand result, op1, op2;
        }tripleOP;
        // IF x [relop] y GOTO z
        struct{
            pOperand op1 , op2, label;
            char relop[32];
        }if_gotoOP;
        // DEC x [size]
        struct{
            pOperand op;
            int size;
        }decOP;
    }
}























#endif