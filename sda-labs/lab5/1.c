#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 999
#define MAX_STRING_SIZE 256
typedef struct {
  int** matrix; /* Matricea de adiacenta a grafului */
  int nodes;    /* Numarul de noduri din graf. */
} matrix_graph_t;

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru matricea de adiacenta a grafului.
 */

/* Graful este ORIENTAT */

/* Nodurile sunt indexate de la 0.*/

matrix_graph_t* mg_create(int nodes) {
  matrix_graph_t* mg = malloc(sizeof(matrix_graph_t));
  mg->nodes = nodes;
  mg->matrix = (int**)malloc(sizeof(int*) * nodes);
  for (int i = 0; i < nodes; i++) {
    mg->matrix[i] = (int*)malloc(sizeof(int) * nodes);
  }
  for (int i = 0; i < nodes; i++) {
    for (int j = 0; j < nodes; j++) {
      mg->matrix[i][j] = INF;
    }
  }
  return mg;
}

/* Adauga o muchie intre nodurile sursa si destinatie */
void mg_add_edge(matrix_graph_t* graph, int src, int dest) {
  graph->matrix[src][dest] = 1;
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int mg_has_edge(matrix_graph_t* graph, int src, int dest) {
  if (graph->matrix[src][dest] == 1) return 1;
  return 0;
}

/* Elimina muchia dintre nodurile sursa si destinatie */
void mg_remove_edge(matrix_graph_t* graph, int src, int dest) {
  graph->matrix[src][dest] = INF;
}

/* Elibereaza memoria folosita de matricea de adiacenta a grafului */
void mg_free(matrix_graph_t* graph) {
  for (int i = 0; i < graph->nodes; i++) {
    free(graph->matrix[i]);
  }
  free(graph->matrix);
  free(graph);
}

void floydWarshall(matrix_graph_t* mg) {
  for (int k = 0; k < mg->nodes; k++) {
    for (int i = 0; i < mg->nodes; i++) {
      for (int j = 0; j < mg->nodes; j++) {
        if (mg->matrix[i][k] + mg->matrix[k][j] < mg->matrix[i][j]) {
          mg->matrix[i][j] = mg->matrix[i][k] + mg->matrix[k][j];
        }
      }
    }
  }
  for (int i = 0; i < mg->nodes; i++) {
    for (int j = 0; j < mg->nodes; j++) {
      if (mg->matrix[i][j] == INF)
        printf("-1 ");
      else
        printf("%d ", mg->matrix[i][j]);
    }
    printf("\n");
  }
}

int main() {
  matrix_graph_t* mg = NULL;

  while (1) {
    char command[MAX_STRING_SIZE];
    int nr1, nr2;
    int start_node;
    int nr_nodes;

    scanf("%s", command);

    if (strncmp(command, "create_mg", 9) == 0) {
      scanf("%d", &nr_nodes);
      mg = mg_create(nr_nodes);
    }

    if (strncmp(command, "add_edge", 8) == 0) {
      if (mg != NULL) {
        scanf("%d %d", &nr1, &nr2);
        mg_add_edge(mg, nr1, nr2);
      } else {
        printf("Create a graph first!\n");
        exit(0);
      }
    }

    if (strncmp(command, "floyd", 3) == 0) {
      if (mg != NULL) {
        floydWarshall(mg);
      } else {
        printf("Create a graph first!\n");
        exit(0);
      }
    }

    if (strncmp(command, "free", 4) == 0) {
      if (mg != NULL) {
        mg_free(mg);
      } else {
        printf("Create a graph first!\n");
        exit(0);
      }
      break;
    }
  }

  return 0;
}
