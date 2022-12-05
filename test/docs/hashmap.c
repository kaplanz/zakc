#include <stdlib.h> // for EXIT_FAILURE, EXIT_SUCCESS

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
