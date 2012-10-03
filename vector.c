#include "vector.h"

#include <stdio.h>
#include <stdlib.h>


/* Renvoie un nouveau tableau */
Vector* vector_new() {
  Vector *v = malloc(sizeof(Vector));
  v->content = malloc(sizeof(void*));
  v->size = 1;

  return v;
}

/* Libère la mémoire occupée par le tableau */
void vector_free(Vector* v) {
  free(v->content);
  free(v);
}

/* Fonction permettant d'accéder à un élément connaissant 
   son indice. (Il n'y a pas de protection contre le dépassement
   mémoire puisque ce n'est pas le cas avec les tableaux natifs
   en C */
void* vector_get(Vector* v, int id) {
  return v->content[id];
}

/* Fonction permettant de modifier la valeur d'une case                          
   du tableau connaissant son indice. Si le tableau est trop petit                          
   il est étendu de manière à ce que sa nouvelle taille convienne */
void vector_set(Vector* v, int id, void* value) {
  if(id < v->size) {
    v->content[id] = value;
  } else {
    for(;id >= v->size; v->size *= 2);

    v->content = realloc(v->content, v->size * sizeof(void*));
    vector_set(v, id, value);
  }
}

