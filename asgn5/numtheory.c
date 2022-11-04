#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    while (b != 0) {
        d = b;
        b = a % b;
        a = d;
    }
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    mpz_t r;
    mpz_init_set_ui(r);
    r = n;
    mpz_t r_prime;
    mpz_init_set_ui(r_prime);
    r_prime = a;
    mpz_t t;
    mpz_init_set_ui(t);
    t = 0;
    mpz_t t_prime;
    mpz_init_set_ui(t_prime);
    t_prime = 1;
    while (r_prime != 0) {
        mpz_t q;
        mpz_init_set_ui(q);
        q = r/r_prime;
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
    mpz_t v;
    mpz_init_set_ui(v);
    v = 1;
    mpz_t p;
    mpz_init_set_ui(p);
    p = a;
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
    mpz_t d;
    mpz_init_set_ui(d);
    d = n - 1;
    uint64_t s = 0;
    whle (d % 2 == 0) {
        d /= 2;
        s += 1;
    }
    for (uint64_t i = 0; i < iters; i += 1) {
        mpz_t a;
        mpz_init_set_ui(a);
        a = 0;
        while (a < 2) {a = random() % (n-2);}
        mpz_t y;
        mpz_init_set_ui(y);
        pow_mod(y, a, r, n);
        if (y != 1 && y != (n-1)) {
            uint64_t j = 1;
            while (j < s && y != (n-1)) {
                pow_mod(y, y, 2, n);
                if (y == 1) {return false;}
                j += 1;
            }
            if (y != (n-1)) {return false;}
        }
    }
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    uint64_t last_bit = 2;
    for (uint64_t i = 2; i < bits; i += 1) {
        last_bit *= 2;
    }
    p = 0;
    while (!is_prime(p, iters)) {
        uint64_t prime_canidate = gmp_urandomb_ui(state, bits);
        prime_canidate &= last_bit;
        prime_canidate &= 0x1;
        p = prime_canidate;
    }
}
