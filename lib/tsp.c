#include "vector.h"
#include "ftree.h"
#include "towns.h"
#include "kruskal.h"
#include "tsp.h"

#include <stdlib.h>

Town* townListRev(Town* list) {
  Town *prev, *cur, *next;
  prev = NULL;
  cur = list;
  while(cur != NULL) {
    next = cur->next;
    cur->next = prev;
    prev = cur;
    cur = next;
  }
  return prev;
}

Town* prefixTravel_(Ftree* t, Ftree* parent, Town* path) {
  int i = 0;
  Town* new_town = malloc(sizeof(Town));
  new_town->id = (t->data).i;
  new_town->next = path;
  path = new_town;
  for(i = 0; i < t->neighborsNb; i++) {
    if(vector_get(t->neighbors, i).p != parent) {
      path = prefixTravel_(vector_get(t->neighbors, i).p, t, path);
    }
  }
  return path;
}

Town* prefixTravel(Ftree* t) {
  return townListRev(prefixTravel_(t, NULL, NULL));
}

/* Fonction de libération de la liste des villes retournée par
   tsp() */
void freeTowns(Town* list) {
  Town* t;
  while(list != NULL) {
    t = list->next;
    free(list);
    list = t;
  }
}

/* Algorithme TSP */
Town* tsp(Towns* t) {
  Ftree* couvrant = kruskal(t);

  Town* path = prefixTravel(couvrant);

  ftree_free(couvrant);

  return path;
}
