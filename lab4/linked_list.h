//
//  linked_list.h
//  LinkedList
//
//  Created by Кирилл on 17.02.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef linked_list_h
#define linked_list_h

#include <stdbool.h>

typedef struct Node {
    struct Node *next;
    void* value;
} Node;

typedef struct List {
    struct Node *head;
} List;

#ifdef __cplusplus
extern "C" {
#endif

/** Создает новый список. */
List* listNew();
    
/** Возвращает элемент списка по запрашиваемой позиции.
 * Если запрашиваемая позиция больше размера списка, возвращает NULL. */
void* listGetItem(List* list, size_t position);
    
/** Добавляет элемент в начало спика. 
 *  Добавление NULL запрещено. */
int listPush(List* list, void* item);
    
/** Добавляет элемент в конец списка. 
 * Добавление NULL запрещено. */
int listPushBack(List* list, void* item);
    
/** Удаляет элемент из начала списка и возвращает его значение.
 * Если список был пуст, возвращает NULL. */
void* listPop(List* list);
    
/** Удаляет элемент из конца списка и возвращает его значение.
 * Если список был пуст, возвращает NULL. */
void* listPopBack(List* list);

/** Вставляет элемента на данную позицию.
 * Если требуемой позиции в списке нет, добавляет в конец.
 * Добавление NULL запрещено. */
int listInsertItem(List* list, size_t position, void* value);

/** Удаляет элемент на данной позиции и возвращает его значение.
 * Если на данной позиции нет элемента, возвщает NULL. */
void* listRemoveItem(List* list, size_t position);

/** Заменяет элемент на требуемой позиции и возвращает предыдущее значение.
 * Если заменяемой позиции нет, возвращает NULL. */
void* listReplaceItem(List* list, size_t position, void* value);

/** Для каждого элемента списка вызывает функцию cycle. */
void listForeach(List* list, void(cycle)(void*));

/** Для каждого элемента списка вызывает функцию compare с аргументами для сравнения.
 * Если compare возвращает true, функция возвращает элемент на котором compare вернула true.
 * Если compare ни разу не вернула true, функция возвращает NULL. */
void* listFindFirst(List* list, const void* request, bool(compare)(const void*, const void*));
    
/** Возвращает длину списка. */
size_t listSize(List* list);
    
/** Очищает список.
 * Если freeValues - истина, применяет free к элементам списка. 
 * Используйте осторожно, чтобы не использовать освобожденную память. */
void listFree(List* list, bool freeValues);

#ifdef __cplusplus
}
#endif

#endif
