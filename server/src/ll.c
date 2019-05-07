#include "ll.h"

#include <stdlib.h>
#include <assert.h>


struct list* linked_list_create() {
  struct list *ll = malloc(sizeof(struct list));

  ll->count = 0;
  ll->head = NULL;
  ll->tail = NULL;

  return ll;
}

void linked_list_destroy(struct list *ll) {
  assert(ll != NULL);

  void *data = NULL;

  // empty list
  do{
    data = linked_list_pop(ll);
  } while(data != NULL);

  free(ll);
}

void linked_list_push(struct list *ll, void *data) {
  assert(ll != NULL);
  assert(data != NULL);

  struct node *new_node = malloc(sizeof(struct node));

  new_node->data = data;

  // if first item, head=tail
  if(ll->count == 0) {
    ll->head = new_node;
    ll->tail = ll->head;
    new_node->next = NULL;
  } else {
  // otherwise, new_node is always head
    new_node->next = ll->head;
    ll->head = new_node;
  }

  ll->count += 1;
}

void* linked_list_pop(struct list *ll) {
  assert(ll != NULL);

  if(ll->count == 0)
    return NULL;

  struct node *current_node = ll->head;
  struct node *previous = NULL;

  while(current_node != ll->tail) {
    previous = current_node;
    current_node = current_node->next;
  }

  if(previous != NULL)
    previous->next = NULL;
    
  ll->tail = previous;
  ll->count -= 1;

  void *data = current_node->data;
  free(current_node);

  return data;
}