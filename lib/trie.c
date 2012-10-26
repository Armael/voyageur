#include <stdlib.h>

#include "vector.h"
#include "trie.h"

Trie* trie_new() {
  Trie* t = malloc(sizeof(Trie));
  t->coord_x = -1;
  t->coord_y = -1;
  t->is_word = 0;
  t->next = NULL;
}

Trie* trie_addTown(Trie* t, char* name, int x, int y) {
  if(t == NULL) {
    t = trie_new();
  }
  
  if(name[0] == '\0') {
    t->is_word = 1;
    t->coord_x = x;
    t->coord_y = y;
  } else {
    if(t->next == NULL) {
      t->next = vector_fill(vector_newWithSize(CHAR_NUMBER), CHAR_NUMBER, NULL);
    }
    
    vector_set(t->next, name[0], trie_addTown(vector_get(t->next, name[0]),
					      &name[1], x, y));
  }
  return t;
}

void trie_free(Trie* t) {
  int i;
  if(t != NULL) {
    if(t->next != NULL) {
      for(i=0; i < CHAR_NUMBER; i++) {
	Trie* child;
	if((child = vector_get(t->next, i)) != NULL) {
          trie_free(child);
	}
      }
      vector_free(t->next);
    }
  free(t);
  }
}

int trie_getCoord(Trie* t, char* name, int* x, int* y) {
   if(t == NULL) {
     return -1;
   }
  
  if(name[0] == '\0') {
    if(t->is_word) {
      *x = t->coord_x;
      *y = t->coord_y;
      return 0;
    } else {
      return -1;
    }
  } else {
    if(t->next == NULL) {
      return -1;
    } else {
      return trie_getCoord(vector_get(t->next, name[0]),
			   &name[1], x, y);
    }
  }
}


