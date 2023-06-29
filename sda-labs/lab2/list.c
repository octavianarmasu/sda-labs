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

typedef struct dll_node_t {
  void* data; /* Pentru ca datele stocate sa poata avea orice tip, folosim un
                             pointer la void. */
  struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct doubly_linked_list_t {
  dll_node_t* head;
  unsigned int data_size;
  unsigned int size;
} doubly_linked_list_t;

/*
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii list).
 */
doubly_linked_list_t* dll_create(unsigned int data_size) {
  doubly_linked_list_t* list = malloc(sizeof(doubly_linked_list_t));
  if (!list) {
    return NULL;
  }
  list->data_size = data_size;
  list->head = NULL;
  list->size = 0;
  return list;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere?
 * Atentie: n>=0 (nu trebuie tratat cazul in care n este negativ).
 */
dll_node_t* dll_get_nth_node(doubly_linked_list_t* list, unsigned int n) {
  if (n >= list->size) n = n % list->size;
  dll_node_t* node = malloc(sizeof(dll_node_t));
  DIE(node == NULL, "malloc failed");
  node = list->head;
  for (int i = 0; i < n; i++) node = node->next;
  return node;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei.
 * Atentie: n>=0 (nu trebuie tratat cazul in care n este negativ).
 */
void dll_add_nth_node(doubly_linked_list_t* list, unsigned int n,
                      const void* data) {
  dll_node_t* curr = list->head;
  dll_node_t* node = NULL;
  if (n > list->size) n = list->size;
  node = malloc(sizeof(dll_node_t));
  if (!node) {
    return;
  }
  node->data = NULL;
  node->data = malloc(list->data_size);
  node->next = NULL;
  if (!node->data) {
    return;
  }
  memcpy(node->data, data, list->data_size);
  if (n == 0) {
    if (list->size != 0) {
      node->next = list->head;
      list->head = node;
      node->prev = curr->prev;
      curr->prev = node;
      for (int i = 0; i < list->size - 1; i++) curr = curr->next;
      curr->next = node;
    } else {
      list->head = node;
      node->prev = node;
      node->next = node;
    }
    list->size++;
  } else {
    for (int i = 0; i < n - 1; i++) curr = curr->next;
    dll_node_t* aux = curr->next;
    node->next = curr->next;
    curr->next = node;
    node->prev = curr;
    aux->prev = node;
    list->size++;
  }
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Este responsabilitatea apelantului sa elibereze memoria
 * acestui nod.
 * Atentie: n>=0 (nu trebuie tratat cazul in care n este negativ).
 */
dll_node_t* dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n) {
  if (n >= list->size) n = list->size - 1;
  if (n == 0) {
    if (list->size != 1) {
      dll_node_t* first = list->head;
      dll_node_t* curr = list->head->prev;
      list->head = first->next;
      list->head->prev = first->prev;
      curr->next = list->head;
      list->size--;
      return first;
    } else {
      dll_node_t* first = list->head;
      list->head = NULL;
      return first;
    }
  } else {
    dll_node_t* node = list->head;
    dll_node_t* curr = list->head->next;
    for (int i = 0; i < n - 1; i++) {
      node = curr;
      curr = curr->next;
    }
    node->next = curr->next;
    dll_node_t* aux = curr->next;
    aux->prev = node;
    list->size--;
    return curr;
  }
  return 0;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int dll_get_size(doubly_linked_list_t* list) { return list->size; }

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void dll_free(doubly_linked_list_t** pp_list) {
  dll_node_t* curr = (*pp_list)->head;
  dll_node_t* node = curr;
  int n = (*pp_list)->size;
  while (n) {
    node = curr;
    curr = curr->next;
    free(node->data);
    free(node);
    n--;
  }
  free(*pp_list);
  *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void dll_print_int(doubly_linked_list_t* list) {
  if (list->size != 0) {
    dll_node_t* node = list->head;
    if (!node) {
      exit(0);
    }
    int n = list->size;
    while (n) {
      printf("%d ", *(int*)node->data);
      node = node->next;
      n--;
    }
  }
  printf("\n");
}

/*
 * Procedura primește ca parametru un pointer la începutul unei liste dublu
 * înlănțuite și construiește două liste în care se vor afla toate elementele
 * de pe poziții pare, respectiv impare, în aceeași ordine.
 */
void split_parity(doubly_linked_list_t* list, doubly_linked_list_t* odd_list,
                  doubly_linked_list_t* even_list) {
  unsigned int n = list->size;
  for (unsigned int i = 0; i < n; i++) {
    dll_node_t* node = dll_get_nth_node(list, i);
    if (i % 2 == 0)
      dll_add_nth_node(even_list, even_list->size, node->data);
    else
      dll_add_nth_node(odd_list, odd_list->size, node->data);
  }
}

int main() {
  doubly_linked_list_t *list, *odd_list, *even_list;

  char command[16], added_elem[64], *end_ptr;
  long size;

  even_list = dll_create(sizeof(int));
  odd_list = dll_create(sizeof(int));

  list = dll_create(sizeof(int));
  scanf("%ld", &size);

  long int curr_nr;
  for (int i = 0; i < size; ++i) {
    scanf("%ld", &curr_nr);
    dll_add_nth_node(list, size, &curr_nr);
  }

  split_parity(list, odd_list, even_list);

  dll_print_int(even_list);
  dll_print_int(odd_list);

  dll_free(&list);

  return 0;
}