# zakc

## Overview

The `zakc` library is a collection of C libraries written by Zakhary Kaplan. It
includes two main collection implementations: a vector and a hash map. Alongside
those are various other useful `typedef`s and macros.

The vector provides a dynamic array data structure for storing and manipulating
a sequence of elements. This module offers functions for appending, inserting,
and removing elements, as well as functions for searching and accessing elements
in the vector. It is designed to be efficient and user-friendly, with automatic
resizing to accommodate new elements.

The hash map implementation offers a hash table data structure for storing and
retrieving key-value pairs. It uses a hash function to efficiently map keys to
values, allowing for fast lookup and insertion of items in the map. This module
provides functions for inserting, removing, and retrieving items in the map, as
well as functions for checking if a given key is in the map. The hash map is
useful for storing and manipulating large datasets, where the keys are unique
and can be efficiently hashed.

Overall, `zakc` offers a powerful set of tools for working with data in C
programs. The vector and hash map implementations are well-designed and easy to
use, providing efficient and effective ways to store and manipulate data.

## Contents

### Vector

The vector library provides a dynamic array data structure that can be used to
store and manipulate a sequence of elements. The vector is similar to a standard
C array, but it has the ability to automatically resize itself as needed to
accommodate additional elements. This allows the vector to grow and shrink
dynamically, providing flexibility and efficiency when working with large
datasets.

To use the vector library, the user must first create a new vector with the
`vector_new()` function. This function returns a pointer to the newly-created
vector, or `NULL` if memory allocation fails. Once a vector has been created,
elements can be added to the end of the vector with the `vector_append()`
function. The vector will automatically resize itself as needed to accommodate
additional elements.

The vector library provides a number of other functions for manipulating the
vector, such as `vector_extend()` for appending elements from another vector,
`vector_pop()` for removing the last element from the vector, `vector_insert()`
for inserting an element at a specific index, and `vector_remove()` for removing
an element at a specific index. The library also provides functions for
searching the vector, such as `vector_contains()` for checking if the vector
contains a given element, and `vector_get()` for retrieving the element at a
specific index.

Here is a brief example of how the vector library can be used to store and
manipulate a sequence of numbers:

```c
#include <stdlib.h>      // for EXIT_FAILURE, EXIT_SUCCESS

#include <zakc/log.h>    // for info
#include <zakc/types.h>  // for i64, usize
#include <zakc/vector.h> // for vector

int main(void) {
    // Create a new vector
    struct vector *vec = vector_new();
    if (!vec) {
        // Handle error
        return EXIT_FAILURE;
    }

    // Append some numbers to the vector
    vector_append(vec, (void *)(i64)1);
    vector_append(vec, (void *)(i64)2);
    vector_append(vec, (void *)(i64)3);

    // Print the numbers in the vector
    info("The vector contains the following values:");
    for (usize i = 0; i < vector_len(vec); i++) {
        info("%lld", (i64)vector_get(vec, i));
    }

    // Clean up
    vector_drop(vec);

    return EXIT_SUCCESS;
}
```

This example creates a new vector and appends the numbers 1, 2, and 3 to the
vector. It then uses a loop to iterate over the elements in the vector and print
each one to the standard output. Finally, it calls `vector_drop()` to clean up
the vector and free any allocated memory.

### Hash Map

The hash map library provides a hash table data structure that can be used to
store and retrieve key-value pairs. The hash map uses a hash function to
efficiently map keys to values, allowing for fast lookup and insertion of items
in the map. The hash map is useful for storing and manipulating large datasets,
where the keys are unique and can be efficiently hashed.

To use the hash map library, the user must first create a new hash map with the
`hashmap_new()` function. This function takes two arguments: a hash function and
a comparison function. The hash function is used to compute a hash value for
each key, and the comparison function is used to compare keys for equality. The
`hashmap_new()` function returns a pointer to the newly-created hash map, or
`NULL` if memory allocation fails.

Once a hash map has been created, items can be inserted into the map with the
`hashmap_insert()` function. This function takes three arguments: a pointer to
the hash map, a pointer to the key, and a pointer to the value. The key and
value are added to the map, and the value associated with the key is updated if
the key already exists in the map. The hash map library also provides a number
of other functions for manipulating the hash map, such as `hashmap_remove()` for
removing an item with a given key, `hashmap_contains()` for checking if a key is
in the map, and `hashmap_get()` for retrieving the value associated with a given
key.

Here is a brief example of how the hash map library can be used to store and
manipulate a set of key-value pairs:

```c
#include <stdlib.h>       // for EXIT_FAILURE, EXIT_SUCCESS

#include <zakc/hashmap.h> // for hashmap
#include <zakc/log.h>     // for info
#include <zakc/types.h>   // for i64, usize

int main(void) {
    // Create a new hash map
    struct hashmap *map = hashmap_new(str_hash, str_cmp);
    if (!map) {
        // Handle error
        return EXIT_FAILURE;
    }

    // Insert some key-value pairs into the hash map
    hashmap_insert(map, "foo", (void *)(i64)1);
    hashmap_insert(map, "bar", (void *)(i64)2);
    hashmap_insert(map, "baz", (void *)(i64)3);

    // Check if the map contains a given key
    if (hashmap_contains(map, "foo")) {
        info("The map contains the key 'foo'.");
    }

    // Get the value associated with a given key
    info(
        "The value associated with the key 'bar' is %lld.",
        (i64)hashmap_get(map, "bar")
    );

    // Clean up
    hashmap_drop(map);

    return EXIT_SUCCESS;
}
```

This example creates a new hash map and inserts the following key-value pairs
into the map: `("foo", 1)`, `("bar", 2)`, and `("baz", 3)`. It then uses the
`hashmap_contains()` function to check if the map contains a given key, and the
`hashmap_get()` function to retrieve the value associated with a given key.
Finally, it calls `hashmap_drop()` to clean up the hash map and free any
allocated memory.

## Credits

Thanks to ChatGPT for being a key contributor to the vector and hash map
implementations, as well as writing this README. ChatGPT's ability to enable
quick iteration was essential in creating high-quality and user-friendly
libraries that are an asset to the `zakc` library. We are grateful for its
contributions and appreciate the time and effort it saved for this project.
