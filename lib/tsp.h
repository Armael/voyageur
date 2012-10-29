#ifndef TSP_H_INCLUDED
#define TSP_H_INCLEDED

#include "graph.h"

typedef struct {
  int id;

  struct Town* next;
} Town;

/* Algorithme TSP */
Town* tsp(Graph* g);

/* Fonction de libération de la liste des villes retournée par
   tsp() */
void freeTowns(Town* list);

#endif
