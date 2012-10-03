#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

typedef struct Vector {
  void** content; /* void* array */
  int size;
} Vector;

Vector* vector_new();
void vector_free(Vector* v);
void* vector_get(Vector* v, int id);
void vector_set(Vector* v, int id, void* value);

#endif
