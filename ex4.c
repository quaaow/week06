#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

int main() {
    char line[1024];
    char *args[64];
    char path[256];

    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0);

        printf("> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        int i = 0;
        char *token = strtok(line, " \t");
        while (token != NULL && i < 63) {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        pid_t pid = fork();
        if (pid == 0) {
            if (args[0][0] == '/' || args[0][0] == '.') {
                execve(args[0], args, environ);
            } else {
                snprintf(path, sizeof(path), "/bin/%s", args[0]);
                execve(path, args, environ);
                snprintf(path, sizeof(path), "/usr/bin/%s", args[0]);
                execve(path, args, environ);
            }
            execvp(args[0], args);
            perror("execve");
            exit(1);
        }
        usleep(50000);
    }

    return 0;
}