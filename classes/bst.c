#include "bst.h"
#include <stdlib.h>

// Initialize a new BST
BST* init_bst() {
    BST* new_tree = (BST*)malloc(sizeof(BST));
    if (new_tree == NULL)
        return NULL;  // Memory allocation failed
    
    new_tree->root = NULL;
    return new_tree;
}

// Create a new BST node
BSTNode* create_bst_node(int key, Node* dll_node) {
    BSTNode* new_node = (BSTNode*)malloc(sizeof(BSTNode));
    if (new_node == NULL)
        return NULL;  // Memory allocation failed
    
    new_node->key = key;
    new_node->dll_node = dll_node;
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

// Helper function to recursively insert a node
BSTNode* insert_recursive(BSTNode* root, int key, Node* dll_node) {
    // Base case: empty tree or reached a leaf
    if (root == NULL)
        return create_bst_node(key, dll_node);
    
    // Recursive cases
    if (key < root->key)
        root->left = insert_recursive(root->left, key, dll_node);
    else if (key > root->key)
        root->right = insert_recursive(root->right, key, dll_node);
    else {
        // Key already exists, update dll_node pointer
        root->dll_node = dll_node;
    }
    
    return root;
}

// Insert a node into the BST
void bst_insert(BST* tree, int key, Node* dll_node) {
    if (tree == NULL)
        return;
    
    tree->root = insert_recursive(tree->root, key, dll_node);
}

// Helper function to recursively find a node
BSTNode* find_recursive(BSTNode* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    
    if (key < root->key)
        return find_recursive(root->left, key);
    else
        return find_recursive(root->right, key);
}

// Find a node in the BST by key
BSTNode* bst_find(BST* tree, int key) {
    if (tree == NULL)
        return NULL;
    
    return find_recursive(tree->root, key);
}

// Helper function to find minimum node
BSTNode* find_min(BSTNode* node) {
    BSTNode* current = node;
    
    // Find the leftmost leaf
    while (current && current->left != NULL)
        current = current->left;
    
    return current;
}

// Helper function to recursively delete a node
BSTNode* delete_recursive(BSTNode* root, int key) {
    // Base case
    if (root == NULL)
        return root;
    
    // Recursive case
    if (key < root->key)
        root->left = delete_recursive(root->left, key);
    else if (key > root->key)
        root->right = delete_recursive(root->right, key);
    else {
        // Node with only one child or no child
        if (root->left == NULL) {
            BSTNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            BSTNode* temp = root->left;
            free(root);
            return temp;
        }
        
        // Node with two children: Get the inorder successor (smallest
        // in the right subtree)
        BSTNode* temp = find_min(root->right);
        
        // Copy the inorder successor's content to this node
        root->key = temp->key;
        root->dll_node = temp->dll_node;
        
        // Delete the inorder successor
        root->right = delete_recursive(root->right, temp->key);
    }
    return root;
}

// Delete a node from the BST by key
void bst_delete(BST* tree, int key) {
    if (tree == NULL)
        return;
    
    tree->root = delete_recursive(tree->root, key);
}

// Helper function to recursively free nodes
void free_recursive(BSTNode* root) {
    if (root == NULL)
        return;
    
    // Post-order traversal to free children before parent
    free_recursive(root->left);
    free_recursive(root->right);
    free(root);
}

// Free the entire BST
void free_bst(BST* tree) {
    if (tree == NULL)
        return;
    
    free_recursive(tree->root);
    free(tree);
}