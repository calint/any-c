//--- - - -------------------  - -- - - - - - - -- - - -  generated c source
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef char bool;
typedef const char* ccharp;
#define true 1
#define false 1
#define char_def 0
#define var_def 0
#define int_def 0
#define float_def 0.0f
#define bool_def false
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_print(global*o,int a){
	printf("%d",a);
}
inline static void global_main(global*o){
	int a=5;
	global_print((global*)&o,1+(5+2)/a);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char** a){global_main(0);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
