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

typedef struct ll_node_t ll_node_t;
struct ll_node_t {
  void* data;
  ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t {
  ll_node_t* head;
  unsigned int data_size;
  unsigned int size;
};

linked_list_t* ll_create(unsigned int data_size) {
  linked_list_t* ll;

  ll = malloc(sizeof(linked_list_t));
  DIE(ll == NULL, "linked_list malloc");

  ll->head = NULL;
  ll->data_size = data_size;
  ll->size = 0;

  return ll;
}

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

ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n) {
  ll_node_t *prev, *curr;

  if (list == NULL) {
    return NULL;
  }

  if (list->head == NULL) {
    return NULL;
  }

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
    list->head = curr->next;
  } else {
    prev->next = curr->next;
  }

  list->size--;

  return curr;
}

unsigned int ll_get_size(linked_list_t* list) {
  if (list == NULL) {
    return -1;
  }

  return list->size;
}

void ll_free(linked_list_t** pp_list) {
  ll_node_t* currNode;

  if (pp_list == NULL || *pp_list == NULL) {
    return;
  }

  while (ll_get_size(*pp_list) > 0) {
    currNode = ll_remove_nth_node(*pp_list, 0);
    free(currNode->data);
    free(currNode);
  }

  free(*pp_list);
  *pp_list = NULL;
}

void ll_print_int(linked_list_t* list) {
  ll_node_t* curr;

  if (list == NULL) {
    return;
  }

  curr = list->head;
  while (curr != NULL) {
    printf("%d ", *((int*)curr->data));
    curr = curr->next;
  }

  printf("\n");
}

struct Stack {
  linked_list_t* list;
};

typedef struct {
  char c;
  int nr;
} caracter_t;

void ll_print_string(struct Stack* stack) {
  ll_node_t* curr = stack->list->head;

  if (stack == NULL) {
    return;
  }

  while (curr != NULL) {
    printf("%c", ((caracter_t*)curr->data)->c);
    curr = curr->next;
  }

  printf("\n");
}

/* ---------------------------------- TODO: Stack implementation
 * ----------------------------------------*/

void init_stack(struct Stack* stack, unsigned int data_size) {
  stack->list = ll_create(data_size);
}

int get_size_stack(struct Stack* stack) {
  /* TODO */
  return stack->list->size;
}

int is_empty_stack(struct Stack* stack) {
  if (stack->list->size == 0) {
    return 1;
  }
  return 0;
}

void* peek_stack(struct Stack* stack) {
  if (stack == NULL) {
    return NULL;
  }
  if (stack->list == NULL) {
    return NULL;
  }
  ll_node_t* node = stack->list->head;
  while (node->next != NULL) {
    node = node->next;
  }
  return node->data;
}

void pop_stack(struct Stack* stack) {
  if (stack == NULL || stack->list == NULL) {
    return;
  }
  ll_node_t* node = ll_remove_nth_node(stack->list, stack->list->size);
  free(node);
}

void push_stack(struct Stack* stack, void* new_data) {
  ll_add_nth_node(stack->list, stack->list->size, new_data);
}

void clear_stack(struct Stack* stack) {
  /* TODO */
  struct ll_node_t* node;
  while (!is_empty_stack(stack)) {
    node = ll_remove_nth_node(stack->list, 0);
    free(node);
  }
}

void purge_stack(struct Stack* stack) {
  /* TODO */
  clear_stack(stack);
  free(stack->list);
}

/*
 * Functie care sterge duplicatele de dimensiun k dintr-un sir
 */

void removeDuplicates(char* s, int k) {
  struct Stack* stack = malloc(sizeof(*stack));

  init_stack(stack, sizeof(caracter_t));

  int i, j;
  caracter_t* a = malloc(sizeof(caracter_t));
  for (i = 0; i < strlen(s); i++) {
    if (get_size_stack(stack) == 0) {
      a->c = s[i];
      a->nr = 1;
      push_stack(stack, a);
    } else {
      a = peek_stack(stack);
      if (s[i] == a->c) {
        a->nr++;
        pop_stack(stack);
        push_stack(stack, a);

        if (a->nr == k) {
          pop_stack(stack);
        }
      } else {
        a->c = s[i];
        a->nr = 1;
        push_stack(stack, a);
      }
    }
  }
  ll_node_t* node = stack->list->head;
  while (node != NULL) {
    a = node->data;
    for (i = 0; i < a->nr; i++) {
      printf("%c", a->c);
    }
    node = node->next;
  }
  printf("\n");
}

int main() {
  char s[64];
  int k;

  scanf("%d", &k);
  scanf("%s", s);

  removeDuplicates(s, k);

  return 0;
}
