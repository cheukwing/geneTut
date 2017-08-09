#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "func.h"

#define CHROMOSOME_SIZE 36

typedef __uint64_t chromosome_t;

typedef int (*func_t)(int a, int b);

typedef struct children {
  chromosome_t fstChild;
  chromosome_t sndChild;
} children_t;

typedef enum gene {
  ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
  ADD, MINUS, MULTIPLY, NONE
} gene_t;

__uint64_t createMask(int num) {
  __uint64_t mask = 0;
  for (int i = 0; i < num; ++i) {
    mask <<= 1;
    ++mask;
  }
  return mask;
}

children_t breed(chromosome_t mother, chromosome_t father, int crossover) {
  __uint64_t mask = createMask(crossover);
  chromosome_t fstChild = (mother & mask) + (father & ~mask);
  chromosome_t sndChild = (father & mask) + (mother & ~mask);
  return {fstChild, sndChild};
}

chromosome_t mutate(chromosome_t child) {
  __uint64_t mutationMask = 0;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    if (rand() % 1000 + 1 > 999) {
      ++mutationMask;
    }
    mutationMask <<= 1;
  }

  return child ^ mutationMask;
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

int main() {
  srand((unsigned int) time(NULL));
  printf("Hello, World!\n");
  return 0;
}