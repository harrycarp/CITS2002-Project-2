//
// Created by harrycarpenter on 10/10/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int create_output_tar(char * temp_directory, char * output_file) {



    char * temp_out_dir = (char *) malloc(100);
    sprintf(temp_out_dir, "%s/temp_out", temp_directory);
    printf("scouting temp dir %s\n", temp_out_dir);

    DIR *temp_dir = opendir(temp_out_dir);

    struct dirent  *entity;


    char * command = (char *) malloc(1024);
    sprintf(command, "tar -cf %s ", output_file);

    while ((entity = readdir(temp_dir)) != NULL) {
        if(strcmp(".", entity->d_name) == 0 || strcmp("..", entity->d_name) == 0) continue;

        char * dir = (char *) malloc(128);
        sprintf(dir, "\"%s/%s\" ", temp_out_dir, entity->d_name);
        strcat(command, dir);
        free(dir);
    }

    printf("zipping up output with cmd: %s\n", command);

    if(system(command) == -1){
        perror("sys command failed");
    }

    free(command); free(temp_out_dir);

    return 0;
}