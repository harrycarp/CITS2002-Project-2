//
// Created by harrycarpenter on 10/10/2020.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

// ------- inline function definitions ------- //
char split_command(char * command);
// ------- end inline definitions ------------ //

int run_cmd(char *input) {
    pid_t pid;
    int status;
    pid_t ret;
    char *const args[3] = {"/bin/sh", input, NULL};
    char **env;
    extern char **environ;

    /* ... Sanitize arguments ... */

    printf("extracting using command:\n %s\n", input);

    pid = fork();
    if (pid == -1) {
        /* Handle error */
        perror("failed to fork process");
        return -1;
    } else if (pid != 0) {
        while ((ret = waitpid(pid, &status, 0)) == -1) {
            if (errno != EINTR) {
                /* Handle error */
                perror("unexpected pid error");
                break;
            }
        }
        if ((ret != -1) &&
            (!WIFEXITED(status) || !WEXITSTATUS(status)) ) {
            /* Report unexpected child status */
            printf("unexpected child status!\n");
            return -1;
        }
    } else {
        /* ... Initialize env as a sanitized copy of environ ... */
        if (execv("/bin/sh", args) == -1) {
            /* Handle error */
            perror("failed to run execve");
            exit(127);
        }

    }
    return 0;
}

char split_command(char * command){
    int i = 0;
    char *p = strtok (command, " ");
    char *array[3];

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, "/");
    }

    for (i = 0; i < 3; ++i)
        printf("%s\n", array[i]);

    return array;
}