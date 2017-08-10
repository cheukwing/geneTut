#ifndef GENETUT_UTILS_H
#define GENETUT_UTILS_H

#define CHROMOSOME_SIZE 36
#define GENE_SIZE 4
#define NUM_GENES CHROMOSOME_SIZE / GENE_SIZE
#define CROSSOVER_CHANCE 0.7
#define MUTATION_CHANCE 0.1
#define TOTAL_CHROMOSOMES 100

typedef __uint64_t gene_array_t;

typedef struct chromosome {
  gene_array_t gene_array;
  double fitness;
} chromosome_t;

typedef struct children {
  chromosome_t fstChild;
  chromosome_t sndChild;
} children_t;

typedef enum gene {
  ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
  ADD, MINUS, MULTIPLY, NONE
} gene_t;

__uint64_t createMask(int);

gene_t getGene(gene_array_t);

_Bool isNumeric(gene_t);

_Bool isOperator(gene_t);

void printGeneArray(gene_array_t);

#endif //GENETUT_UTILS_H
