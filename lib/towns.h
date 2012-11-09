#ifndef TOWNS_H_INCLUDED
#define TOWNS_H_INCLUDED

#include "vector.h"

typedef struct {
  Vector* names;
  Vector* x;
  Vector* y;

  int nb;
} Towns;

Towns* towns_new();
void towns_free(Towns* t);

#endif
