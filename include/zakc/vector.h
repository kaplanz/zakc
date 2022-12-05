// File:        vector.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     05 Dec 2022
// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h> // for bool

#include "zakc/types.h" // for usize

// Vector structure
struct vector;

/**
 * Create a new vector.
 *
 * @return  Pointer to the newly-created vector, or `NULL` if memory
 *          allocation failed.
 */
struct vector *vector_new(void);

/**
 * Delete the vector.
 *
 * @param vec Pointer to the vector to delete.
 */
void vector_drop(struct vector *vec);

/**
 * Append an element to the end of the vector.
 *
 * If the vector has reached its capacity, it will be resized to accommodate
 * the new element.
 *
 * @param vec   Pointer to the vector.
 * @param data  Pointer to the element to append.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool vector_append(struct vector *vec, void *data);

/**
 * Append elements from another vector to the end of this vector.
 *
 * The capacity of this vector will be increased if necessary to store the
 * new elements.
 *
 * @param vec    Pointer to the vector.
 * @param other  Pointer to the vector whose elements will be appended.
 * @return       `true` if the operation was successful, `false` otherwise.
 */
bool vector_extend(struct vector *vec, const struct vector *other);

/**
 * Remove the last element from the vector.
 *
 * The size of the vector will be reduced by one.
 *
 * If the vector is empty, the operation will be considered invalid.
 *
 * @param vec    Pointer to the vector.
 * @return       Pointer to the removed element, or `NULL` if the operation
 *               was invalid.
 */
void *vector_pop(struct vector *vec);

/**
 * Insert an element into the vector at a given index.
 *
 * The element at the given index, and all subsequent elements, will be
 * shifted to the right to make room for the new element.
 *
 * If the index is greater than the current size of the vector, the operation
 * will be considered invalid.
 *
 * If the vector has reached its capacity, it will be resized to accommodate
 * the new element.
 *
 * @param vec    Pointer to the vector.
 * @param index  Index at which to insert the element.
 * @param data   Pointer to the element to insert.
 * @return       `true` if the operation was successful, `false` otherwise.
 */
bool vector_insert(struct vector *vec, usize index, void *data);

/**
 * Remove an element from the vector.
 *
 * The elements following the removed element will be shifted to the left,
 * and the size of the vector will be reduced by one.
 *
 * If the index is out of bounds, the operation will be considered invalid.
 *
 * @param vec    Pointer to the vector.
 * @param index  Index of the element to remove.
 * @return       Pointer to the removed element, or `NULL` if the operation
 *               was invalid.
 */
void *vector_remove(struct vector *vec, usize index);

/**
 * Check if the vector contains a given element.
 *
 * @param vec   Pointer to the vector.
 * @param data  Pointer to the element to search for.
 * @return      `true` if the vector contains the element, `false` otherwise.
 */
bool vector_contains(const struct vector *vec, void *data);

/**
 * Get the element at a given index in the vector.
 *
 * If the index is out of bounds, the operation will be considered invalid.
 *
 * @param vec    Pointer to the vector.
 * @param index  Index of the element to get.
 * @return       Pointer to the element at the given index, or `NULL` if the
 *               operation was invalid.
 */
void *vector_get(const struct vector *vec, usize index);

/**
 * Set the element at a given index in the vector.
 *
 * If the index is greater than or equal to the current size of the vector,
 * the operation will be considered invalid.
 *
 * @param vec    Pointer to the vector.
 * @param index  Index of the element to set.
 * @param data   Pointer to the element to set.
 * @return       `true` if the operation was successful, `false` otherwise.
 */
bool vector_set(struct vector *vec, usize index, void *data);

/**
 * Get the capacity of the vector.
 *
 * The capacity of the vector is the number of elements that the vector
 * can store before it needs to be resized.
 *
 * @param vec   Pointer to the vector.
 * @return      The capacity of the vector, or `0` if the vector is `NULL`.
 */
usize vector_capacity(const struct vector *vec);

/**
 * Get the number of elements in the vector.
 *
 * @param vec   Pointer to the vector.
 * @return      The number of elements in the vector.
 */
usize vector_len(const struct vector *vec);

/**
 * Check if the vector is empty.
 *
 * @param vec   Pointer to the vector.
 * @return      `true` if the vector is empty, `false` otherwise.
 */
bool vector_is_empty(const struct vector *vec);

/**
 * Get a pointer to the array of all elements in the vector.
 *
 * @param vec   Pointer to the vector.
 * @return      Pointer to the array of elements, or `NULL` if the vector is
 *              `NULL`.
 */
void **vector_array(const struct vector *vec);

/**
 * Reserve a given amount of capacity for the vector.
 *
 * If the new capacity is smaller than the current size of the vector, the
 * operation will be considered invalid.
 *
 * @param vec       Pointer to the vector.
 * @param capacity  Amount of capacity to reserve.
 * @return          `true` if the operation was successful, `false` otherwise.
 */
bool vector_reserve(struct vector *vec, usize capacity);

/**
 * Reduce the capacity of the vector to the minimum necessary to store the
 * current number of elements.
 *
 * @param vec   Pointer to the vector.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool vector_shrink_to_fit(struct vector *vec);

/**
 * Resize the vector to a given length.
 *
 * If the length is less than the current length, the vector will be truncated
 * to the new length. If the length is greater than the current length, the
 * vector's capacity will be increased to the new length, and uninitialized
 * elements will be zeroed out.
 *
 * @param vec   Pointer to the vector.
 * @param len   New length of the vector.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool vector_resize(struct vector *vec, usize len);
