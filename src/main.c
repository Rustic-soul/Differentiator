#include "main.h"
#include "mydef.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // printf("{{%lf}}\n", tan(2));

    FILE     *fp_exp   = fopen("../images/expression", "r");
    DiffNode *RootTree = CreateTree(fp_exp);
    fclose(fp_exp);
    
    FILE     *fp_tree   = fopen("../images/tree.dot", "w");
    PrintTreeDot(fp_tree, RootTree);
    fclose(fp_tree);
    // system("dot -Tsvg ../images/tree.dot -o ../images/tree.svg");

    DiffNode *DiffTree = Differentiator(RootTree, VrX);
    FILE     *fp_diff  = fopen("../images/diff.dot", "w");
    PrintTreeDot(fp_diff, DiffTree);
    fclose(fp_diff);
    // system("dot -Tsvg ../images/diff.dot -o ../images/diff.svg");

    #if 1 
    FILE *fp_opt1 = fopen("../images/opt1.dot", "w");
    RootTree = Optimizer1(RootTree);
    PrintTreeDot(fp_opt1, RootTree);
    fclose(fp_opt1);
    // system("dot -Tsvg ../images/opt1.dot -o ../images/opt1.svg");
    #endif 

    #if 1
    FILE *fp_opt1_d = fopen("../images/opt1_d.dot", "w");
    DiffTree = Optimizer1(DiffTree);
    PrintTreeDot(fp_opt1_d, DiffTree);
    fclose(fp_opt1_d);
    // system("dot -Tsvg ../images/opt1_d.dot -o ../images/opt1_d.svg");
    #endif

    #if 1
    FILE *fp_opt2 = fopen("../images/opt2.dot", "w");
    RootTree = Optimizer2(RootTree);
    PrintTreeDot(fp_opt2, RootTree);
    fclose(fp_opt2);
    // system("dot -Tsvg ../images/opt2.dot -o ../images/opt2.svg");
    #endif
    
    #if 1 
    FILE *fp_opt2_d = fopen("../images/opt2_d.dot", "w");
    DiffTree = Optimizer2(DiffTree);
    PrintTreeDot(fp_opt2_d, DiffTree);
    fclose(fp_opt2_d);
    // system("dot -Tsvg ../images/opt2_d.dot -o ../images/opt2_d.svg");
    #endif

    printf("Ответ1: [%lf]\n", CalculateTree(RootTree));
    printf("Ответ2: [%lf]\n", CalculateTree(DiffTree));

    DtorTree(RootTree);
    // printf("\n----------------\n\n");
    DtorTree(DiffTree);

}
