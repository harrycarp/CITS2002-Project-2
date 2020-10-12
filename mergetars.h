//
// Created by Harry.Carpenter on 25/09/2020.
//

#ifndef CITS2002_PROJECT_2_MERGETARS_H
#define CITS2002_PROJECT_2_MERGETARS_H

int comparefiles(int tarc, char *temp_directory);
int create_output_tar(char * temp_directory, char * output_file);
int cleanup (char * temp_dir);
int expand_tars(int tarc, char *tars[], char *dir_name);
int cp(const char * to, const char * from);

#endif //CITS2002_PROJECT_2_MERGETARS_H
