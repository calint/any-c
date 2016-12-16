#/bin/sh -x
sh make.sh && 
./any-c > c.c && 
gcc -O3 c.c &&
 ./a.out
 