#include "darr.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define SYS_32BIT_PTR_SIZE 4

struct darr *darr_new(uint32_t capacity)
{
  struct darr *arr = (struct darr *)malloc(sizeof(struct darr));
  arr->capacity = capacity;
  arr->initial_capacity = capacity;
  arr->length = 0;
  arr->items = calloc(capacity, sizeof(void *));

  return arr;
}

// free a dynamic array
void darr_free(struct darr *ptr)
{
  // for (int i = 0; i < ptr->length; i++)
  //   free(ptr->items[i]);

  free(ptr);
}

void darr_hfree(struct darr *ptr)
{
  for (int i = 0; i < ptr->length; i++)
    free(ptr->items[i]);

  free(ptr);
}

void resize_arr(struct darr *ptr)
{
  void **new_items = realloc(ptr->items, (ptr->capacity + ptr->initial_capacity) * sizeof(void *));

  if (new_items == NULL)
  {
    exit(EXIT_FAILURE);
  }
  ptr->items = new_items;
  ptr->capacity += ptr->initial_capacity;
}

void nresize_arr(struct darr *ptr, uint32_t capacity)
{
  // Free memory for items that will be removed
  for (int i = capacity; i < ptr->length; i++)
  {
    free(ptr->items[i]);
  }

  void **new_items = realloc(ptr->items, capacity * sizeof(void *));

  if (new_items == NULL)
  {
    exit(EXIT_FAILURE);
  }
  ptr->items = new_items;
  ptr->capacity = capacity;
}

int darr_add(struct darr *arr, void *ptr)
{
  if (arr->capacity <= arr->length)
  {
    resize_arr(arr);
  }

  arr->items[arr->length] = ptr;
  arr->length++;
  return arr->length - 1;
}

void *darr_get(struct darr *arr, int index)
{
  return arr->items[index];
}
// linear search for now
int darr_find(struct darr *arr, void *a)
{
  for (int i = 0; i < arr->length; i++)
  {
    if (a == arr->items[i])
    {
      return i;
    }
  }

  return -1;
}

// returns 0 if deleted correctly, -1 if not, and 1 if the array was resized
int darr_delete(struct darr *arr, int index)
{

  if (arr->length == 0)
    return -1;

  if (darr_get(arr, index) == NULL)
  {
    return -1;
  }

  arr->items[index] = NULL;

  for (int i = index; i < arr->length - 1; i++)
  {
    arr->items[i] = arr->items[i + 1];
  }

  // Shrink the array if the capacity is 2 the initial capacity above length
  if ((arr->length + 2 * arr->initial_capacity) < arr->capacity)
  {
    nresize_arr(arr, arr->capacity - arr->initial_capacity);
    return 1;
  }

  arr->length--;
  return 0;
}
