//  CITS2002 Project 2 2020
//  Name(s):             Harry Carpenter, My Inner Conscience
//  Student number(s):   22723303, ---

#include "mergetars.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

// I suppose we're assuming that the user IS passing through valid .tar files.
// The program will throw if it's not able to be unzipped via tar command, so that's the "safeguard"
// if you could call it that.

int main(int argc, char *argv[]){

    if(argc == 1){
        printf("You have not provided any input criteria, aborting");
        exit(0);
    } else if( argc == 2) {
        printf("You need to provide both input and output files");
        exit(0);
    }

    char *tar_array[argc];
    int tarc = 0;
    for(int i = 1; i < argc-1; i++){
        tar_array[i-1] = argv[i];
        printf("->%s\n", tar_array[tarc]);
        tarc++;
    }


    // construct the temporary directory
    char template[] = "/tmp/tmpdir.XXXXXX";
    char *temp_dir = mkdtemp(template);
    // if the temp creation fails, drop everything and exit.
    if(temp_dir == NULL){ perror("mkdtemp failed: "); exit( 0); }

    //expand the tar files into the temp directory
    expand_tars(tarc, tar_array, temp_dir);

    // compare the extracted files for the merge
    // this will copy and valid files into a working _out directory within the temp folder.
    // this is so we can easily compartmentalise and work with duplicate files, because we
    // don't want to override anything, yet.
    compare_files(temp_dir);


    //this needs to propagate to the children in future, otherwise it won't work
    /* Don't forget to delete the folder afterwards. */
//    if(rmdir(temp_dir) == -1)
//    {
//        perror("rmdir failed: ");
//        return 0;
//    }

    return 0;
}
