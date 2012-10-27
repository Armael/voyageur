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

/* Renvoie un nouveau tableau préalloué à la taille indiquée */
Vector* vector_newWithSize(int size) {
  Vector* v = malloc(sizeof(Vector));
  v->content = malloc(size*sizeof(void*));
  v->size = size;

  return v;
}

/* Initialise les nb premiers éléments du vector à la valeur value */
void vector_fill(Vector* v, int nb, void* value) {
  int i;
  for(i=0; i < nb; i++)
    v->content[i] = value;
}

/* Libère la mémoire occupée par le tableau */
void vector_free(Vector* v) {
  if(v != NULL) {
    free(v->content);
    free(v);
  }
}

/* Fonction permettant d'accéder à un élément connaissant son
   indice. (Il n'y a pas de protection contre le dépassement mémoire
   puisque ce n'est pas le cas avec les tableaux natifs en C */
void* vector_get(Vector* v, int id) {
  return v->content[id];
}

/* Fonction permettant de modifier la valeur d'une case du tableau
   connaissant son indice. Si le tableau est trop petit il est étendu
   de manière à ce que sa nouvelle taille convienne. L'expansion du
   tableau se fait en doublant la taille à chaque fois. Ceci garantit
   une complexité en O(1) amorti (Cormen chap. 17 section 4).
*/
void vector_set(Vector* v, int id, void* value) {
  if(id < v->size) {
    v->content[id] = value;
  } else {
    for(;id >= v->size; v->size *= 2);

    v->content = realloc(v->content, v->size * sizeof(void*));
    vector_set(v, id, value);
  }
}

