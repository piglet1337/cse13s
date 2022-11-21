
//Include Libraries
#include "bf.h"
#include "bv.h"
#include "city.h"
#include "ht.h"
#include "ll.h"
#include "node.h"
#include "parser.h"
#include "messages.h"
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
    bool stats = false;
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
            stats = true;
            break;
        case 'h':
            printf("Usage: ./banhammer [options]\n  ./banhammer will read in words from stdin, identify any badspeak or old speak and output an\n  appropriate punishment message. The badspeak and oldspeak (with the newspeak translation)\n  that caused the punishment will be printed after the message. If statistics are enabled\n  punishment messages are suppressed and only statistics will be printed.\n    -t <ht_size>: Hash table size set to <ht_size>. (default: 10000)\n    -f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)\n    -s          : Enables the printing of statistics.\n    -m          : Enables move-to-front rule.\n    -h          : Display program synopsis and usage.\n");
            return 0;
        default:
            printf("Usage: ./banhammer [options]\n  ./banhammer will read in words from stdin, identify any badspeak or old speak and output an\n  appropriate punishment message. The badspeak and oldspeak (with the newspeak translation)\n  that caused the punishment will be printed after the message. If statistics are enabled\n  punishment messages are suppressed and only statistics will be printed.\n    -t <ht_size>: Hash table size set to <ht_size>. (default: 10000)\n    -f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)\n    -s          : Enables the printing of statistics.\n    -m          : Enables move-to-front rule.\n    -h          : Display program synopsis and usage.\n");
            return 0;
        }
    }
    //initialize Bloom filter and hash table
    BloomFilter *bf = bf_create(bf_size);
    HashTable *ht = ht_create(ht_size, mtf);
    //open badspeak.txt and newspeak.txt
    FILE *badspeak;
    badspeak = fopen("badspeak.txt", "r");
    FILE *newspeak;
    newspeak = fopen("newspeak.txt", "r");
    //fill Bloom filter and hash table with badspeak
    while (1) {
        char str[30];
        if (fgets(str, 30, badspeak) == NULL) {break;}
        bf_insert(bf, str);
        ht_insert(ht, str, NULL);
    }
    //fill Bloom filter and hash table with newspeak
    while (1) {
        char str[60];
        char oldspeak_str[30];
        char newspeak_str[30];
        if (fgets(str, 60, newspeak) == NULL) {break;}
        int i;
        for (i = 0; str[i] != ' '; i += 1) {
            oldspeak_str[i] = str[i];
        }
        oldspeak_str[i] = '\0';
        i += 1;
        for (int j = 0; str[i] != '\0'; j += 1) {
            newspeak_str[j] = oldspeak_str[i];
        }
        newspeak_str[i] = '\0';
        bf_insert(bf, oldspeak_str);
        ht_insert(ht, oldspeak_str, newspeak_str);
    }
    //create parser
    Parser *p = parser_create(stdin);
    //create linked lists for badspeak and oldspeak
    LinkedList *badspeak_list = ll_create(mtf);
    LinkedList *oldspeak_list = ll_create(mtf);
    while (1) {
        //get next word from parser
        char word[30];
        next_word(p, word);
        //check if word is in the Bloom filter
        if (bf_probe(bf, word)) {
            //check if word is in hashtable
            Node *hash_result = ht_lookup(ht, word);
            if (hash_result == NULL) {continue;}
            if (ll_lookup(oldspeak_list, hash_result->oldspeak) != NULL) {continue;}
            //add word to oldspeak_list or badspeak_list if necessary
            if (hash_result->newspeak) {
                ll_insert(oldspeak_list, hash_result->oldspeak, hash_result->newspeak);
                continue;
            }
            if (hash_result->oldspeak) {
                ll_insert(badspeak_list, hash_result->oldspeak, NULL);
                continue;
            }
        }
    }
    if (stats) {
        return 0;
    }
    if (ll_length(badspeak_list) != 0 && ll_length(oldspeak_list) != 0) {
        printf("%s", mixspeak_message);
        ll_print(badspeak_list);
        ll_print(oldspeak_list);
    }
    else if (ll_length(badspeak_list) != 0) {
        printf("%s", badspeak_message);
        ll_print(badspeak_list);
    }
    else if (ll_length(oldspeak_list) != 0) {
        printf("%s", goodspeak_message);
        ll_print(oldspeak_list);
    }
    return 0;
}
