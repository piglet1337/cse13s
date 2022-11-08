#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>
#include <stdio.h>

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t temp_a, temp_b;
    mpz_init_set(temp_a, a);
    mpz_init_set(temp_b, b);
    while (mpz_cmp_si(b, 0)) {
        mpz_set(d, temp_b);
        mpz_mod(temp_b, temp_a, temp_b);
        mpz_set(temp_a, d);
    }
    mpz_set(d, a);
    mpz_clears(temp_a, temp_b, NULL);
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
        mpz_t temp;
        mpz_init(temp);
        mpz_init(q);
        mpz_fdiv_q(q, r, r_prime);
        mpz_set(temp, r_prime);
        mpz_mul(r_prime, q, r_prime);
        mpz_sub(r_prime, r, r_prime);
        mpz_set(r, temp);
        mpz_set(temp, t_prime);
        mpz_mul(t_prime, q, t_prime);
        mpz_sub(t_prime, t, t_prime);
        mpz_set(t, temp);
        mpz_clears(q, temp, NULL);
    }
    if (mpz_cmp_si(r, 1) > 0) {return;}
    if (mpz_cmp_si(t, 0) < 0) {mpz_add(t, t, n);}
    mpz_set(o, t);
    mpz_clears(r, r_prime, t, t_prime, NULL);
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t v;
    mpz_init_set_ui(v, 1);
    mpz_t p;
    mpz_init_set(p, a);
    mpz_t temp_d;
    mpz_init_set(temp_d, d);
    while (mpz_cmp_ui(temp_d, 0) > 0) {
        if (mpz_fdiv_ui(temp_d, 2)==1) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);
        (void) mpz_fdiv_q_ui(temp_d, temp_d, 2);
    }
    mpz_set(o, v);
    mpz_clears(v, p, temp_d, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t r;
    mpz_init(r);
    mpz_sub_ui(r, n, 1);
    uint64_t s = 0;
    while (mpz_fdiv_ui(r, 2) == 0) {
        (void) mpz_fdiv_q_ui(r, r, 2);
        s += 1;
    }
    for (uint64_t i = 0; i < iters; i += 1) {
        mpz_t a, temp;
        mpz_inits(a, temp, NULL);
        mpz_sub_ui(temp, n, 2);
        mpz_urandomm(a, state, temp);
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
        mpz_clears(a, temp, y, n_1, NULL);
    }
    mpz_clear(r);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_set_ui(p, 80);
    mpz_t bitwise;
    mpz_init_set_ui(bitwise, 1);
    while (!is_prime(p, iters)) {
        mpz_urandomb(p, state, bits);
        mpz_ior(p, p, bitwise);
        if (mpz_tstbit(p, bits-1)) {
            mpz_combit(p, bits-1);
        }
    }
    mpz_clear(bitwise);
}
