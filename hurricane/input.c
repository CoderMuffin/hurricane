#ifndef HC_INPUT
#define HC_INPUT
#include <stdbool.h>
#include <stdlib.h>

typedef struct linked_list {
  void *node;
  struct linked_list *next;
} linked_list;

typedef struct hc_input_key_event {
  unsigned int code;
} hc_input_key_event;

enum hc_input_event_type { HC_INPUT_KEYDOWN, HC_INPUT_KEYUP };

struct hc_internal_input_subscription {
  int id;
  void (*callback)(void *);
  enum hc_input_event_type event_type;
};

linked_list *hc_internal_input_subscribers = NULL;

int hc_input_subscribe(void (*callback)(void *), enum hc_input_event_type evt) {
  static int subscription_count = 0;
  int id = ++subscription_count;
  
  struct hc_internal_input_subscription *sub =
        malloc(sizeof(struct hc_internal_input_subscription));
    sub->callback = callback;
    sub->event_type = evt;
    sub->id = id;
  
  if (hc_internal_input_subscribers == NULL) {
    hc_internal_input_subscribers = malloc(sizeof(linked_list));
    hc_internal_input_subscribers->next = NULL;
    hc_internal_input_subscribers->node = sub;
  } else {
    linked_list *old_first = hc_internal_input_subscribers;
    hc_internal_input_subscribers = malloc(sizeof(linked_list));
    hc_internal_input_subscribers->next = old_first;
    hc_internal_input_subscribers->node = sub;
  }
  return id;
}

void hc_input_unsubscribe(int id) { exit(1); }

void hc_internal_input_dispatch_event(void *evt,
                                      enum hc_input_event_type type) {
  linked_list *node = hc_internal_input_subscribers;
  //printf("starting dispatch\n");
  while (node) {
    //printf("testing dispatch\n");
    struct hc_internal_input_subscription *sub = node->node;
    if (sub->event_type == type) {
      //printf("found dispatch\n");
      sub->callback(evt);
    }
    node = node->next;
  }
}

void hc_internal_keydown(unsigned int key) {
  hc_input_key_event event = {.code = key};
  hc_internal_input_dispatch_event(&event, HC_INPUT_KEYDOWN);
}

void hc_internal_keyup(unsigned int key) {
  hc_input_key_event event = {.code = key};
  hc_internal_input_dispatch_event(&event, HC_INPUT_KEYUP);
}

#endif
