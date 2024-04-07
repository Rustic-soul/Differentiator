#include "main.h"
#include <bits/types/FILE.h>
#define PRINT_CPP

#include "differentiator.h"
#include "mydef.h"
#include "config.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int PrintExpression(FILE *fp, DiffNode *node);

int CreateResTex(FILE *fp, DiffNode *TreeExp, DiffNode *TreeDiff)
{
    assert(fp != NULL);

    fprintf(fp, "\\input{preamble} \n\n");

    fprintf(fp, "\\begin{document} \n\n");

    fprintf(fp, "\\input{title} \n\n");
    
    // fprintf(fp, "\\text{Исходное выражение:}\n");

    fprintf(fp, "\\begin{dmath}\n\t");
    PrintExpression(fp, TreeExp);
    fprintf(fp, "\n\\end{dmath}\n\n");

    // fprintf(fp, "\\text{Производная данного выражения:}\n");

    fprintf(fp, "\\begin{dmath}\n\t");
    PrintExpression(fp, TreeDiff);
    fprintf(fp, "\n\\end{dmath}\n\n");

    fprintf(fp, "\\end{document}");

    return 0;
}

// Функция для записи узла в формат .dot
int PrintNodesDot(FILE *fp, DiffNode *node)
{
    fprintf(fp, "\t\"%p\" [", node);
    if (NTYPE == TpOp)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | TypeOperation: %c | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], ArrayOp[NDTYPE], node, LNODE, RNODE, OP_COLOR, OP_SHAPE);
    }
    else if (NTYPE == TpNm)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | Num: %.2lf | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], node->Data.Num, node, LNODE, RNODE, NM_COLOR, NM_SHAPE);
        // fprintf(fp, "label = \"%.2lf\", style = filled, fillcolor = %s, shape = %s]\n", node->Data.Num, NM_COLOR, NM_SHAPE);
    }
    else if (NTYPE == TpVr)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | Var: %c | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], NDTYPE, node, LNODE, RNODE, VR_COLOR, VR_SHAPE);
        // fprintf(fp, "label = \"%c\", style = filled, fillcolor = %s, shape = %s]\n", ArrayVr[node->Data.AdditionalType], VR_COLOR, VR_SHAPE);
    }
    else if (NTYPE == TpFn)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | TypeFunction: %s | %p | { %p | %p } }\", fontsize = 18, style = filled, fillcolor = %s, shape = %s]\n", ArrayTp[NTYPE], ArrayFn[NDTYPE], node, LNODE, RNODE, FN_COLOR, FN_SHAPE);
        // fprintf(fp, "label = \"%s\", style = filled, fillcolor = %s, shape = %s]\n", ArrayFn[node->Data.AdditionalType], FN_COLOR, FN_SHAPE);
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

int PrintExpression(FILE *fp, DiffNode *node)
{
    if (NTYPE == TpOp)
    {
        if (NDTYPE == OpPow) 
        {
            if ((LNTYPE != TpVr) && (LNTYPE != TpNm) && (LNTYPE != TpFn)) { fprintf(fp, "("); }
            PrintExpression(fp, LNODE);
            if ((LNTYPE != TpVr) && (LNTYPE != TpNm) && (LNTYPE != TpFn)) { fprintf(fp, ")"); }

            fprintf(fp, "^");

            fprintf(fp, "{");            
            PrintExpression(fp, RNODE);
            fprintf(fp, "}");
        }
        else if (NDTYPE == OpMul)
        {
            if ((LNTYPE == TpOp) && ((LNODE->Data.AdditionalType == OpAdd) || (LNODE->Data.AdditionalType == OpSub))) { fprintf(fp, "("); }
            PrintExpression(fp, LNODE);
            if ((LNTYPE == TpOp) && ((LNODE->Data.AdditionalType == OpAdd) || (LNODE->Data.AdditionalType == OpSub))) { fprintf(fp, ")"); }

            fprintf(fp, " \\times ");

            if ((RNTYPE == TpOp) && ((RNODE->Data.AdditionalType == OpAdd) || (RNODE->Data.AdditionalType == OpSub))) { fprintf(fp, "("); }
            PrintExpression(fp, RNODE);
            if ((RNTYPE == TpOp) && ((RNODE->Data.AdditionalType == OpAdd) || (RNODE->Data.AdditionalType == OpSub))) { fprintf(fp, ")"); }
        }
        else if (NDTYPE == OpDiv)
        {
            fprintf(fp, "\\frac");
            fprintf(fp, "{");
            PrintExpression(fp, LNODE);
            fprintf(fp, "}");

            fprintf(fp, "{");
            PrintExpression(fp, RNODE);
            fprintf(fp, "}");
        }
        else if ((NDTYPE == OpAdd) || (NDTYPE == OpSub))
        {
            PrintExpression(fp, LNODE);

            fprintf(fp, " %c ", ArrayOp[NDTYPE]);  

            PrintExpression(fp, RNODE);
        }
        // Добавить обработку ошибки
    }
    else if (NTYPE == TpFn)
    {
        fprintf(fp, "%s", ArrayFn[NDTYPE]);
        fprintf(fp, "(");
        PrintExpression(fp, LNODE);
        if (NDTYPE == FnLog)
        {
            fprintf(fp, ", ");
            PrintExpression(fp, RNODE);
        }
        fprintf(fp, ")");
    }
    else if (NTYPE == TpVr)
    {
        fprintf(fp, "%c", NDTYPE);
    }
    else if (NTYPE == TpNm)
    {
        if (node->Data.Num < -1e-6) { fprintf(fp, "("); }
        fprintf(fp, "%.2lf", node->Data.Num);
        if (node->Data.Num < -1e-6) { fprintf(fp, ")"); }
    }

    return 0;
}
