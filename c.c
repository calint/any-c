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
typedef struct object{
    int i;
}object;
#define object_def (object){4,}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct global{}global;
#define global_def (global){}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void global_main(global*o,int i,char**c ){
	int j=1;
	object o1=object_def;
	o1.i=2;
	printf("%d=2\n",o1.i);
	o1.i++;
	printf("%d=3\n",o1.i);
	o1.i--;
	printf("%d=2\n",o1.i);
	o1.i=j;
	printf("%d=1\n",o1.i);
	object o2=object_def;
	o2.i=o1.i;
	printf("%d=1\n",o2.i);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char** a){global_main(0,c,a);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
