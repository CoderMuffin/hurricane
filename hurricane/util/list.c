#ifndef HC_LIST
#define HC_LIST

#include <hurricane/util/log.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hc_list {
  int length;
  int allocated;
  void **data;
} hc_list;

void hc_list_new(hc_list *l) {
  l->length = 0;
  l->allocated = 1;
  l->data = malloc(sizeof(void *) * l->allocated);
}

void hc_list_add(hc_list *l, void *item) {
  l->length++;
  if (l->allocated <= l->length) {
    l->allocated <<= 1;
    l->data = realloc(l->data, sizeof(void *) * l->allocated);
  }
  l->data[l->length - 1] = item;
}

void *hc_list_get(hc_list *l, int i) {
  if (i >= l->length || i < 0) {
    hc_internal_error("Index %d out of bounds for list of "
                      "length %d",
                      i, l->length);
    fflush(stdout);
  }
  return l->data[i];
}

void hc_list_remove(hc_list *l, int i) {
  if (i >= l->length || i < 0) {
    hc_internal_error("Index %d out of bounds for list of "
                      "length %d",
                      i, l->length);
  }
  memmove(l->data + i, l->data + i + 1, (l->length - i + 1) * sizeof(void *));
  l->length--;
}

#endif
