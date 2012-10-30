#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "minunit.h"
#include "kruskal.h"

void split(Kedge* list, Kedge** part1, Kedge** part2);
Kedge* merge(Kedge*, Kedge*);

mu_INIT();

int kedge_list_equal(Kedge* l1, Kedge* l2) {
  if(!l1 && !l2) return 1;
  if(!l1 || !l2) return 0;

  return (l1->n1 == l2->n1 &&
	  l1->addr1 == l2->addr2 &&
	  l1->n2 == l2->n2 &&
	  l1->addr2 == l2->addr2 &&
	  l1->weight == l2->weight &&
	  kedge_list_equal(l1->next, l2->next));
}

void kedge_list_print(Kedge* l1) {
  while(l1 != NULL) {
    printf("%f, \n", l1->weight);
    l1 = l1->next;
  }
  printf("\n");
}
		   
Kedge* kedge_weights(unsigned int nb, ...) {
  va_list ap;
  float w;
  Kedge* list = NULL;
  Kedge* prev = NULL;
  
  va_start(ap, nb);
  for(; nb > 0; nb--) {
    w = (float)va_arg(ap, int);
    Kedge* new = calloc(1, sizeof(Kedge));
    new->weight = w;
    new->next = NULL;
        
    if(prev == NULL) {
      list = new;
      prev = list;
    } else {
      prev->next = new;
      prev = new;
    }
  }

  va_end(ap);

  return list;
}

START_TEST(list_gen) {
  Kedge* l1 = NULL;
  l1 = kruskal_cons(l1, 0,0,0,0, 3);
  l1 = kruskal_cons(l1, 0,0,0,0, 2);
  l1 = kruskal_cons(l1, 0,0,0,0, 1);

  Kedge* l2 = kedge_weights(3, 1, 2, 3);

  mu_assert(kedge_list_equal(l1, l2),

	    "La fonction de création de liste est incorrecte");
  freeKedgeList(l1);
  freeKedgeList(l2);
}
END_TEST

START_TEST(split) {
  Kedge *s1, *s2;
  split(NULL, &s1, &s2);
  mu_assert(s1 == NULL && s2 == NULL,
	    "Mauvais split sur le cas de base NULL");

  Kedge* l1 = kruskal_cons(NULL, 0,0,0,0, 42);
  split(l1, &s1, &s2);

  freeKedgeList(s1);
  freeKedgeList(s2);
}
END_TEST
  

START_TEST(merge) {
  Kedge *s1, *s2;
  Kedge* l;
  Kedge* f;

  l = kedge_weights(7, 4,5,6,7,7,9,32);
  s1 = kedge_weights(4, 4,5,7,9);
  s2 = kedge_weights(3, 6,7,32);

  f = merge(s1, s2);

  mu_assert(kedge_list_equal(f, l),
  	    "Mauvaise fusion");
  
  freeKedgeList(l);
  freeKedgeList(f);
}
END_TEST

START_TEST(mergeSort) {
  Kedge* l1 = NULL;
  Kedge* l2 = NULL;

  l1 = kedge_weights(6, 7,4,5,9,6,7);

  l2 = kedge_weights(6, 4,5,6,7,7,9);

  l1 = mergeSort(l1);

  mu_assert(kedge_list_equal(l1, l2),
	    "La fonction de tri ne trie pas correctement");

  mu_assert(mergeSort(NULL) == NULL,
	    "Mauvais tri du cas de base NULL (liste vide)");

  freeKedgeList(l1);
  freeKedgeList(l2);
}
END_TEST


int main() {

  mu_run_test(list_gen);
  mu_run_test(split);
  mu_run_test(merge);

  mu_run_test(mergeSort);

  mu_summary();
  return __tests_failed;
}
