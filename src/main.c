#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "func.h"

int evaluate(gene_array_t gene_array);

// PRE number of chrs is equal to TOTAL_CHROMOSOMES
chromosome_t rouletteSelect(chromosome_t *chrs, double sumFitness) {
  double random = drand48() * sumFitness;
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

chromosome_t generateRandom() {
  chromosome_t chr = {(__uint64_t) (rand() % (int) (pow(2, CHROMOSOME_SIZE)))};
  return chr;
}

chromosome_t mutate(chromosome_t child) {
  __uint64_t mutationMask = 0;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    if (drand48() < MUTATION_CHANCE) {
      ++mutationMask;
    }
    mutationMask <<= 1;
  }

  child.gene_array = child.gene_array ^ mutationMask;
  return child;
}

children_t breed(chromosome_t mother, chromosome_t father) {
  chromosome_t fstChild = mother;
  chromosome_t sndChild = father;
  if (drand48() < CROSSOVER_CHANCE) {
    __uint64_t mask = createMask(rand() % CHROMOSOME_SIZE);
    fstChild.gene_array = (fstChild.gene_array & mask) + (father.gene_array & ~mask);
    sndChild.gene_array = (sndChild.gene_array & mask) + (mother.gene_array & ~mask);
  }
  fstChild = mutate(fstChild);
  sndChild = mutate(sndChild);
  children_t children = {fstChild, sndChild};
  return children;
}

// checks if target chromosome has occurred and returns it, else assigns fitness
chromosome_t *assignFitnessCheckTarget(chromosome_t *chrs, int target) {
  for (int i = 0; i < TOTAL_CHROMOSOMES; ++i) {
    int evaluation = evaluate(chrs[i].gene_array);
    if (evaluation == target) {
      return &chrs[i];
    }
    chrs[i].fitness = 1.0 / abs(evaluation - target);
  }
  return NULL;
}

// LEFT TO RIGHT EVALUATION
int evaluate(gene_array_t gene_array) {
  gene_t gene;
  int genesEvaluated = 0;
  do {
    gene = getGene(gene_array);
    gene_array >>= GENE_SIZE;
    ++genesEvaluated;
  } while (!isNumeric(gene));

  int result = gene;

  // while have not checked all genes
  while (genesEvaluated < NUM_GENES) {
    gene = getGene(gene_array);
    gene_array >>= GENE_SIZE;
    ++genesEvaluated;
    // find next operator, ignore all numeric
    if (isOperator(gene)) {
      func_t func = operatorToFunction(gene);
      // find next numeric, ignore all operator
      while (genesEvaluated < NUM_GENES && !isNumeric(gene)) {
        gene = getGene(gene_array);
        gene_array >>= GENE_SIZE;
        ++genesEvaluated;
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
  srand48(time(NULL));

  printf("Setting up the target...\n");
  gene_array_t targetGeneArray = 0;
  // if i is even, then get a random numeric, else get a random operator
  for (int i = 0; i < NUM_GENES; ++i) {
    targetGeneArray <<= GENE_SIZE;
    int randomGeneOrdinal = (i % 2) ? rand() % (NONE - ADD) + ADD : rand() % ADD;
    targetGeneArray += randomGeneOrdinal;
  }
  int target = evaluate(targetGeneArray);
  printf("The target is: %d\n", target);
  printf("Can be reached via: ");
  printGeneArray(targetGeneArray);

  printf("Setting up the first generation...\n");
  chromosome_t *currentGen = malloc(sizeof(chromosome_t) * TOTAL_CHROMOSOMES);
  if (currentGen == NULL) {
    perror("Could not allocate memory to currentGen in main!\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < TOTAL_CHROMOSOMES; ++i) {
    currentGen[i] = generateRandom();
  }

  chromosome_t *potentialSolution = assignFitnessCheckTarget(currentGen, target);
  printf("Generation 0 have formed...\n");

  int generation = 1;
  while (potentialSolution == NULL) {
    double sumFitness = 0;
    for (int i = 0; i < TOTAL_CHROMOSOMES; ++i) {
      sumFitness = currentGen[i].fitness;
    }

    chromosome_t *nextGen = malloc(sizeof(chromosome_t) * TOTAL_CHROMOSOMES);
    if (nextGen == NULL) {
      perror("Could not allocate memory to nextGen in main!\n");
      free(currentGen);
      exit(EXIT_FAILURE);
    }

    // breed the next generation
    for (int i = 0; i < TOTAL_CHROMOSOMES / 2; i += 2) {
      chromosome_t mother = rouletteSelect(currentGen, sumFitness);
      chromosome_t father = rouletteSelect(currentGen, sumFitness);
      children_t children = breed(mother, father);
      nextGen[i] = children.fstChild;
      nextGen[i + 1] = children.sndChild;
    }

    free(currentGen);
    currentGen = nextGen;
    potentialSolution = assignFitnessCheckTarget(currentGen, target);
    printf("Generation %d have been bred...\n", generation);
    ++generation;
  }

  printf("Found a solution!\n");
  printGeneArray(potentialSolution->gene_array);

  free(currentGen);
  return EXIT_SUCCESS;
}