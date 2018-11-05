//--- - - -------------------  - -- - - - - - - -- - - - -- - -  tidy salami
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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - location
typedef struct location{
    int id;
    strc name;
}location;
#define location_def (location){0,""}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void location_p_name(location*o){
    printf("%s",o->name);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int id;
    strc name;
    location*location;
}entity;
#define entity_def (entity){0,"",null}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_p_name(entity*o){
    printf("%s",o->name);
}
inline static void entity_p_where(entity*o){
    entity_p_name(o);
    printf("is at");
    location_p_name(o->location);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    location roome=location_def;
    roome.id=1;
    roome.name="roome";
    entity e=entity_def;
    e.name="me";
    e.location=&roome;
    entity_p_where(&e);
}
inline static void global_init(global*o){}
inline static void global_free(global*o){}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char**a){
    global g=global_def;
    global_init(&g);
    global_main(&g);
    global_free(&g);
    return 0;
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
