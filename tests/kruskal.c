#include <stdio.h>
#include <stdarg.h>

#include "minunit.h"
#include "kruskal.h"

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
		   
START_TEST(mergeSort) {
  Kedge* l1 = NULL;
  Kedge* l2 = NULL;

  /* l1 = 7,4,5,9,6,7,32 */
  l1 = kruskal_cons(l1, 0, 0, 0, 0, 7);
  l1 = kruskal_cons(l1, 0, 0, 0, 0, 4);
  l1 = kruskal_cons(l1, 0, 0, 0, 0, 5);
  l1 = kruskal_cons(l1, 0, 0, 0, 0, 9);
  l1 = kruskal_cons(l1, 0, 0, 0, 0, 6);
  l1 = kruskal_cons(l1, 0, 0, 0, 0, 7);
  l1 = kruskal_cons(l1, 0, 0, 0, 0, 32);

  /* l2 = 4,5,6,7,7,9,32 */
  l2 = kruskal_cons(l1, 0, 0, 0, 0, 4);
  l2 = kruskal_cons(l1, 0, 0, 0, 0, 5);
  l2 = kruskal_cons(l1, 0, 0, 0, 0, 6);
  l2 = kruskal_cons(l1, 0, 0, 0, 0, 7);
  l2 = kruskal_cons(l1, 0, 0, 0, 0, 7);
  l2 = kruskal_cons(l1, 0, 0, 0, 0, 9);
  l2 = kruskal_cons(l1, 0, 0, 0, 0, 32);

  mu_assert(kedge_list_equal(l1, l2),
	    "La fonction de tri ne trie pas correctement");

  mu_assert(mergeSort(NULL) == NULL,
	    "Mauvais tri du cas de base NULL (liste vide)");

  freeKedgeList(l1);
  freeKedgeList(l2);
}
END_TEST


int main() {
  mu_run_test(mergeSort);

  mu_summary();
  return __tests_failed;
}
