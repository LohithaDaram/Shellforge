#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "expand.h"
#include "parser.h"

static void print_tokens(const Pipeline *pipeline)
{
    int i;
    int j;
    int token_number = 0;

    printf("\n========== TOKENS ==========\n");

    for (i = 0; i < pipeline->command_count; i++) {
        for (j = 0; pipeline->commands[i].argv[j] != NULL; j++) {
            printf("%d : WORD\t%s\n",
                   token_number++,
                   pipeline->commands[i].argv[j]);
        }
    }

    printf("%d : END\tEND\n", token_number);
    printf("============================\n");
}

int main(void)
{
    char *line;

    printf("\n====================================\n");
    printf("A Unix Style Shell written in C\n");
    printf("====================================\n");

    while ((line = readline("shellforge$ ")) != NULL) {
        Pipeline pipeline;

        if (*line != '\0')
            add_history(line);

        if (*line == '\0') {
            free(line);
            continue;
        }

        if (parse_line(line, &pipeline) != 0) {
            fprintf(stderr, "Parse error: invalid command syntax\n");
            free(line);
            continue;
        }

        expand_pipeline(&pipeline);
        print_tokens(&pipeline);
        print_pipeline(&pipeline);

        free_pipeline(&pipeline);
        free(line);
    }

    printf("\n");
    return 0;
}
