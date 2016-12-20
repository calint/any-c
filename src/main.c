#include<stdlib.h>
#include<stdio.h>
#include"ci/ci.h"

int main(int c,char**a){
	return(ci_compile_file(c>1?a[1]:"main.ci"));
}
