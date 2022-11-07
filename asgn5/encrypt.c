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
#include <sys/stat.h>

//Define OPTIONS constant
#define OPTIONS "i:o:n:vh"

gmp_randstate_t state;

int main(int argc, char **argv) {
    //define variables
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pbfile = "rsa.pub";
    int verbose = 0;

    int opt = 0;
    //takes comand line arguments and runs associated code.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            break;
        case 'n':
            pbfile = optarg;
            break;
        case 'v':
            verbose = 1;
            break;
        case 'h':
            fprintf(stderr, "Usage: ./encrypt-dist [options]\n  ./encrypt-dist encrypts an input file using the specified public key file,\n  writing the result to the specified output file.\n    -i <infile> : Read input from <infile>. Default: standard input.\n    -o <outfile>: Write output to <outfile>. Default: standard output.\n    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        default:
            fprintf(stderr, "Usage: ./encrypt-dist [options]\n  ./encrypt-dist encrypts an input file using the specified public key file,\n  writing the result to the specified output file.\n    -i <infile> : Read input from <infile>. Default: standard input.\n    -o <outfile>: Write output to <outfile>. Default: standard output.\n    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        }
    }
    //open public key file.
    FILE *pbkey;
    pbkey = fopen(pbfile, "r");
    //print error message if unable to open.
    if (pbkey == NULL) {
        fprintf(stderr, "Unable to open public key file.");
        exit(0);
    }
    //read public key from opened file.
    mpz_t n, e, s, username;
    mpz_inits(n, e, s, username, NULL);
    char *user = "";
    rsa_read_pub(n, e, s, user, pbkey);
    //print to stderr if verbose is enabled.
    if (verbose) {
        gmp_fprintf(stderr, "username: %s\nuser signature (%u bits): %Zd\nn - modulus (%u bits): %Zd\ne - public exponent (%u bits): %Zd\n", user, mpz_sizeinbase(s, 2), s, mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e);
    }
    //convert username into mpz_t.
    mpz_set_str(username, user, 62);
    if (!rsa_verify(username, s, e, n)) {
        gmp_fprintf(stderr, "Couldn't verify user signature - exiting");
        exit(0);
    }
    //encrypt file.
    rsa_encrypt_file(infile, outfile, n, e);
    //close files and clear mpz_t variables.
    fclose(pbkey);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, e, s, username, NULL);
}
