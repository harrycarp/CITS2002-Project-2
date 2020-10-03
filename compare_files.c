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


#define BUFFER_SIZE 1024


// --------------- in-line definitions -------------------//

int is_regular_file(char *dir);
static int bufcmp(const void *p, const void *q, size_t n);
bool exists_in_dir(char *file_name, char *dir);

//-------------- end in-line definitions -----------------//


/*
 * In this I am assuming that we are only dealing with files and folders,
 * and NOT devices, pipes, symbolic links, sockets etc.
 */
void compare_files(char *temp_directory){
    printf("comparing files for merge....\n");

    DIR *o_dir = opendir(temp_directory);

    if(o_dir == NULL){
        printf("\n");
        perror("Error, failed to open directory:");
    }

    struct dirent  *dp;

    int files_processed = 0;

    while ((dp = readdir(o_dir)) != NULL) {
        struct stat buffer;
        int status;

        // check if the readdir is the current folder or parent, if so, don't run the compare on it
        if(strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0) continue;

        char * f_dir = (char *) malloc(100);
        strcpy(f_dir, temp_directory);
        strcat(f_dir, "/");
        strcat(f_dir, dp->d_name);
        if(is_regular_file(f_dir) == 1){
            // copy the first file, doesn't need comparing
            char * f_out = (char *) malloc(100);
            strcpy(f_out, temp_directory);
            strcat(f_out, "/out/");
            if(files_processed == 0){
                printf("copying file to %s\n", f_out);
                if(copy_files(f_dir, f_out, dp->d_name) != 0) {
                    printf("\n");
                    perror("Error, failed to copy file");
                }
            } else {
                // for the other files, time to compare them to the already copies
                if(!exists_in_dir(dp->d_name, f_out)) {
                    printf("DOESNT EXIST\n");
                    char *f_out = (char *) malloc(100);
                    strcpy(f_out, temp_directory);
                    strcat(f_out, "/out/");
                    printf("copying file to %s\n", f_out);
                    if (copy_files(f_dir, f_out, dp->d_name) != 0) {
                        printf("\n");
                        perror("Error, failed to copy file");
                    }
                }

            }
            files_processed++;
        } else {
            // this will be for copying folders, will need to drill down into the folder to comp the files
            // I wonder if I can just make this func drill recurring?
            // compare_files(f_dir);
        }
        free(f_dir);
    }

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

static int bufcmp(const void *p, const void *q, size_t n)
{
    const char *p1 = p;
    const char *p2 = q;

    while (n-- > 0) {
        if (*p1++ != *p2++)
            return 0;
    }

    return 1;
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