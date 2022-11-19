#include "parser.h"
#include <stdlib.h>
#include <ctype.h>

struct Parser {
    FILE *f;
    char current_line[MAX_PARSER_LINE_LENGTH + 1];
    uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
    Parser *p = (Parser *) malloc(sizeof(Parser));
    if (p != NULL) {
        p->f = f;
        p->line_offset = 0;
    }
    return p;
}

void parser_delete(Parser **p) {
    free(p[0]->current_line);
    free(*p);
    p = NULL;
}

bool next_word(Parser *p, char *word) {
    if (p->line_offset == 0) {
        if(!fscanf(p->f, "%s", p->current_line)) {return false;}
    }
    while (p->current_line[p->line_offset] == ' ') {
        p->line_offset += 1;
    }
    if (p->current_line[p->line_offset] == '\0') {
            p->line_offset = 0;
            if (next_word(p, word)) {return true;}
            return false;
    }
    int i = 0;
    while (isalnum(p->current_line[p->line_offset]) || p->current_line[p->line_offset] == '-' || p->current_line[p->line_offset] == '\'') {
        word[i] = tolower(p->current_line[p->line_offset]);
        i += 1;
        p->line_offset += 1;
    }
    word[i] = '\0';
    return true;
}