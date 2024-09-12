#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct linkedlist *linkedlist_new()
{
  struct linkedlist *ptr = (struct linkedlist*) malloc(sizeof(struct linkedlist));
  ptr->head=0;
  ptr->tail=0;
  ptr->length=0;

  return ptr;
}

void linkedlist_append(struct linkedlist *linkedlist, void* ptr)
{
  struct node* tail = linkedlist->tail;
  struct node* new = (struct node*) malloc(sizeof(struct node));

  if(new == NULL)
  {
    printf("ERROR: Could not allocate size for new node\n");
    return;
  }

  new->ptr = ptr;
  new->next=NULL;
  
  if(linkedlist->head == NULL)
  {
    linkedlist->head = new;
    return;
  }

  if(tail == NULL)
  {
    linkedlist->tail = new;
    return;
  }
  tail->next=new;
  linkedlist->tail=new;
  linkedlist->length++;
}

// free a linkedlist with heap elements
// this function automatically frees the nodes and 
// pointers that each node point to
//
// will cause a segfault if the items in the list are 
// not in the heap
void linkedlist_hfree(struct linkedlist *ptr)
{
  if(ptr == NULL) return;
  struct node* cur = ptr->head;
  struct node* next = NULL;

  while(cur->next != NULL)  {
    next = cur->next;
    free(cur->ptr);
    free(cur);
    cur = next;
  }

  free(ptr);
}

// free a linkedlist 
void linkedlist_free(struct linkedlist *ptr)
{
  if(ptr == NULL) return;
  struct node* cur = ptr->head;
  struct node* next = NULL;

  while(cur->next != NULL)  {
    next = cur->next;
    free(cur);
    cur = next;
  }

  free(ptr);
}

void linkedlist_prepend(struct linkedlist *linkedlist, void *ptr)
{
  struct node * new = (struct node*) malloc(sizeof(struct node));

  if(new == NULL)
  {
    printf("ERROR: Could not allocate size for new node\n");
    return;
  }
  new->ptr=ptr;
  new->next = NULL;

  if(linkedlist->head == NULL)
  {
    linkedlist->head = new;
    return;
  }

  if(linkedlist->tail == NULL)
  {
    linkedlist->tail = linkedlist->head;
    linkedlist->head = new;
    new->next=linkedlist->tail;
    return;
  }

  struct node* prev = linkedlist->head;
  new->next=prev;
  linkedlist->head=new;
  linkedlist->length++;
}

struct node *linkedlist_delete(struct linkedlist *linkedlist, int index)
{
  if(index >= linkedlist->length)
  {
    return NULL;
  }

  if(index < 0)
  {
    return NULL;
  }

  if(linkedlist->length <= 0)
  {
    return NULL;
  }

  struct node *ptr = linkedlist->head;
  for(int i = 0; i < index-1; i++)
  {
    ptr = ptr->next;
  }

  struct node* deleted = ptr->next;
  ptr->next = ptr->next->next;
  linkedlist->length = linkedlist->length-1;

  return deleted;
}

struct node *linkedlist_get(struct linkedlist *linkedlist, int index)
{
  struct node *ptr = linkedlist->head;
  for(int i = 0; i < index; i++)
  {
    ptr = ptr->next;
  }

  return ptr;
}

struct node *linkedlist_pop(struct linkedlist *linkedlist)
{
  struct node *ptr = linkedlist->head;
  for(int i = 0; i < linkedlist->length-1; i++)
  {
    if(ptr->next == NULL)
    {
      break;
    }
    ptr = ptr->next;
  }

  struct node * ret = linkedlist->head;
  linkedlist->head = ptr;
  ptr->next=NULL;

  return ret;
}

