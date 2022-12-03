//include libraries
#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <inttypes.h>

// Define OPTIONS constant
#define OPTIONS "hi:o:v"

int main(int argc, char **argv) {
    // define variables
    char *infile = "stdin";
    char *outfile = "stdout";
    // bool stats = false;
    int opt = 0;
    //run code associated with inputted arguments
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = optarg;
            break;
        case 'o':
            outfile = optarg;
            break;
        case 'v':
            // stats = true;
        case 'h':
            printf("SYNOPSIS\n  A Huffman encoder.\n  Compresses a file using the Huffman coding algorithm.\n\nUSAGE\n  ./encode [-h] [-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v             Print compression statistics.\n  -i infile      Input file to compress.\n  -o outfile     Output of compressed data.\n");
            return 0;
        default:
            return 0;
        }
    }
    //open input file
    int input_file = open(infile, O_RDONLY);
    //create histogram that counts occurences of each symbol
    uint64_t histogram[ALPHABET];
    for (int i = 0; i < ALPHABET; i += 1) {
        histogram[i] = 0;
    }
    while (true) {
        uint8_t buf[BLOCK];
        int result = read_bytes(input_file, buf, BLOCK);
        if (result == 0) {break;}
        for (int i = 0; i < result; i += 1) {
            histogram[buf[i]] += 1;
        }
    }
    //ensure at least two symbols have non-zero counts
    if (histogram[0] == 0) {histogram[0] = 1;}
    if (histogram[1] == 0) {histogram[1] = 1;}
    //construct huffman tree
    Node *root = build_tree(histogram);
    //construct code table
    Code table[ALPHABET];
    build_codes(root, table);
    //constuct header
    Header header;
    header.magic = MAGIC;
    struct stat stats;
    fstat(input_file, &stats);
    header.permissions = stats.st_mode;
    int total = 0;
    for (int i = 0; i < ALPHABET; i += 1) {
        if (histogram[i]) {total += 1;}
    }
    header.tree_size = total*3 - 1;
    header.file_size = stats.st_size;
    FILE *out_file;
    //write header to outfile
    out_file = fopen(outfile, "w");
    fwrite(&header, sizeof(Header), 1, out_file);
    fclose(out_file);
    //open outfile and set permissions
    int output_file = open(outfile, O_APPEND | O_WRONLY);
    fchmod(output_file, stats.st_mode);
    // write tree to outfile
    dump_tree(output_file, root);
    //go through symbols and write corresponding code for each symbol.
    while (true) {
        uint8_t buf[BLOCK];
        int result = read_bytes(input_file, buf, BLOCK);
        if (result == 0) {break;}
        for (int i = 0; i < result; i += 1) {
            write_code(output_file, &(table[buf[i]]));
        }
        flush_codes(output_file);
    }
    close(input_file);
    close(output_file);
    delete_tree(&root);
    return 0;
}
