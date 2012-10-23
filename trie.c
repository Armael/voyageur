#include <stdlib.h>

#include "vector.h"
#include "trie.h"

Trie* trie_new() {
	Trie* t = malloc(sizeof(Trie));
	t->coord_x = 0;
	t->coord_y = 0;
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
	}
	else if(t->next == NULL) {
		t->next = vector_fill(vector_newWithSize(CHAR_NUMBER), CHAR_NUMBER, NULL);
		t->next = trie_addTown(vector_get(t->next, name[0]), &name[1], x, y);
	}
	else {
		t->next = trie_addTown(vector_get(t->next, name[0]), &name[1], x, y);
	}
	return t;
}


Trie* trie_free(Trie* t) {
  int i;
  if(t->next!=NULL) {
    for(i=0; i < CHAR_NUMBER; i++) {
      if(vector_get(t->next, i) != NULL) {
          free_tree(vector_get(t->next, i));
      }
    }
  }
  free(t);
}


void trie_getCoord(Trie* t, char* name, int* x, int* y) {
  	if(t == NULL) {
		*x = -1;
		*y = -1;
	}
	if(name[0] == '\0') {
		*x = t->coord_x;
		*y = t->coord_y;
	}
	else if(t->next == NULL) {
		*x = -1;
		*y = -1;
	}
	else {
		trie_getCoord(vector_get(t->next, name[0]), &name[1], x, y);
	}
}


