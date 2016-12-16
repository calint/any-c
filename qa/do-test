#/bin/sh
../any-c $1.ci > c.c && gcc -O3 c.c && ./a.out > compare && diff -q compare $1.cmp && rm -f a.out compare c.c && echo $1 : ok
