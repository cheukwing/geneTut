#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "func.h"

children_t breed(chromosome_t mother, chromosome_t father, int crossover) {
  __uint64_t mask = createMask(crossover);
  chromosome_t fstChild = (mother & mask) + (father & ~mask);
  chromosome_t sndChild = (father & mask) + (mother & ~mask);
  children_t children = {fstChild, sndChild};
  return children;
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

// LEFT TO RIGHT EVALUTION
int evaluate(chromosome_t chr) {
  gene_t gene;
  int genesEvaulated = 0;
  do {
    gene = getGene(chr);
    chr <<= GENE_SIZE;
    ++genesEvaulated;
  } while (!isNumeric(gene));

  int result = gene;
  while (genesEvaulated < NUM_GENES) {
    gene = getGene(chr);
    chr <<= GENE_SIZE;
    ++genesEvaulated;
    if (isOperator(gene)) {
      func_t func = operatorToFunction(gene);
      while (genesEvaulated < NUM_GENES || !isNumeric(gene)) {
        gene = getGene(chr);
        chr <<= GENE_SIZE;
        ++genesEvaulated;
      }

      if (isNumeric(gene)) {
        result = func(result, gene);
      }
    }
  }

  return result;
}

int main() {
  srand((unsigned int) time(NULL));
  printf("%d\n", ADD);
  return 0;
}