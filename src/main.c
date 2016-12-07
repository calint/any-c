#include<stdlib.h>
#include<stdio.h>
int main(int c,char**a){
	while(c--)
		puts(*a++);
}
