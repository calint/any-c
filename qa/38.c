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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - material
typedef struct material{
    int id;
}material;
#define material_def (material){3}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void material_print(material*o){
    printf("material %d\n",o->id);
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - -  materials
typedef struct materials{
    material m1;
    material m2;
}materials;
#define materials_def (materials){material_def,material_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int id;
    material*mref;
}entity;
#define entity_def (entity){0,null}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
    printf("entity %d\n",o->id);
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - entities
typedef struct entities{
    entity e1;
    entity e2;
}entities;
#define entities_def (entities){entity_def,entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static entity*entities_get(entities*o,cstr nm){
    return &o->e1;
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entities entities;
    materials materials;
}global;
#define global_def (global){entities_def,materials_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    entity*e=entities_get((entities*)&o->entities,"a");
    e->mref=&o->materials.m1;
    material_print((material*)e->mref);
    o->materials.m1.id=4;
    material_print((material*)e->mref);
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
