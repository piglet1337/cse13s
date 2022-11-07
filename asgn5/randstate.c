#include "randstate.h"
#include <stdlib.h>

void randstate_init(uint64_t seed) {
    (void) srandom(seed);
    __gmp_randinit_mt(state);
    __gmp_randseed_ui(state, seed);
}


void randstate_clear(void) {
    __gmp_randclear(state);
}
