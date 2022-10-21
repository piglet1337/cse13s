#include "bubble.h"
#include "stats.h"

void bubble_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
    for (uint32_t i = 0; i < n_elements-1; i += 1) {
        int not_swapped = 1;
        for (uint32_t j = n_elements-1; j > i; j -= 1) {
            if (cmp(stats, arr[j], arr[j-1]) == -1) {
                swap(stats, &arr[j], &arr[j-1]);
                not_swapped = 0;
            }
        }
        if (not_swapped) {break;}
    }
}
