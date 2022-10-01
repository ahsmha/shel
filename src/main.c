#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SMASH_RL_BUFSIZE 1024
#define SMASH_TOK_BUFSIZE 64
#define SMASH_TOK_DELIM " \t\r\n\a"

/* 
 * functions for builtin shell commands:
 * cd
 * help
 * exit
 */
int smash_cd(char **args);
int smash_help(char **args);
int smash_exit(char **args);

/*
 * list of builtin commands
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &smash_cd,
    &smash_help,
    &smash_exit
};

int smash_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
 * Buitin function implementations
 */
int smash_cd(char **args) 
{
    if (args[1] == NULL) {
        fprintf(stderr, "smash: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1] != 0)) {
            perror("smash");
        }
    }
    return 1;
}

int smash_help(char **args) 
{
    int i;
    printf("Ahmed's SMASH\n");
    printf("Type program names and args, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < smash_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int smash_exit(char **args) 
{
    return 0;
}

char *smash_read_line() 
{
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

char **smash_split_line(char *line) 
{
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
int smash_launch(char **args) 
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            perror("smash");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // error forking
        perror("smash");
    } else {
        // parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
int smash_execute(char **args) 
{
    int i;

    if (args[0] == NULL) {
        // empty command was entered
        return 1;
    }

    for (i = 0; i < smash_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return smash_launch(args);
}
void smash_loop() 
{
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
int main(int argc, char **argv) 
{
    // load config files, 
    // run command loop
    smash_loop();
    // perform any shutdown/cleanup
    return EXIT_SUCCESS;
}
