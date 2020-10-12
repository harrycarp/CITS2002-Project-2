//
// Created by harrycarpenter on 11/10/2020.
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

int main(){

    char * path = "/tmp/tmpdir.SqByis/1";

    DIR *o_dir = opendir(path);

    struct dirent *dp;
    while ((dp = readdir(o_dir)) != NULL) {
        printf("entity name: %s\n", dp->d_name);
    }

    return 0;
}

