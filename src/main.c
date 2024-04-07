#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "mydef.h"

int main()
{
    FILE     *fp_exp  = fopen("../images/exp/expression1.txt", "r");
    DiffNode *TreeExp = ReadTreeFile(fp_exp);
    fclose(fp_exp);

    DiffNode *TreeDiff = Differentiator(TreeExp, 'x');
    
    FILE *fp_exp_dot = fopen("../images/dot/exp.dot", "w");
    PrintTreeDot(fp_exp_dot, TreeExp);
    fclose(fp_exp_dot);

    FILE *fp_diff_dot = fopen("../images/dot/diff.dot", "w");
    PrintTreeDot(fp_diff_dot, TreeDiff);
    fclose(fp_diff_dot);

#if 1
    FILE *fp_tex = fopen("../images/tex/res.tex", "w");
    CreateResTex(fp_tex, TreeExp, TreeDiff);
    fclose(fp_tex);
#endif

    DtorTree(TreeExp);
    DtorTree(TreeDiff);
}

