#ifndef LRU_H
#define LRU_H

#include "bst.h"
#include "dll.h"
#include "rodcut.h"

// LRU Cache structure
typedef struct LRUCache {
    List* dll;     // Doubly linked list for access order
    BST* bst;      // BST for O(log n) lookups by key
    int capacity;  // Maximum capacity of the cache
    int size;      // Current size of the cache
} LRUCache;

// Initialize a new LRU cache with given capacity
LRUCache* init_lru_cache(int capacity);

// Get value from cache; returns -1 if not found
int lru_cache_get(LRUCache* cache, int key);

// Put key-value pair into cache
void lru_cache_put(LRUCache* cache, int key, int value);

// Free the entire cache including structure
void free_lru_cache(LRUCache* cache);

// create the cache function, with the help of a cache function ptr. THESE ARE
// THE RODCUT SPECIFIC FUNCTIONS tried to use 03 and 05 as inspiration for this,
// not sure if its correct
// typedef void (*cache_function_ptr)(LRUCache* cache,
//                                    best_cut_pairs* output_values,
//                                    cut_information* length_value_array,
//                                    int rod_length, int argument_array_length,
//                                    store_max_val_ptr op);

int cached_rod_cut(LRUCache* cache,
                                  best_cut_pairs* output_values,
                                  cut_information* length_value_array,
                                  int rod_length, int argument_array_length,
                                  store_max_val_ptr op);

void init_rod_cut_cache(LRUCache* cache, int capacity);
void cleanup_rod_cut_cache(LRUCache* cache);
#endif  // LRU_H
