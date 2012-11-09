#ifndef PARSERS_H_INCLUDED
#define PARSERS_H_INCLUDED

#include "trie.h"
#include "towns.h"

#include <stdio.h>
#include <math.h>

/*Graph* parse_EdgesDistances(FILE* f);
  Graph* parse_VerticesCoordinates(FILE* f); */
Trie* parse_Towns(FILE* f);
Towns* parse_VerticesCoordinates(FILE* f);
#endif
