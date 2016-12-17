#include<stdlib.h>
#include<stdio.h>
typedef char bool;
#define true 1
#define false 1
#define char_def 0
#define int_def 0
#define float_def 0.0f
#define bool_def false
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct type{
    int i;
}type;
#define type_def (type){0,}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct object{
    type type;
}object;
#define object_def (object){type_def,}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct global{}global;
#define global_def (global){}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void global_main(global*o,int i,char**c ){
	object o1=object_def;
	o1.type.i=3;
	printf("3=%d\n",o1.type.i);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char** a){global_main(0,c,a);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
