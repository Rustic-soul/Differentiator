#include "differentiator.h"
#include "main.h"
#include "mydef.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

DiffNode *Differentiator(DiffNode *node, int part)
{
    assert(node != NULL);

    DiffNode *tmp = FullOptimizer(_CPY(node));
    assert(tmp != NULL);

    DiffNode *diff = DifferentiatorRec(tmp, part);
    assert(diff != NULL);

    DtorTree(tmp);
    
    return FullOptimizer(diff);
}

DiffNode *DifferentiatorRec(DiffNode *node, int part)
{
    assert(node != NULL);

    switch (NTYPE)
    {
    case TpNm: return _NUM(0);
    
    case TpVr: return _NUM((NDTYPE == part) ? 1.0 : 0);
    
    case TpOp:
        switch (NDTYPE)
        {
        case OpAdd: return _ADD(_DIFF(LNODE), _DIFF(RNODE));

        case OpSub: return _SUB(_DIFF(LNODE), _DIFF(RNODE));

        case OpMul: return _ADD(_MUL(_DIFF(LNODE), _CPY(RNODE)), _MUL(_CPY(LNODE), _DIFF(RNODE)));

        case OpDiv: return _DIV(_SUB(_MUL(_DIFF(LNODE), _CPY(RNODE)), _MUL(_CPY(LNODE), _DIFF(RNODE))), _POW(_CPY(RNODE), _NUM(2)));

        case OpPow:
            if (SearchVar(LNODE, part) == 0)
            {
                return _MUL(_LN(_CPY(LNODE)), _MUL(_POW(_CPY(LNODE), _CPY(RNODE)), _DIFF(RNODE)));              
            }
            else if ((SearchVar(LNODE, part) == 1) && (SearchVar(RNODE, part) == 0))
            {
                return _MUL(_MUL(_CPY(RNODE), _POW(_CPY(LNODE), _SUB(_CPY(RNODE), _NUM(1)))), _DIFF(LNODE));
            }
            else
            {
                return _MUL(_POW(_CPY(LNODE), _CPY(RNODE)), _ADD(_DIV(_MUL(_CPY(RNODE), _DIFF(LNODE)), _CPY(LNODE)), _MUL(_DIFF(RNODE), _LN(_CPY(LNODE)))));
            }
        
        default:
            PRINT_ERROR("Неизвестный тип операциии [%d]\n", NDTYPE);
            exit(ERROR_UNKNOWN_OPERATION_TYPE);
        }
    
    case TpFn:
        switch (NDTYPE)
        {
        case FnSin:  return _MUL(_COS(_CPY(LNODE)), _DIFF(LNODE));

        case FnCos:  return _MUL(_NUM(-1), _MUL(_SIN(_CPY(LNODE)), _DIFF(LNODE)));

        case FnTg:   return _MUL(_DIV(_NUM(1), _POW(_COS(_CPY(LNODE)), _NUM(2))), _DIFF(LNODE));

        case FnCtg:  return _MUL(_MUL(_NUM(-1), _DIV(_NUM(1), _POW(_SIN(_CPY(LNODE)), _NUM(2)))), _DIFF(LNODE));

        case FnAsin: return _MUL(_DIV(_NUM(1), _SQRT(_SUB(_NUM(1), _POW(_CPY(LNODE), _NUM(2))))), _DIFF(LNODE));
        
        case FnAcos: return _MUL(_NUM(-1), _MUL(_DIV(_NUM(1), _SQRT(_SUB(_NUM(1), _POW(_CPY(LNODE), _NUM(2))))), _DIFF(LNODE)));
        
        case FnAtg:  return _MUL(_DIV(_NUM(1), _ADD(_NUM(1), _POW(_CPY(LNODE), _NUM(2)))), _DIFF(LNODE));

        case FnActg: return _MUL(_NUM(-1),_MUL(_DIV(_NUM(1), _ADD(_NUM(1), _POW(_CPY(LNODE), _NUM(2)))), _DIFF(LNODE)));

        case FnSh:   return _MUL(_CH(_CPY(LNODE)), _DIFF(LNODE));
        
        case FnCh:   return _MUL(_SH(_CPY(LNODE)), _DIFF(LNODE));
        
        case FnTh:   return _MUL(_DIV(_NUM(1), _POW(_CH(_CPY(LNODE)), _NUM(2))), _DIFF(LNODE));

        case FnCth:  return _MUL(_NUM(-1), _MUL(_DIV(_NUM(1), _POW(_SH(_CPY(LNODE)), _NUM(2))), _DIFF(LNODE)));

        case FnExp:  return _MUL(_EXP(_CPY(LNODE)), _DIFF(LNODE));

        case FnSqrt: return _MUL(_DIV(_NUM(1), _MUL(_NUM(2), _SQRT(_CPY(LNODE)))), _DIFF(LNODE));
            
        case FnLn:   return _MUL(_DIV(_NUM(1), _CPY(LNODE)), _DIFF(LNODE));

        case FnLg:   return _MUL(_DIV(_NUM(1), _MUL(_LN(_NUM(10)), _CPY(LNODE))), _DIFF(LNODE));

        case FnLog:  return DifferentiatorLog(node, part);

        default:
            PRINT_ERROR("Неизвестный тип функции [%d]\n", NDTYPE);
            exit(ERROR_UNKNOWN_FUNCTION_TYPE);
        }

    default:
        PRINT_ERROR("Неизвестный тип узла [%d]\n", NTYPE);
        exit(ERROR_UNKNOWN_NODE_TYPE);
    }
}

DiffNode *DifferentiatorLog(DiffNode *node, int part)
{
    if (SearchVar(LNODE, part))
    {
        DiffNode *div_ln   = _DIV(_LN(_CPY(RNODE)), _LN(_CPY(LNODE)));
        DiffNode *diff_log = _DIFF(div_ln);

        DtorTree(div_ln);
        return diff_log;
    }
    else
    {
        if (DoubleEqual(CalculateTree(LNODE), 0))
        {
            PRINT_ERROR("Функция логарифма по основанию ноль не определена\n");
            exit(ERROR_RANGE_ACCEPTABLE_LOGARITHM_VALUES);
        }
        return _MUL(_DIV(_NUM(1), _MUL(_LN(_CPY(LNODE)), _CPY(RNODE))), _DIFF(RNODE));
    }
}

DiffNode *FullOptimizer (DiffNode *node) { return RemoveNeutralElements(ConvolutionConstants(node)); }

DiffNode *ConvolutionConstants(DiffNode *node)
{
    if ((NTYPE == TpVr) || (NTYPE == TpNm)) { return node; }

    LNODE = ConvolutionConstants(LNODE);

    if ((NTYPE != TpFn) || ((NTYPE == TpFn) && (NDTYPE == FnLog))) { RNODE = ConvolutionConstants(RNODE); }

    if ((NTYPE == TpOp) && (LNTYPE == TpNm) && (RNTYPE == TpNm))
    {
        switch (NDTYPE)
        {
        case OpAdd: node->Data.Num = LNODE->Data.Num + RNODE->Data.Num; break;
        case OpSub: node->Data.Num = LNODE->Data.Num - RNODE->Data.Num; break;
        case OpMul: node->Data.Num = LNODE->Data.Num * RNODE->Data.Num; break;
        case OpDiv: node->Data.Num = LNODE->Data.Num / RNODE->Data.Num; break;
        
        case OpPow: node->Data.Num = pow(LNODE->Data.Num, RNODE->Data.Num); break;
    
        default:
            PRINT_ERROR("Неизвестный тип операции [%d]\n", NDTYPE);
            exit(ERROR_UNKNOWN_OPERATION_TYPE);
        }

        free(LNODE);
        free(RNODE);
        
        NTYPE = TpNm;
        LNODE = NULL;
        RNODE = NULL;
    }
    else if (((NTYPE == TpFn) && (LNTYPE == TpNm) && (NDTYPE != FnLog)) || ((NTYPE == TpFn) && (LNTYPE == TpNm) && (RNTYPE == TpNm) && (NDTYPE == FnLog)))
    {
        switch (NDTYPE)
        {
        case FnSin:  node->Data.Num = sin  (LNODE->Data.Num); break;
        case FnCos:  node->Data.Num = cos  (LNODE->Data.Num); break;
        case FnTg:   node->Data.Num = tan  (LNODE->Data.Num); break;
        case FnAtg:  node->Data.Num = atan (LNODE->Data.Num); break;
        case FnSh:   node->Data.Num = sinh (LNODE->Data.Num); break;
        case FnCh:   node->Data.Num = cosh (LNODE->Data.Num); break;
        case FnTh:   node->Data.Num = tanh (LNODE->Data.Num); break;
        case FnExp:  node->Data.Num = exp  (LNODE->Data.Num); break;
        case FnSqrt: node->Data.Num = sqrt (LNODE->Data.Num); break;
        case FnLn:   node->Data.Num = log  (LNODE->Data.Num); break;
        case FnLg:   node->Data.Num = log10(LNODE->Data.Num); break;
        
        case FnCtg:  node->Data.Num = tan(3.14/2 - LNODE->Data.Num);                 break;

        case FnActg: node->Data.Num = 3.14/2 - atan(LNODE->Data.Num);                break;
        
        case FnCth:  node->Data.Num = cosh(LNODE->Data.Num) / sinh(LNODE->Data.Num); break;
        
        default:
            PRINT_ERROR("Неизвестный тип функции [%d]\n", NDTYPE);
            exit(ERROR_UNKNOWN_FUNCTION_TYPE);
        }
        
        if (NDTYPE == FnLog)
        {
            node->Data.Num = log(RNODE->Data.Num) / log(LNODE->Data.Num); 
            free(RNODE);
        }

        free(LNODE);
        
        NTYPE = TpNm;
        LNODE = NULL;
        RNODE = NULL;
    }

    return node;
}

DiffNode *RemoveNeutralElements(DiffNode *node)
{
    if (NTYPE == TpOp)
    {
        LNODE = RemoveNeutralElements(LNODE);
        RNODE = RemoveNeutralElements(RNODE);
        
        if ((LNTYPE == TpNm) || (RNTYPE == TpNm))
        {
            DiffNode *num_node = (LNTYPE == TpNm) ? LNODE : RNODE;
            DiffNode *nan_node = (LNTYPE == TpNm) ? RNODE : LNODE;
            
            switch (NDTYPE)
            {
            case OpAdd:
                if (DoubleEqual(num_node->Data.Num, 0))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpSub:
                if (DoubleEqual(num_node->Data.Num, 0))
                {
                    if (num_node == RNODE) { return node; } //  Мб нужно придумывать умножение на (-1)
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpMul:
                if (DoubleEqual(num_node->Data.Num, 0))
                {
                    DtorTree(nan_node);
                    free(node);
                    return num_node;
                }
                else if (DoubleEqual(num_node->Data.Num, 1))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpDiv:
                if ((num_node == LNODE) && DoubleEqual(num_node->Data.Num, 0))
                {
                    DtorTree(nan_node);
                    free(node);
                    return num_node;
                }
                else if ((num_node == RNODE) && DoubleEqual(num_node->Data.Num, 1))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpPow:
                if ((num_node == LNODE) && (DoubleEqual(num_node->Data.Num, 1) || DoubleEqual(num_node->Data.Num, 0)))
                {
                    DtorTree(nan_node);
                    free(node);
                    return num_node;
                }
                else if ((num_node == RNODE) && DoubleEqual(num_node->Data.Num, 1))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                else if ((num_node == RNODE) && DoubleEqual(num_node->Data.Num, 0))
                {
                    DtorTree(node);
                    return _NUM(1);
                }
                break;

            default:
                PRINT_ERROR("Неизвестный тип операции [%d]\n", NDTYPE);
                exit(ERROR_UNKNOWN_OPERATION_TYPE);
            }
        }
    }

    return node;
}

int SearchVar(DiffNode *node, int var)
{
    int tmp = 0;
    
    if ((NTYPE == TpVr) && (NDTYPE == var)) { tmp = 1; }
    
    if ((LNODE != NULL) && (tmp != 1)) { tmp = SearchVar(LNODE, var); }

    if ((RNODE != NULL) && (tmp != 1)) { tmp = SearchVar(RNODE, var); }
    
    return tmp;
}

int DoubleEqual(double num1, double num2)
{
    const double EPSILON = 1e-6;

    return (fabs(num1 - num2) < EPSILON);
}

DiffNode *CopyNode(DiffNode *node)
{
    if (node == NULL) { return NULL; }

    DiffNode *newnode = CreateNode(NTYPE, node->Data, _CPY(LNODE), _CPY(RNODE));

    return newnode;
}

DiffNode *CreateNode(char type, NodeData data, DiffNode *LeftNode, DiffNode *RightNode)
{
    DiffNode *newnode  = (DiffNode *)malloc(sizeof(DiffNode));
    
    PRINT_DEBUG("CreateNode: {%p}\n", newnode);

    newnode->Type      = type;
    newnode->Data      = data;
    newnode->LeftNode  = LeftNode;
    newnode->RightNode = RightNode;

    return newnode;
}

int DtorTree(DiffNode *node)
{
    if (node->LeftNode != NULL)  { DtorTree(node->LeftNode); }

    if (node->RightNode != NULL) { DtorTree(node->RightNode); }

    PRINT_DEBUG("FREE: [%p]\n", node);
    free(node);
    return 0;
}
