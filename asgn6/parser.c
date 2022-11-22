//include necessary libraries
#include "parser.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

//define structure for Parser
struct Parser {
  FILE *f;
  char current_line[MAX_PARSER_LINE_LENGTH + 1];
  uint32_t line_offset;
};

//creates a parser and returns it
Parser *parser_create(FILE *f) {
  Parser *p = (Parser *)malloc(sizeof(Parser));
  if (p != NULL) {
    p->f = f;
    p->line_offset = 0;
  }
  return p;
}

//deletes parser and frees used memory
void parser_delete(Parser **p) {
  free(*p);
  p = NULL;
}

//sets word equal to the next word in the parser and returns true
//returns false when it reaches the end of the file
bool next_word(Parser *p, char *word) {
  if (p->line_offset == 0) {
    if (!fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f)) {
      return false;
    }
  }
  while ((!isalnum(p->current_line[p->line_offset])) && p->current_line[p->line_offset] != '-' && p->current_line[p->line_offset] != '\'' && p->current_line[p->line_offset] != '\0' && p->current_line[p->line_offset] != '\n') {
    p->line_offset += 1;
  }
  if (p->current_line[p->line_offset] == '\0' || p->current_line[p->line_offset] == '\n') {
    p->line_offset = 0;
    if (next_word(p, word)) {
      return true;
    }
    return false;
  }
  int i = 0;
  while (isalnum(p->current_line[p->line_offset]) ||
         p->current_line[p->line_offset] == '-' ||
         p->current_line[p->line_offset] == '\'') {
    word[i] = tolower(p->current_line[p->line_offset]);
    i += 1;
    p->line_offset += 1;
  }
  word[i] = '\0';
  return true;
}
