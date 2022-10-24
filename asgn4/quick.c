#include "quick.h"
#include "stats.h"
#include "shell.h"
#include <stdint.h>

#include <stdio.h>
#include <inttypes.h>

#define SMALL 8


void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
    if (n_elements < SMALL) {
        shell_sort(stats, arr, n_elements);
        return;
    }
    uint32_t pivot = (move(stats, arr[0]) + move(stats, arr[n_elements/2]) + move(stats, arr[n_elements-1])) / 3;
    uint32_t right_n = 0;
    for (uint32_t i = 0; i < n_elements; i += 1) {
        if (cmp(stats, arr[i], pivot) == 1) {
            swap(stats, &arr[right_n], &arr[i]);
            right_n += 1;
        }
    }
    quick_sort(stats, arr, right_n);
    for (uint32_t i = 0; i < right_n; i += 1) {
        swap(stats, &arr[n_elements-i-1], &arr[right_n-i-1]);
    }
    quick_sort(stats, arr, n_elements-right_n);
}
