//
// Created by harrycarpenter on 28/09/2020.
//

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int copy_files(char * source, char * dest) {

    int src_fd, dst_fd, n, err;
    unsigned char buffer[4096];
    char * src_path, dst_path;

    // Assume that the program takes two arguments the source path followed
    // by the destination path.

    src_path = source;
    dst_path = dest;

    src_fd = open(src_path, O_RDONLY);
    dst_fd = open(dst_path, O_CREAT | O_WRONLY);

    while (1) {
        err = read(src_fd, buffer, 4096);
        if (err == -1) {
            printf("Error reading file.\n");
            return 1;
        }
        n = err;

        if (n == 0) break;

        err = write(dst_fd, buffer, n);
        if (err == -1) {
            printf("Error writing to file.\n");
            return 1;
        }
    }

    close(src_fd);
    close(dst_fd);
}