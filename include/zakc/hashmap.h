// File:        hashmap.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     05 Dec 2022
// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h> // for bool

#include "zakc/types.h" // for 64, usize

// Hash map structure
struct hashmap;

/*
 * Hash Functions
 */
// C-strings
u64 str_hash(const void *key);
bool str_cmp(const void *left, const void *right);
// Raw bytes
u64 bytes_hash(const void *key, usize len);
bool bytes_cmp(const void *left, const void *right, usize len);

/**
 * Create a new hash map.
 *
 * @param hash  Hash function for keys.
 * @param cmp   Comparison function for keys.
 * @return      Pointer to the newly-created hash map, or `NULL` if memory
 *              allocation failed.
 */
struct hashmap *hashmap_new(
    u64 (*hash)(const void *key),
    bool (*cmp)(const void *left, const void *right)
);

/**
 * Delete the hash map.
 *
 * @param map  Pointer to the hash map to delete.
 */
void hashmap_drop(struct hashmap *map);

/**
 * Insert an item into the hash map.
 *
 * If the key already exists in the map, the item will be replaced with the
 * new item.
 *
 * @param map   Pointer to the hash map.
 * @param key   Key of the item to insert.
 * @param data  Data of the item to insert.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool hashmap_insert(struct hashmap *map, const void *key, void *data);

/**
 * Remove an item with the given key from the hash map.
 *
 * If the key is not present in the hash map, the operation will be considered
 * invalid.
 *
 * @param map   Pointer to the hash map.
 * @param key   Key of the item to remove.
 * @return      Pointer to the removed data, or `NULL` if the operation was
 *              invalid.
 */
void *hashmap_remove(struct hashmap *map, const void *key);

/**
 * Check if a key is in the hash map.
 *
 * @param map  The hash map to search.
 * @param key  The key to search for.
 * @return     `true` if the key exists in the map, `false` otherwise.
 */
bool hashmap_contains(const struct hashmap *map, const void *key);

/**
 * Get the value associated with a key in the hash map.
 *
 * @param map  Pointer to the hash map.
 * @param key  Key to look up.
 * @return     Pointer to the value associated with the key, or `NULL` if the
 *             map is `NULL`, the key is `NULL`, or the item was not found.
 */
void *hashmap_get(const struct hashmap *map, const void *key);

/**
 * Get the capacity of the hash map.
 *
 * @param map  Pointer to the hash map.
 * @return     The capacity of the hash map.
 */
usize hashmap_capacity(const struct hashmap *map);

/**
 * Get the number of items in the hash map.
 *
 * @param map  Pointer to the hash map.
 * @return     Number of items in the hash map, or 0 if the map is `NULL`.
 */
usize hashmap_len(const struct hashmap *map);

/**
 * Reserve space for a given number of items in the hash map.
 *
 * If the given capacity is less than the current number of items, the function
 * will have no effect. If the given capacity is greater than the current number
 * of items, the capacity of the hash map will be increased to accommodate the
 * additional items.
 *
 * @param map       Pointer to the hash map.
 * @param capacity  Number of items to reserve space for.
 * @return          `true` if the operation was successful, `false` otherwise.
 */
bool hashmap_reserve(struct hashmap *map, usize capacity);

/**
 * Iterate over the items in the hash map.
 *
 * @param map       Pointer to the hash map.
 * @param callback  Callback function to call for each item.
 * @param context   User-defined context to pass to the callback function.
 */
void hashmap_iter(
    const struct hashmap *map,
    void (*callback)(const void *key, void *data, void *context),
    void *context
);
