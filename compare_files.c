//
// Created by harrycarpenter on 28/09/2020.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mergetars.h"
#include <unistd.h>

#define BUFFER_SIZE 1024


// --------------- in-line definitions -------------------//

int is_regular_file(char *dir);
bool exists_in_dir(char *file_name, char *dir);

//-------------- end in-line definitions -----------------//


/*
 * In this I am assuming that we are only dealing with files and folders,
 * and NOT devices, pipes, symbolic links, sockets etc.
 */
void compare_files(char *temp_directory, int tarc){
    printf("comparing files for merge....\n");

    // create OUT directory
    char * temp_out = (char *) malloc(100);
    sprintf(temp_out, "%s/temp_out", temp_directory);
    mkdir(temp_out, 0777);

    // check if creations successful
    if(!opendir(temp_out)){
        perror("failed to create temp out folder");
    }

    int files_processed = 0;

    //loop through each tar count, to nav to the folder
    for(int i = 0; i < tarc; i++){
        printf("\n");
        char * tarc_folder = (char *) malloc(100);
        sprintf(tarc_folder, "%s/%i", temp_directory, i);

        printf("scouting TAR folder: %s\n", tarc_folder);

        DIR *o_dir = opendir(tarc_folder);

        if(o_dir == NULL){
            perror("failed to open directory");
            continue;
        }

        struct dirent  *dp;

        while ((dp = readdir(o_dir)) != NULL) {

            //check if trying to read current or parent directory, if so, skip this loop
            if(strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0) continue;
            printf("%i", dp->d_type);

            if(dp->d_type == DT_DIR) {
                printf("IS FOLDER: %s\n", dp->d_name);
            }

            printf("processing file %s\n", dp->d_name);


            // if first file to be processed, copy it.
            // this way we don't use any unwanted memory searching the empty output directory
            if(files_processed == 0){
                temp_out = (char *) malloc(100);
                sprintf(temp_out, "%s/temp_out/%s", temp_directory, dp->d_name);


                char * source_file = (char *) malloc(100);
                sprintf(source_file, "%s/%s", tarc_folder, dp->d_name);

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

                strcat(f_out_check, dp->d_name);

                // construct directory variables
                DIR *temp_out_dir = opendir(temp_out);
                struct dirent *out;


                bool can_copy = false;
                printf("access: %i\n", access( f_out_check, F_OK ));
                if( access( f_out_check, F_OK ) != -1 ) {
                    // the file exists, time to compare and override
                    printf("file already exists - now to compare\n");
                } else {
                    // not same, happy to copy
                    can_copy = true;
                }

                if(can_copy){
                    char * s1 = (char *) malloc(100);
                    sprintf(s1, "%s/%s", tarc_folder, dp->d_name);

                    if(cp(temp_out, s1) != 0) {
                       perror("failed to copy file");
                    }

                    files_processed++;
                    free(s1);
                    break;
                }
            }
        }

        free(tarc_folder);
    }

    free(temp_out);

    printf("\nfinished, processed %i files\n", files_processed);

//    DIR *o_dir = opendir(temp_directory);
//
//    if(o_dir == NULL){
//        printf("\n");
//        perror("Error, failed to open directory:");
//    }
//
//    struct dirent  *dp;
//
//    int files_processed = 0;
//
//    while ((dp = readdir(o_dir)) != NULL) {
//
//    }

}

/*
 * This function checks to see if the file is indeed a file, or a folder to be
 * drilled into later.
 */
int is_regular_file(char *dir) {
    struct stat path_stat;
    stat(dir, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool exists_in_dir(char *file_name, char *dir){
    struct dirent  *entry;
    DIR *output_dir = opendir(dir);
    bool exists = false;
    while ((entry = readdir(output_dir)) != NULL) {
        if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
        printf("entry: %s, dp: %s\n", entry->d_name, file_name);
        if(entry->d_name == file_name){
            printf("\nFILE EXISTS\n");
            //if same file name, check which is newer
            struct stat entry_stat;
            struct stat new_stat;

            printf(" Copied modify time %s\n",
                   ctime(&entry_stat.st_mtime)
            );

            printf(" New modify time %s\n",
                   ctime(&new_stat.st_mtime)
            );
            return true;
        }
    }

    return false;
}