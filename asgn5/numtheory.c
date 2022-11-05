#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    while (mpz_cmp_si(b, 0)) {
        mpz_set(d, b);
        mpz_mod(b, a, b);
        mpz_set(a, d);
    }
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    mpz_t r;
    mpz_init_set(r, n);
    mpz_t r_prime;
    mpz_init_set(r_prime, a);
    mpz_t t;
    mpz_init_set_si(t, 0);
    mpz_t t_prime;
    mpz_init_set_si(t_prime, 1);
    while (mpz_cmp_si(r_prime, 0)) {
        mpz_t q;
        mpz_init(q);
        mpz_fdiv_q(q, r, r_prime);
        mpz_set(r, r_prime);
        mpz_mul(r_prime, q, r_prime);
        mpz_sub(r_prime, r, r_prime);
        mpz_set(t, t_prime);
        mpz_mul(t_prime, q, t_prime);
        mpz_sub(t_prime, t, t_prime);
    }
    if (mpz_cmp_si(r, 1) > 0) {return;}
    if (mpz_cmp_si(t, 0) < 0) {mpz_add(t, t, n);}
    mpz_set(o, t);
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t v;
    mpz_init_set_si(v, 1);
    mpz_t p;
    mpz_init_set(p, a);
    while (mpz_cmp_si(d, 0) > 0) {
        if (mpz_fdiv_ui(d, 2)==1) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);
        (void) mpz_fdiv_q_ui(d, d, 2);
    }
    mpz_set(o, v);
}

bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t d;
    mpz_init(d);
    mpz_sub_ui(d, n, 1);
    uint64_t s = 0;
    whle (mpz_fdiv_ui(d, 2) == 0) {
        (void) mpz_fdiv_q_ui(d, d, 2);
        s += 1;
    }
    for (uint64_t i = 0; i < iters; i += 1) {
        mpz_t a;
        mpz_init(a);
        mpz_sub_ui(a, n, 4);
        mpz_t temp;
        mpz_init_set_ui(temp, random());
        mpz_mod(a, temp, a);
        mpz_add_ui(a, a, 2);
        mpz_t y;
        mpz_init(y);
        pow_mod(y, a, r, n);
        mpz_t n_1;
        mpz_init(n_1);
        mpz_sub_ui(n_1, n_1, 1);
        if (mpz_cmp_ui(y, 1) && mpz_cmp(y, n_1)) {
            uint64_t j = 1;
            while (j < s && mpz_cmp(y, n_1)) {
                mpz_set_ui(temp, 2);
                pow_mod(y, y, temp, n);
                if (!mpz_cmp_ui(y, 1)) {return false;}
                j += 1;
            }
            if (mpz_cmp(y, n_1)) {return false;}
        }
    }
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    uint64_t last_bit = 2;
    for (uint64_t i = 2; i < bits; i += 1) {
        last_bit *= 2;
    }
    mpz_set_ui(p, 0);
    while (!is_prime(p, iters)) {
        uint64_t prime_canidate = gmp_urandomb_ui(state, bits);
        prime_canidate &= last_bit;
        prime_canidate &= 0x1;
        mpz_set_ui(p, prime_canidate);
    }
}
