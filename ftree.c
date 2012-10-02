#include "ftree.h"
#include "vector.h"
#include <stdlib.h>

Ftree* ftree_create(void *data) {
  Ftree *p = malloc(sizeof(Ftree));
  p->data = data;
  p->neighbors = vector_new();
  p->neighborsNb = 0;

  return p;
}

void ftree_free_(Ftree *tree, Ftree *previous) {
  int i = 0;
  for(i = 0; i < tree->neighborsNb; i++) {
    if(vector_get(tree->neighbors, i) != previous) {
      ftree_free(vector_get(tree->neighbors, i), tree);
    }
  }
  vector_free(tree->neighbors);
  free(tree);
}

void ftree_free(Ftree *tree) {
  ftree_free_(tree, NULL);
}

void ftree_join(Ftree *tree1, Ftree *tree2) {
  vector_set(tree1->neighbors, tree1->neighborsNb, tree2);
  vector_set(tree2->neighbors, tree2->neighborsNb, tree1);
  tree1->neighborsNb++;
  tree2->neighborsNb++;
}
