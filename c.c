//--- - - -------------------  - -- - - - - - - -- - - -  generated c source
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef char bool;
typedef const char* ccharp;
#define true 1
#define false 1
#define char_def 0
#define int_def 0
#define float_def 0.0f
#define bool_def false
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o,int c,char** a){
	int b=0;
	while(1){
		if (b++==10) 
			break;
		else if (b--==10) {
			break;
		}
		if (b++==10) 
			break;
		if (b==10) {
			break;
		}
		b++;
		b--;
		printf("%d. hello world\t",b);
	};
	printf("\n");
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char** a){global_main(0,c,a);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
