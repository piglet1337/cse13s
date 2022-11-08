#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

//finds greatest common denominator of a and b and sets d equal to it. 
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

//takes the modular inverse of a and n and sets o equal to it.
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

//takes the power mod: o = a^d mod n
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

//checks if n is prime using miller-robin primality test with iters iterations.
bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t a, temp, y, n_1, r;
    mpz_inits(a, temp, y, n_1, r, NULL);
    mpz_sub_ui(r, n, 1);
    uint64_t s = 0;
    while (mpz_fdiv_ui(r, 2) == 0) {
        (void) mpz_fdiv_q_ui(r, r, 2);
        s += 1;
    }
    for (uint64_t i = 0; i < iters; i += 1) {
        mpz_sub_ui(temp, n, 2);
        mpz_urandomm(a, state, temp);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);
        mpz_sub_ui(n_1, n, 1);
        mpz_set_ui(temp, 2);
        if (mpz_cmp_ui(y, 1) && mpz_cmp(y, n_1)) {
            uint64_t j = 1;
            while (j <= s - 1 && mpz_cmp(y, n_1)) {
                pow_mod(y, y, temp, n);
                if (!mpz_cmp_ui(y, 1)) {return false;}
                j += 1;
            }
            if (mpz_cmp(y, n_1)) {
                return false;
            }
        }
    }
    mpz_clears(a, temp, y, n_1, r, NULL);
    return true;
}

//generates a random prime number p that is bits bits long. iters defines the amount of iterations for is_prime.
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_set_ui(p, 80);
    mpz_t bitwise, last_bit;
    mpz_init_set_ui(bitwise, 1);
    mpz_init_set_ui(last_bit, 2);
    for (uint64_t i = 2; i < bits; i += 1) {
        mpz_mul_ui(last_bit, last_bit, 2);
    }
    while (!is_prime(p, iters)) {
        mpz_urandomb(p, state, bits);
        mpz_ior(p, p, bitwise);
        mpz_ior(p, p, last_bit);
    }
    mpz_clears(bitwise, last_bit, NULL);
}
