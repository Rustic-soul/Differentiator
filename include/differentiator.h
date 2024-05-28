#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>

union Node_data_t {
    double num;
    int    additional_type;
};
typedef union Node_data_t node_data_t;

struct Node_t {
    char type;

    node_data_t data;

    struct Node_t *LeftNode;
    struct Node_t *RightNode;
};
typedef struct Node_t node_t;

enum ClassNodeType {
    TpNm = 0,
    TpVr = 1,
    TpOp = 2,
    TpFn = 3,
    TpCn = 4,
};

enum ClassOperation {
    OpAdd = 0,
    OpSub = 1,
    OpMul = 2,
    OpDiv = 3,
    OpPow  = 4 
};

enum ClassFunction {
    FnSin  = 0,
    FnCos  = 1,
    FnTg   = 2,
    FnCtg  = 3,
    FnSh   = 4,
    FnCh   = 5,
    FnLn   = 6,
    FnLg   = 7,
    FnLog  = 8,
    FnSqrt = 9,
    FnAsin = 10,
    FnAcos = 11,
    FnAtg  = 12,
    FnActg = 13,
    FnTh   = 14,
    FnCth  = 15,
    FnExp  = 16,
};

enum ClassConstant {
    CnE  = 0,
    CnPi = 1,
};

static const char *ArrayCn[] = { "e", "pi" };

static const double ConstArray[] = { 2.7, 3.14 };

static const char* ArrayFn[] = {
    "sin",
    "cos",
    "tg",
    "ctg",
    "sh",
    "ch",
    "ln",
    "lg",
    "log",
    "sqrt",
    "arcsin",
    "arccos",
    "arctg",
    "arcctg",
    "th",
    "cth",
    "exp"
};

static const char* ArrayTp[] = {
    "Num",
    "Variable",
    "Operation",
    "Function",
    "Constant"
};

static const char ArrayOp[] = { '+', '-', '*', '/', '^' };

enum ErrorCode {
    ERROR_UNKNOWN_NODE_TYPE      = -1,
    ERROR_UNKNOWN_OPERATION_TYPE = -2,
    ERROR_UNKNOWN_FUNCTION_TYPE  = -3,
    ERROR_SYNTAX                 = -4,
    ERROR_RANGE_ACCEPTABLE_LOGARITHM_VALUES = -5,
    ERROR_NULL_PTR = -6,
    ERROR_FTELL_FAILED = -7
};

int       SearchVar(node_t *node, int var);
int       DoubleEqual(double num1, double num2);
node_t* CopyNode(node_t *node);
node_t* CreateNode(char type, node_data_t data, node_t *LeftNode, node_t *RightNode);

node_t* convolution_constants (node_t *node);
node_t* remove_neutral_elements(node_t *node);
node_t* FullOptimizer        (node_t *node);

node_t* ReadTreeFile  (FILE     *fp);
node_t* Differentiator(node_t *node, int part);
double    CalculateTree (node_t *node);

int       DtorTree      (node_t *node);

int       PrintTreeDot  (FILE *fp, node_t *node);
int       CreateResTex  (FILE *fp, node_t *TreeExp, node_t *TreeDiff);

int PrintExpression(FILE *fp, node_t *node);

#endif
