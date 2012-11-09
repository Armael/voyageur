#include <stdlib.h>

#include "towns.h"

Towns* towns_new() {
  Towns* t = malloc(sizeof(Towns));
  t->names = vector_new();
  t->x = vector_new();
  t->y = vector_new();

  t->nb = 0;

  return t;
}

void towns_free(Towns* t) {
  vector_free(t->names);
  vector_free(t->x);
  vector_free(t->y);
  free(t);
}


