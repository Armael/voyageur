#ifndef TSP_H_INCLUDED
#define TSP_H_INCLEDED

#include "towns.h"

typedef struct town {
  int id;

  struct town* next;
} Town;

/* Algorithme TSP */
Town* tsp(Towns* t);

/* Fonction de libération de la liste des villes retournée par
   tsp() */
void freeTowns(Town* list);

#endif
