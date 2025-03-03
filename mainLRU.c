// implement policy 1 - LRU

#include <stdio.h>
#include <stdlib.h>

#include "classes/bst.h"
#include "classes/dll.h"
#include "classes/lru.h"
#include "classes/rodcut.h"

#define CUT_LENGTH_BUFFER_LENGTH 10
#define ROD_LENGTH_BUFFER_LENGTH 8

#define CAPACITY 10

store_max_val_ptr original_provider = store_max_value;  // Point to the original rod cutting function

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Parse filename from the command line argument after executable
    // error if NULL
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Please enter a file to read from\n");
        return 1;
    }
    // Variables for reading input for the cutting lenghts
    char cut_pair_buffer[CUT_LENGTH_BUFFER_LENGTH];
    int length                      = 0;
    int value                       = 0;
    int length_value_array_capacity = CAPACITY;  // Initial capacity
    int length_value_array_size     = 0;         // Actual number of elements

    // Allocate memory for cut_information array
    cut_information *length_value_array =
        malloc(length_value_array_capacity * sizeof(cut_information));
    if (length_value_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    while (fgets(cut_pair_buffer, sizeof(cut_pair_buffer), file) != NULL) {
        if (sscanf(cut_pair_buffer, "%d, %d", &length, &value) == 2) {
            // make sure length and value are non-negative values
            if (length < 0 || value < 0) {
                fprintf(stderr,
                        "Cannot compute a negative, please try again\n");
                continue;
            }

            // Resize array if needed, figured this out with the help of some AI 
            if (length_value_array_size >= length_value_array_capacity) {
                length_value_array_capacity *= 2;
                cut_information *temp =
                    realloc(length_value_array, length_value_array_capacity *
                                                    sizeof(cut_information));
                if (temp == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(length_value_array);
                    return 1;
                }
                length_value_array = temp;
            }

            // Store the piece information, set cut count to zero since there
            // havent been any made yet
            length_value_array[length_value_array_size].count  = 0;
            length_value_array[length_value_array_size].length = length;
            length_value_array[length_value_array_size].value  = value;
            length_value_array_size++;
        } else {
            fprintf(stderr,
                    "Invalid input format. Expected: <length>, <value>\n");
            free(length_value_array);
            return 1;
        }
    }

    printf("Printing out Cutting Pairs:\n");

    for (int ix = 0; ix < length_value_array_size; ix++) {
        printf("Pair #%d\n", ix);
        int length = length_value_array[ix].length;
        int value  = length_value_array[ix].value;
        int count  = length_value_array[ix].count;
        printf("Length: %d\nValue: %d\nCount: %d\n", length, value, count);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //create the cache to store the values, for now set capacity to 10
    LRUCache* lru_cache = init_lru_cache(CAPACITY);
    if (lru_cache == NULL) {
        fprintf(stderr, "Failed to initialize LRU cache\n"); //
        return 1;
    }


    // Variables for reading input for the rod lengths
    char rod_length_buffer[ROD_LENGTH_BUFFER_LENGTH];
    int rod_length = 0;

    printf(
        "Please input rod lengths in format: length. Enter `ctrl + d` "
        "when finished\n");
    while (fgets(rod_length_buffer, sizeof(rod_length_buffer), stdin) != NULL) {
        if (sscanf(rod_length_buffer, "%d", &rod_length) == 1) {
            // make sure length and value are non-negative values
            if (rod_length < 0) {
                fprintf(
                    stderr,
                    "Cannot compute a negative rod length, please try again\n");
                continue;
            }

            printf("rod_length: %d\n", rod_length);

            // if rod_length >= 0, we can call our recursive functions to work
            // on this. Allocate memory for cuts_made and cut_lengths. Need to
            // check cuts made for each length of the rod, 2^n-1 where n
            // is the
            // length.
            int *cuts_made = (int *)calloc(rod_length + 1, sizeof(int));
            // for multiple rod lengths
            int *cut_lengths =
                (int *)calloc(length_value_array_size, sizeof(int));

            if (cuts_made == NULL || cut_lengths == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(length_value_array);
                return 1;
            }

            best_cut_pairs output_information;
            output_information.output_text = NULL;
            output_information.total_value = 0;

            cached_rod_cut(lru_cache, &output_information, length_value_array, rod_length, length_value_array_size, *original_provider);


            // Print the solution
            print_best_cut();

            // Clean up
            free(cuts_made);
            free(cut_lengths);
        } else {
            fprintf(stderr,
                    "Invalid input format. Expected: <rod_length>, up to 6 "
                    "digits\n");
            free(length_value_array);
            return 1;
        }
    }
    free(length_value_array);
    fclose(file);

    if (global_best_cuts != NULL) {
        free(global_best_cuts);
        global_best_cuts = NULL;
    }

    free_lru_cache(lru_cache);


    return 0;
}
