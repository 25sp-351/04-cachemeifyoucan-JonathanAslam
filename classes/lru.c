#include "lru.h"

#include "rodcut.h"
// #include "dll.h"
// #include "bst.h"

#include <stdlib.h>

extern store_max_val_ptr original_provider;  // point to rodcut's store_max_value

// Initialize a new LRU cache with given capacity
LRUCache* init_lru_cache(int capacity) {
    if (capacity <= 0)
        return NULL;

    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    if (cache == NULL)
        return NULL;  // Memory allocation failed

    cache->dll = init_list();
    cache->bst = init_bst();

    if (cache->dll == NULL || cache->bst == NULL) {
        free_list(cache->dll);
        free_bst(cache->bst);
        free(cache);
        return NULL;  // Memory allocation failed
    }

    cache->capacity = capacity;
    cache->size     = 0;

    return cache;
}

// Helper function to move a node to the front (most recently used), this is how
// we keep track according to youtube explainatons
void move_to_front(LRUCache* cache, Node* node) {
    if (cache == NULL || node == NULL)
        return;

    // Remove from current position
    node->prev->next = node->next;
    node->next->prev = node->prev;

    // Insert after dummy head (at front)
    node->next                   = cache->dll->head->next;
    node->prev                   = cache->dll->head;
    cache->dll->head->next->prev = node;
    cache->dll->head->next       = node;
}

// Get value from cache; returns -1 if not found
int lru_cache_get(LRUCache* cache, int key) {
    if (cache == NULL)
        return -1;

    BSTNode* bst_node = bst_find(cache->bst, key);
    if (bst_node == NULL)
        return -1;  // Key not found

    // Move the node to the front (most recently used)
    move_to_front(cache, bst_node->dll_node);

    return bst_node->dll_node->value;
}

// Put key-value pair into cache
void lru_cache_put(LRUCache* cache, int key, int value) {
    if (cache == NULL)
        return;

    // Check if key already exists
    BSTNode* bst_node = bst_find(cache->bst, key);

    // node is found, return the node after moving it to the front
    if (bst_node != NULL) {
        // Update existing entry
        bst_node->dll_node->value = value;
        move_to_front(cache, bst_node->dll_node);
        return;
    }

    // If we've reached capacity, remove the least recently used item (at tail),
    // we always want at least one free space in the cache
    if (cache->size >= cache->capacity) {
        Node* node_to_remove = cache->dll->tail->prev;

        // Remove from BST
        bst_delete(cache->bst, node_to_remove->key);

        // Remove from DLL
        node_to_remove->prev->next = node_to_remove->next;
        node_to_remove->next->prev = node_to_remove->prev;

        // Free the node
        free(node_to_remove);

        cache->size--;
    }

    // Create new node and add to front of list
    Node* new_node = create_node(key, value);
    if (new_node == NULL)
        return;  // Memory allocation failed

    insert_after(cache->dll->head, new_node);

    // Add to BST for fast lookup, works alongside the cache
    bst_insert(cache->bst, key, new_node);

    cache->size++;
}

// Free the entire cache including structure
void free_lru_cache(LRUCache* cache) {
    if (cache == NULL)
        return;

    free_list(cache->dll);
    free_bst(cache->bst);
    free(cache);
}

////////// THESE ARE THE RODCUT SPECIFIC FUNCTIONS

int cached_rod_cut(LRUCache* cache, best_cut_pairs* output_values,
                   cut_information* length_value_array, int rod_length,
                   int argument_array_length, store_max_val_ptr op) {
    // check to see if the result for the rod_length is already cached. we are
    // trying to save the result as the value associated to the key (rod_length)
    int cached_result = lru_cache_get(cache, rod_length);

    // if it exists in the cache, grab the values
    if (cached_result != -1) {
        output_values->total_value = cached_result;
        return cached_result;
    }

    // if not found, run rodcut_function and cache/return result, call the
    // store_max_value function
    original_provider = op;
    cached_result = (*original_provider)(output_values, length_value_array,
                                         rod_length, argument_array_length);

    lru_cache_put(cache, rod_length, cached_result);

    return cached_result;
}

// Initialize the cache and set up function pointers
void init_rod_cut_cache(LRUCache* cache, int capacity) {
    if (cache == NULL && capacity > 0) {
        cache = init_lru_cache(capacity);
    }
}

void cleanup_rod_cut_cache(LRUCache* cache) {
    if (cache != NULL) {
        free_lru_cache(cache);
        cache = NULL;
    }
}
