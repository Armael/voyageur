#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

typedef struct Vector {
  void** content; /* void* array */
  int size;
} Vector;

/* Renvoie un nouveau tableau */
Vector* vector_new();

/* Libère la mémoire occupée par le tableau */
void vector_free(Vector* v);

/* Fonction permettant d'accéder à un élément connaissant son
   indice. (Il n'y a pas de protection contre le dépassement mémoire
   puisque ce n'est pas le cas avec les tableaux natifs en C */
void* vector_get(Vector* v, int id);

/* Fonction permettant de modifier la valeur d'une case du tableau
   connaissant son indice. Si le tableau est trop petit il est étendu
   de manière à ce que sa nouvelle taille convienne */
void vector_set(Vector* v, int id, void* value);

#endif
