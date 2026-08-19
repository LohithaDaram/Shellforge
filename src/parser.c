#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

static char *copy_text(const char *text)
{
    char *copy = malloc(strlen(text) + 1);

    if (copy != NULL)
        strcpy(copy, text);

    return copy;
}

static void init_command(Command *command)
{
    memset(command, 0, sizeof(*command));
}

int parse_line(const char *line, Pipeline *pipeline)
{
    char buffer[2048];
    char *word;
    Command *command;
    int argument = 0;

    memset(pipeline, 0, sizeof(*pipeline));
    pipeline->command_count = 1;
    command = &pipeline->commands[0];
    init_command(command);

    strncpy(buffer, line, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    word = strtok(buffer, " \t");

    while (word != NULL) {
        if (strcmp(word, "|") == 0) {
            if (argument == 0 || pipeline->command_count >= MAX_COMMANDS)
                return -1;

            pipeline->command_count++;
            command = &pipeline->commands[pipeline->command_count - 1];
            init_command(command);
            argument = 0;
        } else if (strcmp(word, "<") == 0) {
            word = strtok(NULL, " \t");

            if (word == NULL)
                return -1;

            command->input_file = copy_text(word);
        } else if (strcmp(word, ">") == 0) {
            word = strtok(NULL, " \t");

            if (word == NULL)
                return -1;

            command->output_file = copy_text(word);
            command->append = 0;
        } else if (strcmp(word, ">>") == 0) {
            word = strtok(NULL, " \t");

            if (word == NULL)
                return -1;

            command->output_file = copy_text(word);
            command->append = 1;
        } else if (strcmp(word, "&") == 0) {
            pipeline->background = 1;
            command->background = 1;
        } else {
            if (argument >= MAX_ARGS - 1)
                return -1;

            command->argv[argument++] = copy_text(word);
            command->argv[argument] = NULL;
        }

        word = strtok(NULL, " \t");
    }

    return command->argv[0] == NULL ? -1 : 0;
}

void free_pipeline(Pipeline *pipeline)
{
    int i;
    int j;

    for (i = 0; i < pipeline->command_count; i++) {
        for (j = 0; pipeline->commands[i].argv[j] != NULL; j++)
            free(pipeline->commands[i].argv[j]);

        free(pipeline->commands[i].input_file);
        free(pipeline->commands[i].output_file);
    }

    memset(pipeline, 0, sizeof(*pipeline));
}

void print_pipeline(const Pipeline *pipeline)
{
    int i;
    int j;

    printf("\n========== PIPELINE ==========\n\n");

    for (i = 0; i < pipeline->command_count; i++) {
        const Command *command = &pipeline->commands[i];

        printf("Command %d\n", i + 1);
        printf("------------------------------\n");
        printf("Arguments\n");

        for (j = 0; command->argv[j] != NULL; j++)
            printf("argv[%d] = %s\n", j, command->argv[j]);

        printf("Input      : %s\n",
               command->input_file ? command->input_file : "None");
        printf("Output     : %s\n",
               command->output_file ? command->output_file : "None");
        printf("Append     : %s\n", command->append ? "Yes" : "No");
        printf("Background : %s\n",
               command->background ? "Yes" : "No");
        printf("==============================\n");
    }
}
