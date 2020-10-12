//
// Created by harrycarpenter on 28/09/2020.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mergetars.h"
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024

void process_folder(struct dirent *dp, char * browsing_folder, char * out_dir);
bool is_newer(char * source, char * dest);

/*
 * In this I am assuming that we are only dealing with files and folders,
 * and NOT devices, pipes, symbolic links, sockets etc.
 */
int comparefiles(int tarc, char * temp_directory){
    printf("comparing files for merge....\n");

    // create OUT directory
    char * temp_out = (char *) malloc(100);
    sprintf(temp_out, "%s/temp_out", temp_directory);
    mkdir(temp_out, 0777);

    // check if creations successful
    if(!opendir(temp_out)){
        perror("failed to create temp out folder");
        return 1;
    }

    int files_processed = 0;

    //loop through each tar count, to nav to the folder
    for(int i = 0; i < tarc; i++){
        printf("\n");
        char * tarc_folder = (char *) malloc(512 * sizeof(char));
        sprintf(tarc_folder, "%s/%i", temp_directory, i);

        printf("scouting TAR folder: %s\n", tarc_folder);

        DIR *tar_dir = opendir(tarc_folder);

        if(tar_dir == NULL){
            perror("failed to open directory");
            continue;
        }

        struct dirent *entry;

        while ((entry = readdir(tar_dir)) != NULL) {
            //check if trying to read current or parent directory, if so, skip this loop
            if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;


            // skip over folder
            // there is some weird glitch where this loop doesn't detect folders, but if i put the EXACT
            // same structured loop later down the stack, it finds folders perfectly fine...?
            // if anyone could shed light on this that'd be great :)
            if(entry->d_type == DT_DIR) continue;


            printf("entity type: %i\n", entry->d_type);
            printf("processing file %s\n", entry->d_name);

            // if first file to be processed, copy it.
            // this way we don't use any unwanted memory searching the empty output directory
            if(files_processed == 0){
                temp_out = (char *) malloc(100);
                sprintf(temp_out, "%s/temp_out/%s", temp_directory, entry->d_name);


                char * source_file = (char *) malloc(100);
                sprintf(source_file, "%s/%s", tarc_folder, entry->d_name);

                printf("copy file %s\n", source_file);

                if(cp(temp_out, source_file) != 0) {
                    printf("\n"); perror("failed to copy file :(");
                }

                files_processed++;
                free(source_file);
            }
            else{
                printf("not first file, compare with out dir\n");
                temp_out = (char *) malloc(100);
                sprintf(temp_out, "%s/temp_out/", temp_directory);

                char *f_out_check = temp_out;

                strcat(f_out_check, entry->d_name);

                // construct directory variables
                DIR *temp_out_dir = opendir(temp_out);
                struct dirent *out;


                bool can_copy = false;
                printf("access: %i\n", access( f_out_check, F_OK ));
                if( access( f_out_check, F_OK ) != -1 ) {
                    // the file exists, time to compare and override
                    printf("file already exists - now to compare\n");

                    // s1 is the file found in tar, f_check_out is the file already in output
                    char * s1 = (char *) malloc(100);
                    sprintf(s1, "%s/%s", tarc_folder, entry->d_name);

                    if(is_newer(s1, f_out_check)){
                        printf("in tar is newer that in the output folder\n");
                    } else {
                        printf("not newer, do not override\n");
                    }

                } else {
                    // not same, happy to copy
                    can_copy = true;
                }

                if(can_copy){
                    char * s1 = (char *) malloc(100);
                    sprintf(s1, "%s/%s", tarc_folder, entry->d_name);

                    if(cp(temp_out, s1) != 0) {
                       perror("failed to copy file");
                    }

                    files_processed++;
                    free(s1);
                    break;
                }
                printf("\n");
            }

        }

        printf("\nfinished, processed %i files, moving onto folders\n\n", files_processed);

        struct dirent *dp;
        while ((dp = readdir(tar_dir)) != NULL) {
            // skip files that aren't directories
            printf("dpfile: %s\n", dp->d_name);
            if(dp->d_type != DT_DIR) continue;
            if(strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0) continue;\

            char * temp_out = (char *) malloc(100);
            sprintf(temp_out, "%s/temp_out", temp_directory);

            process_folder(dp, tarc_folder, temp_out);
        }
        printf("\n");
        free(tarc_folder);
    }

    free(temp_out);

    return 0;
}


/**
 * Loops through the folder that's being scouted
 * If it finds another folder, it call's itself and repeats the process :).
 * @param dp
 * @param browsing_folder
 * @param out_dir
 */
void process_folder(struct dirent *dp, char * browsing_folder, char * out_dir){

    // for some reason this only works for one layer of folders, and if there's
    // multiple sub directories, it just fails to fire once... strange...


    printf("outdir: %s\n", out_dir);
    char * dir_to_make = malloc(1024 * sizeof(char));
    sprintf(dir_to_make, "%s/%s", out_dir, dp->d_name);
//    printf("dir to make: %s\n", dir_to_make);

    // if the folder doesn't already exist, make it!
    if(access( dir_to_make, F_OK ) == -1) {
        mkdir(dir_to_make, 0777);
    }

    char * dir_to_browse = malloc(1024 * sizeof(char));
    sprintf(dir_to_browse, "%s/%s", browsing_folder, dp->d_name);

    DIR *fdir = opendir(dir_to_browse);
    struct dirent *content;

    while ((content = readdir(fdir)) != NULL) {
        if(strcmp(".", content->d_name) == 0 || strcmp("..", content->d_name) == 0) continue;
        char * dest_dir = malloc(1024 * sizeof(char));
        sprintf(dest_dir, "%s/%s", dir_to_make, content->d_name);

        char * source_dir = malloc(1024 * sizeof(char));
        sprintf(source_dir, "%s/%s", dir_to_browse, content->d_name);

//        printf("source dir:%s\ndest_dir:%s\n", source_dir, dest_dir);

        if(content->d_type == 8){
            if(access( dest_dir, F_OK ) != -1){
                //folder already exists, run through content and copy across necessary
                printf("file already exists in this folder\n");

                char * s1 = (char *) malloc(100);
                sprintf(s1, "%s/%s", dir_to_browse, content->d_name);


                if(is_newer(s1, dest_dir)){
                    //in tar is newer that in the output folder, therefore copy it across
                    cp(dest_dir, source_dir);
                }

            } else {
                cp(dest_dir, source_dir);
            }

        } else if ((content->d_type) == DT_DIR){
            // if the the child is a folder, repeat the process using the working directory of this folder
            process_folder(content, source_dir, dest_dir);
        }

        free(dest_dir);
        free(source_dir);

    }

    free(dir_to_browse);
    free(dir_to_make);
}

/**
 * This function is pretty self explanatory, it checks which file, based off source strings, is newer.
 * @param source : the source file
 * @param dest : the destination file
 * @return if the source file is newer than the destination file
 */
bool is_newer(char * source, char * dest){
    struct stat tar_attrib; // attribute for the file in tar to be (potentially) copied
    stat(source, &tar_attrib);
    char time_in[50];
    strftime(time_in, 50, "%Y-%m-%d %H:%M:%S", localtime(&tar_attrib.st_mtime));
    printf ("in tar: %s\n", time_in);

    struct stat out_attrib; // attribute for the file in tar to be (potentially) copied
    stat(dest, &out_attrib);
    char time_out[50];
    strftime(time_out, 50, "%Y-%m-%d %H:%M:%S", localtime(&out_attrib.st_mtime));
    printf ("in out folder: %s\n", time_out);

    double t_diff = difftime((time_t) time_in, (time_t) time_out);

    return (t_diff > 0);
}