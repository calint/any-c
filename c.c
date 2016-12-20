//--- - - -------------------  - -- - - - - - - -- - - -  generated c source
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef const char*cstr;
typedef char bool;
#define true 1
#define false 1
#define cstr_def ""
#define bool_def false
#define char_def 0
#define int_def 0
#define float_def 0.0f
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - -  id
typedef struct id{
    int i;
}id;
#define id_def (id){0}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void id__free(id*o){
	printf("free id %d\n",o->i);
}
inline static void id_free(id*o){
    id__free(o);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    id id;
    float f;
}entity;
#define entity_def (entity){id_def,2.2f}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
	printf("entity: %d %f\n",o->id.i,o->f);
}
inline static void entity_free(entity*o){
    id_free(&o->id);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entity e1;
    entity e2;
}global;
#define global_def (global){entity_def,entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
	o->e1.id.i=1;
	entity_print((entity*)&o->e1);
	o->e2.id.i=2;
	entity_print((entity*)&o->e2);
	o->e1.id.i=3;
	entity_print((entity*)&o->e1);
}
inline static void global_free(global*o){
    entity_free(&o->e2);
    entity_free(&o->e1);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char**a){
    global g=global_def;
    global_main(&g);
    global_free(&g);
    return 0;
}//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
