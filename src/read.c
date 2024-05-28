#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#include "differentiator.h"
#include "config.h"
#include "array.h"
#include "read.h"
#include "DSL.h"
#include "mydef.h"

node_t *ReadTreeFile(FILE *fp)
{
    array_t* arr = ctor_array(fp);
    
    PRINT_DEBUG("Исходное выражение [%s]\n", arr->array);

    char*   tmp  = arr->array;
    node_t* tree = GetG(&(tmp));

    dtor_array(arr);

    return tree;
}

node_t *GetG(char **exp)
{
    PRINT_DEBUG("Locate: func GetG\n");
    SKIP_SPACES((*exp));
    
    node_t *node = GetE(exp);

    if (**exp != '\0') { SYNTAX_ERROR(); }

    return node;
}

node_t *GetE(char **exp)
{
    PRINT_DEBUG("Locate: func GetE\n");
    node_t *node = GetT(exp);
    
    while ((**exp == '+') || (**exp == '-'))
    {
        char op = **exp;
        (*exp)++;   
        
        SKIP_SPACES((*exp));
    
        node_t *rnode = GetT(exp);
        
        if (op == '+') { node =  _ADD(node, rnode); }
        else           { node =  _SUB(node, rnode); }
    }

    return node;
}

node_t *GetT(char **exp)
{
    PRINT_DEBUG("Locate: func GetT\n");

    node_t *node = GetV(exp);
    
    while ((**exp == '*') || (**exp == '/'))
    {
        char op = **exp;
        (*exp)++;
        
        SKIP_SPACES((*exp));

        node_t *rnode = GetV(exp);

        if (op == '*') { node = _MUL(node, rnode); }
        else           { node = _DIV(node, rnode); }
    }
    
    return node;
}

node_t *GetV(char **exp)
{
    PRINT_DEBUG("Locate: func GetV\n");

    node_t *node = GetP(exp);
    
    while (**exp == '^')
    {
        (*exp)++;
        
        SKIP_SPACES((*exp));

        node_t *rnode = GetP(exp);
        
        node = _POW(node, rnode);
    }
    
    return node;

}

node_t *GetP(char **exp)
{
    PRINT_DEBUG("Locate: func GetP\n");

    node_t *node = NULL;

    if (**exp == '(')
    {
        (*exp)++;
        
        SKIP_SPACES((*exp));

        node = GetE(exp);
        if (**exp != ')') { SYNTAX_ERROR(); }
        else              { (*exp)++; }
        
        SKIP_SPACES((*exp));
    }
    else if (isalpha(**exp))
    {
        if (isalpha(*(*exp + 1))) { node = GetF(exp); }
        else                      { node = GetK(exp); }
    }
    else { node = GetN(exp); }

    return node;
}

node_t *GetK(char **exp)
{
    PRINT_DEBUG("Locate: func GetK\n");

    int vtype = **exp;
    (*exp)++;

    SKIP_SPACES((*exp));

    return _VAR(vtype);
}

node_t *GetF(char **exp)
{
    PRINT_DEBUG("Locate: func GetF\n");

    int ftype = dt_func(exp);
    PRINT_DEBUG("dt_func = %d\n", ftype);

    node_t *lnode = NULL;
    node_t *rnode = NULL;

    if (**exp == '(')
    {
        (*exp)++;
          
        SKIP_SPACES((*exp));

        lnode = GetE(exp);

        if (ftype == FnLog)
        {
            if (**exp == ',')
            {
                (*exp)++;
                SKIP_SPACES((*exp));
                
                rnode = GetE(exp);
            }
            else { SYNTAX_ERROR(); }
        }

        if (**exp == ')') { (*exp)++; } 
        else              { SYNTAX_ERROR(); }
        
        SKIP_SPACES((*exp));
    }
    else { SYNTAX_ERROR(); }
        
    return CreateNode(TpFn, (node_data_t){.additional_type = ftype}, lnode, rnode);
}

node_t *GetN(char **exp)
{
    PRINT_DEBUG("Locate: func GetN\n");

    const char *OldS = *exp;

    double val0 = strtod(*exp, exp);

    if (OldS == *exp) { SYNTAX_ERROR(); }

    SKIP_SPACES((*exp));

    printf("Функция GetN() вернула: [%.2lf]\n", val0);
    return _NUM(val0);
}

int dt_func(char **exp)
{
    char fname[MAX_LEN_FUNC_NAME] = "";

    int i = 0;
    while(isalpha(**exp)) { fname[i] = **exp; (*exp)++; i++; } fname[i] = '\0';
    SKIP_SPACES((*exp));

    PRINT_DEBUG("fname = {%s}\n", fname);
    
    for (int j = 0; (size_t)j < sizeof(ArrayFn) / sizeof(*ArrayFn) ; j++)
    {
        if (strcmp(fname, ArrayFn[j]) == 0) { return j; }
    }
    
    return ERROR_UNKNOWN_FUNCTION_TYPE;
}