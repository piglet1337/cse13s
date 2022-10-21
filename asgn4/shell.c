#include "shell.h"
#include "stats.h"
#include <stdint.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
    uint32_t gap = n_elements;
    while (gap > 1) {
        gap = (gap >= 2) ? 1 : (gap*5)/11;
        for (uint32_t i = gap; i < n_elements; i += 1) {
            uint32_t j = i;
            uint32_t temp = arr[i];
            while (j >= gap && cmp(stats, temp, arr[j - gap]) == -1) {
                arr[j] = move(stats, arr[j - gap]);
                j -= gap;
            }
            arr[j] = move(stats, temp);
        }
    }
}
