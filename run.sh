ssh harrycarps@localhost:2222 -p w6a9YjhuMN7Q
cd "/mnt/c/Users/harrycarpenter/OneDrive/Documents/UWA/Year 2/Semester 2/CITS2002/CITS2002-Project-2" &&
gcc mergetars.c mergetars.h expand_tars.c &&
./a.out ./testing_stage/tar1.tar ./testing_stage/tar2.tar ./testing_stage/tarout.tar
wait