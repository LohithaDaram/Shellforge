#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "expand.h"

char *expand_word(const char *word)
{
    char result[4096];
    int out = 0;
    int i = 0;

    if (word == NULL)
        return NULL;

    /* Expand ~ or ~/... into the user's home directory */
    if (word[0] == '~' && (word[1] == '\0' || word[1] == '/')) {
        const char *home = getenv("HOME");

        if (home != NULL) {
            strcpy(result, home);
            out = (int)strlen(result);
            i = 1;
        }
    }

    /* Expand $VARIABLE and ${VARIABLE} */
    while (word[i] != '\0' && out < (int)sizeof(result) - 1) {
        if (word[i] == '$') {
            char variable[256];
            int variable_index = 0;
            const char *value;

            i++;

            if (word[i] == '{') {
                i++;

                while (word[i] != '\0' &&
                       word[i] != '}' &&
                       variable_index < 255) {
                    variable[variable_index++] = word[i++];
                }

                if (word[i] == '}')
                    i++;
            } else {
                while ((isalnum((unsigned char)word[i]) || word[i] == '_') &&
                       variable_index < 255) {
                    variable[variable_index++] = word[i++];
                }
            }

            variable[variable_index] = '\0';
            value = getenv(variable);

            if (value != NULL) {
                while (*value != '\0' &&
                       out < (int)sizeof(result) - 1) {
                    result[out++] = *value++;
                }
            }

            continue;
        }

        result[out++] = word[i++];
    }

    result[out] = '\0';

    {
        char *expanded = malloc(strlen(result) + 1);

        if (expanded != NULL)
            strcpy(expanded, result);

        return expanded;
    }
}

void expand_command(Command *command)
{
    int i;

    for (i = 0; command->argv[i] != NULL; i++) {
        char *expanded = expand_word(command->argv[i]);

        if (expanded != NULL) {
            free(command->argv[i]);
            command->argv[i] = expanded;
        }
    }

    if (command->input_file != NULL) {
        char *expanded = expand_word(command->input_file);

        if (expanded != NULL) {
            free(command->input_file);
            command->input_file = expanded;
        }
    }

    if (command->output_file != NULL) {
        char *expanded = expand_word(command->output_file);

        if (expanded != NULL) {
            free(command->output_file);
            command->output_file = expanded;
        }
    }
}

void expand_pipeline(Pipeline *pipeline)
{
    int i;

    for (i = 0; i < pipeline->command_count; i++)
        expand_command(&pipeline->commands[i]);
}
