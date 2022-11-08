#include "randstate.h"
#include <stdlib.h>

//initiliazes random state and sets seeds for random number generation.
void randstate_init(uint64_t seed) {
    (void) srandom(seed);
    __gmp_randinit_mt(state);
    __gmp_randseed_ui(state, seed);
}

//clears random state.
void randstate_clear(void) {
    __gmp_randclear(state);
}
