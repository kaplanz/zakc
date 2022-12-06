// File:        hashmap.c
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     05 Dec 2022
// SPDX-License-Identifier: MIT

#include "zakc/hashmap.h"

#include <stdlib.h> // for free, {c,m}alloc
#include <string.h> // for strcmp

#include "zakc/types.h" // for u{8,64}, usize

// Hash map structure
struct hashmap {
    // Hash function for keys
    u64 (*hash)(const void *key);
    // Comparison function for keys
    bool (*cmp)(const void *left, const void *right);
    // Array of linked lists of items
    struct item **items;
    // Capacity of the array
    usize capacity;
    // Number of items in the hash map
    usize nitems;
};

// Item structure
struct item {
    // Key of the item
    const void *key;
    // Data of the item
    void *data;
    // Pointer to the next item in the linked list
    struct item *next;
};

// Hash function for C-strings
u64 str_hash(const void *key) {
    const char *str = (const char *)key;
    u64 hash = 5381;
    u64 c;

    while ((c = *str++))
        // hash * 33 xor c
        hash = ((hash << 5) + hash) ^ c;

    return hash;
}

// Comparison function for C-strings
bool str_cmp(const void *left, const void *right) {
    return !(bool)strcmp((const char *)left, (const char *)right);
}

// Hash function for an array of bytes
u64 bytes_hash(const void *key, usize len) {
    const u8 *data = (const u8 *)key;
    u64 hash = 5381;

    for (u64 i = 0; i < len; i++)
        // hash * 33 xor data[i]
        hash = ((hash << 5) + hash) ^ data[i];

    return hash;
}

// Comparison function for an array of bytes
bool bytes_cmp(const void *left, const void *right, usize len) {
    const u8 *datl = (const u8 *)left;
    const u8 *datr = (const u8 *)right;
    return !(bool)memcmp(datl, datr, len);
}

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
) {
    // Allocate memory for the hash map
    struct hashmap *map = malloc(sizeof(struct hashmap));
    if (!map)
        // Return `NULL` if memory allocation failed
        return NULL;

    // Initialize the hash function, comparison function, and number of items
    *map = (struct hashmap){
        .hash = hash,
        .cmp = cmp,
        .nitems = 0,
    };

    // Initialize the capacity and array of linked lists to be `NULL`
    map->capacity = 0;
    map->items = NULL;

    // Return the newly-created hash map
    return map;
}

/**
 * Delete the hash map.
 *
 * @param map  Pointer to the hash map to delete.
 */
void hashmap_drop(struct hashmap *map) {
    if (!map)
        // Return early if the hash map is `NULL`
        return;
    // Free all items in the hash map
    struct item *tmp, *item;
    for (usize i = 0; i < map->capacity; i++) {
        item = map->items[i];
        while (item != NULL) {
            tmp = item;
            item = item->next;
            free(tmp);
        }
    }
    free(map->items);
    free(map);
}

/**
 * Insert a new item into the hash map.
 *
 * If the key is already present in the hash map, its associated data will be
 * overwritten with the new data.
 *
 * The hash map will be resized if necessary to accommodate the new item.
 *
 * @param map   Pointer to the hash map.
 * @param key   Key of the new item.
 * @param data  Data of the new item.
 * @return      `true` if the operation was successful, `false` otherwise.
 */
bool hashmap_insert(struct hashmap *map, const void *key, void *data) {
    if (!map)
        // Return `false` if the hash map is `NULL`
        return false;

    // Check if the `items` field is `NULL`
    if (!map->items) {
        // Reserve space for at least one item in the hash map
        if (!hashmap_reserve(map, 1))
            return false;
    }

    // Calculate the index of the item in the `items` array
    u64 index = map->hash(key) % map->capacity;
    // Get the pointer to the head of the linked list at the index
    struct item *item = map->items[index];

    // Check if the key is already present in the hash map
    while (item && !map->cmp(item->key, key))
        item = item->next;

    // Check if we need to resize the `items` array
    if (map->nitems + 1 > map->capacity * 0.8) {
        // Increase the capacity of the `items` array
        if (!hashmap_reserve(map, map->capacity == 0 ? 1 : map->capacity * 2))
            // Return `false` if unable to resize the `items` array
            return false;
        // Update the index of the item after resizing the `items` array
        index = map->hash(key) % map->capacity;
    }

    if (item) {
        // Overwrite the data of the existing item
        item->data = data;
    } else {
        // Allocate memory for the new item
        item = malloc(sizeof(struct item));
        if (!item)
            // Return `false` if memory allocation failed
            return false;
        // Initialize the new item
        *item = (struct item){
            .key = key,
            .data = data,
            .next = map->items[index],
        };
        // Insert the new item into the linked list at the index
        map->items[index] = item;
        // Increase the number of items in the hash map
        map->nitems++;
    }

    // Return `true` to indicate success
    return true;
}

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
void *hashmap_remove(struct hashmap *map, const void *key) {
    if (!map || !map->items)
        // Return `NULL` if the hash map or `items` field is `NULL`
        return NULL;

    // Calculate the index of the item in the `items` array
    const u64 index = map->hash(key) % map->capacity;
    // Get the pointer to the head of the linked list at the index
    struct item *item = map->items[index];

    if (item && map->cmp(item->key, key)) {
        // Get the data of the removed item
        void *data = item->data;
        // Update the head of the linked list to point to the next item
        map->items[index] = item->next;
        // Free the removed item
        free(item);
        // Update the number of items in the hash map
        map->nitems--;
        // Return the data of the removed item
        return data;
    }

    // Check if the key is present in the linked list
    while (item && item->next) {
        if (map->cmp(item->next->key, key)) {
            // Get the data of the removed item
            void *data = item->next->data;
            // Update the next pointer of the current item to point to the
            // next item in the linked list
            item->next = item->next->next;
            // Free the removed item
            free(item->next);
            // Update the number of items in the hash map
            map->nitems--;
            // Return the data of the removed item
            return data;
        }
        item = item->next;
    }

    // Return `NULL` if the key is not present in the hash map
    return NULL;
}

/**
 * Check if an item with the given key is present in the hash map.
 *
 * @param map   Pointer to the hash map.
 * @param key   Key of the item to check.
 * @return      `true` if the item is present, `false` otherwise.
 */
bool hashmap_contains(const struct hashmap *map, const void *key) {
    if (!map || !map->items)
        // Return `false` if the hash map or `items` field is `NULL`
        return false;

    // Calculate the index of the item in the `items` array
    const u64 index = map->hash(key) % map->capacity;
    // Get the pointer to the head of the linked list at the index
    const struct item *item = map->items[index];

    // Check if the key is present in the linked list
    while (item) {
        if (map->cmp(item->key, key))
            // Return `true` if the key is found
            return true;
        item = item->next;
    }

    // Return `false` if the key is not present in the hash map
    return false;
}

/**
 * Get the data of an item with the given key from the hash map.
 *
 * If the key is not present in the hash map, the operation will be considered
 * invalid.
 *
 * @param map   Pointer to the hash map.
 * @param key   Key of the item to get.
 * @return      Pointer to the data of the item, or `NULL` if the operation was
 *              invalid.
 */
void *hashmap_get(const struct hashmap *map, const void *key) {
    if (!map || !map->items)
        // Return `NULL` if the hash map or `items` field is `NULL`
        return NULL;

    // Calculate the index of the item in the `items` array
    const u64 index = map->hash(key) % map->capacity;
    // Get the pointer to the head of the linked list at the index
    const struct item *item = map->items[index];

    // Check if the key is present in the linked list
    while (item) {
        if (map->cmp(item->key, key))
            // Return the data of the item if the key is found
            return item->data;
        item = item->next;
    }

    // Return `NULL` if the key is not present in the hash map
    return NULL;
}

/**
 * Get the capacity of the hash map.
 *
 * The capacity of the hash map is the number of items that the hash map can
 * store before it needs to be resized.
 *
 * @param map  Pointer to the hash map.
 * @return     The capacity of the hash map.
 */
usize hashmap_capacity(const struct hashmap *map) {
    if (!map)
        // Return 0 if the hash map is `NULL`
        return 0;
    // Return the capacity of the hash map
    return map->capacity;
}

/**
 * Get the number of items in the hash map.
 *
 * @param map  Pointer to the hash map.
 * @return     Number of items in the hash map, or 0 if the hash map is `NULL`.
 */
usize hashmap_len(const struct hashmap *map) {
    if (!map)
        // Return 0 if the hash map is `NULL`
        return 0;
    // Return the number of items in the hash map
    return map->nitems;
}

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
bool hashmap_reserve(struct hashmap *map, usize capacity) {
    if (!map)
        // Return `false` if the hash map is `NULL`
        return false;

    // Return early if the given capacity is less than the current number of
    // items
    if (capacity < map->nitems)
        return true;

    // Allocate memory for the new array of linked lists
    struct item **items = calloc(capacity, sizeof(struct item *));
    if (!items)
        return false;

    // Rehash all existing items into the new array of linked lists
    for (usize i = 0; i < map->capacity; i++) {
        struct item *tmp, *item = map->items[i];
        while (item != NULL) {
            // Compute the new hash value for the item
            u64 hash = map->hash(item->key) % capacity;
            // Insert the item into the new array of linked lists
            tmp = item->next;
            item->next = items[hash];
            items[hash] = item;
            // Move to the next item in the old array of linked lists
            item = tmp;
        }
    }

    // Free the old array of linked lists
    free(map->items);

    // Update the capacity and items fields of the hash map
    map->capacity = capacity;
    map->items = items;

    return true;
}

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
) {
    if (!map || !callback)
        // Return early if the hash map or callback is `NULL`
        return;
    // Iterate over the array of linked lists
    for (usize i = 0; i < map->capacity; i++) {
        struct item *item = map->items[i];
        while (item) {
            // Call the callback function for each item
            callback(item->key, item->data, context);
            item = item->next;
        }
    }
}
