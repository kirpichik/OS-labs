//
//  linked_list.c
//  LinkedList
//
//  Created by Кирилл on 17.02.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include "linked_list.h"
#include <stdlib.h>

#define CHECK_NULL(a) \
  if (!a)             \
  return

/** Создает новый список. */
List* listNew() {
  List* list = (List*)malloc(sizeof(List));
  if (!list)
    return NULL;
  list->head = NULL;
  return list;
}

/** Возвращает элемент списка по запрашиваемой позиции.
 * Если запрашиваемая позиция больше размера списка, возвращает NULL. */
void* listGetItem(List* list, size_t position) {
  CHECK_NULL(list) NULL;
  Node* node = list->head;
  while (node && position) {
    node = node->next;
    position--;
  }

  // Элемента нет в списке
  if (position || !node)
    return NULL;

  return node->value;
}

/** Добавляет элемент в начало спика.
 * Добавление NULL запрещено. */
int listPush(List* list, void* item) {
  CHECK_NULL(list) - 2;
  CHECK_NULL(item) - 2;
  Node* node = (Node*)malloc(sizeof(Node));
  if (!node)
    return -1;
  node->value = item;
  node->next = list->head;
  list->head = node;
  return 0;
}

/** Добавляет элемент в конец списка.
 * Добавление NULL запрещено. */
int listPushBack(List* list, void* item) {
  CHECK_NULL(list) - 2;
  CHECK_NULL(item) - 2;
  Node* new_node = (Node*)malloc(sizeof(Node));
  if (!new_node)
    return -1;
  if (!list->head) {
    list->head = new_node;
    new_node->next = NULL;
  } else {
    Node* node = list->head;
    while (node->next)
      node = node->next;
    new_node->next = node->next;
    node->next = new_node;
  }
  new_node->value = item;
  return 0;
}

/** Удаляет элемент из начала списка и возвращает его значение.
 * Если список был пуст, возвращает NULL. */
void* listPop(List* list) {
  CHECK_NULL(list) NULL;
  CHECK_NULL(list->head) NULL;

  void* value = list->head->value;
  Node* node = list->head;
  list->head = list->head->next;
  free(node);
  return value;
}

/** Удаляет элемент из конца списка и возвращает его значение.
 * Если список был пуст, возвращает NULL. */
void* listPopBack(List* list) {
  CHECK_NULL(list) NULL;
  Node* node = list->head;
  CHECK_NULL(node) listPop(list);
  CHECK_NULL(node->next) listPop(list);

  Node* before = NULL;
  while (node->next) {
    before = node;
    node = node->next;
  }
  void* value = node->value;
  before->next = NULL;
  free(node);
  return value;
}

/** Вставляет элемента на данную позицию.
 * Если требуемой позиции в списке нет, добавляет в конец.
 * Добавление NULL запрещено. */
int listInsertItem(List* list, size_t position, void* value) {
  CHECK_NULL(list) - 2;
  CHECK_NULL(value) - 2;
  if (!position || !list->head)
    return listPush(list, value);

  position--;
  Node* node = list->head;
  while (node->next && position-- > 0)
    node = node->next;

  Node* new_node = (Node*)malloc(sizeof(Node));
  if (!new_node)
    return -1;
  new_node->next = node->next;
  new_node->value = value;
  node->next = new_node;
  return 0;
}

/** Удаляет элемент на данной позиции и возвращает его значение.
 * Если на данной позиции нет элемента, возвщает NULL. */
void* listRemoveItem(List* list, size_t position) {
  CHECK_NULL(list) NULL;
  CHECK_NULL(list->head) NULL;

  if (!position)
    return listPop(list);

  position--;
  Node* node = list->head;
  while (node->next && position-- > 0)
    node = node->next;

  Node* remove = node->next;
  if (!remove)
    return NULL;

  void* value = remove->value;
  node->next = remove->next;
  free(remove);

  return value;
}

/** Заменяет элемент на требуемой позиции и возвращает предыдущее значение.
 * Если заменяемой позиции нет, возвращает NULL. */
void* listReplaceItem(List* list, size_t position, void* value) {
  CHECK_NULL(list) NULL;
  CHECK_NULL(list->head) NULL;

  if (!position) {
    void* old_value = list->head->value;
    list->head->value = value;

    return old_value;
  }

  position--;
  Node* node = list->head;
  while (node->next && position-- > 0)
    node = node->next;

  Node* replace = node->next;
  if (!replace)
    return NULL;

  void* old_value = replace->value;
  replace->value = value;

  return old_value;
}

/** Для каждого элемента списка вызывает функцию cycle. */
void listForeach(List* list, void(cycle)(void*)) {
  CHECK_NULL(list);

  Node* node = list->head;
  while (node) {
    cycle(node->value);
    node = node->next;
  }
}

/** Для каждого элемента списка вызывает функцию compare с аргументами для
 * сравнения. Если compare возвращает true, функция возвращает указатель на
 * элемент на котором compare вернула true. Если compare ни разу не вернула
 * true, функция возвращает NULL. */
void* listFindFirst(List* list,
                    const void* request,
                    bool(compare)(const void*, const void*)) {
  CHECK_NULL(list) NULL;
  CHECK_NULL(request) NULL;

  Node* node = list->head;
  while (node) {
    if (compare(request, node->value))
      return node->value;
    node = node->next;
  }

  return NULL;
}

/** Возвращает длину списка. */
size_t listSize(List* list) {
  CHECK_NULL(list) 0;
  Node* item = list->head;
  size_t size = 0;
  while (item) {
    item = item->next;
    size++;
  }
  return size;
}

/** Очищает список.
 * Если freeValues - истина, применяет free к элементам списка.
 * Используйте осторожно, чтобы не использовать освобожденную память. */
void listFree(List* list, bool freeValues) {
  CHECK_NULL(list);
  while (list->head) {
    void* value = listPop(list);
    if (freeValues)
      free(value);
  }
}

#undef CHECK_NULL
