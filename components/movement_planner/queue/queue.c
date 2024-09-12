#include "queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

queue * new_queue()
{
  queue * ptr = (queue *) malloc(sizeof(queue));
  
  if(ptr == NULL)
  {
    printf("ERROR: could not assign a pointer for queue.\n");
  }

  ptr->length = 0;
  ptr->head = NULL;
  ptr->tail = NULL;
  return ptr;
}

void queue_enqueue(queue *queue, void* ptr)
{
  struct node *node = (struct node*) malloc(sizeof(struct node));

  node->next=NULL;
  node->ptr = ptr;

  if(queue->length == 0)
  {
    queue->head = node;
    queue->tail = NULL;
    goto out;
  }

  if(queue->length == 1)
  {
    queue->tail = node;
    queue->head->next = node;
    goto out;
  }

  queue->tail->next = node;
  queue->tail = node;
out:
  queue->length++;
}

void *queue_dequeue(queue *queue)
{
  if(queue->length == 0 || queue->head == NULL)
  {
    printf("No items in queue.\n");
    return NULL;
  }

  void *ptr = queue->head->ptr;
  struct node* node_to_free = queue->head;
  queue->head = queue->head->next;
  
  queue->length--;

  if(queue->length == 0)
  {
    queue->tail = NULL;
  }

  free(node_to_free);

  return ptr;
}

void queue_delete_first(queue *queue)
{
  if(queue->head == NULL)
  {
    return;
  }

  struct node *node = queue->head; 
  queue->head=node->next;
  free(node);
}

struct node * queue_node_dequeue(queue *queue)
{
  if(queue->length == 0 || queue->head == NULL)
  {
    printf("No items in queue.\n");
    return NULL;
  }

  struct node *ptr = queue->head;
  struct node* node_to_free = queue->head;
  queue->head = queue->head->next;

  queue->length--;

  if(queue->length == 0)
  {
    queue->tail = NULL;
  }

  free(node_to_free);

  return ptr;

}


// free a queue with heap elements
// this function automatically frees the nodes and 
// pointers that each node point to
//
// will cause a segfault if the items in the queue are 
// not in the heap
void queue_hfree(queue *queue)
{
  while(queue->length > 0)
  {
    struct node* cur = queue_node_dequeue(queue);
    free(cur->ptr);
    free(cur);
    queue->length--;
  }
}

// free a queue
void queue_free(queue *queue)
{
  while(queue->length > 0)
  {
    struct node* cur = queue_node_dequeue(queue);
    free(cur);
    queue->length--;
  }
}




