#include "dreidel.h"
#include "mtrand.h"
#include <stdint.h>

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
}

int play_game (int n_players, int coins_per_player, int * n_rounds) {
    int pot = 0;
    int play_coins[n_players];
    for (int i = 0; i<n_players; i+=1) {
        play_coins[i] = coins_per_player;
    }
    for (n_rounds = 0; 1; n_rounds +=1) {
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
                int half_pot = pot/2;
                play_coins[i] += half_pot;
                pot -= half_pot;
                break;
            case 'N':
                break;
            case 'S':
                play_coins[i] -= 1;
                if (play_coins[i] > -1) {pot += 1;}
                break;
            }
            if (play_coins[i] == coins_per_player * n_players) {
                return i;
            }
            if (winner == -1) {
                winner = i;
                continue;
            }
            winner = -2;
        }
        if (winner != -2) {return winner;}
    }
}
