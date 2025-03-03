#ifndef DLL_H
#define DLL_H

typedef struct Node Node;

// CREATE A DOUBLY LINKED LIST WHICH HAS A NODE (KEY, VALUE) AND A PREVOUS /
// NEXT POINTER
struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
};

typedef struct {
    Node* head;
    Node* tail;
} List;

List* init_list();

// insert a new node after the given position node
void insert_after(Node* position, Node* new_node);

// delete the given node
void delete_node_from_list(Node* node);

Node* create_node(int key, int value);

void create_and_insert_node(List* list, int key, int value);

void insert_at_end(List* list, int key, int value);

Node* find_by_key(List* list, int key);

int delete_by_key(List* list, int key);

void print_list(List* list);

int is_empty(List* list);

int list_size(List* list);

void clear_list(List* list);

void free_list(List* list);

#endif  // DLL_H
