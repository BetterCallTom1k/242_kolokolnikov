#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 64

static void err_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

static int parse_cmd(char *cmd, char *args[], int max)
{
    int n = 0;
    char *tok = strtok(cmd, " \t");
    while (tok && n < max - 1) {
        args[n++] = tok;
        tok = strtok(NULL, " \t");
    }
    args[n] = NULL;
    return n;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s \"cmd arg0 arg1\" ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        char *copy = strdup(argv[i]);
        if (!copy) err_exit("strdup");

        char *args[MAX_ARGS];
        if (parse_cmd(copy, args, MAX_ARGS) == 0) {
            free(copy);
            continue;
        }

        int pfd[2];
        if (pipe(pfd) == -1) err_exit("pipe");

        pid_t pid1 = fork();
        if (pid1 == -1) err_exit("fork");
        if (pid1 == 0) {
            close(pfd[0]);
            if (dup2(pfd[1], STDOUT_FILENO) == -1) err_exit("dup2");
            close(pfd[1]);
            execvp(args[0], args);
            err_exit("execvp");
        }

        pid_t pid2 = fork();
        if (pid2 == -1) err_exit("fork");
        if (pid2 == 0) {
            close(pfd[1]);
            if (dup2(pfd[0], STDIN_FILENO) == -1) err_exit("dup2");
            close(pfd[0]);
            execlp("wc", "wc", "-c", (char *)NULL);
            err_exit("execlp wc");
        }

        close(pfd[0]);
        close(pfd[1]);
        if (waitpid(pid1, NULL, 0) == -1) err_exit("waitpid");
        if (waitpid(pid2, NULL, 0) == -1) err_exit("waitpid");

        free(copy);
    }

    return 0;
}
