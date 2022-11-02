#include "numtheory.h"

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    while (b != 0) {
        d = b;
        b = a % b;
        a = d;
    }
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    mpz_t r = n;
    mpz_t r_prime = a;
    mpz_t t = 0;
    mpz_t t_prime = 1;
    while (r_prime != 0) {
        mpz_t q = r/r_prime;
        r = r_prime;
        r_prime = r - q*r_prime;
        t = t_prime;
        t_prime = t - q*t_prime;
    }
    if (r > 1) {return;}
    if (t < 0) {t += n;}
    o = t;
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t v = 1;
    mpz_t p = a;
    while (d > 0) {
        if (d%2==1) {
            v = v*p % n;
        }
        p = p*p % n;
        d = d/2;
    }
    o = v;
}

bool is_prime(mpz_t n, uint64_t iters) {
    
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters);
