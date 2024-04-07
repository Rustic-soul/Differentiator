#ifndef MAIN_H
#define MAIN_H

#include <bits/types/FILE.h>
#include <stdio.h>

typedef union NodeData {
    double Num;
    int    AdditionalType;
} NodeData;

typedef struct DiffNode {
    char Type;

    NodeData Data;

    struct DiffNode *LeftNode;
    struct DiffNode *RightNode;
} DiffNode;

enum ClassVariable {
    VrX = 0,
    VrY = 1,
    VrZ = 2,
};

DiffNode *ReadTreeFile  (FILE     *fp);
DiffNode *Differentiator(DiffNode *node, int part);
double    CalculateTree (DiffNode *node);

int       DtorTree      (DiffNode *node);

int       PrintTreeDot  (FILE *fp, DiffNode *node);
int       CreateResTex  (FILE *fp, DiffNode *TreeExp, DiffNode *TreeDiff);

#endif // !MAIN_H
