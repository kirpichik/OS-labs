
#ifndef SYNC_LIST_H
#define SYNC_LIST_H

typedef struct sync_node {
  char* value;
  struct sync_node* next;
} sync_node_t;

typedef struct sync_list {
  sync_node_t* head;
} sync_list_t;

sync_list_t* list_create();

int list_push_forward(sync_list_t* list, char* item);

int list_sort(sync_list_t* list);

int list_print(sync_list_t* list);

void list_destroy(sync_list_t* list);

#endif
