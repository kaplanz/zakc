#include <stdlib.h> // for EXIT_FAILURE, EXIT_SUCCESS

#include <zakc/list.h>  // for list
#include <zakc/log.h>   // for info
#include <zakc/types.h> // for usize

int main(void) {
    // Create a new linked list
    struct list *list = list_new();
    if (!list) {
        // Handle error
        return EXIT_FAILURE;
    }

    // Append some numbers to the list
    list_append(list, (void *)(i64)1);
    list_append(list, (void *)(i64)2);
    list_append(list, (void *)(i64)3);

    // Prepend the number 0 to the beginning of the list
    list_prepend(list, (void *)(i64)0);

    // Insert the number 4 at the end of the list
    list_insert(list, 4, (void *)(i64)4);

    // Remove the first element of the list
    list_shift(list);

    // Reverse the list
    list_reverse(list);

    // Print the elements of the list
    info("The list contains the following values:");
    for (usize i = 0; i < list_len(list); i++) {
        info("%lld", (i64)list_get(list, i));
    }

    // Clean up
    list_drop(list);

    return EXIT_SUCCESS;
}
