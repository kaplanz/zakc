// File:        list.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     06 Dec 2022
// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h> // for bool

#include "zakc/types.h" // for usize

// Linked list structure
struct list;

/**
 * Create a new linked list.
 *
 * @return  Pointer to the newly-created linked list, or `NULL` if memory
 *          allocation failed.
 */
struct list *list_new(void);

/**
 * Delete the linked list.
 *
 * @param list  Pointer to the linked list to delete.
 */
void list_drop(struct list *list);

/**
 * Append an item to the end of the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param data  Data of the item to append.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_append(struct list *list, void *data);

/**
 * Prepend an item to the beginning of the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param data  Data of the item to prepend.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_prepend(struct list *list, void *data);

/**
 * Remove the last item from the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      Pointer to the removed element, or `NULL` if the operation was
 *              invalid.
 */
void *list_pop(struct list *list);

/**
 * Remove the first item from the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      Pointer to the removed element, or `NULL` if the operation was
 *              invalid.
 */
void *list_shift(struct list *list);

/**
 * Insert an item at the given index in the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index at which to insert the item.
 * @param data  Data of the item to insert.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_insert(struct list *list, usize index, void *data);

/**
 * Remove the item at the given index from the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index of the item to remove.
 * @return      Pointer to the removed element, or `NULL` if the operation was
 *              invalid.
 */
void *list_remove(struct list *list, usize index);

/**
 * Reverse the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_reverse(struct list *list);

/**
 * Check if the linked list contains the given item.
 *
 * @param list  Pointer to the linked list.
 * @param data  Data of the item to search for.
 * @return      `true` if the item was found, `false` otherwise.
 */
bool list_contains(const struct list *list, void *data);

/**
 * Get the item at the given index in the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index of the item to get.
 * @return      Pointer to the item at the given index, or `NULL` if the index
 *              is out of bounds or the list is `NULL`.
 */
void *list_get(const struct list *list, usize index);

/**
 * Set the item at the given index in the linked list.
 *
 * @param list  Pointer to the linked list.
 * @param index Index of the item to set.
 * @param data  Data of the item to set.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool list_set(struct list *list, usize index, void *data);

/**
 * Get the length of the linked list.
 *
 * @param list  Pointer to the linked list.
 * @return      The number of items in the linked list, or 0 if the list is
 *              `NULL`.
 */
usize list_len(const struct list *list);
