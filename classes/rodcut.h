#ifndef RODCUT_H
#define RODCUT_H

// #include "lru.h"
// both structs built off explination from the zoom meeting with prof
typedef struct LRUCache LRUCache;

typedef struct {
    int count;
    int length;
    int value;
} cut_information;

typedef struct {
    int total_value;
    char *output_text;
} best_cut_pairs;

extern int global_best_value;
extern int *global_best_cuts;
extern int global_rod_length;
extern cut_information *global_length_value_array;
extern int global_array_length;

// create a function pointer for the store_max_value function
typedef int (*store_max_val_ptr)(best_cut_pairs *output_values,
                                 cut_information *length_value_array,
                                 int rod_length, int argument_array_length);

// create a function pointer for the main recursive function: find_best_cut
typedef void (*best_cut_ptr)(cut_information *length_value_array,
                             int rod_length, int argument_array_length,
                             int *current_cuts, int current_value);

extern best_cut_ptr best_cut_provider;
// extern store_max_val_ptr store_max_val_provider;

// functions
void find_best_cut(cut_information *length_value_array, int rod_length,
                   int argument_array_length, int *current_cuts,
                   int current_value);

int calculate_and_store_max_value(cut_information *length_value_array,
                                  int rod_length, int argument_array_length,
                                  int *cuts_made, int *cut_lengths);

int store_max_value(best_cut_pairs *output_values,
                    cut_information *length_value_array, int rod_length,
                    int argument_array_length);

void print_best_cut();
#endif  // RODCUT_H
