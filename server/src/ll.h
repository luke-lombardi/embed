#ifndef LL_H
#define LL_H

#include <stdint.h>

struct node {
  struct node *next;
  void *data;
};

struct list {
  struct node *head;
  struct node *tail;
  uint32_t count;
};

struct list* linked_list_create();
void linked_list_destroy(struct list *ll);

void linked_list_push(struct list *ll, void *data);
void* linked_list_pop(struct list *ll);


#endif