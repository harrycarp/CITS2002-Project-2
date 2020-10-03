CC = gcc
CFLAGS=-std=c99

mergetars: mergetars.c mergetars.h expand_tars.c compare_files.c copy_files.c
	gcc -o mergetars  mergetars.c mergetars.h expand_tars.c compare_files.c copy_files.c
