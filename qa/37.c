//--- - - -------------------  - -- - - - - - - -- - - -  generated c source
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef const char*cstr;
typedef char bool;
#define true 1
#define false 0
#define cstr_def ""
#define bool_def false
#define char_def 0
#define int_def 0
#define float_def 0.0f
#define null 0
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int id;
}entity;
#define entity_def (entity){0}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
	printf("entity %d\n",o->id);
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - entities
typedef struct entities{
    entity e;
}entities;
#define entities_def (entities){entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static entity*entities_get(entities*o,cstr nm){
	return &o->e;
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entities es;
}global;
#define global_def (global){entities_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
	o->es.e.id=1;
	entity*a=entities_get((entities*)&o->es,"a");
	entity_print((entity*)a);
	o->es.e.id=2;
	a=entities_get((entities*)&o->es,"a");
	entity_print((entity*)a);
}

inline static void global_init(global*o){
}
inline static void global_free(global*o){
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char**a){
    global g=global_def;
    global_init(&g);
    global_main(&g);
    global_free(&g);
    return 0;
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
