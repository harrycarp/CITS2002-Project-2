//
// Created by harrycarpenter on 10/10/2020.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int run_cmd(char *input) {
    pid_t pid;
    int status;
    pid_t ret;
    char *const args[3] = { input, NULL};
    char **env;
    extern char **environ;


    /* ... Sanitize arguments ... */

    pid = fork();
    if (pid == -1) {
        /* Handle error */
        perror("invalid pid for fork command\n");
        return -1;
    }  else {
        /* ... Initialize env as a sanitized copy of environ ... */
        printf("running command %s\n", input);
        if (execl("/bin/sh", "/bin/sh", "-c", input, 0) == -1) {
            /* Handle error */
            perror("exec error");
            _Exit(127);
        } else {
            printf("cmd success!\n");
            return 0;
        }
    }


}