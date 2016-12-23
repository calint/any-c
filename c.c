//--- - - -------------------  - -- - - - - - - -- - - -  generated c source
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef const char*strc;
typedef char bool;
#define true 1
#define false 0
#define strc_def ""
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
    printf("entity: %d\n",o->id);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - entities
typedef struct entities{
    entity*all;
}entities;
#define entities_def (entities){null}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entities__init(entities*o){
    o->all=malloc(4*10);
}
inline static void entities__free(entities*o){
    free(o->all);
}
inline static entity*entities_get(entities*o,int ix){
    return o->all+ix*4;
}
inline static void entities_init(entities*o){
    entities__init(o);
}
inline static void entities_free(entities*o){
    entities__free(o);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entities es;
}global;
#define global_def (global){entities_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    entity*e=entities_get(&o->es,100);
    entity_print(e);
    e->id=2;
    entity_print(e);
    entity e2=*entities_get(&o->es,1);
    entity_print(&e2);
}
inline static void global_init(global*o){
    entities_init(&o->es);
}
inline static void global_free(global*o){
    entities_free(&o->es);
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
