//--- - - -------------------  - -- - - - - - - -- - - -  generated c source
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef char bool;
typedef const char* cstr;
#define true 1
#define false 1
#define char_def 0
#define var_def 0
#define int_def 0
#define float_def 0.0f
#define bool_def false
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int i;
    float f;
}entity;
#define entity_def (entity){1,2.2}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
	printf("entity: %d %f\n",o->i,o->f);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entity e1;
    entity e2;
}global;
#define global_def (global){entity_def,entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
	entity_print((entity*)&o->e1);
	entity_print((entity*)&o->e2);
	o->e1.i=2;
	entity_print((entity*)&o->e1);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char**a){global g=global_def;global_main(&g);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
