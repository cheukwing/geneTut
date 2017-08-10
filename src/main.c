#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "func.h"

int evaluate(gene_array_t gene_array);

float fitness(gene_array_t gene_array, int target);

chromosome_t generateRandom(int target) {
  gene_array_t gene_array = (__uint64_t) (rand() % (int) (pow(2, CHROMOSOME_SIZE)));
  chromosome_t chr = {gene_array, fitness(gene_array, target)};
  return chr;
}

children_t breed(chromosome_t mother, chromosome_t father, int target) {
  chromosome_t fstChild = mother;
  chromosome_t sndChild = father;
  // crossover chance 0.7
  if (rand() % (100 + 1) > 70) {
    __uint64_t mask = createMask(rand() % 32);
    fstChild.gene_array = (fstChild.gene_array & mask) + (father.gene_array & ~mask);
    sndChild.gene_array = (sndChild.gene_array & mask) + (mother.gene_array & ~mask);
    fstChild.fitness = fitness(fstChild.gene_array, target);
    sndChild.fitness = fitness(sndChild.gene_array, target);
  }
  children_t children = {fstChild, sndChild};
  return children;
}

chromosome_t mutate(chromosome_t child) {
  __uint64_t mutationMask = 0;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    // mutation chance for each bit 0.001
    if (rand() % (1000 + 1) > 999) {
      ++mutationMask;
    }
    mutationMask <<= 1;
  }

  child.gene_array = child.gene_array ^ mutationMask;
  child.fitness = evaluate(child.gene_array);
  return child;
}

// LEFT TO RIGHT EVALUTION
int evaluate(gene_array_t gene_array) {
  gene_t gene;
  int genesEvaulated = 0;
  do {
    gene = getGene(gene_array);
    gene_array <<= GENE_SIZE;
    ++genesEvaulated;
  } while (!isNumeric(gene));

  int result = gene;

  // while have not checked all genes
  while (genesEvaulated < NUM_GENES) {
    gene = getGene(gene_array);
    gene_array <<= GENE_SIZE;
    ++genesEvaulated;
    // find next operator, ignore all numeric
    if (isOperator(gene)) {
      func_t func = operatorToFunction(gene);
      // find next numeric, ignore all operator
      while (genesEvaulated < NUM_GENES || !isNumeric(gene)) {
        gene = getGene(gene_array);
        gene_array <<= GENE_SIZE;
        ++genesEvaulated;
      }

      // if found numeric, then apply with operator
      if (isNumeric(gene)) {
        result = func(result, gene);
      }
    }
  }
  return result;
}

float fitness(gene_array_t gene_array, int target) {
  return 1 / abs(evaluate(gene_array) - target);
}

int main() {
  
  return EXIT_SUCCESS;
}