#include "mtrand.h"
#include "dreidel.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>

int print_mes = 0;
//define OPTIONS constant with all available command line arguments.
#define OPTIONS "p:c:s:v"

//main function
int main(int argc, char **argv) {
    char * names[] = {
        "Aharon",
        "Batsheva",
        "Chanah",
        "David",
        "Ephraim",
        "Faige",
        "Gamaliel",
        "Hannah",
    };
    int opt = 0;
    extern char *optarg;
    int num_players = 4;
    int num_coins = 3;
    uint64_t seed = 613;
    //takes comand line arguments and runs associated code.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'p':
            num_players = atoi(optarg);
            break;
        case 'c':
            num_coins = atoi(optarg);
            break;
        case 's':
            seed = atoll(optarg);
            break;
        case 'v':
            print_mes = 1;
            break;
        }
    }
    if (2 > num_players || 8 < num_players) {return 1;}
    if (1 > num_coins || 20 < num_coins) {return 2;}
    if (1 > seed || 9999999999 < seed) {return 3;}
    mtrand_seed(seed);
    int rounds = 0;
    int result = play_game(num_players, num_coins, &rounds);
    printf("%s %d %d %d %" PRIu64 "\n", names[result], num_players, num_coins, rounds, seed);
    return 0;
}
