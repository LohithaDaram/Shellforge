#ifndef EXPAND_H
#define EXPAND_H

#include "parser.h"

/* Expands ~, $VARIABLE, and ${VARIABLE}. */
char *expand_word(const char *word);

/* Expands every argument and redirect filename in one command. */
void expand_command(Command *command);

/* Expands every command in a pipeline. */
void expand_pipeline(Pipeline *pipeline);

#endif
