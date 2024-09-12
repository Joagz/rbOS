#ifndef QUEUE_H
#define QUEUE_H

#include "linkedlist.h"
typedef struct linkedlist queue;

queue * new_queue();
void queue_enqueue(queue *queue, void* ptr);
void *queue_dequeue(queue *queue);
void queue_delete_first(queue *queue);
void queue_hfree(queue *queue);
void queue_free(queue *queue);
#endif // !QUEUE_H
