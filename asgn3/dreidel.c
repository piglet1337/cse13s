//include libraries
#include "dreidel.h"
#include "mtrand.h"
#include <stdint.h>
#include <stdio.h>

//picks a random character from G, H, N, and S
char spin_dreidel(void) {
    uint64_t random_int = mtrand_rand64() % 4;
    switch (random_int)
    {
    case 0:
        return 'G';
    case 1:
        return 'H';
    case 2:
        return 'N';
    case 3:
        return 'S';
    }
    return 'N';
}

//plays a full game of dreidal recording number of rounds passed and returns the winning players position.
int play_game (int n_players, int coins_per_player, int * n_rounds) {
    int pot = 0;
    int play_coins[n_players];
    for (int i = 0; i<n_players; i+=1) {
        play_coins[i] = coins_per_player;
    }
    while (1) {
        int winner = -1;
        for (int i=0; i<n_players; i+=1) {
            if (play_coins[i] < 0) {continue;}
            switch (spin_dreidel())
            {
            case 'G':
                play_coins[i] += pot;
                pot = 0;
                break;
            case 'H':
                play_coins[i] += pot/2;
                pot -= pot/2;
                break;
            case 'N':
                break;
            case 'S':
                play_coins[i] -= 1;
                if (play_coins[i] > -1) {pot += 1;}
                //print_mes is a global variable that tracks if the print_message option is provided
                else if (print_mes){
                    printf("%s: elmiminated in round %d of a %d player game.\n", names[i], *n_rounds, n_players);
                }
                break;
            }
            //checks if only one player remains.
            if (winner == -1) {
                winner = i;
                continue;
            }
            winner = -2;
        }
        if (winner != -2) {return winner;}
        *n_rounds += 1;
    }
}
