#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 64
#define MAX_COMMANDS 16

typedef struct {
    char *argv[MAX_ARGS];
    char *input_file;
    char *output_file;
    int append;
    int background;
} Command;

typedef struct {
    Command commands[MAX_COMMANDS];
    int command_count;
    int background;
} Pipeline;

int parse_line(const char *line, Pipeline *pipeline);
void free_pipeline(Pipeline *pipeline);
void print_pipeline(const Pipeline *pipeline);

#endif
