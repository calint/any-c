#include<stdlib.h>
#include<stdio.h>
#include"ninjas.h"

inline static int run(int c,char**a){
	puts("run");
	ninja*o;
	o=ninja_new(0);
	o=ninja_new(0);
	ninjas_print(&_ninjas);
	return 0;
}

int main(int c,char**a){while(c--)puts(*a++);return run(c,a);}
