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
    char *pvfile = "rsa.priv";
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
            pvfile = optarg;
            break;
        case 'v':
            verbose = 1;
            break;
        case 'h':
            fprintf(stderr, "Usage: ./decrypt-dist [options]\n  ./decrypt-dist decrypts an input file using the specified private key file,\n  writing the result to the specified output file.\n    -i <infile> : Read input from <infile>. Default: standard input.\n    -o <outfile>: Write output to <outfile>. Default: standard output.\n   -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        default:
            fprintf(stderr, "Usage: ./decrypt-dist [options]\n  ./decrypt-dist decrypts an input file using the specified private key file,\n  writing the result to the specified output file.\n    -i <infile> : Read input from <infile>. Default: standard input.\n    -o <outfile>: Write output to <outfile>. Default: standard output.\n   -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        }
    }
    //open private key file.
    FILE *pvkey;
    pvkey = fopen(pvfile, "r");
    //print error message if unable to open.
    if (pvkey == NULL) {
        fprintf(stderr, "Unable to open private key file.");
        exit(0);
    }
    //read private key from file.
    mpz_t n, d;
    mpz_inits(n, d, NULL);
    rsa_read_priv(n, d, pvkey);
    //print to stderr if verbose is enabled.
    if (verbose) {
        gmp_fprintf(stderr, "n - modulus (%u bits): %Zd\nd - private exponent (%u bits): %Zd\n", mpz_sizeinbase(n, 2), n, mpz_sizeinbase(d, 2), d);
    }
    //decrypt file.
    rsa_decrypt_file(infile, outfile, n, d);
    //close files and clear mpz_t variables.
    fclose(pvkey);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, d, NULL);
}
