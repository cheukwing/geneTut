#ifndef GENETUT_UTILS_H
#define GENETUT_UTILS_H

#define CHROMOSOME_SIZE 36
#define GENE_SIZE 4
#define NUM_GENES CHROMOSOME_SIZE / GENE_SIZE
#define ORD_ADD 10
#define ORD_MINUS 11
#define ORD_MULTIPLY 12

typedef __uint64_t gene_array_t;

typedef struct chromosome {
  gene_array_t gene_array;
  float fitness;
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

#endif //GENETUT_UTILS_H
