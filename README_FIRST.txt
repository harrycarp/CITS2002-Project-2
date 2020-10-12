CITS2002 Project 2
Harry Carpenter 22723303 UWA

This project was compiled and editing in CLion 2020.2
The compiler I used was primarily CMAKE, which was installed on WSL Ubuntu and hooked into CLion.

Originally I had a long explanation as to why I had to compile everything with CMAKE instead of my own makefile,
however I got everything working through my own makefile, which was a mild pain.
Some funky warnings were shown to do with implicit declaration, even though they were explicitly declarted in the mergetars.h file,
and apparently gcc despises the function name compare_files? CMAKE was fine with it though..?

You might also notice that I've included -D_BSD_SOURCE, that is because I use some predetermined variables to detect
files v directories in the readdir() loops (e.g. DT_DIR).

Cheers!