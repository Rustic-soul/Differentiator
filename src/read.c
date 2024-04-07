#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

#include "differentiator.h"
#include "mydef.h"
#include "main.h"

#define SYNTAX_ERROR()                                  \
{                                                       \
    PRINT_ERROR("Неправильный синтаксис\n");            \
    exit(ERROR_SYNTAX);                                 \
}

static DiffNode *GetG  (char **exp);
static DiffNode *GetE  (char **exp);
static DiffNode *GetT  (char **exp);
static DiffNode *GetP  (char **exp);
static DiffNode *GetN  (char **exp);
static DiffNode *GetV  (char **exp);
static DiffNode *GetF  (char **exp);
static DiffNode *GetK  (char **exp);
static int       DTFunc(char **exp);
static int       DTVar (const char   var);

ssize_t   SearchSizeFile(FILE *fp);
array_t  *CtorArray     (FILE *fp);
int       DtorArray     (array_t *arr);

DiffNode *ReadTreeFile(FILE *fp)
{
    array_t *arr = CtorArray(fp);
    
    PRINT_DEBUG("Исходное выражение [%s]\n", arr->arr_ptr);

    char *tmp = arr->arr_ptr;
    DiffNode *tree = GetG(&(tmp));

    DtorArray(arr);

    return tree;
}

DiffNode *GetG(char **exp)
{
    PRINT_DEBUG("Locate: func GetG\n");
    SKIP_SPACES((*exp));
    
    DiffNode *node = GetE(exp);

    if (**exp != '\0') { SYNTAX_ERROR(); }

    return node;
}

DiffNode *GetE(char **exp)
{
    PRINT_DEBUG("Locate: func GetE\n");
    DiffNode *node = GetT(exp);
    
    while ((**exp == '+') || (**exp == '-'))
    {
        char op = **exp;
        (*exp)++;   
        
        SKIP_SPACES((*exp));
    
        DiffNode *rnode = GetT(exp);
        
        if (op == '+') { node =  _ADD(node, rnode); }
        else           { node =  _SUB(node, rnode); }
    }

    return node;
}

DiffNode *GetT(char **exp)
{
    PRINT_DEBUG("Locate: func GetT\n");

    DiffNode *node = GetV(exp);
    
    while ((**exp == '*') || (**exp == '/'))
    {
        char op = **exp;
        (*exp)++;
        
        SKIP_SPACES((*exp));

        DiffNode *rnode = GetV(exp);

        if (op == '*') { node = _MUL(node, rnode); }
        else           { node = _DIV(node, rnode); }
    }
    
    return node;
}

DiffNode *GetV(char **exp)
{
    PRINT_DEBUG("Locate: func GetV\n");

    DiffNode *node = GetP(exp);
    
    while (**exp == '^')
    {
        (*exp)++;
        
        SKIP_SPACES((*exp));

        DiffNode *rnode = GetP(exp);
        
        node = _POW(node, rnode);
    }
    
    return node;

}

DiffNode *GetP(char **exp)
{
    PRINT_DEBUG("Locate: func GetP\n");

    DiffNode *node = NULL;

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

DiffNode *GetK(char **exp)
{
    PRINT_DEBUG("Locate: func GetK\n");

    int vtype = **exp;
    (*exp)++;

    SKIP_SPACES((*exp));

    return _VAR(vtype);
}

DiffNode *GetF(char **exp)
{
    PRINT_DEBUG("Locate: func GetF\n");

    int ftype = DTFunc(exp);
    PRINT_DEBUG("DTFunc = %d\n", ftype);

    DiffNode *lnode = NULL;
    DiffNode *rnode = NULL;

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
        
    return CreateNode(TpFn, (NodeData){.AdditionalType = ftype}, lnode, rnode);
}

DiffNode *GetN(char **exp)
{
    PRINT_DEBUG("Locate: func GetN\n");

    const char *OldS = *exp;

    double val0 = strtod(*exp, exp);

    if (OldS == *exp) { SYNTAX_ERROR(); }

    SKIP_SPACES((*exp));

    printf("Функция GetN() вернула: [%.2lf]\n", val0);
    return _NUM(val0);
}

int DTFunc(char **exp)
{
    char fname[MAX_LEN_FUNC_NAME] = "";

    int i = 0;
    while(isalpha(**exp)) { fname[i] = **exp; (*exp)++; i++; } fname[i] = '\0';
    SKIP_SPACES((*exp));

    PRINT_DEBUG("fname = {%s}\n", fname);
    
    if (strlen(fname) == 1)
    {
        PRINT_DEBUG("Ищем переменную\n");
        for (int j = 0; (size_t)j < sizeof(ArrayVr) / sizeof(*ArrayVr); j++)
        {
            if (fname[0] == ArrayVr[j]) { return (-1)*j - 10; }
        }
    }
    else
    {
        for (int j = 0; (size_t)j < sizeof(ArrayFn) / sizeof(*ArrayFn) ; j++)
        {
            if (strcmp(fname, ArrayFn[j]) == 0) { return j; }
        }
    }

    return ERROR_UNKNOWN_FUNCTION_TYPE;
}

array_t *CtorArray(FILE *fp)
{
    if (fp == NULL)
    {
        PRINT_ERROR("File pointer is null\n");
        return NULL;
    }

    ssize_t sz_file = SearchSizeFile(fp);
    if (sz_file < 0)
    {
        PRINT_ERROR("SearchSizeFile function failed\n");
        return NULL;
    }
    char  *array   = (char *)calloc(sz_file + 1, sizeof(char));

    assert(fread(array, sizeof(char), sz_file, fp) == sz_file);
    array[sz_file] = '\0';
    
    array_t *new_sarr  = (array_t *)malloc(sizeof(array_t));
    new_sarr->arr_ptr  = array;
    new_sarr->size_arr = sz_file;

    return new_sarr;
}

int DtorArray(array_t *arr)
{
    free(arr->arr_ptr);
    free(arr);

    return 0;
}

ssize_t SearchSizeFile(FILE *fp)
{
    if (fp == NULL)
    {
        PRINT_ERROR("File pointer is null\n");
        return ERROR_NULL_PTR;
    }

    ssize_t current_pos = ftell(fp);

    if (current_pos == -1L)
    {
        PRINT_ERROR("Ftell function failed\n");
        return ERROR_FTELL_FAILED;
    }

    fseek(fp, 0L, SEEK_END);

    ssize_t size_file = ftell(fp);

    if (size_file == -1L)
    {
        PRINT_ERROR("Ftell function failed\n");
        return ERROR_FTELL_FAILED;
    }

    fseek(fp, current_pos, SEEK_SET);

    return size_file;
}