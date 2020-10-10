//
// Created by Harry.Carpenter on 25/09/2020.
//

#ifndef CITS2002_PROJECT_2_MERGETARS_H
#define CITS2002_PROJECT_2_MERGETARS_H


int expand_tars(int tarc, char *tars[], char *dir_name);
void compare_files(char *dir_name, int tarc);
int is_regular_file(char *dir);
int copy_files(char * source, char * dest, char * file_name);
int cp(const char * to, const char * from);
int create_output_tar(char * temp_directory, char * output_file);
int cleanup (char * temp_dir);

#endif //CITS2002_PROJECT_2_MERGETARS_H
