# CITS2002 Project 2

   A medium-sized business has decided to migrate its files to 
   cloud-based storage, requiring it to first identify all files to migrate. 
   A critical disk failure at the worst possible time now requires all files 
   to be recovered from recent backups. However, the business' IT wizard has 
   recently left for a lucrative position at a cloud-based storage company.
    
   Management has located the backups, but they have been poorly labeled, 
   making it impossible to easily identify what is contained in each 
   backup and when each was made. The decision has been made to migrate 
   just the latest copy of each file to the cloud, which will require 
   an 'intelligent merging' of the backups' contents.

   The backups have been made using the widely available tar command, 
   a well-defined file format whose name is a contraction of tape 
   archive, reflecting the backup media with which the command was 
   first used. While the tar command supports many actions to create, 
   list, extract, and append tar archive files, it offers no support 
   to merge archives together.
   
   The business has located many backups each holding thousands of files. 
   The task to identify all duplicate files, and to find the most recent 
   version of similar files, is too large to be performed manually, and 
   your team has been contracted to develop a new command-line utility 
   program to intelligently merge all of the backups' 
   contents into a single (large) tar archive.
   
    The goal of this project is to write a command-line utility program in C99, 
    named mergetars, which merges the contents of multiple tar archive files into a 
    single tar archive file.
     
    Successful completion of the project will develop your understanding of 
    advanced features of the C99 programming language, and many useful Linux 
    operating system system-calls and POSIX function calls.
    
### Program Invocation

The purpose of your mergetars command-line utility is to merge the contents of multiple tar archive files into a single 
tar archive. The program receives the name of one or more input filenames, and a single output filename (if only a 
single input filename is provided, then mergetars will act like a simple file-copying program, although there is no 
requirement to check for this special case). 

A typical program invocation is:

    prompt>  ./mergetars input_tarfile1 [input_tarfile2 ...] output_tarfile
    
Filenames will always end with the suffix  .tar   – indicating that the archive does not involve any compression – or 
with the suffix  .tar.gz  or  .tgz  – indicating that the archive is (or will be) compressed using the GZIP compression 
algorithm. The standard tar utility supports these cases using its  -z  command-line option. There is no requirement for 
mergetars to support any other compression schemes

### The merging criteria

The inputs are merged to form the output according to the following definitions and rules:

* Two or more files from different archives are considered the same file if they have the same relative pathname 
(including the name of the directory holding the file and the filename itself).

    If from different archives, the files "project2/src/mergetars.c" and "project2/src/mergetars.c" are considered the same file.
    In contrast, "monday/project2/src/mergetars.c" and "wednesday/project2/src/mergetars.c" are considered different files.
    
* Two or more files from different archives with the same relative pathname, are considered different versions of the 
same file. The output archive should contain just the latest of all such versions.

* All other files with different relative pathnames are considered different files. The output archive should contain 
one copy of each different file.

### Suggested Approach

The project can be completed by following these recommended (but not required) steps:

* Learn how to use the standard tar utility from the command-line.
* Create two or more 'similar' tar archives, and determine (by hand) which files should appear in a merged archive.
* Create 'skeleton' C source files containing 'empty' functions for each distinct responsibility. Create and test a 
Makefile to compile and link the files.
* Then, check the program's command-line arguments.
* Use your system's standard tar utility (called from your C99 code) to expand each input tar archive into a new 
directory.
* Identify and copy (to a new directory structure) all files that should be uniquely added to the output tar archive. 
Remember to set the modification time of each file, appropriately.
* Use your system's standard tar utility to create the new output tar archive.
* Cleanup up before exiting, removing any temporary files and directories that you have created.

It is anticipated (though not required) that a successful project will use (at least) the following system-calls, 
and standard C99 & POSIX functions: 
* perror(),  exit(), 
* mkdtemp(),  opendir(),  readdir(),  closedir(),  stat(), 
* fork(),  execl(),  wait(), 
* open(),  read(),  write(),  close(), utime(), 
* strcpy(),  strcmp(),  strdup(), 
* realloc(), and  free(). 

### Project Requirements

1. Your project must be developed in multiple source files and must be compiled and linked using a Makefile, containing 
approriate variable definitions and automatic variables.

2. The default target of your Makefile must be named mergetars, and its execution must produce a program named mergetars.

3. Your program must 'clean up after itself'. If your program creates any temporary files or directories, then these 
must all be removed before your program exits. Your program does not have to free all of its dynamically allocated 
memory before it exits.

4. Your project must employ sound programming practices, including the use of meaningful comments, well chosen 
identifier names; appropriate choice of basic data-structures, data-types, and functions; and appropriate choice of control-flow constructs.