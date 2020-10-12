//
// Created by harrycarpenter on 10/10/2020.
//

#include <stdlib.h>
#include <stdio.h>

/**
 * For this cleanup command I simply ran the rm -rf command as this program is unix based.
 * This is the most efficient method as looping through every file and sub-directory
 * Is time consuming and bad practice when it comes to efficiency.
 * @param temp_dir: the directory to clean
 * @return success state (0 for pass, -1 for fail)
 */
int cleanup (char * temp_dir) {

    char * delete_command = (char *) malloc(128);
    sprintf(delete_command, "rm -rf %s", temp_dir);

    if(system(delete_command) == -1){
        perror("sys del failed");
        return -1;
    }

    printf("cleanup successful\n");
    return 0;
}