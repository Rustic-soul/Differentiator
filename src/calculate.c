#define CALCULATE_CPP

#include <stdio.h>
#include <stdlib.h>

#include "differentiator.h"
#include "mydef.h"
#include "config.h"

#include <math.h>

double CalculateTree(DiffNode *node)
{
    switch (NTYPE)
    {
    case TpNm: PRINT_DEBUG("node <--> Num\n"); return node->Data.Num;

    case TpVr: PRINT_DEBUG("node <--> Var\n"); return VarArray[OP_VR_TYPE];

    case TpCn: PRINT_DEBUG("node <--> Const\n"); return ConstArray[OP_VR_TYPE];

    case TpOp:
        switch (OP_VR_TYPE)
        {
        case OpAdd: PRINT_DEBUG("node <--> Add\n"); return CalculateTree(LNODE) + CalculateTree(RNODE);
        
        case OpSub: PRINT_DEBUG("node <--> Sub\n"); return CalculateTree(LNODE) - CalculateTree(RNODE);
        
        case OpMul: PRINT_DEBUG("node <--> Mul\n"); return CalculateTree(LNODE) * CalculateTree(RNODE);
        
        case OpDiv: PRINT_DEBUG("node <--> Div\n"); return CalculateTree(LNODE) / CalculateTree(RNODE);
        
        case OpPow: PRINT_DEBUG("node <--> Pow\n"); return pow(CalculateTree(LNODE), CalculateTree(RNODE));
        
        default:
            PRINT_ERROR("Неизвестный тип операции [%d]\n", OP_VR_TYPE);
            exit(ERROR_UNKNOWN_OPERATION_TYPE);
        }

    case TpFn:
        switch (OP_VR_TYPE)
        {
        case FnSin:  PRINT_DEBUG("node <--> Sin\n"); return sin  (CalculateTree(LNODE)); 
        case FnCos:  PRINT_DEBUG("node <--> Cos\n"); return cos  (CalculateTree(LNODE)); 
        case FnTg:   PRINT_DEBUG("node <--> Tg \n"); return tan  (CalculateTree(LNODE)); 
        case FnAtg:  PRINT_DEBUG("node <--> Atg\n"); return atan (CalculateTree(LNODE)); 
        case FnSh:   PRINT_DEBUG("node <--> Sh \n"); return sinh (CalculateTree(LNODE)); 
        case FnCh:   PRINT_DEBUG("node <--> Ch \n"); return cosh (CalculateTree(LNODE)); 
        case FnTh:   PRINT_DEBUG("node <--> Th \n"); return tanh (CalculateTree(LNODE)); 
        case FnExp:  PRINT_DEBUG("node <--> Exp\n"); return exp  (CalculateTree(LNODE)); 
        case FnSqrt: PRINT_DEBUG("node <--> Sqr\n"); return sqrt (CalculateTree(LNODE)); 
        case FnLn:   PRINT_DEBUG("node <--> Ln \n"); return log  (CalculateTree(LNODE));
        case FnLg:   PRINT_DEBUG("node <--> Lg\n"); return log10(CalculateTree(LNODE));
        
        case FnCth:  PRINT_DEBUG("node <--> Cth\n"); return cosh (CalculateTree(LNODE)) / sinh(CalculateTree(LNODE)); 
        
        case FnCtg:  PRINT_DEBUG("node <--> Ctg\n"); return tan(3.14/2 - CalculateTree(LNODE)); 
        
        case FnActg: PRINT_DEBUG("node <--> Atg\n"); return 3.14/2 - atan(CalculateTree(LNODE)); 
        
        case FnLog:  PRINT_DEBUG("node <--> Log\n"); printf("Да вроде норм\n"); return log(CalculateTree(RNODE)) / log(CalculateTree(LNODE)); 
        
        default:
            PRINT_ERROR("Неизвестный тип функции [%d]\n", OP_VR_TYPE);
            exit(ERROR_UNKNOWN_FUNCTION_TYPE);
        }

    default:
        PRINT_ERROR("Неизвестный тип узла [%d]\n", NTYPE);
        exit(ERROR_UNKNOWN_NODE_TYPE);
    }

    PRINT_DEBUG("node <--> Nan Неправильное поведение\n"); 
    return NAN;
}
