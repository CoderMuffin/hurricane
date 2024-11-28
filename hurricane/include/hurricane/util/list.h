#ifndef HC_LIST_H
#define HC_LIST_H

typedef struct hc_list {
  int length;
  int allocated;
  void **data;
} hc_list;

void hc_list_new(hc_list *l);
void hc_list_add(hc_list *l, void *item);
void *hc_list_get(hc_list *l, int i);
void hc_list_remove(hc_list *l, int i);

#endif
