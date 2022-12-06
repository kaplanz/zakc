// File:        cli.c
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     06 Dec 2022
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zakc/hashmap.h"
#include "zakc/log.h"
#include "zakc/types.h"

#define NAME    "cli"
#define VERSION "0.1.0"

// Forward declarations
void loop(void);
void cmd_help(void);
void cmd_print(void);
void cmd_new(void);
void cmd_drop(void);
void cmd_insert(void);
void cmd_remove(void);
void cmd_contains(void);
void cmd_get(void);
void cmd_cap(void);
void cmd_len(void);
void cmd_reserve(void);

// Hash map structure
struct hashmap *map = NULL;

// Print help instructions
void help(void) {
    println("%s %s", NAME, VERSION);
    println();
    println("Usage: %s [OPTIONS]", NAME);
    println();
    println("Options:");
    println("  -l, --log <LEVEL>    Logging level [default: warn]");
    println("  -h, --help           Print help information");
    println("  -V, --version        Print version information");
}

struct args {
    enum loglevel log;
};

struct args parse(int argc, char *argv[]) {
    struct args args;

    // Set the default values
    args.log = Warn;

    // Parse the command-line arguments
    for (usize i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
            exit(0);
        } else if (strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--version") == 0) {
            println("%s %s", NAME, VERSION);
            exit(0);
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log") == 0) {
            if (i + 1 < argc) {
                if (strcmp(argv[i + 1], "none") == 0) {
                    args.log = Off;
                } else if (strcmp(argv[i + 1], "error") == 0) {
                    args.log = Error;
                } else if (strcmp(argv[i + 1], "warn") == 0) {
                    args.log = Warn;
                } else if (strcmp(argv[i + 1], "info") == 0) {
                    args.log = Info;
                } else if (strcmp(argv[i + 1], "debug") == 0) {
                    args.log = Debug;
                } else if (strcmp(argv[i + 1], "trace") == 0) {
                    args.log = Trace;
                } else {
                    error("invalid log level");
                    error("usage: %s -l<LEVEL>", NAME);
                    exit(1);
                }
                i++;
            } else {
                error("missing log level");
                error("usage: %s -l<LEVEL>", NAME);
                exit(1);
            }
        } else {
            error("invalid option: %s", argv[i]);
            exit(1);
        }
    }

    return args;
}

int main(int argc, char *argv[]) {
    // Parse args
    struct args args = parse(argc, argv);

    // Set the logging level
    loglevel = args.log;

    // Enter the command loop
    loop();
}

// Command loop
void loop(void) {
    while (true) {
        // Print the prompt
        print("> ");

        // Read the command from the user
        char cmd[16];
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = '\0'; // remove newline from the input

        // Process the command
        if (strcmp(cmd, "help") == 0) {
            cmd_help();
        } else if (strcmp(cmd, "print") == 0) {
            cmd_print();
        } else if (strcmp(cmd, "new") == 0) {
            cmd_new();
        } else if (strcmp(cmd, "insert") == 0) {
            cmd_insert();
        } else if (strcmp(cmd, "remove") == 0) {
            cmd_remove();
        } else if (strcmp(cmd, "get") == 0) {
            cmd_get();
        } else if (strcmp(cmd, "contains") == 0) {
            cmd_contains();
        } else if (strcmp(cmd, "drop") == 0) {
            cmd_drop();
        } else if (strcmp(cmd, "len") == 0) {
            cmd_len();
        } else if (strcmp(cmd, "capacity") == 0) {
            cmd_cap();
        } else if (strcmp(cmd, "reserve") == 0) {
            cmd_reserve();
        } else if (strcmp(cmd, "quit") == 0) {
            // Repeat the loop unless the user entered the "quit" command
            break;
        } else {
            error("invalid command");
        }
    }
}

// Print a list of available commands and their descriptions
void cmd_help(void) {
    println("Available commands:");
    println("  help        Print this help message");
    println("  print       Print the entire hash map");
    println("  new         Create a new hash map");
    println("  insert      Insert a new key-value pair into the hash map");
    println("  remove      Remove a key-value pair from the hash map");
    println("  get         Retrieve the value associated with a given key");
    println("  contains    Check if the hash map contains a given key");
    println("  drop        Delete the entire hash map");
    println("  len         Print the number of items in the hash map");
    println("  capacity    Print the current capacity of the hash map");
    println("  reserve     Change the capacity of the hash map");
    println("  quit        Exit the program");
}

// Print the entire hash map
static void cmd_print_item(const void *key, void *data, void *_context) {
    println("  %s => %d", (char *)key, *((int *)data));
}
void cmd_print(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Print the items in the hash map
    if (!hashmap_len(map)) {
        info("hash map is empty");
    } else {
        println("Hash map:");
        hashmap_iter(map, cmd_print_item, NULL);
    }
    debug("cap: %zu", hashmap_capacity(map));
    debug("len: %zu", hashmap_len(map));
}

// Create a new hash map with string keys and integer values
void cmd_new(void) {
    if (map != NULL) {
        error("hash map already exists");
        return;
    }

    map = hashmap_new(str_hash, str_cmp);
    if (map == NULL) {
        error("failed to create hash map");
        return;
    }

    info("hash map created");
}

// Delete the hash map
static void cmd_drop_item(const void *key, void *data, void *_context) {
    debug("  %s => %d", (char *)key, *((int *)data));
    free((void *)key);
    free((void *)data);
}
void cmd_drop(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Delete the hash map
    if (hashmap_len(map)) {
        debug("deleting items:");
        hashmap_iter(map, cmd_drop_item, NULL);
    }
    hashmap_drop(map);
    map = NULL;
    info("hash map deleted");
}

// Insert an item with a string key and integer value into the hash map
void cmd_insert(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Read key and value from the user
    char keybuf[64], *key;
    print("Enter key: ");
    fgets(keybuf, sizeof(keybuf), stdin);
    keybuf[strcspn(keybuf, "\n")] = '\0'; // remove newline from the input
    key = strndup(keybuf, sizeof(keybuf));
    print("Enter value: ");
    int *value = malloc(sizeof(int));
    scanf("%d", value);
    fflush(stdin); // clear the input buffer

    // Insert item into the hash map
    if (hashmap_insert(map, key, value)) {
        info("item inserted");
    } else {
        error("failed to insert item");
    }
}

// Remove an item with the given string key from the hash map
void cmd_remove(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Read key from the user
    char key[64];
    print("Enter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // remove newline from the input

    // Remove item with the given key from the hash map
    int *value = hashmap_remove(map, key);
    if (value != NULL) {
        info("item removed (value = %d)", *value);
    } else {
        error("item not found");
    }
}

// Check if a key is in the hash map
void cmd_contains(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Read key from the user
    char key[64];
    print("Enter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // remove newline from the input

    // Check if the key is in the hash map
    bool result = hashmap_contains(map, key);
    if (result) {
        info("key exists in hash map");
    } else {
        warn("key does not exist in hash map");
    }
}

// Get the value associated with a key in the hash map
void cmd_get(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Read key from the user
    char key[64];
    print("Enter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // remove newline from the input

    // Get the value associated with the key
    int *value = hashmap_get(map, key);
    if (value != NULL) {
        info("value: %d", *value);
    } else {
        error("key not found");
    }
}

// Get the capacity of the hash map
void cmd_cap(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Get the capacity of the hash map
    usize capacity = hashmap_capacity(map);
    println("Capacity of hash map: %zu", capacity);
}

// Get the number of items in the hash map
void cmd_len(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Get the number of items in the hash map
    usize len = hashmap_len(map);
    println("Number of items in hash map: %zu", len);
}

// Reserve space for a given number of items in the hash map
void cmd_reserve(void) {
    if (map == NULL) {
        error("hash map is not created");
        return;
    }

    // Read the number of items to reserve space for from the user
    print("Enter number of items to reserve space for: ");
    usize capacity;
    scanf("%zu", &capacity);
    fflush(stdin); // clear the input buffer

    // Reserve space for the given number of items
    if (hashmap_reserve(map, capacity)) {
        info("space reserved");
    } else {
        error("failed to reserve space");
    }
}
