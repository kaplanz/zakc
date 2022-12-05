#include <stdlib.h> // for EXIT_FAILURE, EXIT_SUCCESS

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
