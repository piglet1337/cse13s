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
            printf("SYNOPSIS\n  A Huffman decoder.\n  Decompresses a file using the Huffman coding algorithm.\n\nUSAGE\n  ./decode [-h] [-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v             Print compression statistics.\n  -i infile      Input file to decompress.\n  -o outfile     Output of decompressed data.\n");
            return 0;
        default:
            printf("SYNOPSIS\n  A Huffman decoder.\n  Decompresses a file using the Huffman coding algorithm.\n\nUSAGE\n  ./decode [-h] [-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v             Print compression statistics.\n  -i infile      Input file to decompress.\n  -o outfile     Output of decompressed data.\n");
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
    //read in header from input file
    Header header;
    read(input_file, &header, sizeof(Header));
    //ensure header.magic is equal to MAGIC
    if (header.magic != MAGIC) {
        printf("Invalid magic number.\n");
        return 1;
    }
    //read tree from input_file
    uint8_t tree[header.tree_size];
    read(input_file, tree, header.tree_size);
    //rebuild huffman tree
    Node *root = rebuild_tree(header.tree_size, tree);
    //open outfile
    int output_file;
    if (infile == NULL) {
        output_file = 1;
    }
    else {
        output_file = open(outfile, O_WRONLY);
    }
    Node *current_node = root;
    uint64_t total = 0;
    // uint8_t buffer[BLOCK];
    while (total < header.file_size) {
        if (current_node->left == NULL && current_node->right == NULL) {
            write_bytes(output_file, &current_node->symbol, 1);
            current_node = root;
            total += 1;
            continue;
        }
        uint8_t bit;
        read_bit(input_file, &bit);
        if (bit) {
            current_node = current_node->right;
        }
        else {
            current_node = current_node->left;
        }
    }
    struct stat stats;
    fstat(output_file, &stats);
    uint64_t commpressed_size = stats.st_size;
    close(output_file);
    close(input_file);
    delete_tree(&root);
    if (statistics) {
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", header.file_size);
        fprintf(stderr, "Compressed file size: %lu bytes\n", commpressed_size);
        float quotient = (float) commpressed_size/header.file_size;
        fprintf(stderr, "Space saving: %.2f%%\n", 100 * (1-quotient));
    }
    return 0;
}
