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


int run_cmd(char *input[6]) {
    pid_t pid;
    int status;
    pid_t ret;
    char **env;
    extern char **environ;



    char * full_command = malloc(512 * sizeof(char));
    for(int i = 0; i < 7; i++){
        if(input[i] != NULL){
            strcat(full_command, input[i]);
            strcat(full_command, " ");
//            printf("%s\n", full_command);
        }
    }

    // please read below :)
    system(full_command);
    return 0;

    // !!!!!!!!!! READ ME !!!!!!!!!!!!!
    // Below here is my failed attempt at making the execv, execve or execl commands to actually work.
    // I got it to partially work, however, it returned a TAR not found even when the directory, relative or absolute,
    // was indeed valid. I ensured this by converting relative directoreis to absolute, as exec commands can't handle
    // relative paths.

    /* ... Sanitize arguments ... */

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

        return 0;
        // for some reason this isn't working :(
        // probably something tiny i'm overlooking

        if (execve("/bin/sh", input, env) == -1) {
            /* Handle error */
            perror("failed to run execve");
            exit(127);
        }

    }
    return 0;
}