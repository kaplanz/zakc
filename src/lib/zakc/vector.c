// File:        vector.c
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     05 Dec 2022
// SPDX-License-Identifier: MIT

#include "zakc/vector.h"

#include <stdlib.h> // for free, {m,re}alloc
#include <string.h> // for memset

#include "zakc/types.h" // for usize

// Vector structure
struct vector {
    // Array of elements
    void **data;
    // Capacity of the array
    usize capacity;
    // Length of the array
    usize len;
};

/**
 * Create a new vector.
 *
 * @return  Pointer to the newly-created vector, or `NULL` if memory
 *          allocation failed.
 */
struct vector *vector_new(void) {
    // Allocate memory for the vector
    struct vector *vec = malloc(sizeof(struct vector));
    if (!vec)
        // Return `NULL` if memory allocation failed
        return NULL;

    // Initialize the vector to have no capacity and no length
    *vec = (struct vector){
        .capacity = 0,
        .len = 0,
        .data = NULL,
    };

    // Return the newly-created vector
    return vec;
}

/**
 * Delete the vector.
 *
 * @param vec Pointer to the vector to delete.
 */
void vector_drop(struct vector *vec) {
    if (!vec)
        // Return early if the vector is `NULL`
        return;
    // Free the array of elements
    free(vec->data);
    // Free the vector
    free(vec);
}

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
bool vector_append(struct vector *vec, void *data) {
    if (!vec)
        // Return `false` if the vector is `NULL`
        return false;
    if (vec->len + 1 > vec->capacity) {
        // Reserve more capacity if necessary
        if (!vector_reserve(vec, vec->capacity == 0 ? 1 : vec->capacity * 2))
            // Return `false` if unable to reserve more capacity
            return false;
    }
    // Add the new element to the vector
    vec->data[vec->len++] = data;
    // Return `true` to indicate success
    return true;
}

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
bool vector_extend(struct vector *vec, const struct vector *other) {
    if (!vec || !vec->data || !other || !other->data)
        // Return `false` if either vector or data array is `NULL`
        return false;
    // Increase the capacity of this vector if necessary
    if (vec->len + other->len > vec->capacity)
        vector_reserve(vec, vec->len + other->len);
    // Append the elements from the other vector
    for (usize i = 0; i < other->len; i++)
        vec->data[vec->len + i] = other->data[i];
    // Increase the size of this vector by the size of the other vector
    vec->len += other->len;
    // Return `true` to indicate success
    return true;
}

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
void *vector_pop(struct vector *vec) {
    if (!vec || vec->len == 0)
        // Return `NULL` if the vector is `NULL` or empty
        return NULL;
    // Save a pointer to the removed element
    void *removed = vec->data[vec->len - 1];
    // Reduce the size of the vector by one
    vec->len--;
    // Resize the vector if necessary
    if (vec->len < vec->capacity / 2 && vec->capacity > 1)
        vector_resize(vec, vec->capacity / 2);
    // Return the removed element
    return removed;
}

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
bool vector_insert(struct vector *vec, usize index, void *data) {
    if (!vec || index > vec->len)
        // Return `false` if the vector is `NULL` or the index is out of bounds
        return false;
    if (vec->len == vec->capacity) {
        // Reserve more capacity if necessary
        if (!vector_reserve(vec, vec->capacity == 0 ? 1 : vec->capacity * 2))
            // Return `false` if unable to reserve more capacity
            return false;
    }
    // Shift elements to the right to make room for the new element
    for (usize i = vec->len; i > index; i--)
        vec->data[i] = vec->data[i - 1];
    // Insert the new element
    vec->data[index] = data;
    // Increase the size of the vector by one
    vec->len++;
    // Return `true` to indicate success
    return true;
}

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
void *vector_remove(struct vector *vec, usize index) {
    if (!vec || !vec->data || vec->len == 0)
        // Return `NULL` if the vector or data array is `NULL` or the vector is
        // empty
        return NULL;
    // Save a pointer to the removed element
    void *removed = vec->data[index];
    // Shift elements to the left to fill the gap left by the removed element
    for (usize i = index; i < vec->len - 1; i++)
        vec->data[i] = vec->data[i + 1];
    // Reduce the size of the vector by one
    vec->len--;
    // Resize the vector if necessary
    if (vec->len < vec->capacity / 2 && vec->capacity > 1)
        vector_resize(vec, vec->capacity / 2);
    // Return the removed element
    return removed;
}

/**
 * Check if the vector contains a given element.
 *
 * @param vec   Pointer to the vector.
 * @param data  Pointer to the element to search for.
 * @return      `true` if the vector contains the element, `false` otherwise.
 */
bool vector_contains(const struct vector *vec, void *data) {
    if (!vec)
        // Return `false` if the vector is `NULL`
        return false;
    // Search for the element
    for (usize i = 0; i < vec->len; i++)
        if (vec->data[i] == data)
            // Return `true` if the element was found
            return true;
    // Return `false` if the element was not found
    return false;
}

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
void *vector_get(const struct vector *vec, usize index) {
    if (!vec || !vec->data || index >= vec->len)
        // Return `NULL` if the vector or data array is `NULL` or the index is
        // out of bounds
        return NULL;
    // Return the element at the given index
    return vec->data[index];
}

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
bool vector_set(struct vector *vec, usize index, void *data) {
    if (!vec)
        // Return `false` if the vector is `NULL`
        return false;
    if (index >= vec->len)
        // Return `false` if the index is out of bounds
        return false;
    // Set the element
    vec->data[index] = data;
    // Return `true` to indicate success
    return true;
}

/**
 * Get the capacity of the vector.
 *
 * The capacity of the vector is the number of elements that the vector
 * can store before it needs to be resized.
 *
 * @param vec   Pointer to the vector.
 * @return      The capacity of the vector, or `0` if the vector is `NULL`.
 */
usize vector_capacity(const struct vector *vec) {
    if (!vec)
        // Return `0` if the vector is `NULL`
        return 0;
    // Return the capacity of the vector
    return vec->capacity;
}

/**
 * Get the number of elements in the vector.
 *
 * @param vec   Pointer to the vector.
 * @return      The number of elements in the vector.
 */
usize vector_len(const struct vector *vec) {
    if (!vec)
        // Return zero if the vector is `NULL`
        return 0;
    // Return the number of elements in the vector
    return vec->len;
}

/**
 * Check if the vector is empty.
 *
 * @param vec   Pointer to the vector.
 * @return      `true` if the vector is empty, `false` otherwise.
 */
bool vector_is_empty(const struct vector *vec) {
    if (!vec)
        // Return `true` if the vector is `NULL`
        return true;
    // Return `true` if the vector has zero elements
    return vec->len == 0;
}

/**
 * Get a pointer to the array of all elements in the vector.
 *
 * @param vec   Pointer to the vector.
 * @return      Const shared pointer to the array of elements, or `NULL` if the
 * vector
 *              is `NULL`.
 */
void **vector_array(const struct vector *vec) {
    if (!vec)
        // Return `NULL` if the vector is `NULL`
        return NULL;
    // Return the pointer to the array of elements
    return vec->data;
}

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
bool vector_reserve(struct vector *vec, usize capacity) {
    if (!vec)
        // Return `false` if the vector is `NULL`
        return false;
    if (capacity < vec->len)
        // Return `false` if the new capacity is smaller than the current size
        return false;
    if (capacity == vec->capacity)
        // Return `true` if the new capacity is the same as the current capacity
        return true;
    // Resize the array of elements
    void *data = realloc(vec->data, capacity * sizeof(void *));
    if (!data)
        // Return `false` if memory allocation failed
        return false;
    // Update the vector with the new capacity and array of elements
    vec->capacity = capacity;
    vec->data = data;
    // Return `true` to indicate success
    return true;
}

/**
 * Reduce the capacity of the vector to the minimum necessary to store the
 * current number of elements.
 *
 * @param vec   Pointer to the vector.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool vector_shrink_to_fit(struct vector *vec) {
    if (!vec || vec->len == 0)
        // Return `false` if the vector is `NULL` or empty
        return false;
    // Resize the vector to its current size
    return vector_resize(vec, vec->len);
}

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
bool vector_resize(struct vector *vec, usize len) {
    if (!vec)
        // Return `false` if the vector is `NULL`
        return false;
    // Increase the capacity of the vector if necessary
    if (len > vec->capacity && !vector_reserve(vec, len))
        // Return `false` if unable to increase the capacity
        return false;
    // Zero out uninitialized elements if the size is being increased
    if (len > vec->len)
        memset(&vec->data[vec->len], 0, (len - vec->len) * sizeof(void *));
    // Set the new length of the vector
    vec->len = len;
    // Return `true` to indicate success
    return true;
}
