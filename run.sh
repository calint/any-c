#/bin/sh -x
./make.sh && ./any-c main.ci > c.c && 
gcc -O3 c.c &&
 ./a.out
 