#include "heap.h"
#include "stats.h"
#include <stdint.h>

static uint32_t l_child (uint32_t n) {return 2 * n + 1;}
static uint32_t r_child (uint32_t n) {return 2 * n + 2;}
static uint32_t parent (uint32_t n) {return (n-1)/2;}

static void up_heap (Stats *stats, uint32_t *a, uint32_t n) {
    while (n > 0 && cmp(stats, a[n], a[parent(n)] == 1)) {
        swap(stats, &a[n], &a[parent(n)]);
        n = parent(n);
    }
}

static void down_heap (Stats *stats, uint32_t *a, uint32_t heap_size) {
    uint32_t n = 0;
    while (l_child(n) < heap_size) {
        uint32_t bigger;
        if (r_child(n) == heap_size) {bigger = l_child(n);}
        else {
            bigger = (cmp(stats, a[l_child(n)], a[r_child(n)]) == 1) ? l_child(n) : r_child(n);
        }
        if (cmp(stats, a[n], a[bigger]) == 1) {break;}
        swap(stats, &a[n], &a[bigger]);
        n = bigger;
    }
}

static void build_heap (Stats *stats, uint32_t *a, uint32_t *heap, uint32_t n) {
    for (uint32_t i = 0; i < n; i += 1) {
        heap[i] = move(stats, a[i]);
        up_heap(stats, heap, n);
    }
}

void heap_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
    uint32_t heap[n_elements] = {0};
    build_heap(stats, &arr, &heap, n_elements);
    for (uint32_t i = 0; i < n_elements; i += 1) {
        arr[i] = move(stats, heap[0]);
        heap[0] = move(stats, heap[n_elements - i - 1]);
        down_heap(stats, heap, n_elements - i);
    }
}
