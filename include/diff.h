#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdio.h>

union Data {
    double   Num;
    uint64_t TypeOpVr;  
};

typedef struct DiffNode {
    char Type;
    
    union Data Data;

    struct DiffNode *LeftNode;
    struct DiffNode *RightNode;
} DiffNode;

enum ClassType {
    TpNm  = 0,
    TpVr  = 1,
    TpOp  = 2,
};

enum ClassOperation {
    OpAdd = 0,
    OpSub = 1,
    OpMul = 2,
    OpDiv = 3,
};

enum ClassVariable {
    VrX = 0,
    VrY = 1,
    VrZ = 2,
};

static const char* OP_COLOR = "red";
static const char* NM_COLOR = "blue";
static const char* VR_COLOR = "green";

static const char* OP_SHAPE = "box";
static const char* NM_SHAPE = "oval";
static const char* VR_SHAPE = "octagon";

static const char *ArrayOp[] = {
    "+",
    "-",
    "*",
    ":",
};

static const char *ArrayVr[] = {
    "x",
    "y",
    "z",
};

int PrintNodesDot(FILE* fp, DiffNode *node);
int PrintTreeDot (FILE *fp, DiffNode *node);

#endif // !MAIN_H
