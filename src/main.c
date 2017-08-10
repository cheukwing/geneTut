#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "func.h"

int evaluate(gene_array_t gene_array);

// PRE number of chrs is equal to TOTAL_CHROMOSOMES
chromosome_t rouletteSelect(chromosome_t *chrs, double sumFitness) {
  double random = (rand() / RAND_MAX) * sumFitness;
  double cumulativeTotal = 0;
  for (int i = 0; i < TOTAL_CHROMOSOMES; ++i) {
    cumulativeTotal += chrs[i].fitness;
    if (random < cumulativeTotal) {
      return chrs[i];
    }
  }

  printf("random: %f, cTotal: %f\n", random, cumulativeTotal);
  perror("rouletteSelect could not select a chromosome!");
  exit(EXIT_FAILURE);
}

chromosome_t generateRandom(int target) {
  chromosome_t chr = {(__uint64_t) (rand() % (int) (pow(2, CHROMOSOME_SIZE)))};
  return chr;
}

children_t breed(chromosome_t mother, chromosome_t father, int target) {
  chromosome_t fstChild = mother;
  chromosome_t sndChild = father;
  if (rand() / RAND_MAX < CROSSOVER_CHANCE) {
    __uint64_t mask = createMask(rand() % 32);
    fstChild.gene_array = (fstChild.gene_array & mask) + (father.gene_array & ~mask);
    sndChild.gene_array = (sndChild.gene_array & mask) + (mother.gene_array & ~mask);
  }
  children_t children = {fstChild, sndChild};
  return children;
}

chromosome_t mutate(chromosome_t child) {
  __uint64_t mutationMask = 0;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    // mutation chance for each bit 0.001
    if (rand() / RAND_MAX < MUTATION_CHANCE) {
      ++mutationMask;
    }
    mutationMask <<= 1;
  }

  child.gene_array = child.gene_array ^ mutationMask;
  return child;
}

// checks if target chromosome has occurred and returns it, else assigns fitness
chromosome_t assignFitnessCheckTarget(chromosome_t *chrs, int target) {
  for (int i = 0; i < TOTAL_CHROMOSOMES; ++i) {
    int evaluation = evaluate(chrs[i].gene_array);
    if (evaluation == target) {
      return chrs[i];
    }
    chrs[i].fitness = 1 / abs(evaluation - target);
  }
  return NULL;
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
        gene_array >>= GENE_SIZE;
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

int main() {
  srand((unsigned) time(NULL));

  printf("Setting up the target...\n");
  gene_array_t targetGeneArray = 0;
  // if i is even, then get a random numeric, else get a random operator
  for (int i = 0; i < NUM_GENES; ++i) {
    targetGeneArray <<= GENE_SIZE;
    int randomGeneOrdinal = (i % 2) ? (rand() % NONE) + ADD : (rand() % ADD);
    targetGeneArray += randomGeneOrdinal;
  }
  int target = evaluate(targetGeneArray);
  printf("The target is: %d\n", target);
  printf("Can be reached via: ");
  printGeneArray(targetGeneArray);


  printf("Setting up the first generation...\n");
  chromosome_t *chrs = malloc(sizeof(chromosome_t) * TOTAL_CHROMOSOMES);
  if (chrs == NULL) {
    perror("Could not allocate memory to chrs in main!");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < TOTAL_CHROMOSOMES; ++i) {
    chrs[i] = generateRandom(target);
  }
  assignFitnessCheckTarget(chrs, target);


  return EXIT_SUCCESS;
}