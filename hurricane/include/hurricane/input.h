#ifndef HC_INPUT_H
#define HC_INPUT_H

#include <stdbool.h>

typedef struct linked_list {
  void *node;
  struct linked_list *next;
} linked_list;

typedef struct hc_input_key_event {
  unsigned int code;
} hc_input_key_event;

enum hc_input_event_type { HC_INPUT_KEYDOWN, HC_INPUT_KEYUP };

int hc_input_subscribe(void (*callback)(void *), enum hc_input_event_type evt);
void hc_input_unsubscribe(int id);
void hc_internal_keydown(unsigned int key);
void hc_internal_keyup(unsigned int key);

#endif
