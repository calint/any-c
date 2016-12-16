#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<execinfo.h>
#define stacktrace_depth 128
void stacktrace_print(FILE*f){
	void*array[stacktrace_depth];
	int size=backtrace(array,stacktrace_depth);
	char**strings=backtrace_symbols(array,size);
//	fprintf(f,"stacktrace %d frames:\n",size);

	for(int i=0;i<size;i++){
//		char s1[256];
//		char s2[256];
//		sscanf(strings[i],"%s [%s]",&s1,&s2);
//		addr2line -fe <executable>

		fprintf(f,"%s\n",strings[i]);
	}

	free (strings);
}
