#ifndef LINKEDLIST_H 
#define LINKEDLIST_H

#include <stdint.h>
#include <strings.h>

struct node {
  void* ptr;
  struct node *next;
};

struct linkedlist {
  uint32_t length;
  struct node *head;
  struct node *tail;
};


struct linkedlist *linkedlist_new();
void linkedlist_prepend(struct linkedlist *linkedlist, void *ptr);
void linkedlist_append(struct linkedlist *linkedlist, void*ptr);
struct node *linkedlist_delete(struct linkedlist *linkedlist, int index);
struct node *linkedlist_get(struct linkedlist *linkedlist, int index);
struct node *linkedlist_pop(struct linkedlist *linkedlist);
void linkedlist_tfree(struct linkedlist *ptr);
void linkedlist_free(struct linkedlist *ptr);

#endif // !LINKEDLIST_H 
