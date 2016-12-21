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
inline static void id__init(id*o){
	printf("init id\n");
	o->i=1;
}

inline static void id__free(id*o){
	printf("free id\n");
}

inline static void id_init(id*o){
    id__init(o);
}
inline static void id_free(id*o){
    id__free(o);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    id id;
}entity;
#define entity_def (entity){id_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
	printf("entity: %i\n",o->id.i);
}

inline static void entity_init(entity*o){
    id_init(&o->id);
}
inline static void entity_free(entity*o){
    id_free(&o->id);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entity e;
}global;
#define global_def (global){entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global__init(global*o){
	printf("init global\n");
}

inline static void global_main(global*o){
	entity_print((entity*)&o->e);
}

inline static void global__free(global*o){
	printf("free global\n");
}

inline static void global_init(global*o){
    entity_init(&o->e);
    global__init(o);
}
inline static void global_free(global*o){
    global__free(o);
    entity_free(&o->e);
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
