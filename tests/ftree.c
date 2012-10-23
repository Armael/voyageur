#include <stdio.h>

#include "vector.h"
#include "ftree.h"
#include "minunit.h"

mu_INIT();

static Ftree *t1, *t2;

START_TEST(test_join) {
  ftree_join(t1, t2);

  mu_assert(t1->neighborsNb == 1,
	    "1 n'a pas un nombre de voisins égal à 1");
  mu_assert(t2->neighborsNb == 1,
	    "2 n'a pas un nombre de voisins égal à 1");
  mu_assert(vector_get(t1->neighbors, t1->neighborsNb-1) == t2,
	    "2 n'est pas le dernier voisin de 1");
  mu_assert(vector_get(t2->neighbors, t2->neighborsNb-1) == t1,
	    "1 n'est pas le dernier voisin de 2");
}
END_TEST

int main() {
  t1 = ftree_create((void*)1);
  t2 = ftree_create((void*)2);

  mu_run_test(test_join);

  ftree_free(t1);

  mu_summary();
  return __tests_failed;
}
  

