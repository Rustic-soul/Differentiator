#ifndef READ_H
#define READ_H

#include "differentiator.h"

#define SYNTAX_ERROR()                                  \
{                                                       \
    PRINT_ERROR("Неправильный синтаксис\n");            \
    exit(ERROR_SYNTAX);                                 \
}

#define SKIP_SPACES(pstr) while ((*pstr == ' ') || (*pstr == '\n')) { pstr++; }

static node_t* GetG  (char **exp);
static node_t* GetE  (char **exp);
static node_t* GetT  (char **exp);
static node_t* GetP  (char **exp);
static node_t* GetN  (char **exp);
static node_t* GetV  (char **exp);
static node_t* GetF  (char **exp);
static node_t* GetK  (char **exp);

static int dt_func(char** exp);

#endif