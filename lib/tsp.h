#ifndef TSP_H_INCLUDED
#define TSP_H_INCLEDED

typedef struct {
  int id;

  struct Nodes* next;
} Town*;

/* Algorithme TSP */
Town* tsp(Graph* g);

/* Fonction de libération de la liste des villes retournée par
   tsp() */
void freeTowns(Town* list);

#endif
