#include <stdio.h>

#include "graph.h"
#include "vector.h"
#include "minunit.h"

static Graph* g = NULL;
static int n1, n2, n3;

mu_INIT();

START_TEST(test_add_nodes) {
  n1 = graph_addNode(g);
  n2 = graph_addNode(g);
  n3 = graph_addNode(g);

  mu_assert(vector_get(g->nodes, n1) == NULL,
	    "Nouveau nœud 1 mal initialisé");
  mu_assert(vector_get(g->nodes, n2) == NULL,
	    "Nouveau nœud 2 mal initialisé");
  mu_assert(vector_get(g->nodes, n3) == NULL,
	    "Nouveau nœud 3 mal initialisé");
}
END_TEST

START_TEST(test_edge) {
  graph_addEdge(g, n1, n2, 42);

  mu_assert(vector_get(g->nodes, n1) != NULL,
	    "Arête 1->2 absente");
  mu_assert(((Edge*)vector_get(g->nodes, n1))->dest == n2,
	    "Mauvaise destination de l'arête 1->2");
  mu_assert(((Edge*)vector_get(g->nodes, n1))->weight == 42,
	    "Mauvais poids de l'arête 1->2");

  mu_assert(vector_get(g->nodes, n2) != NULL,
	    "Arête 2->1 absente; le graphe n'est pas orienté, \
            il faut des arêtes dans les deux sens");
  mu_assert(((Edge*)vector_get(g->nodes, n2))->dest == n1,
	    "Mauvaise destination de l'arête 2->1");
  mu_assert(((Edge*)vector_get(g->nodes, n2))->weight == 42,
	    "Mauvais poids de l'arête 2->1");
}
END_TEST

int main() {

  g = graph_new();

  mu_run_test(test_add_nodes);
  mu_run_test(test_edge);

  graph_toNeato(g, "graph.dot");
  printf("Fichier .dot en sortie : graph.dot\n");
  graph_free(g);

  mu_summary();

  return 0;
}
