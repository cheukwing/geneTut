#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int add(int a, int b) {
  return a + b;
}

int minus(int a, int b) {
  return a - b;
}

int multiply(int a, int b) {
  return a * b;
}

func_t operatorToFunction(gene_t operator) {
  switch (operator) {
    case ADD:
      return add;
    case MINUS:
      return minus;
    case MULTIPLY:
      return multiply;
    default:
      perror("operatorToFunction was not passed an operator!");
      exit(EXIT_FAILURE);
  }
}
