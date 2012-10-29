#include <stdio.h>
#include "vector.h"

#include "minunit.h"

static Vector* v = NULL;

mu_INIT();

START_TEST(test_null_vect) {
  mu_assert(v != NULL,
	    "Impossible de créer un vecteur");
}
END_TEST

START_TEST(test_read_10) {
  int i;
  for(i=0; i < 10; i++) {
    mu_assert(i == vector_get(v, i).i,
	      "Information dans le tableau erronée (%d au lieu de %d)",
	      vector_get(v, i).i, i);
  }
}
END_TEST

int main() {
  v = vector_new();
  mu_run_test(test_null_vect);

  int i;
  for(i=0; i < 10; i++) {
    vector_set(v, i, (Generic)i);
  }

  mu_run_test(test_read_10);

  vector_free(v);

  mu_summary();
  return __tests_failed;
}
