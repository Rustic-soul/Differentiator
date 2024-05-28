#include <stdio.h>

#include "differentiator.h"
#include "config.h"
#include "mydef.h"

int main()
{
    FILE     *fp_exp  = fopen(PATH_TO_EXP, "r");
    node_t *TreeExp = ReadTreeFile(fp_exp);
    fclose(fp_exp);

    node_t *TreeDiff = Differentiator(TreeExp, 'x');
    
    FILE *fp_exp_dot = fopen(PATH_TO_EXP_DOT, "w");
    PrintTreeDot(fp_exp_dot, TreeExp);
    fclose(fp_exp_dot);

    FILE *fp_diff_dot = fopen(PATH_TO_DIFF_DOT, "w");
    PrintTreeDot(fp_diff_dot, TreeDiff);
    fclose(fp_diff_dot);

    FILE *fp_tex = fopen(PATH_TO_RES_TEX, "w");
    CreateResTex(fp_tex, TreeExp, TreeDiff);
    fclose(fp_tex);

    DtorTree(TreeExp);
    DtorTree(TreeDiff);
}