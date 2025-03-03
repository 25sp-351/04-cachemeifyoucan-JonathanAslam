#ifndef BST_H
#define BST_H

#include "dll.h"

// BST Node structure
typedef struct BSTNode {
    int key;           // Key for lookup
    Node* dll_node;    // Pointer to the corresponding node in DLL
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// BST structure
typedef struct {
    BSTNode* root;
} BST;

// Initialize a new BST
BST* init_bst();

// Create a new BST node
BSTNode* create_bst_node(int key, Node* dll_node);

// Insert a node into the BST
void bst_insert(BST* tree, int key, Node* dll_node);

// Find a node in the BST by key
BSTNode* bst_find(BST* tree, int key);

// Delete a node from the BST by key
void bst_delete(BST* tree, int key);

// Free the entire BST
void free_bst(BST* tree);

#endif // BST_H