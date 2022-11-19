
//Include Libraries
#include "bf.h"
#include "bv.h"
#include "city.h"
#include "ht.h"
#include "ll.h"
#include "node.h"
#include "parser.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

//Define OPTIONS constant
#define OPTIONS "ht:f:ms"


int main(int argc, char **argv) {
    //define variables
    bool mtf = false;
    uint32_t ht_size = 10000;
    uint32_t bf_size = 0x80000;
    int opt = 0;
    //takes comand line arguments and runs associated code.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            ht_size = atoll(optarg);
            break;
        case 'f':
            bf_size = atoll(optarg);
            break;
        case 'm':
            mtf = true;
            break;
        case 's':
            break;
        case 'h':
            fprintf(stderr, "Usage: ./encrypt [options]\n  ./encrypt encrypts an input file using the specified public key file,\n  writing the result to the specified output file.\n    -i <infile> : Read input from <infile>. Default: standard input.\n    -o <outfile>: Write output to <outfile>. Default: standard output.\n    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        default:
            fprintf(stderr, "Usage: ./encrypt [options]\n  ./encrypt encrypts an input file using the specified public key file,\n  writing the result to the specified output file.\n    -i <infile> : Read input from <infile>. Default: standard input.\n    -o <outfile>: Write output to <outfile>. Default: standard output.\n    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n");
            break;
        }
    }
    

    return 0;
}
