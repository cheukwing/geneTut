#ifndef GENETUT_FUNC_H
#define GENETUT_FUNC_H

#include "utils.h"

typedef int (*func_t)(int a, int b);

int add(int, int);

int minus(int, int);

int multiply(int, int);

func_t operatorToFunction(gene_t);

#endif //GENETUT_FUNC_H
