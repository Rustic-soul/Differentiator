#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "differentiator.h"
#include "mydef.h"
#include "DSL.h"

node_t* DifferentiatorRec(node_t *node, int part);
node_t* DifferentiatorLog(node_t *node, int part);

bool is_num(node_t *node);
bool is_variable(node_t* node);
bool is_binary_operator(node_t* node);
bool is_unary_function(node_t* node);

void free_children(node_t* node);

node_t* remove_neutral_elements_binary_operator(node_t* node);
node_t* remove_neutral_elements_add(node_t* node, node_t* num_node, node_t* nan_node);
node_t* remove_neutral_elements_sub(node_t* node, node_t* num_node, node_t* nan_node);
node_t* remove_neutral_elements_mul(node_t* node, node_t* num_node, node_t* nan_node);
node_t* remove_neutral_elements_div(node_t* node, node_t* num_node, node_t* nan_node);
node_t* remove_neutral_elements_pow(node_t* node, node_t* num_node, node_t* nan_node);

node_t* differentiator_num(node_t *node, int part);
node_t* differentiator_variable(node_t *node, int part);

node_t* differentiator_binary_operator(node_t *node, int part);
node_t* differentiator_binary_operator_add(node_t *node, int part);
node_t* differentiator_binary_operator_sub(node_t *node, int part);
node_t* differentiator_binary_operator_mul(node_t *node, int part);
node_t* differentiator_binary_operator_div(node_t *node, int part);
node_t* differentiator_binary_operator_pow(node_t *node, int part);

node_t* differentiator_unary_function(node_t *node, int part);
node_t* differentiator_unary_function_sin(node_t *node, int part);
node_t* differentiator_unary_function_cos(node_t *node, int part);
node_t* differentiator_unary_function_tg(node_t *node, int part);
node_t* differentiator_unary_function_ctg(node_t *node, int part);
node_t* differentiator_unary_function_asin(node_t *node, int part);
node_t* differentiator_unary_function_acos(node_t *node, int part);
node_t* differentiator_unary_function_atg(node_t *node, int part);
node_t* differentiator_unary_function_actg(node_t *node, int part);
node_t* differentiator_unary_function_sh(node_t *node, int part);
node_t* differentiator_unary_function_ch(node_t *node, int part);
node_t* differentiator_unary_function_th(node_t *node, int part);
node_t* differentiator_unary_function_cth(node_t *node, int part);
node_t* differentiator_unary_function_exp(node_t *node, int part);
node_t* differentiator_unary_function_sqrt(node_t *node, int part);
node_t* differentiator_unary_function_ln(node_t *node, int part);

node_t* Differentiator(node_t *node, int part)
{
    assert(node != NULL);

    node_t *diff = DifferentiatorRec(node, part);
    assert(diff != NULL);
    
    return FullOptimizer(diff);
}

node_t *DifferentiatorRec(node_t *node, int part)
{
    assert(node != NULL);

    switch (NTYPE)
    {
        case TpNm: return differentiator_num(node, part);
        
        case TpVr: return differentiator_variable(node, part);
        
        case TpOp: return differentiator_binary_operator(node, part);
        
        case TpFn: return differentiator_unary_function(node, part);

        default:
            PRINT_ERROR("Неизвестный тип узла [%d]\n", NTYPE);
            exit(ERROR_UNKNOWN_NODE_TYPE);
    }
}

node_t* differentiator_num(node_t *node, int part)
{
    return _NUM(0);
}

node_t* differentiator_variable(node_t *node, int part)
{
    return _NUM((NDTYPE == part) ? 1 : 0);
}

node_t* differentiator_binary_operator(node_t *node, int part)
{
    switch (NDTYPE)
    {
        case OpAdd: return differentiator_binary_operator_add(node, part);

        case OpSub: return differentiator_binary_operator_sub(node, part);

        case OpMul: return differentiator_binary_operator_mul(node, part);

        case OpDiv: return differentiator_binary_operator_div(node, part);

        case OpPow: return differentiator_binary_operator_pow(node, part);
        
        default:
            PRINT_ERROR("Неизвестный тип операциии [%d]\n", NDTYPE);
            exit(ERROR_UNKNOWN_OPERATION_TYPE);
    }
}

node_t* differentiator_binary_operator_add(node_t *node, int part)
{
    return _ADD(_DIFF(LNODE), _DIFF(RNODE));
}

node_t* differentiator_binary_operator_sub(node_t *node, int part)
{
    return _SUB(_DIFF(LNODE), _DIFF(RNODE));
}

node_t* differentiator_binary_operator_mul(node_t *node, int part)
{
    return _ADD(_MUL(_DIFF(LNODE), _CPY(RNODE)), _MUL(_CPY(LNODE), _DIFF(RNODE)));
}

node_t* differentiator_binary_operator_div(node_t *node, int part)
{
    return _DIV(_SUB(_MUL(_DIFF(LNODE), _CPY(RNODE)), _MUL(_CPY(LNODE), _DIFF(RNODE))), _POW(_CPY(RNODE), _NUM(2)));
}

node_t* differentiator_binary_operator_pow(node_t *node, int part)
{
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
}

node_t* differentiator_unary_function(node_t *node, int part)
{
    switch (NDTYPE)
    {
        case FnSin:  return differentiator_unary_function_sin(node, part);

        case FnCos:  return differentiator_unary_function_cos(node, part);

        case FnTg:   return differentiator_unary_function_tg(node, part);

        case FnCtg:  return differentiator_unary_function_ctg(node, part);

        case FnAsin: return differentiator_unary_function_asin(node, part);

        case FnAcos: return differentiator_unary_function_acos(node, part);

        case FnAtg:  return differentiator_unary_function_atg(node, part);

        case FnActg: return differentiator_unary_function_actg(node, part);

        case FnSh:   return differentiator_unary_function_sh(node, part);

        case FnCh:   return differentiator_unary_function_ch(node, part);

        case FnTh:   return differentiator_unary_function_th(node, part);

        case FnCth:  return differentiator_unary_function_cth(node, part);

        case FnExp:  return differentiator_unary_function_exp(node, part);

        case FnSqrt: return differentiator_unary_function_sqrt(node, part);
            
        case FnLn:   return differentiator_unary_function_ln(node, part);

        default:
            PRINT_ERROR("Неизвестный тип функции [%d]\n", NDTYPE);
            exit(ERROR_UNKNOWN_FUNCTION_TYPE);
    }
}

node_t* differentiator_unary_function_sin(node_t *node, int part)
{
    return _MUL(_COS(_CPY(LNODE)), _DIFF(LNODE));
}

node_t* differentiator_unary_function_cos(node_t *node, int part)
{
    return _MUL(_NUM(-1), _MUL(_SIN(_CPY(LNODE)), _DIFF(LNODE)));
}

node_t* differentiator_unary_function_tg(node_t *node, int part)
{
    return _MUL(_DIV(_NUM(1), _POW(_COS(_CPY(LNODE)), _NUM(2))), _DIFF(LNODE));
}

node_t* differentiator_unary_function_ctg(node_t *node, int part)
{
    return _MUL(_MUL(_NUM(-1), _DIV(_NUM(1), _POW(_SIN(_CPY(LNODE)), _NUM(2)))), _DIFF(LNODE));
}

node_t* differentiator_unary_function_asin(node_t *node, int part)
{
    return _MUL(_DIV(_NUM(1), _SQRT(_SUB(_NUM(1), _POW(_CPY(LNODE), _NUM(2))))), _DIFF(LNODE));
}

node_t* differentiator_unary_function_acos(node_t *node, int part)
{
    return _MUL(_NUM(-1), _MUL(_DIV(_NUM(1), _SQRT(_SUB(_NUM(1), _POW(_CPY(LNODE), _NUM(2))))), _DIFF(LNODE)));
}

node_t* differentiator_unary_function_atg(node_t *node, int part)
{
    return _MUL(_DIV(_NUM(1), _ADD(_NUM(1), _POW(_CPY(LNODE), _NUM(2)))), _DIFF(LNODE));
}

node_t* differentiator_unary_function_actg(node_t *node, int part)
{
    return _MUL(_NUM(-1), _MUL(_DIV(_NUM(1), _ADD(_NUM(1), _POW(_CPY(LNODE), _NUM(2)))), _DIFF(LNODE)));
}

node_t* differentiator_unary_function_sh(node_t *node, int part)
{
    return _MUL(_CH(_CPY(LNODE)), _DIFF(LNODE));
}

node_t* differentiator_unary_function_ch(node_t *node, int part)
{
    return _MUL(_SH(_CPY(LNODE)), _DIFF(LNODE));
}

node_t* differentiator_unary_function_th(node_t *node, int part)
{
    return _MUL(_DIV(_NUM(1), _POW(_CH(_CPY(LNODE)), _NUM(2))), _DIFF(LNODE));
}

node_t* differentiator_unary_function_cth(node_t *node, int part)
{
    return _MUL(_NUM(-1), _MUL(_DIV(_NUM(1), _POW(_SH(_CPY(LNODE)), _NUM(2))), _DIFF(LNODE)));
}

node_t* differentiator_unary_function_exp(node_t *node, int part)
{
    return _MUL(_EXP(_CPY(LNODE)), _DIFF(LNODE));
}

node_t* differentiator_unary_function_sqrt(node_t *node, int part)
{
    return _MUL(_DIV(_NUM(1), _MUL(_NUM(2), _SQRT(_CPY(LNODE)))), _DIFF(LNODE));
}

node_t* differentiator_unary_function_ln(node_t *node, int part)
{
    return _MUL(_DIV(_NUM(1), _CPY(LNODE)), _DIFF(LNODE));
}

node_t *FullOptimizer (node_t *node) { return remove_neutral_elements(convolution_constants(node)); }

node_t *convolution_constants(node_t *node)
{
    if ((node == NULL) ||is_num(node) || is_variable(node)) { return node; }

    LNODE = convolution_constants(LNODE);
    RNODE = convolution_constants(RNODE);

    if (is_binary_operator(node) && is_num(LNODE) && is_num(RNODE))
    {
        NDNUM = CalculateTree(node);
        free_children(node);
    }
    else if (is_unary_function(node) && is_num(LNODE))
    {
        NDNUM = CalculateTree(node);
        free_children(node);
    }

    return node;
}

bool is_num(node_t *node)
{
    return (NTYPE == TpNm);
}

bool is_variable(node_t* node)
{
    return (NTYPE == TpVr);
}

bool is_binary_operator(node_t* node)
{
    return (NTYPE == TpOp);
}

bool is_unary_function(node_t* node)
{
    return (NTYPE == TpFn);
}

void free_children(node_t* node)
{
    free(LNODE);
    free(RNODE);
    LNODE = NULL;
    RNODE = NULL;
}

node_t *remove_neutral_elements(node_t *node)
{
    if (node == NULL) { return node; }

    LNODE = remove_neutral_elements(LNODE);
    RNODE = remove_neutral_elements(RNODE);
    
    if (is_binary_operator(node))
    {
        if (is_num(LNODE) || is_num(RNODE))
        {
            return remove_neutral_elements_binary_operator(node);
        }
    }

    return node;
}

node_t* remove_neutral_elements_binary_operator(node_t* node)
{
    node_t *num_node = is_num(LNODE) ? LNODE : RNODE;
    node_t *nan_node = is_num(LNODE) ? RNODE : LNODE;
    
    switch (NDTYPE)
    {
        case OpAdd: return remove_neutral_elements_add(node, num_node, nan_node);

        case OpSub: return remove_neutral_elements_sub(node, num_node, nan_node);
        
        case OpMul: return remove_neutral_elements_mul(node, num_node, nan_node);

        case OpDiv: return remove_neutral_elements_div(node, num_node, nan_node);

        case OpPow: return remove_neutral_elements_pow(node, num_node, nan_node);

        default:
            PRINT_ERROR("Неизвестный тип операции [%d]\n", NDTYPE);
            exit(ERROR_UNKNOWN_OPERATION_TYPE);
    }
}

node_t* remove_neutral_elements_add(node_t* node, node_t* num_node, node_t* nan_node)
{
    if (DoubleEqual(num_node->data.num, 0))
    {
        free(num_node);
        free(node);
        return nan_node;
    }

    return node;
}

node_t* remove_neutral_elements_sub(node_t* node, node_t* num_node, node_t* nan_node)
{
    if (DoubleEqual(num_node->data.num, 0))
    {
        if (num_node == LNODE) { return node; } //TODO  Мб нужно придумывать умножение на (-1)
        
        free(num_node);
        free(node);
        return nan_node;
    }
    
    return node;
}

node_t* remove_neutral_elements_mul(node_t* node, node_t* num_node, node_t* nan_node)
{
    if (DoubleEqual(num_node->data.num, 0))
    {
        DtorTree(node);
        return _NUM(0);
    }
    else if (DoubleEqual(num_node->data.num, 1))
    {
        free(num_node);
        free(node);
        return nan_node;
    }
    
    return node;
}

node_t* remove_neutral_elements_div(node_t* node, node_t* num_node, node_t* nan_node)
{
    if ((num_node == LNODE) && DoubleEqual(num_node->data.num, 0))
    {
        DtorTree(node);
        return _NUM(0);
    }
    else if ((num_node == RNODE) && DoubleEqual(num_node->data.num, 1))
    {
        free(num_node);
        free(node);
        return nan_node;
    }
    
    return node;
}

node_t* remove_neutral_elements_pow(node_t* node, node_t* num_node, node_t* nan_node)
{
    if ((num_node == LNODE) && (DoubleEqual(num_node->data.num, 1) || DoubleEqual(num_node->data.num, 0)))
    {
        DtorTree(nan_node);
        free(node);
        return num_node;
    }
    else if ((num_node == RNODE) && DoubleEqual(num_node->data.num, 1))
    {
        free(num_node);
        free(node);
        return nan_node;
    }
    else if ((num_node == RNODE) && DoubleEqual(num_node->data.num, 0))
    {
        DtorTree(node);
        return _NUM(1);
    }

    return node;
}

int SearchVar(node_t *node, int var)
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

node_t *CopyNode(node_t *node)
{
    if (node == NULL) { return NULL; }

    node_t *newnode = CreateNode(NTYPE, node->data, _CPY(LNODE), _CPY(RNODE));

    return newnode;
}

node_t *CreateNode(char type, node_data_t data, node_t *LeftNode, node_t *RightNode)
{
    node_t *newnode  = (node_t *)malloc(sizeof(node_t));
    
    PRINT_DEBUG("CreateNode: {%p}\n", newnode);

    newnode->type      = type;
    newnode->data      = data;
    newnode->LeftNode  = LeftNode;
    newnode->RightNode = RightNode;

    return newnode;
}

int DtorTree(node_t *node)
{
    if (node->LeftNode != NULL)  { DtorTree(node->LeftNode); }

    if (node->RightNode != NULL) { DtorTree(node->RightNode); }

    PRINT_DEBUG("FREE: [%p]\n", node);
    free(node);
    return 0;
}