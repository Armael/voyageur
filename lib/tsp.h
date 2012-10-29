#ifndef TSP_H_INCLUDED
#define TSP_H_INCLEDED

#include "graph.h"

typedef struct town {
  int id;

  struct town* next;
} Town;

/* Algorithme TSP */
Town* tsp(Graph* g);

/* Fonction de libération de la liste des villes retournée par
   tsp() */
void freeTowns(Town* list);

#endif
