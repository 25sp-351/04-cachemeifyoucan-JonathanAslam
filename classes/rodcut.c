#include "rodcut.h"

#include <stdio.h>
#include <stdlib.h>


// Define to globals from rod.h
int global_best_value = 0;
int *global_best_cuts = NULL;
int global_rod_length = 0;
cut_information *global_length_value_array = NULL;
int global_array_length = 0;

//extern function ptr included here;
best_cut_ptr best_cut_provider = find_best_cut;

// Pure recursive function to explore all possible cutting combinations - based
// of the example prof gave and the diagram he drew used to find the list of
// best possible cutting lengths
void find_best_cut(cut_information *length_value_array, int rod_length,
                   int argument_array_length, int *current_cuts,
                   int current_value) {

    // create variable to check if t/f, using 1 and 0, use later
    int did_find_cut = 0;

    // loop all possible cuts
    for (int ix = 0; ix < argument_array_length; ix++) {
        int piece_length = length_value_array[ix].length;

        // Skip if piece is too large for remaining rod
        if (piece_length > rod_length) {
            continue;  // jumps to the check below to update the
                       // global_best_value
        }

        // change to true if the piece length <= rod length
        did_find_cut = 1;

        // Make this cut, increment the amount of cuts currently made for this
        // length
        current_cuts[ix]++;

        // Recursively try all possibilities with remaining rod, pass on
        // current_cut and backtrack after recursive call to avoid the cut
        // amount exceeding the correct amount. remember we are checking for
        // every possible cut so this value will become very large if not
        // decremented
        (*best_cut_provider)(length_value_array, rod_length - piece_length,
                      argument_array_length, current_cuts,
                      current_value + length_value_array[ix].value);

        // Undo this cut (backtrack)
        current_cuts[ix]--;
    }

    // If we couldn't make any more cuts, check if this is the best solution so
    // far
    if (!did_find_cut) {
        if (current_value > global_best_value) {
            global_best_value = current_value;

            // Save this cutting pattern, we will call on the global best cuts
            // later to print out the correct output text
            for (int ix = 0; ix < argument_array_length; ix++)
                global_best_cuts[ix] = current_cuts[ix];
        }
    }
}

// Calculate and store the maximum value - second part of the recursive logic.
// takes the global variables and calculates the max value from the possbile
// good cutting lenghts
int calculate_and_store_max_value(cut_information *length_value_array,
                                  int rod_length, int argument_array_length,
                                  int *cuts_made, int *cut_lengths) {

    // Store the rod length and array globally
    global_rod_length         = rod_length;
    global_length_value_array = length_value_array;
    global_array_length       = argument_array_length;

    // Reset global best value
    global_best_value = 0;

    // Allocate memory for tracking cuts - used calloc instead of malloc so the
    // arrays are initialized and we dont want garbage values that malloc may
    // produce
    int *current_cuts = (int *)calloc(argument_array_length, sizeof(int));

    // Free previous global_best_cuts if it exists
    if (global_best_cuts != NULL)
        free(global_best_cuts);

    global_best_cuts = (int *)calloc(argument_array_length, sizeof(int));

    if (!current_cuts || !global_best_cuts) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Find the best cutting solution recursively, this is what fills the
    // global_best_cuts_array
    find_best_cut(length_value_array, rod_length, argument_array_length,
                  current_cuts, 0);

    // Copy the best solution to the cuts_made array
    for (int ix = 0; ix < argument_array_length; ix++) {
        if (global_best_cuts[ix] > 0) {
            int length = length_value_array[ix].length;
            cuts_made[length] += global_best_cuts[ix];
            cut_lengths[ix] = length;
        }
    }

    // Clean up
    free(current_cuts);

    return global_best_value;
}

// Store the maximum value in output_values
int store_max_value(best_cut_pairs *output_values,
                    cut_information *length_value_array, int rod_length,
                    int argument_array_length) {

    // Allocate memory for cuts_made and cut_lengths
    int *cuts_made   = (int *)calloc(rod_length + 1, sizeof(int));
    int *cut_lengths = (int *)calloc(argument_array_length, sizeof(int));

    if (!cuts_made || !cut_lengths) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Calculate the maximum value
    int max_value = calculate_and_store_max_value(
        length_value_array, rod_length, argument_array_length, cuts_made,
        cut_lengths);

    // Store the result in output_values
    output_values->total_value = max_value;

    // Create output text for the solution (optional, as we'll use
    // print_best_cut)
    output_values->output_text = NULL;

    // Clean up temporary arrays
    free(cuts_made);
    free(cut_lengths);

    return max_value;
}

// Print the best cutting solution
void print_best_cut() {
    if (global_best_cuts == NULL || global_length_value_array == NULL) {
        printf("No solution calculated yet.\n");
        return;
    }

    int remaining_length = global_rod_length;

    // Print each type of cut, made, tracking the remainder throughout.
    for (int ix = 0; ix < global_array_length; ix++) {
        if (global_best_cuts[ix] > 0) {
            int length = global_length_value_array[ix].length;
            int value  = global_length_value_array[ix].value;
            int count  = global_best_cuts[ix];

            printf("%d @ %d = %d\n", count, length, count * value);
            remaining_length -= count * length;
        }
    }

    // Print remainder and total value
    printf("Remainder: %d\n", remaining_length);
    printf("Value: %d\n", global_best_value);
}
