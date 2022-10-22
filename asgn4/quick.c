#include "quick.h"
#include "stats.h"
#include "shell.h"
#include <stdint.h>

#define SMALL 8

void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
    if (n_elements < SMALL) {shell_sort(stats, arr, n_elements);}
    uint32_t pivot = (arr[0] + arr[n_elements/2] + arr[n_elements-1]) / 3;
    
}
