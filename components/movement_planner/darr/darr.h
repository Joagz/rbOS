#ifndef DARR_H
#define DARR_H 

#include <stdint.h>

struct darr {
  void** items;
  uint32_t length;
  uint32_t capacity;
  uint32_t initial_capacity;
};

struct darr* darr_new(uint32_t capacity);
void darr_free(struct darr *ptr);
void darr_hfree(struct darr *ptr);
int darr_add(struct darr *arr, void*ptr);
void * darr_get(struct darr* arr, int index);
int darr_delete(struct darr* arr, int index);
int darr_find(struct darr* arr, void *ptr);
#endif // !DARR_H
