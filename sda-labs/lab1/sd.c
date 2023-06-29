#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIE(assertion, call_description)                 \
  do {                                                   \
    if (assertion) {                                     \
      fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
      perror(call_description);                          \
      exit(errno);                                       \
    }                                                    \
  } while (0)

#define MAX_STRING_SIZE 64

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
  linked_list_t* list = malloc(sizeof(linked_list_t));
  if (!list) {
    return NULL;
  }
  list->data_size = data_size;
  list->head = NULL;
  list->size = 0;
  return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, ignorati comanda (iesiti din functie).
 */
void ll_add_nth_node(linked_list_t* list, unsigned int n,
                     const void* new_data) {
  ll_node_t* curr = list->head;
  ll_node_t* node = NULL;
  if (n < 0) return;
  if (n > list->size) n = list->size;
  node = malloc(sizeof(ll_node_t));
  if (!node) {
    return;
  }
  node->data = NULL;
  node->data = malloc(list->data_size);
  node->next = NULL;
  if (!node->data) {
    return;
  }
  memcpy(node->data, new_data, list->data_size);
  if (n == 0) {
    node->next = list->head;
    list->head = node;
    list->size++;
  } else {
    for (int i = 0; i < n - 1; i++) curr = curr->next;
    node->next = curr->next;
    curr->next = node;
    list->size++;
  }
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, ignorati comanda (iesiti din functie).
 * Functia intoarce un pointer spre acest nod proaspat eliminat din lista.
 * Este responsabilitatea apelantului sa elibereze memoria acestui nod.
 */
ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n) {
  if (n < 0) exit(0);
  if (n > list->size) n = list->size - 1;
  if (n == 0) {
    ll_node_t* first = malloc(sizeof(ll_node_t));
    if (!first) {
      exit(0);
    }
    first = list->head;
    list->head = first->next;
    list->size--;
    return first;
  } else {
    ll_node_t* node = malloc(sizeof(ll_node_t));
    if (!node) {
      exit(0);
    }
    ll_node_t* curr = malloc(sizeof(ll_node_t));
    if (!curr) {
      exit(0);
    }
    node = list->head;
    curr = list->head->next;
    for (int i = 0; i < n - 1; i++) {
      node = curr;
      curr = curr->next;
    }
    node->next = curr->next;
    list->size--;
    return curr;
  }
  return 0;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int ll_get_size(linked_list_t* list) {
  unsigned int n = 0;
  ll_node_t* node = malloc(sizeof(ll_node_t));
  if (!node) {
    exit(0);
  }
  node = list->head;
  while (node->next != NULL) {
    n++;
    node = node->next;
  }
  return n;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t** pp_list) {
  ll_node_t* curr = (*pp_list)->head;
  ll_node_t* node = malloc(sizeof(ll_node_t));
  while (curr != NULL) {
    node = curr;
    curr = curr->next;
    free(node->data);
    free(node);
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
  if (list->size != 0) {
    ll_node_t* node = malloc(sizeof(ll_node_t));
    if (!node) {
      exit(0);
    }
    node = list->head;
    while (node->next != NULL) {
      printf("%d ", *(int*)node->data);
      node = node->next;
    }
    printf("%d ", *(int*)node->data);
  }
  printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t* list) {
  if (list->size != 0) {
    ll_node_t* node = malloc(sizeof(ll_node_t));
    if (!node) {
      exit(0);
    }
    node = list->head;
    while (node->next != NULL) {
      printf("%s ", (char*)node->data);
      node = node->next;
    }
    printf("%s", (char*)node->data);
  }
  printf("\n");
}

int main() {
  linked_list_t* linkedList;
  int is_int = 0;
  int is_string = 0;

  while (1) {
    char command[16], added_elem[64], *end_ptr;
    long nr, pos;

    scanf("%s", command);

    if (strcmp(command, "create_str") == 0) {
      linkedList = ll_create(MAX_STRING_SIZE);
      is_string = 1;
    }

    if (strcmp(command, "create_int") == 0) {
      linkedList = ll_create(sizeof(int));
      is_int = 1;
    }

    if (strcmp(command, "add") == 0) {
      scanf("%ld", &pos);
      scanf("%s", added_elem);

      nr = strtol(added_elem, &end_ptr, 10);
      if (nr != 0) {
        ll_add_nth_node(linkedList, pos, &nr);
      } else {
        ll_add_nth_node(linkedList, pos, end_ptr);
      }
    }

    if (strcmp(command, "remove") == 0) {
      scanf("%ld", &pos);
      ll_node_t* removed = ll_remove_nth_node(linkedList, pos);
      free(removed->data);
      free(removed);
    }

    if (strcmp(command, "print") == 0) {
      if (is_int == 1) {
        ll_print_int(linkedList);
      }

      if (is_string == 1) {
        ll_print_string(linkedList);
      }
    }

    if (strcmp(command, "free") == 0) {
      ll_free(&linkedList);
      break;
    }
  }

  return 0;
}