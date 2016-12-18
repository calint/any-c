#include<stdlib.h>
#include<stdio.h>
#include"ci/ci.h"

int main(int c,char**a){
	return(toc_compile_file(c>1?a[1]:"ci/main.ci"));
}
