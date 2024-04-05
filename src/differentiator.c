#define DIFFERENTIATOR_CPP

#include "differentiator.h"
#include "main.h"
#include "mydef.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


DiffNode *Differentiator(DiffNode *node, int part)
{
    switch (NTYPE)
    {
    case TpNm: return _NUM(0);
    
    case TpVr: return _NUM((OP_VR_TYPE == part) ? 1.0 : 0);
    
    case TpOp:
        switch (OP_VR_TYPE)
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
            PRINT_ERROR("Неизвестный тип операциии [%d]\n", OP_VR_TYPE);
            exit(ERROR_UNKNOWN_OPERATION_TYPE);
        }
    
    case TpFn:
        switch (OP_VR_TYPE)
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

        case FnLog:
            if (SearchVar(LNODE, part) == 0)
            {
                printf("uuuu\n");
                return _MUL(_DIV(_NUM(1), _MUL(_LN(_CPY(LNODE)), _CPY(RNODE))), _DIFF(RNODE));
            }
            else
            {
                printf("oooo\n");
                DiffNode *diffl = _LN(_CPY(LNODE));
                DiffNode *diffr = _LN(_CPY(RNODE));
                DiffNode * tmp = _DIV(_SUB(_MUL(_DIFF(diffr), _LN(_CPY(LNODE))), _MUL(_DIFF(diffl), _LN(_CPY(RNODE)))), _POW(_LN(_CPY(LNODE)), _NUM(2)));
                DtorTree(diffl);
                DtorTree(diffr);
                return tmp;
            }

        default:
            PRINT_ERROR("Неизвестный тип функции [%d]\n", OP_VR_TYPE);
            exit(ERROR_UNKNOWN_FUNCTION_TYPE);
        }

    default:
        PRINT_ERROR("Неизвестный тип узла [%d]\n", NTYPE);
        exit(ERROR_UNKNOWN_NODE_TYPE);
    }

    return NULL;
}

DiffNode *FullOptimizer (DiffNode *node) { return Optimizer2(Optimizer1(node)); }

DiffNode *Optimizer1(DiffNode *node)
{
    if ((NTYPE == TpVr) || (NTYPE == TpNm)) { return node; }

    LNODE = Optimizer1(LNODE);

    if ((NTYPE != TpFn) || ((NTYPE == TpFn) && (OP_VR_TYPE == FnLog))) { RNODE = Optimizer1(RNODE); }

    if ((NTYPE == TpOp) && (LNTYPE == TpNm) && (RNTYPE == TpNm))
    {
        switch (OP_VR_TYPE)
        {
        case OpAdd: node->Data.Num = LNODE->Data.Num + RNODE->Data.Num; break;
        case OpSub: node->Data.Num = LNODE->Data.Num - RNODE->Data.Num; break;
        case OpMul: node->Data.Num = LNODE->Data.Num * RNODE->Data.Num; break;
        case OpDiv: node->Data.Num = LNODE->Data.Num / RNODE->Data.Num; break;
        
        case OpPow: node->Data.Num = pow(LNODE->Data.Num, RNODE->Data.Num); break;
    
        default:
            PRINT_ERROR("Неизвестный тип операции [%d]\n", OP_VR_TYPE);
            exit(ERROR_UNKNOWN_OPERATION_TYPE);
        }

        free(LNODE);
        free(RNODE);
        
        NTYPE = TpNm;
        LNODE = NULL;
        RNODE = NULL;
    }
    else if (((NTYPE == TpFn) && (LNTYPE == TpNm) && (OP_VR_TYPE != FnLog)) || ((NTYPE == TpFn) && (LNTYPE == TpNm) && (RNTYPE == TpNm) && (OP_VR_TYPE == FnLog)))
    {
        switch (OP_VR_TYPE)
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
            PRINT_ERROR("Неизвестный тип функции [%d]\n", OP_VR_TYPE);
            exit(ERROR_UNKNOWN_FUNCTION_TYPE);
        }
        
        if (OP_VR_TYPE == FnLog)
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

DiffNode *Optimizer2(DiffNode *node)
{
    if (NTYPE == TpOp)
    {
        LNODE = Optimizer2(LNODE);
        RNODE = Optimizer2(RNODE);
        
        if ((LNTYPE == TpNm) || (RNTYPE == TpNm))
        {
            DiffNode *num_node = (LNTYPE == TpNm) ? LNODE : RNODE;
            DiffNode *nan_node = (LNTYPE == TpNm) ? RNODE : LNODE;
            
            switch (OP_VR_TYPE)
            {
            case OpAdd:
                if (equal(num_node->Data.Num, 0))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpSub:
                if (equal(num_node->Data.Num, 0))
                {
                    if (num_node == RNODE) { return node; } //  Мб нужно придумывать умножение на (-1)
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpMul:
                if (equal(num_node->Data.Num, 0))
                {
                    DtorTree(nan_node);
                    free(node);
                    return num_node;
                }
                else if (equal(num_node->Data.Num, 1))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpDiv:
                if ((num_node == LNODE) && equal(num_node->Data.Num, 0))
                {
                    DtorTree(nan_node);
                    free(node);
                    return num_node;
                }
                else if ((num_node == RNODE) && equal(num_node->Data.Num, 1))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                break;

            case OpPow:
                if ((num_node == LNODE) && (equal(num_node->Data.Num, 1) || equal(num_node->Data.Num, 0)))
                {
                    DtorTree(nan_node);
                    free(node);
                    return num_node;
                }
                else if ((num_node == RNODE) && equal(num_node->Data.Num, 1))
                {
                    free(num_node);
                    free(node);
                    return nan_node;
                }
                else if ((num_node == RNODE) && equal(num_node->Data.Num, 0))
                {
                    DtorTree(node);
                    return _NUM(1);
                }
                break;

            default:
                PRINT_ERROR("Неизвестный тип операции [%d]\n", OP_VR_TYPE);
                exit(ERROR_UNKNOWN_OPERATION_TYPE);
            }
        }
    }

    return node;
}

int SearchVar(DiffNode *node, int var)
{
    int tmp = 0;
    
    if ((NTYPE == TpVr) && (OP_VR_TYPE == var)) { tmp = 1; }
    
    if ((LNODE != NULL) && (tmp != 1)) { tmp = SearchVar(LNODE, var); }

    if ((RNODE != NULL) && (tmp != 1)) { tmp = SearchVar(RNODE, var); }
    
    return tmp;
}

int equal(double num1, double num2)
{
    const double EPSILON = 1e-6;

    return (fabs(num1 - num2) < EPSILON);
}

DiffNode *Copy_Node(DiffNode *node)
{
    // DiffNode *newnode  = (DiffNode *)malloc(sizeof(DiffNode));
    // newnode->Type      = node->Type;
    // newnode->Data      = node->Data;
    // newnode->LeftNode  = NULL;
    // newnode->RightNode = NULL;
    DiffNode *newnode = CreateNode(NTYPE, node->Data, NULL, NULL);

    if (LNODE != NULL) { newnode->LeftNode = Copy_Node(LNODE); }

    if (RNODE != NULL) { newnode->RightNode = Copy_Node(RNODE); }

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

DiffNode *CreateTree(FILE *fp)
{
    assert(fp != NULL);

    size_t    cn   = 0;
    array_t  *arr  = CtorArray(fp);
    DiffNode *tree = CreateTreeRec(arr, &cn);

    free(arr->arr_ptr);
    free(arr);

    return tree;
}

DiffNode *CreateTreeRec(array_t *sarr, size_t *count)
{
    assert(sarr != NULL);
    assert(count != NULL);
    assert(sarr->size_arr > *count);

    DiffNode *node = NULL;

    while (sarr->arr_ptr[*count] == ' ') { *count += 1; }

    if (sarr->arr_ptr[*count] == '(')
    {
        *count += 1;

        int    ncr                          = 0;
        double tmp_num                      = 0;
        char   type                         = DefinitionTypeNode(sarr->arr_ptr + (*count));
        char   tmp2                         = '\0';
        char   func_name[MAX_LEN_FUNC_NAME] = "";

        switch (type)
        {
        case -1:
            sscanf(sarr->arr_ptr + (*count), "%s%n", func_name, &ncr);
            *count += (size_t)ncr;
            node = NULL;
            break;

        case TpNm:
            sscanf(sarr->arr_ptr + (*count), "%lf%n", &tmp_num, &ncr);
            *count += (size_t)ncr;
            node = CreateNode(type, (NodeData){tmp_num}, NULL, NULL);
            break;

        case TpFn:
            sscanf(sarr->arr_ptr + (*count), "%s%n", func_name, &ncr);
            *count += (size_t)ncr;
            
            node = CreateNode(type, (NodeData){.TypeOpVr = DTFunc(func_name)}, NULL, NULL);
            break;
        
        case TpOp: case TpVr:
            sscanf(sarr->arr_ptr + (*count), "%s%n", func_name, &ncr);
            *count += (size_t)ncr;
            NodeData tmp3 = {0};
            tmp2          = func_name[0];
            tmp3.TypeOpVr = DefinitionTypeVariableAndOperation(tmp2);

            node = CreateNode(type, tmp3, NULL, NULL);
            break;

        default:
            PRINT_ERROR("Неизвестный тип узла [%d]\n", NTYPE);
            exit(ERROR_UNKNOWN_NODE_TYPE);
        }

        if (sarr->arr_ptr[*count + 1] == ')')
        {
            if (node != NULL)
            {
                node->LeftNode  = NULL;
                node->RightNode = NULL;
            }

            *count += 2;
            return node;
        }
        node->LeftNode  = CreateTreeRec(sarr, count);

        node->RightNode = CreateTreeRec(sarr, count);
    }
    *count += 1;
    return node;
}

int DTFunc(const char* fname)
{
    for (int i = 0; (size_t)i < SIZEOF_ARR(ArrayFn); i++)
    {
        if (strcmp(fname, ArrayFn[i]) == 0) { return i; }
    }

    return -1;
}

int DefinitionTypeVariableAndOperation(char ch)
{
    for (int i = 0; (size_t)i < SIZEOF_ARR(ArrayOp); i++)
    {
        if (ArrayOp[i] == ch) { return i; }
    }

    for (int i = 0; (size_t)i < SIZEOF_ARR(ArrayVr); i++)
    {
        if (ArrayVr[i] == ch) { return i; }
    }

    return -1;
}

char DefinitionTypeNode(char *arr)
{
    double tmp_num                      = 0;
    char   func_name[MAX_LEN_FUNC_NAME] = "";

    if (sscanf(arr, "%lf", &tmp_num) == 1)
    {
        return TpNm;
    }
    else if (sscanf(arr, "%s", func_name) == 1)
    {
        if (strlen(func_name) == 1)
        {
            if (func_name[0] == '$') { return -1; }

            for (int i = 0; (size_t)i < SIZEOF_ARR(ArrayOp); i++)
            {
                if (ArrayOp[i] == func_name[0]) { return TpOp; }
            }
            
            return TpVr;
        }

        return TpFn;
    }
    else
    {
        return -2;
    }
}

int DtorTree(DiffNode *node)
{
    if (node->LeftNode != NULL)  { DtorTree(node->LeftNode); }

    if (node->RightNode != NULL) { DtorTree(node->RightNode); }

    PRINT_DEBUG("FREE: [%p]\n", node);
    free(node);
    return 0;
}

array_t *CtorArray(FILE *fp_src)
{
    assert(fp_src != NULL);

    array_t *new_sarr = (array_t *)malloc(sizeof(array_t));

    size_t sz_file = SearchSizeFile(fp_src);
    char  *array   = (char *)calloc(sz_file + 1, sizeof(char));

    fread(array, sizeof(char), sz_file, fp_src);
    array[sz_file] = '\0';

    new_sarr->arr_ptr  = array;
    new_sarr->size_arr = sz_file;

    return new_sarr;
}

size_t SearchSizeFile(FILE *fp)
{
    assert(fp != NULL);

    fseek(fp, 0L, SEEK_END);
    size_t size_file = (size_t)ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    return size_file;
}
