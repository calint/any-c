#include<stdlib.h>
#include<stdio.h>
#include"ninjas.h"
#include"ci/ci.h"

int main(int c,char**a){
	ci_toc_compile_file(c>1?a[1]:"ci/main.ci");
	return 0;
}
