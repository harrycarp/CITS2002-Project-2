//
// Created by harrycarpenter on 28/09/2020.
//

#define  _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "sysbin.h"
#include <string.h>

char *expand_tars(int tarc, char *tars[], char *dir_name){

    printf("running extract into folder: %s\n", dir_name);

    for(int i = 0; i < tarc; i++){
        //construct the command to extract the tar file to the temp directory
        char* tar_xf_cmd = malloc(1024 * sizeof(char));
        char* tar_xf_dir = malloc(512 * sizeof(char));

        //construct the sub-directory for comparison extraction
        //Since sysctl can't deal with relative paths, we need to convert it to absolute

        //first we get the current working directory
        //limiting to 255 bytes, but can be increased for rather large dir trees
        char *cwd = malloc(255);
        getcwd(cwd, 255);

        sprintf(tar_xf_dir,"%s/%i", dir_name, i);
        mkdir(tar_xf_dir ,0777);

        // depending on whether or not the path is absolute, it will append the working directory
        if(tars[i][0] == *"/"){
            sprintf(tar_xf_cmd,"tar -T \"%s\" -C %s", tars[i], tar_xf_dir);
        } else {
            sprintf(tar_xf_cmd,"tar -T \"%s/%s\" -C %s", cwd, tars[i], tar_xf_dir);
        }

        char *tar_loc  = malloc(512 * sizeof(char));

        //check if tar origin is absolute or relative
        if(tars[i][0] == *"/"){
            sprintf(tar_loc, "\"%s\"", tars[i]);
        } else {
            sprintf(tar_loc, "\"%s/%s\"", cwd, tars[i]);
        }

//        printf("tar loc val: %s\n", tar_loc);

        char * args[6] = {
                "tar",
                "-xvf",
                tar_loc,
                "-C",
                tar_xf_dir,
                NULL
        };

        if(run_cmd(args) == -1){
            perror("tar command failed\n");
            free(tar_xf_cmd);
            exit(0);
        }

        // free up memory used in creating the tar command
        // don't you just "love" string formatting in C <3
        free(tar_xf_cmd);
    }
    return dir_name;
}