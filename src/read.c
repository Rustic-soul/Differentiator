#define READ_C

#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mydef.h"
#include "differentiator.h"
#include "config.h"
#include "main.h"

const char *s = NULL;

int GetG(const char *str);
int GetE();
int GetT();
int GetP();
int GetN();
int GetV();
int GetF();
char DTFType();
int SyntaxError();

int main()
{
    // const char *exp1 = "7";
    // const char *exp2 = "17+3-5";
    // const char *exp3 = "6*7+3-8/2";
    const char *exp4 = "bob  (  mul   ( (2+   2)^( 1 * 2 )+3*(28  -   (13-(56+14)/2) * 7)/   7))";
    
    int res = GetG(exp4);
    printf("[[%d]]\n", res);
}

int SyntaxError()
{
    printf("Синтаксическая ошибка\n");
    exit(1);

    return 0;
}

int GetG(const char *str)
{
    s = str;
    while (*s == ' ') { s++; }
    int val = GetE();
    if (*s != '\0') { printf("{GError}\n"); SyntaxError(); }

    printf("Функция GetG() вернула: [%d]\n", val);
    return val;
}

int GetE()
{
    int val = GetT();
    
    while ((*s == '+') || (*s == '-'))
    {
        char op = *s;
        s++;   
        while (*s == ' ') { s++; }
    
        int val2 = GetT();
        
        if (op == '+') { val += val2; }
        else           { val -= val2; }
    }

    printf("Функция GetE() вернула: [%d]\n", val);
    return val;
}

int GetT()
{
    int val = GetV();
    
    while ((*s == '*') || (*s == '/'))
    {
        char op = *s;
        s++;
        while (*s == ' ') { s++; }
    
        int val2 = GetV();

        if (op == '*') { val *= val2; }
        else           { val /= val2; }
    }
    
    printf("Функция GetT() вернула: [%d]\n", val);
    return val;
}

int GetV()
{
    int val = GetP();
    
    while (*s == '^')
    {
        s++;
        while (*s == ' ') { s++; }
    
        int val2 = GetP();
        
        val = (int)pow(val, val2);
    }
    
    printf("Функция GetV() вернула: [%d]\n", val);
    return val;

}

int GetP()
{
    int val = 0;

    if (*s == '(')
    {
        s++;
        while (*s == ' ') { s++; }
    
        val = GetE();
        if (*s != ')') { SyntaxError(); }
        else           { s++; }
        
        while (*s == ' ') { s++; }
    }
    else if (isalpha(*s)) { val = GetF(); }
    else                  { val = GetN(); }

    printf("Функция GetP() вернула: [%d]\n", val);
    return val;
}

int GetF()
{
    char type = DTFType();

    int val = 0;
    if (*s == '(')
    {
        s++;
        while (*s == ' ') { s++; }
    
        val = GetE();
        if (*s != ')') { SyntaxError(); } 
        else           { s++; }
        while (*s == ' ') { s++; }
    }
    else { SyntaxError(); }

    // Типо switch на функции
    return 5*val;
}

int GetN()
{
    int val = 0;
    const char *OldS = s;

    while (('0' <= *s) && (*s <= '9'))
    { 
        val = val*10 + (*s - '0');
        s++;
    }

    if (OldS == s) { SyntaxError(); }

    while (*s == ' ') { s++; }
    
    printf("Функция GetN() вернула: [%d]\n", val);
    return val;
}

char DTFType()
{
    char func_name[100] = "";

    int i = 0;
    for ( ; *s != '('; i++)
    {
        func_name[i] = *s;
        s++;
    }
    func_name[i] = '\0';
    // тут сравнение с именами функций

    return 2;
}
