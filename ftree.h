#ifndef FTREE_H_INCLUDED
#define FTREE_H_INCLUDED
#include "vector.h"

typedef struct ftree {
  void *data;
  Vector *neighbors;
  int neighborsNb;
} Ftree;

Ftree* ftree_create(void *data);

void ftree_free(Ftree *tree);

void ftree_join(Ftree *tree1, Ftree *tree2);

#endif
