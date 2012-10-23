#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

typedef struct Vector {
  void** content; /* void* array */
  int size;
} Vector;

/* Renvoie un nouveau tableau */
Vector* vector_new();

/* Renvoie un nouveau tableau préalloué à la taille indiquée */
Vector* vector_newWithSize(int size);

/* Initialise les nb premiers éléments du vector à la valeur value */
void vector_fill(Vector* v, int nb, void* value);

/* Libère la mémoire occupée par le tableau */
void vector_free(Vector* v);

/* Fonction permettant d'accéder à un élément connaissant son
   indice. (Il n'y a pas de protection contre le dépassement mémoire
   puisque ce n'est pas le cas avec les tableaux natifs en C */
void* vector_get(Vector* v, int id);

/* Fonction permettant de modifier la valeur d'une case du tableau
   connaissant son indice. Si le tableau est trop petit il est étendu
   de manière à ce que sa nouvelle taille convienne. L'expansion du
   tableau se fait en doublant la taille à chaque fois. Ceci garantit
   une complexité en O(1) amorti (Cormen chap. 17 section 4).
*/
void vector_set(Vector* v, int id, void* value);

#endif
