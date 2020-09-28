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
#include "mergetars.h"


#define BUFFER_SIZE 1024


// --------------- in-line definitions -------------------//

int is_regular_file(char *dir);
static int bufcmp(const void *p, const void *q, size_t n);

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

            if(files_processed == 0){
                char * f_out = (char *) malloc(100);
                strcpy(f_out, temp_directory);
                strcat(f_out, "/out");
                if(copy_files(f_dir, f_out) != 0) {
                    printf("\n");
                    perror("Error, failed to copy file");
                }
            }

            files_processed++;
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