#include "parser.h"
#include "stdint.h"

struct Parser {
    FILE *f;
    char current_line[MAX_PARSER_LINE_LENGTH + 1];
    uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
    Parser *p = (Parser *) malloc(sizeof(Parser));
    if (p != NULL) {
        p->f = f;
        p->current_line = 0;
        p->line_offset = 0;
    }
    return p;
}

void parser_delete(Parser **p) {
    free(*p);
    p = NULL;
}

bool next_word(Parser *p, char *word);
