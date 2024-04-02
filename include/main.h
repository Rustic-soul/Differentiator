#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

typedef union NodeData {
    double Num;
    int    TypeOpVr;
} NodeData;

typedef struct DiffNode
{
    char Type;

    NodeData Data;

    struct DiffNode *LeftNode;
    struct DiffNode *RightNode;
} DiffNode;

enum ClassVariable
{
    VrX = 0,
    VrY = 1,
    VrZ = 2,
};

DiffNode *CreateTree    (FILE     *fp);
int       DtorTree      (DiffNode *node);
int       PrintTreeDot  (FILE     *fp, DiffNode *node);

double    CalculateTree (DiffNode *node);

DiffNode *Differentiator(DiffNode *node, int part);

DiffNode *Optimizer1    (DiffNode *node);
DiffNode *Optimizer2    (DiffNode *node);
DiffNode *FullOptimizer (DiffNode *node);

#endif // !MAIN_H
