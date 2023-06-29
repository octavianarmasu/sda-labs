#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 256
#define MAX_NODES 10
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* --- LINKED LIST SUPPORT START --- */

typedef struct ll_node_t {
  void* data;
  struct ll_node_t* next;
} ll_node_t;

typedef struct linked_list_t {
  ll_node_t* head;
  unsigned int data_size;
  unsigned int size;
} linked_list_t;

linked_list_t* ll_create(unsigned int data_size) {
  linked_list_t* ll;

  ll = malloc(sizeof(*ll));

  ll->head = NULL;
  ll->data_size = data_size;
  ll->size = 0;

  return ll;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se returneaza
 * ultimul nod.
 */
ll_node_t* get_nth_node(linked_list_t* list, unsigned int n) {
  if (!list) {
    return NULL;
  }

  unsigned int len = list->size - 1;
  unsigned int i;
  ll_node_t* node = list->head;

  n = MIN(n, len);

  for (i = 0; i < n; ++i) {
    node = node->next;
  }

  return node;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void ll_add_nth_node(linked_list_t* list, unsigned int n,
                     const void* new_data) {
  ll_node_t *prev, *curr;
  ll_node_t* new_node;

  if (!list) {
    return;
  }

  /* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
  if (n > list->size) {
    n = list->size;
  }

  curr = list->head;
  prev = NULL;
  while (n > 0) {
    prev = curr;
    curr = curr->next;
    --n;
  }

  new_node = malloc(sizeof(*new_node));
  new_node->data = malloc(list->data_size);
  memcpy(new_node->data, new_data, list->data_size);

  new_node->next = curr;
  if (prev == NULL) {
    /* Adica n == 0. */
    list->head = new_node;
  } else {
    prev->next = new_node;
  }

  list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n) {
  ll_node_t *prev, *curr;

  if (!list || !list->head) {
    return NULL;
  }

  /* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
  if (n > list->size - 1) {
    n = list->size - 1;
  }

  curr = list->head;
  prev = NULL;
  while (n > 0) {
    prev = curr;
    curr = curr->next;
    --n;
  }

  if (prev == NULL) {
    /* Adica n == 0. */
    list->head = curr->next;
  } else {
    prev->next = curr->next;
  }

  list->size--;

  return curr;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int ll_get_size(linked_list_t* list) {
  if (!list) {
    return -1;
  }

  return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t** pp_list) {
  ll_node_t* currNode;

  if (!pp_list || !*pp_list) {
    return;
  }

  while (ll_get_size(*pp_list) > 0) {
    currNode = ll_remove_nth_node(*pp_list, 0);
    free(currNode->data);
    currNode->data = NULL;
    free(currNode);
    currNode = NULL;
  }

  free(*pp_list);
  *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t* list) {
  ll_node_t* curr;

  if (!list) {
    return;
  }

  curr = list->head;
  while (curr != NULL) {
    printf("%d ", *((int*)curr->data));
    curr = curr->next;
  }

  printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t* list) {
  ll_node_t* curr;

  if (!list) {
    return;
  }

  curr = list->head;
  while (curr != NULL) {
    printf("%s ", (char*)curr->data);
    curr = curr->next;
  }

  printf("\n");
}

/* --- START IMPLEMENT STACK HERE --- */

typedef struct stack_t stack;
struct stack_t {
  struct linked_list_t* list;
};

stack* st_create(unsigned int data_size) {
  stack* st = malloc(sizeof(*st));
  st->list = ll_create(data_size);

  return st;
}

unsigned int st_get_size(stack* st) {
  if (!st || !st->list) return 0;
  return st->list->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int st_is_empty(stack* st) {
  return !st || !st->list || !st->list->size;
}

void* st_peek(stack* st) {
  if (!st || !st->list || !st->list->size) return NULL;

  return st->list->head->data;
}

void st_pop(stack* st) {
  if (!st || !st->list) return;

  ll_remove_nth_node(st->list, 0);
}

void st_push(stack* st, void* new_data) {
  if (!st || !st->list) return;

  ll_add_nth_node(st->list, 0, new_data);
}

void st_clear(stack* st) {
  if (!st || !st->list) return;

  ll_free(&st->list);
}

void st_free(stack* st) {
  if (!st || !st->list) return;

  ll_free(&st->list);
  free(st);
}

/* --- START IMPLEMENT MATRIX GRAPH HERE --- */

typedef struct {
  int** matrix; /* Matricea de adiacenta a grafului */
  int nodes;    /* Numarul de noduri din graf. */
} matrix_graph_t;

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru matricea de adiacenta a grafului.
 */

/* Graful este ORIENTAT */

matrix_graph_t* mg_create(int nodes) {
  matrix_graph_t* graph = malloc(sizeof(*graph));
  graph->nodes = nodes;
  graph->matrix = malloc(nodes * sizeof(int*));
  for (int i = 0; i < graph->nodes; i++) {
    graph->matrix[i] = malloc(nodes * sizeof(int));
  }
  for (int i = 0; i < graph->nodes; i++) {
    for (int j = 0; j < graph->nodes; j++) {
      graph->matrix[i][j] = 0;
    }
  }
  return graph;
}

/* Adauga o muchie intre nodurile sursa si destinatie */
void mg_add_edge(matrix_graph_t* graph, int src, int dest) {
  graph->matrix[src][dest] = 1;
}

/* Elibereaza memoria folosita de matricea de adiacenta a grafului */
void mg_free(matrix_graph_t* graph) {
  for (int i = 0; i < graph->nodes; i++) {
    free(graph->matrix[i]);
  }
  free(graph->matrix);
  free(graph);
}

/* --- START IMPLEMENT DFS HERE --- */

/* Implementati parcurgerea DFS a unui Matrix Graph
 * Functia trebuie sa printeze un nod, imediat ce acesta este vizitat pentru
 * prima data In alegerea ordinii parcurgerii vecinilor, acestia vor fi parcursi
 * in ordinea fireasca citirii unei linii dintr-o matrice (i=(1..n))
 */

void dfs_matrix_graph(matrix_graph_t* mg, int node) {
  int* visited = malloc(mg->nodes * sizeof(int));
  for (int i = 0; i < mg->nodes; i++) {
    visited[i] = 0;
  }
  stack* st = st_create(sizeof(int));
  st_push(st, &node);
  while (!st_is_empty(st)) {
    int current_node = *(int*)st_peek(st);
    if (!visited[current_node]) {
      printf("%d ", current_node);
      visited[current_node] = 1;
    }
    int gasit = 0;
    for (int i = 0; i < mg->nodes; i++) {
      if (mg->matrix[current_node][i] == 1 && !visited[i]) {
        st_push(st, &i);
        gasit = 1;
        break;
      }
    }
    if (!gasit) {
      st_pop(st);
    }
  }
  free(visited);
  st_free(st);
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

    if (strncmp(command, "dfs", 3) == 0) {
      if (mg != NULL) {
        int color[nr_nodes], t_dest[nr_nodes], t_fin[nr_nodes],
            parents[nr_nodes];
        scanf("%d", &start_node);

        dfs_matrix_graph(mg, start_node);
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
