// File:        list.c
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     06 Dec 2022
// SPDX-License-Identifier: NONE

#include "zakc/list.h"

#include <stdlib.h> // for free, malloc

#include "zakc/types.h" // for usize

// Linked list structure
struct list {
    // Pointer to the first node in the list
    struct node *head;
    // Pointer to the last node in the list
    struct node *tail;
    // Number of items in the list
    usize len;
};

// Linked list node structure
struct node {
    // Pointer to the previous node in the list
    struct node *prev;
    // Pointer to the next node in the list
    struct node *next;
    // Pointer to the data stored in the node
    void *data;
};

/**
 * Create a new linked list.
 *
 * @return  Pointer to the newly-created linked list, or `NULL` if memory
 *          allocation failed.
 */
struct list *list_new(void) {
    // Allocate memory for the linked list
    struct list *list = malloc(sizeof(struct list));
    if (!list)
        // Return `NULL` if memory allocation failed
        return NULL;

    // Initialize the linked list
    *list = (struct list){
        .head = NULL,
        .tail = NULL,
        .len = 0,
    };

    return list;
}

/**
 * Delete the linked list.
 *
 * @param list  Pointer to the linked list to delete.
 */
void list_drop(struct list *list) {
    if (!list)
        // Return early if the list is `NULL`
        return;

    // Iterate over the linked list and free each node
    struct node *curr = list->head;
    while (curr) {
        struct node *next = curr->next;
        free(curr);
        curr = next;
    }

    // Free the linked list
    free(list);
}

/**
 * Append an item to the end of the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param data  Data of the item to append.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_append(struct list *list, void *data) {
    if (!list)
        // Return `false` if the list is `NULL`
        return false;

    // Create a new node for the data
    struct node *node = malloc(sizeof(struct node));
    if (!node)
        // Return `false` if unable to allocate another node
        return false;
    *node = (struct node){
        .prev = list->tail,
        .next = NULL,
        .data = data,
    };

    // Append the node to the end of the linked list
    if (list->tail)
        list->tail->next = node;
    list->tail = node;

    // Update the head pointer if necessary
    if (!list->head)
        list->head = node;

    // Update the length of the linked list
    list->len++;

    return true;
}

/**
 * Prepend an item to the beginning of the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param data  Data of the item to prepend.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_prepend(struct list *list, void *data) {
    if (!list)
        // Return `false` if the list is `NULL`
        return false;

    // Create a new node for the data
    struct node *node = malloc(sizeof(struct node));
    if (!node)
        // Return `false` if unable to allocate another node
        return false;
    *node = (struct node){
        .prev = NULL,
        .next = list->head,
        .data = data,
    };

    // Update the previous node's next pointer and the linked list's head
    // pointer
    if (list->head)
        list->head->prev = node;
    list->head = node;

    // Update the tail pointer if necessary
    if (!list->tail)
        list->tail = node;

    // Update the length of the linked list
    list->len++;

    return true;
}

/**
 * Remove the last item from the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      Pointer to the removed element, or `NULL` if the operation was
 *              invalid.
 */
void *list_pop(struct list *list) {
    if (!list || !list->tail)
        // Return `NULL` if the list is `NULL` or empty
        return NULL;

    // Get the last node and its data
    struct node *last = list->tail;
    void *data = last->data;

    // Update the linked list's tail pointer
    list->tail = last->prev;
    if (list->tail)
        list->tail->next = NULL;
    else
        list->head = NULL;

    // Free the node and update the length of the linked list
    free(last);
    list->len--;

    return data;
}

/**
 * Remove the first item from the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      Pointer to the removed element, or `NULL` if the operation was
 *              invalid.
 */
void *list_shift(struct list *list) {
    if (!list || !list->head)
        // Return `NULL` if the list is `NULL` or empty
        return NULL;

    // Get the first node and its data
    struct node *first = list->head;
    void *data = first->data;

    // Update the linked list's head pointer
    list->head = first->next;
    if (list->head)
        list->head->prev = NULL;
    else
        list->tail = NULL;

    // Free the node and update the length of the linked list
    free(first);
    list->len--;

    return data;
}

/**
 * Insert an item at the given index in the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index at which to insert the item.
 * @param data  Data of the item to insert.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_insert(struct list *list, usize index, void *data) {
    if (!list || index > list->len)
        // Return `false` if the list is `NULL` or the index is out of bounds
        return false;

    // Create a new node for the data
    struct node *node = malloc(sizeof(struct node));
    if (!node)
        // Return `false` if unable to allocate another node
        return false;
    *node = (struct node){
        .prev = NULL,
        .next = NULL,
        .data = data,
    };

    // Insert the node at the given index in the linked list
    if (index == 0) {
        // Prepend the node to the beginning of the list
        node->next = list->head;
        if (list->head)
            list->head->prev = node;
        list->head = node;
    } else if (index == list->len) {
        // Append the node to the end of the list
        node->prev = list->tail;
        if (list->tail)
            list->tail->next = node;
        list->tail = node;
    } else {
        // Find the node at the given index
        struct node *curr = list->head;
        for (usize i = 0; i < index; i++)
            curr = curr->next;

        // Insert the new node between the previous and next nodes
        node->prev = curr->prev;
        node->next = curr;
        curr->prev->next = node;
        curr->prev = node;
    }

    // Update the length of the linked list
    list->len++;

    return true;
}

/**
 * Remove the item at the given index from the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index of the item to remove.
 * @return      Pointer to the removed element, or `NULL` if the operation was
 *              invalid.
 */
void *list_remove(struct list *list, usize index) {
    if (!list || index >= list->len)
        // Return `NULL` if the list is `NULL` or the index is out of bounds
        return NULL;

    // Find the node at the given index
    struct node *node = list->head;
    for (usize i = 0; i < index; i++)
        node = node->next;

    // Remove the node from the linked list
    void *data = node->data;
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    // Free the node and update the length of the linked list
    free(node);
    list->len--;

    return data;
}

/**
 * Reverse the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_reverse(struct list *list) {
    if (!list || !list->head)
        // Return `false` if the list is `NULL` or empty
        return false;

    // Iterate over the linked list and reverse the `prev` and `next` pointers
    // of each node
    struct node *tmp, *node = list->head;
    while (node) {
        tmp = node->prev;
        node->prev = node->next;
        node->next = tmp;
        node = node->prev;
    }

    // Update the head and tail pointers of the linked list
    tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;

    return true;
}

/**
 * Check if the linked list contains the given item.
 *
 * @param list  Pointer to the linked list.
 * @param data  Data of the item to search for.
 * @return      `true` if the item was found, `false` otherwise.
 */
bool list_contains(const struct list *list, void *data) {
    if (!list)
        // Return `false` if the list is `NULL`
        return false;

    // Iterate over the linked list and compare the data of each node to the
    // given item
    struct node *node = list->head;
    while (node) {
        if (node->data == data)
            return true;
        node = node->next;
    }

    // Return `false` if the item was not found in the linked list
    return false;
}

/**
 * Get the item at the given index in the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index of the item to get.
 * @return      Pointer to the item at the given index, or `NULL` if the index
 *              is out of bounds or the list is `NULL`.
 */
void *list_get(const struct list *list, usize index) {
    if (!list || !list->head || index >= list->len)
        // Return `NULL` if the list is `NULL` or empty, or the index is out of
        // bounds
        return NULL;

    // Iterate over the linked list to find the node at the given index
    struct node *node = list->head;
    for (usize i = 0; i < index; i++)
        node = node->next;

    // Return the data stored in the node
    return node->data;
}

/**
 * Set the item at the given index in the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index of the item to set.
 * @param data  Data of the item to set.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_set(struct list *list, usize index, void *data) {
    if (!list || !list->head || index >= list->len)
        // Return `NULL` if the list is `NULL` or empty, or the index is out of
        // bounds
        return false;

    // Iterate over the linked list to find the node at the given index
    struct node *node = list->head;
    for (usize i = 0; i < index; i++)
        node = node->next;

    // Update the data stored in the node
    node->data = data;

    return true;
}

/**
 * Get the length of the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      The number of items in the linked list, or 0 if the list is
 *              `NULL`.
 */
usize list_len(const struct list *list) {
    if (!list)
        // Return 0 if the list is `NULL`
        return 0;

    // Return the length of the linked list
    return list->len;
}
