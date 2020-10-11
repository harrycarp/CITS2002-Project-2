//
// Created by harrycarpenter on 11/10/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <stdbool.h>

void main(){


    char  root_cmd [] = "tar -xf \"/mnt/c/Users/harrycarpenter/OneDrive/Documents/UWA/Year 2/Semester 2/CITS2002/CITS2002-Project-2/cmake-build-debug/../testing_stage/tar3.tar\" -C /tmp/tmpdir.In8pvA/0";

    char ** res  = NULL;
    char *  p    = strtok (root_cmd, " ");
    int n_spaces = 0, i;


/* split string and append tokens to 'res' */

    while (p) {
        res = realloc (res, sizeof (char*) * ++n_spaces);

        if (res == NULL)
            exit (-1); /* memory allocation failed */


        res[n_spaces-1] = p;



        p = strtok (NULL, " ");
    }

    /* realloc one extra element for the last NULL */

    res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;

    /* print the result */

    bool is_in_quote = false;
    int q_ix = -1;
    for (i = 0; i < (n_spaces+1); ++i) {
        printf("qix=%i\n", q_ix);

        if(res[i][0] == *"\"" || res[i][strlen(res[i]) - 1] == *"\"") {
            if(is_in_quote) {
                is_in_quote = false;
                q_ix = -1;
            } else {
                is_in_quote = true;
            }
        }

        if(is_in_quote){
            if(q_ix == -1) q_ix = i;
            printf("entry: %s\n", res[i]);

            char * temp = res[q_ix];
            strcat(temp, res[i]);
            res[q_ix] = temp;
        }



        printf("[%d] = %s\n", i, res[i]);
    }
    /* free the memory allocated */

    free (res);

//    char *program_arguments[] = {
//            "ls",
//            "-l",
//            "-F",
//            NULL
//    };



    execv( "/bin/bash", res );
    exit(EXIT_FAILURE);

}

