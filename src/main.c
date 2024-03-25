#include <stdio.h>
#include "../include/diff.h"

// Пример (3 + 2) * (x - 777)

int main()
{
    DiffNode el1 = {TpNm, {3}, NULL, NULL}; // Узел для числа 3

    DiffNode el2 = {TpNm, {2}, NULL, NULL}; // Узел для числа 2

    DiffNode el3 = {TpOp, {0}, &el1, &el2}; // Узел для операции сложения (3+2)
    el3.Data.TypeOpVr = OpAdd;

    DiffNode el4 = {TpNm, {109}, NULL, NULL}; // Узел для числа 109

    DiffNode el5 = {TpNm, {13}, NULL, NULL}; // Узел для числа 13

    DiffNode el6 = {TpNm, {37}, NULL, NULL}; // Узел для числа 37

    DiffNode el7 = {TpNm, {16}, NULL, NULL}; // Узел для числа 16

    DiffNode el8 = {TpOp, {0}, &el6, &el7}; // Узел для операции вычитания (37-16)
    el8.Data.TypeOpVr = OpSub;

    DiffNode el9 = {TpOp, {0}, &el5, &el8}; // Узел для операции умножения (13*(37-16))
    el9.Data.TypeOpVr = OpMul;

    DiffNode el10 = {TpNm, {18}, NULL, NULL}; // Узел для числа 18

    DiffNode el11 = {TpOp, {0}, &el9, &el10}; // Узел для операции умножения (13*(37-16)*18)
    el11.Data.TypeOpVr = OpMul;

    DiffNode el12 = {TpNm, {238}, NULL, NULL}; // Узел для числа 238

    DiffNode el13 = {TpOp, {0}, &el11, &el12}; // Узел для операции сложения (13*(37-16)*18 + 238)
    el13.Data.TypeOpVr = OpAdd;

    DiffNode el14 = {TpOp, {0}, &el4, &el13}; // Узел для операции умножения (109*(13*(37-16)*18 + 238))
    el14.Data.TypeOpVr = OpMul;

    DiffNode el15 = {TpNm, {392}, NULL, NULL}; // Узел для числа 392

    DiffNode el16 = {TpOp, {0}, &el14, &el15}; // Узел для операции вычитания (109*(13*(37-16)*18 + 238) - 392)
    el16.Data.TypeOpVr = OpSub;

    DiffNode el17 = {TpOp, {0}, &el3, &el16}; // Узел для операции сложения ((3+2) + 109*(13*(37-16)*18 + 238) - 392)
    el17.Data.TypeOpVr = OpAdd;

    // DiffNode el1 = {TpNm, {3}, NULL, NULL};
    //
    // DiffNode el2 = {TpNm, {2}, NULL, NULL};
    //
    // DiffNode el3 = {TpOp, {0}, &el1, &el2};
    // el3.Data.TypeOpVr = OpAdd;
    //
    // DiffNode el4 = {TpVr, {0}, NULL, NULL};
    // el4.Data.TypeOpVr = VrX;
    // 
    // DiffNode el5 = {TpNm, {777}, NULL, NULL};
    //
    // DiffNode el6 = {TpOp, {0}, &el4, &el5};
    // el6.Data.TypeOpVr = OpSub;
    // 
    // DiffNode el7 = {TpOp, {0}, &el3, &el6};
    // el7.Data.TypeOpVr = OpMul;

    FILE *fp = fopen("test.dot", "w");
    PrintTreeDot(fp, &el17);
    fclose(fp);

    return 0;
}

// Функция для записи узла в формат .dot
int PrintNodesDot(FILE* fp, DiffNode* node)
{
    fprintf(fp, "\t\"%p\" [", node);
    if (node->Type == TpOp)
    {
        fprintf(fp, "label = \"%s\", color = %s, shape = %s]\n", ArrayOp[node->Data.TypeOpVr], OP_COLOR, OP_SHAPE);
    }
    else if (node->Type == TpNm)
    {   
        fprintf(fp, "label = \"%.2lf\", color = %s, shape = %s]\n", node->Data.Num, NM_COLOR, NM_SHAPE);
    }
    else if (node->Type == TpVr)
    {
        fprintf(fp, "label = \"%s\", color = %s, shape = %s]\n", ArrayVr[node->Data.TypeOpVr], VR_COLOR, VR_SHAPE);
    }

    if (node->LeftNode != NULL)
    {
        fprintf(fp, "\t\"%p\" -> \"%p\"\n", node, node->LeftNode);
        PrintNodesDot(fp, node->LeftNode);
    }
    if (node->RightNode)
    {
        fprintf(fp, "\t\"%p\" -> \"%p\"\n", node, node->RightNode);
        PrintNodesDot(fp, node->RightNode);
    }

    return 0;
}

// Функция для записи дерева в формат .dot
int PrintTreeDot(FILE *fp, DiffNode *node)
{
    fprintf(fp, "digraph {\n");
    PrintNodesDot(fp, node);
    fprintf(fp, "}\n");
    
    return 0;
}

