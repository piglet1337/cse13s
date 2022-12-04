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
    char *infile = NULL;
    char *outfile = NULL;
    bool statistics = false;
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
            statistics = true;
            break;
        case 'h':
            fprintf(stderr, "SYNOPSIS\n  A Huffman encoder.\n  Compresses a file using the Huffman coding algorithm.\n\nUSAGE\n  ./encode [-h] [-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v             Print compression statistics.\n  -i infile      Input file to compress.\n  -o outfile     Output of compressed data.\n");
            return 0;
        default:
            fprintf(stderr, "SYNOPSIS\n  A Huffman encoder.\n  Compresses a file using the Huffman coding algorithm.\n\nUSAGE\n  ./encode [-h] [-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v             Print compression statistics.\n  -i infile      Input file to compress.\n  -o outfile     Output of compressed data.\n");
            return 0;
        }
    }
    //open input file
    int input_file;
    if (infile == NULL) {
        input_file = 0;
    }
    else {
        input_file = open(infile, O_RDONLY);
    }
    //create histogram that counts occurences of each symbol
    uint64_t histogram[ALPHABET];
    for (int i = 0; i < ALPHABET; i += 1) {
        histogram[i] = 0;
    }
    int temp_file = open("/tmp", __O_TMPFILE | O_RDWR);
    while (true) {
        uint8_t buf[BLOCK];
        int result = read_bytes(input_file, buf, BLOCK);
        write_bytes(temp_file, buf, result);
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
    fstat(temp_file, &stats);
    header.permissions = stats.st_mode;
    int total = 0;
    for (int i = 0; i < ALPHABET; i += 1) {
        if (histogram[i]) {total += 1;}
    }
    header.tree_size = total*3 - 1;
    header.file_size = stats.st_size;
    //write header to outfile
    int output_file;
    if (outfile == NULL) {
        output_file = 1;
    }
    else {
        output_file = open(outfile, O_WRONLY);
    }
    write(output_file, &header, sizeof(Header));
    //open outfile and set permissions
    fchmod(output_file, stats.st_mode);
    // write tree to outfile
    dump_tree(output_file, root);
    //go through symbols and write corresponding code for each symbol.
    lseek(temp_file, 0, SEEK_SET);
    while (true) {
        uint8_t buf[BLOCK];
        int result = read_bytes(temp_file, buf, BLOCK);
        if (result == 0) {break;}
        for (int i = 0; i < result; i += 1) {
            write_code(output_file, &(table[buf[i]]));
        }
        flush_codes(output_file);
    }
    close(input_file);
    close(temp_file);
    fstat(output_file, &stats);
    uint64_t commpressed_size = stats.st_size;
    close(output_file);
    delete_tree(&root);
    if (statistics) {
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", header.file_size);
        fprintf(stderr, "Compressed file size: %lu bytes\n", commpressed_size);
        float quotient = (float) commpressed_size/header.file_size;
        fprintf(stderr, "Space saving: %.2f%%\n", 100 * (1-quotient));
    }
    return 0;
}
