
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sync_common.h"
#include "sync_list.h"

typedef struct list_ext {
  sync_node_t* head;
  pthread_mutex_t mutex;
} list_ext_t;

sync_list_t* list_create() {
  int error;

  list_ext_t* list = (list_ext_t*)malloc(sizeof(list_ext_t));
  if (list == NULL)
    return NULL;

  if ((error = pthread_mutex_init(&list->mutex, NULL)) != 0) {
    free(list);
    errno = error;
    return NULL;
  }

  list->head = NULL;
  return (sync_list_t*)list;
}

int list_push_forward(sync_list_t* slist, char* item) {
  list_ext_t* list = (list_ext_t*)slist;
  int error;

  if ((error = pthread_mutex_lock(&list->mutex)) != 0)
    return error;

  sync_node_t* node = (sync_node_t*)malloc(sizeof(sync_node_t));
  if (node == NULL) {
    error = errno;
    pthread_mutex_unlock(&list->mutex);
    return error;
  }

  node->value = item;
  node->next = list->head;
  list->head = node;

  return pthread_mutex_unlock(&list->mutex);
}

int list_sort(sync_list_t* slist) {
  list_ext_t* list = (list_ext_t*)slist;
  sync_node_t* prev = NULL;
  sync_node_t *curr, *target, *prev_target, *temp;
  int error;

  if ((error = pthread_mutex_lock(&list->mutex)) != 0)
    return error;

  curr = list->head;
  while (curr) {
    prev_target = NULL;
    target = curr->next;
    while (target) {
      if (strcmp(curr->value, target->value) > 0) {
        printf("\nSwapping \"%s\" and \"%s\"\n", curr->value, target->value);
        if (prev)
          prev->next = target;
        if (prev_target)
          prev_target->next = curr;
        temp = curr->next;
        curr->next = target->next;
        target->next = temp;
        // TODO - FIX
        temp = curr;
        curr = target;
        target = temp;
      }

      prev_target = target;
      target = target->next;
    }

    prev = curr;
    curr = curr->next;
  }

  return pthread_mutex_unlock(&list->mutex);
}

int list_print(sync_list_t* slist) {
  list_ext_t* list = (list_ext_t*)slist;
  sync_node_t* node;
  int error;

  if ((error = pthread_mutex_lock(&list->mutex)) != 0)
    return error;

  node = list->head;
  while (node) {
    printf("%s\n", node->value);
    node = node->next;
  }

  return pthread_mutex_unlock(&list->mutex);
}

void list_destroy(sync_list_t* slist) {
  list_ext_t* list = (list_ext_t*)slist;
  sync_node_t* node = list->head;
  sync_node_t* temp;

  while (node) {
    free(node->value);
    temp = node->next;
    free(node);
    node = temp;
  }

  pthread_mutex_destroy(&list->mutex);
  free(list);
}
