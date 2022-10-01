#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SMASH_RL_BUFSIZE 1024
#define SMASH_TOK_BUFSIZE 64
#define SMASH_TOK_DELIM " \t\r\n\a"

char *smash_read_line() {
    int bufsize = SMASH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "smash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a char
        c = getchar();

        // if hit EOF, replace it with a null char and return 
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // if exceeded the buffer; reallocate
        if (position >= bufsize) {
            bufsize += SMASH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "smash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **smash_split_line(char *line) {
    int bufsize = SMASH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "smash: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, SMASH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += SMASH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "smash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SMASH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
void smash_execute(char **args) {
}
void smash_loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = smash_read_line();
        args = smash_split_line(line);

        free(line);
        free(args);
    } while (status);
}
int main(int argc, char **argv) {
    // load config files, 
    // run command loop
    smash_loop();
    // perform any shutdown/cleanup
    return EXIT_SUCCESS;
}
