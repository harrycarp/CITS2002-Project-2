//
// Created by harrycarpenter on 28/09/2020.
//

#define  _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

char *expand_tars(int tarc, char *tars[], char *dir_name){

    printf("running extract into folder: %s\n", dir_name);

    for(int i = 0; i < tarc; i++){
        //construct the command to extract the tar file to the temp directory
        char* tar_xf_cmd = malloc(128 * sizeof(char));
        char* tar_xf_dir = malloc(128 * sizeof(char));

        //construct the sub-directory for comparison extraction
        sprintf(tar_xf_dir,"%s/%i", dir_name, i);
        mkdir(tar_xf_dir ,0777);
        sprintf(tar_xf_cmd,"tar -xf %s -C %s", tars[i], tar_xf_dir);

        //TODO: replace with execl, fork and wait for running command
        if(system(tar_xf_cmd) == -1){
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