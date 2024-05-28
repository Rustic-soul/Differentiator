#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#include "differentiator.h"
#include "config.h"
#include "DSL.h"
#include "mydef.h"

int       PrintNodesDot(FILE *fp, node_t *node);

int CreateResTex(FILE *fp, node_t *TreeExp, node_t *TreeDiff)
{
    assert(fp != NULL);

    fprintf(fp, "\\input{preamble} \n\n");

    fprintf(fp, "\\begin{document} \n\n");

    fprintf(fp, "\\input{title} \n\n");
    
    fprintf(fp, "\\text{Исходное выражение:}\n");
    fprintf(fp, "\\begin{dmath}\n\t");
    PrintExpression(fp, TreeExp);
    fprintf(fp, "\n\\end{dmath}\n\n");

    fprintf(fp, "\\text{Производная данного выражения:}\n");
    fprintf(fp, "\\begin{dmath}\n\t");
    PrintExpression(fp, TreeDiff);
    fprintf(fp, "\n\\end{dmath}\n\n");

    fprintf(fp, "\\end{document}");

    return 0;
}

// Функция для записи узла в формат .dot
int PrintNodesDot(FILE *fp, node_t *node)
{
    fprintf(fp, "\t\"%p\" [", node);
    if (NTYPE == TpOp)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | TypeOperation: %c | %p | { %p | %p } }\", fontsize = %d, style = %s, fillcolor = %s, shape = %s]\n",
            ArrayTp[NTYPE], ArrayOp[NDTYPE], node, LNODE, RNODE, DOT_FONT_SIZE, DOT_OP_STYLE, DOT_OP_COLOR, DOT_OP_SHAPE);
    }
    else if (NTYPE == TpNm)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | Num: %.2lf | %p | { %p | %p } }\", fontsize = %d, style = %s, fillcolor = %s, shape = %s]\n",
            ArrayTp[NTYPE], node->data.num, node, LNODE, RNODE, DOT_FONT_SIZE, DOT_NM_STYLE, DOT_NM_COLOR, DOT_NM_SHAPE);
    }
    else if (NTYPE == TpVr)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | Var: %c | %p | { %p | %p } }\", fontsize = %d, style = %s, fillcolor = %s, shape = %s]\n",
            ArrayTp[NTYPE], NDTYPE, node, LNODE, RNODE, DOT_FONT_SIZE, DOT_VR_STYLE, DOT_VR_COLOR, DOT_VR_SHAPE);
    }
    else if (NTYPE == TpFn)
    {
        fprintf(fp, "label = \"{ Nodetype: %s | TypeFunction: %s | %p | { %p | %p } }\", fontsize = %d, style = %s, fillcolor = %s, shape = %s]\n",
            ArrayTp[NTYPE], ArrayFn[NDTYPE], node, LNODE, RNODE, DOT_FONT_SIZE, DOT_FN_STYLE, DOT_FN_COLOR, DOT_FN_SHAPE);
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
int PrintTreeDot(FILE *fp, node_t *node)
{
    assert(fp != NULL);
    assert(node != NULL);

    fprintf(fp, "digraph {\n");
    PrintNodesDot(fp, node);
    fprintf(fp, "}\n");

    return 0;
}

int PrintExpression(FILE *fp, node_t *node)
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
            if ((LNTYPE == TpOp) && ((LNODE->data.additional_type == OpAdd) || (LNODE->data.additional_type == OpSub))) { fprintf(fp, "("); }
            PrintExpression(fp, LNODE);
            if ((LNTYPE == TpOp) && ((LNODE->data.additional_type == OpAdd) || (LNODE->data.additional_type == OpSub))) { fprintf(fp, ")"); }

            fprintf(fp, " \\times ");

            if ((RNTYPE == TpOp) && ((RNODE->data.additional_type == OpAdd) || (RNODE->data.additional_type == OpSub))) { fprintf(fp, "("); }
            PrintExpression(fp, RNODE);
            if ((RNTYPE == TpOp) && ((RNODE->data.additional_type == OpAdd) || (RNODE->data.additional_type == OpSub))) { fprintf(fp, ")"); }
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
        //TODO Добавить обработку ошибки
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
        if (node->data.num < -1e-6) { fprintf(fp, "("); }
        fprintf(fp, "%.2lf", node->data.num);
        if (node->data.num < -1e-6) { fprintf(fp, ")"); }
    }

    return 0;
}
