//Include Libraries
#include "bubble.h"
#include "heap.h"
#include "mtrand.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

//Define OPTIONS constant
#define OPTIONS "asbqhr:n:p:H"

int main(int argc, char **argv) {
    //define variables
    uint64_t seed = 13371453;
    uint32_t size = 100;
    uint32_t elements = 100;
    Set algorithms = set_empty();

    int opt = 0;

    //takes comand line arguments and runs associated code.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            algorithms = set_insert(algorithms, 0);
            algorithms = set_insert(algorithms, 1);
            algorithms = set_insert(algorithms, 2);
            algorithms = set_insert(algorithms, 3);
            break;
        case 's':
            algorithms = set_insert(algorithms, 3);
            break;
        case 'b':
            algorithms = set_insert(algorithms, 0);
            break;
        case 'q':
            algorithms = set_insert(algorithms, 2);
            break;
        case 'h':
            algorithms = set_insert(algorithms, 1);
            break;
        case 'r':
            seed = atoll(optarg);
            break;
        case 'n':
            size = atol(optarg);
            if (size < 1 || size > 250000000) {return 3;}
            break;
        case 'p':
            elements = atol(optarg);
            break;
        case 'H':
            printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n   -H              Display program help and usage.\n   -a              Enable all sorts.\n   -b              Enable Bubble Sort.\n   -h              Enable Heap Sort.\n   -q              Enable Quick Sort.\n   -s              Enable Shell Sort.\n   -n length       Specify number of array elements (default: 100).\n   -p elements     Specify number of elements to print (default: 100).\n   -r seed         Specify random seed (default: 13371453).\n");
            return 0;
        default:
            printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n   -H              Display program help and usage.\n   -a              Enable all sorts.\n   -b              Enable Bubble Sort.\n   -h              Enable Heap Sort.\n   -q              Enable Quick Sort.\n   -s              Enable Shell Sort.\n   -n length       Specify number of array elements (default: 100).\n   -p elements     Specify number of elements to print (default: 100).\n   -r seed         Specify random seed (default: 13371453).\n");
            return 3;
        }
    }
    // ensures that elements is not bigger than size
    if (elements > size) {elements = size;}
    //sets random seed
    mtrand_seed(seed);
    //makes random array
    uint32_t random_array[size];
    for (uint32_t i = 0; i < size; i += 1) {
        uint32_t random_value = mtrand_rand64() & 0x3FFFFFFF;
        random_array[i] = random_value;
    }
    //Tests enabled Sorting algorithms
    for (int i = 0; i < 4; i += 1) {
        if (!set_member(algorithms, i)) {continue;}
        uint32_t array_copy[size];
        for (uint32_t i = 0; i < size; i += 1) {
            array_copy[i] = random_array[i];
        }
        Stats stats;
        stats.compares = 0;
        stats.moves = 0;
        switch (i)
        {
        case 0:
            bubble_sort(&stats, array_copy, size);
            printf("Bubble Sort, ");
            break;
        case 1:
            heap_sort(&stats, array_copy, size);
            printf("Heap Sort, ");
            break;
        case 2:
            quick_sort(&stats, array_copy, size);
            printf("Quick Sort, ");
            break;
        case 3:
            shell_sort(&stats, array_copy, size);
            printf("Shell Sort, ");
            break;
        }
        printf("%" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares", size, stats.moves, stats.compares);
        for (uint32_t i = 0; i < elements; i += 1) {
            if (i % 5 == 0) {printf("\n");}
            printf("%13" PRIu32, array_copy[i]);
        }
        printf("\n");
    }

    return 0; 
}
