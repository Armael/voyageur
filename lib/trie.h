#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdlib.h>

#include "vector.h"

#define CHAR_NUMBER = 256

typedef struct trie {
	int coord_x;
	int coord_y;
	int is_word;
	vector* next;
} Trie;

Trie* trie_new();

Trie* trie_addTown(Trie* t, char* name, int x, int y);

void trie_free(Trie* t);

void trie_getCoord(Trie* t, char* name, int* x, int* y);



#endif
