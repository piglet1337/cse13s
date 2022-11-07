//Include Libraries
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>

//Define OPTIONS constant
#define OPTIONS "b:i:n:d:s:vh"

gmp_randstate_t state;

int main(int argc, char **argv) {
    //define variables
    uint64_t bit_count = 1024;
    uint64_t iterations = 50;
    char *pbfile = "rsa.pub";
    char *pvfile = "rsa.priv";
    uint64_t seed = time(NULL);
    int verbose = 0;

    int opt = 0;
    //takes comand line arguments and runs associated code.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b':
            bit_count = atoll(optarg);
            break;
        case 'i':
            iterations = atoll(optarg);
            break;
        case 'n':
            pbfile = optarg;
            break;
        case 'd':
            pvfile = optarg;
            break;
        case 's':
            seed = atoll(optarg);
            break;
        case 'v':
            verbose = 1;
            break;
        case 'h':
            fprintf(stderr, "Usage: ./keygen-dist [options]\n  ./keygen-dist generates a public / private key pair, placing the keys into the public and private\n  key files as specified below. The keys have a modulus (n) whose length is specified in\n  the program options.\n    -s <seed>   : Use <seed> as the random number seed. Default: time()\n    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        default:
            fprintf(stderr, "Usage: ./keygen-dist [options]\n  ./keygen-dist generates a public / private key pair, placing the keys into the public and private\n  key files as specified below. The keys have a modulus (n) whose length is specified in\n  the program options.\n    -s <seed>   : Use <seed> as the random number seed. Default: time()\n    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        }
    }
    //open public and private key files.
    FILE *pbkey;
    FILE *pvkey;
    pbkey = fopen(pbfile, "w");
    pvkey = fopen(pvfile, "w");
    //print error message if files fail to open.
    if (pbkey == NULL) {
        fprintf(stderr, "Unable to open public key file.");
        exit(0);
    }
    if (pvkey == NULL) {
        fprintf(stderr, "Unable to open private key file.");
        exit(0);
    }
    //set permissions for private key file.
    fchmod(fileno(pvkey), 0600);
    //initialize random state.
    randstate_init(seed);
    //make public and private keys.
    mpz_t p, q, n, e, d, username, s;
    mpz_inits(p, q, n, e, d, username, s);
    rsa_make_pub(p, q, n, e, bit_count, iterations);
    rsa_make_priv(d, e, p, q);
    //get current username.
    char *user = getenv("USER");
    //convert username to mpz_t.
    mpz_set_str(username, user, 62);
    rsa_sign(s, username, d, n);
    //write public and private keys to there files.
    rsa_write_priv(n, d, pvkey);
    rsa_write_pub(n, e, s, user, pbkey);
    //print to stderr if verbose is enabled.
    if (verbose) {
        gmp_fprintf(stderr, "username: %s\nuser signature (%d bits): %Zd\np (%d bits): %Zd\nq (%d bits): %Zd\nn - modulus (%d bits): %Zd\ne - public exponent (%d bits): %Zd\nd - private exponent (%d bits): %Zd\n", user, mpz_sizeinbase(s, 2), s, mpz_sizeinbase(p, 2), p, mpz_sizeinbase(q, 2), q, mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e, mpz_sizeinbase(d, 2), d);
    }
    //close files, clear random state, and clear mpz_t variables.
    fclose(pvkey);
    fclose(pbkey);
    randstate_clear();
    mpz_clears(p, q, n, e, d, username, s);
}
