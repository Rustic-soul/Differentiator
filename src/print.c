#define PRINT_CPP

#include "differentiator.h"
#include "mydef.h"
#include "config.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

// Функция для записи узла в формат .dot
int PrintNodesDot(FILE *fp, DiffNode *node)
{

    fprintf(fp, "\t\"%p\" [", node);
    if (NTYPE == TpOp)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | TypeOperation: %c | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], ArrayOp[OP_VR_TYPE], node, LNODE, RNODE, OP_COLOR, OP_SHAPE);
    }
    else if (NTYPE == TpNm)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | Num: %.2lf | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], node->Data.Num, node, LNODE, RNODE, NM_COLOR, NM_SHAPE);
        // fprintf(fp, "label = \"%.2lf\", style = filled, fillcolor = %s, shape = %s]\n", node->Data.Num, NM_COLOR, NM_SHAPE);
    }
    else if (NTYPE == TpVr)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | Var: %c | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], ArrayVr[OP_VR_TYPE], node, LNODE, RNODE, VR_COLOR, VR_SHAPE);
        // fprintf(fp, "label = \"%c\", style = filled, fillcolor = %s, shape = %s]\n", ArrayVr[node->Data.TypeOpVr], VR_COLOR, VR_SHAPE);
    }
    else if (NTYPE == TpFn)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | TypeFunction: %s | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], ArrayFn[OP_VR_TYPE], node, LNODE, RNODE, FN_COLOR, FN_SHAPE);
        // fprintf(fp, "label = \"%s\", style = filled, fillcolor = %s, shape = %s]\n", ArrayFn[node->Data.TypeOpVr], FN_COLOR, FN_SHAPE);
    }

    if (LNODE != NULL)
    {
        fprintf(fp, "\t\"%p\" -> \"%p\"\n", node, node->LeftNode);
        PrintNodesDot(fp, node->LeftNode);
    }
    if (RNODE != NULL)
    {
        fprintf(fp, "\t\"%p\" -> \"%p\"\n", node, node->RightNode);
        PrintNodesDot(fp, node->RightNode);
    }

    return 0;
}

// Функция для записи дерева в формат .dot
int PrintTreeDot(FILE *fp, DiffNode *node)
{
    assert(fp != NULL);
    assert(node != NULL);

    fprintf(fp, "digraph {\n");
    PrintNodesDot(fp, node);
    fprintf(fp, "}\n");

    return 0;
}
