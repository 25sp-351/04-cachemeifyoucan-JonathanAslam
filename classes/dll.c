#include "dll.h"

#include <stdio.h>
#include <stdlib.h>

#include "dll.h"

// Initialize a new doubly linked list with dummy head and tail nodes
List* init_list() {
    List* new_list = (List*)malloc(sizeof(List));
    if (new_list == NULL)
        return NULL;  // Memory allocation failed

    // Allocate dummy head and tail nodes
    new_list->head = (Node*)malloc(sizeof(Node));
    new_list->tail = (Node*)malloc(sizeof(Node));

    if (new_list->head == NULL || new_list->tail == NULL) {
        free(new_list->head);
        free(new_list->tail);
        free(new_list);
        return NULL;  // Memory allocation failed
    }

    // Initialize dummy nodes with special values
    new_list->head->key   = -1;
    new_list->head->value = -1;
    new_list->tail->key   = -1;
    new_list->tail->value = -1;

    // Properly link head and tail
    new_list->head->prev = NULL;
    new_list->head->next = new_list->tail;
    new_list->tail->prev = new_list->head;
    new_list->tail->next = NULL;

    return new_list;
}

// Insert a new node after the specified position
void insert_after(Node* position, Node* new_node) {
    if (position == NULL || new_node == NULL)
        return;  // Invalid parameters

    // Link the new node to its neighbors
    new_node->next = position->next;
    new_node->prev = position;

    // Update the links of neighboring nodes
    if (position->next != NULL)
        position->next->prev = new_node;

    position->next = new_node;
}

// Delete a node from the list
void delete_node_from_list(Node* node) {
    if (node == NULL)
        return;  // Invalid parameter

    // Skip deletion if it's a dummy head or tail node
    if (node->prev == NULL || node->next == NULL)
        return;

    // Connect the previous and next nodes to each other
    node->prev->next = node->next;
    node->next->prev = node->prev;

    // Free the removed node
    free(node);
}

// Additional needed functions

// Create a new node with given key and value
Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL)
        return NULL;  // Memory allocation failed

    new_node->key   = key;
    new_node->value = value;
    new_node->next  = NULL;
    new_node->prev  = NULL;

    return new_node;
}

// Insert at the beginning of the list (after dummy head)
void create_and_insert_node(List* list, int key, int value) {
    if (list == NULL)
        return;

    Node* new_node = create_node(key, value);
    if (new_node == NULL)
        return;

    insert_after(list->head, new_node);
}

// Insert at the end of the list (before dummy tail)
void insert_at_end(List* list, int key, int value) {
    if (list == NULL)
        return;

    Node* new_node = create_node(key, value);
    if (new_node == NULL)
        return;

    insert_after(list->tail->prev, new_node);
}

// Find a node by key
Node* find_by_key(List* list, int key) {
    if (list == NULL)
        return NULL;

    Node* current = list->head->next;
    while (current != list->tail) {
        if (current->key == key)
            return current;
        current = current->next;
    }

    return NULL;  // Key not found
}

// Delete a node by key
int delete_by_key(List* list, int key) {
    if (list == NULL)
        return 0;

    Node* node = find_by_key(list, key);
    if (node == NULL)
        return 0;  // Key not found

    delete_node_from_list(node);
    return 1;  // Deletion successful
}

// Print the list
void print_list(List* list) {
    if (list == NULL)
        return;

    printf("List: ");
    Node* current = list->head->next;
    while (current != list->tail) {
        printf("(%d, %d) ", current->key, current->value);
        current = current->next;
    }
    printf("\n");
}

// Check if the list is empty
int is_empty(List* list) {
    if (list == NULL)
        return 1;  // Consider NULL list as empty

    return list->head->next == list->tail;
}

// Get the size of the list
int list_size(List* list) {
    if (list == NULL)
        return 0;

    int count     = 0;
    Node* current = list->head->next;
    while (current != list->tail) {
        count++;
        current = current->next;
    }

    return count;
}

// Clear the list (remove all nodes but keep the list structure)
void clear_list(List* list) {
    if (list == NULL)
        return;

    Node* current = list->head->next;
    while (current != list->tail) {
        Node* temp = current;
        current    = current->next;
        free(temp);
    }

    // Reset the list to empty state
    list->head->next = list->tail;
    list->tail->prev = list->head;
}

// Free the entire list including structure
void free_list(List* list) {
    if (list == NULL)
        return;

    // Free all nodes between head and tail
    clear_list(list);

    // Free dummy nodes and list structure
    free(list->head);
    free(list->tail);
    free(list);
}
