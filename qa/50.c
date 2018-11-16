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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - - ext
typedef struct ext{
    strc name;
    int locid;
}ext;
#define ext_def (ext){"",0}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static strc ext_get_name(ext*o){
    return o->name;
}
inline static void ext_set_name(ext*o,strc nm){
    o->name=nm;
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - location
typedef struct location{
    int id;
    strc name;
    ext xn;
    ext xe;
    ext xs;
    ext xw;
}location;
#define location_def (location){0,"",ext_def,ext_def,ext_def,ext_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int id;
    strc name;
    location*location;
}entity;
#define entity_def (entity){0,"",null}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_move_to(entity*o,location*loc){
    o->location=loc;
    printf("%s leaves to %s\n",o->name,loc->name);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    location roome;
    location hall;
    entity me;
}global;
#define global_def (global){location_def,location_def,entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    int id=0b11;
    strc descr=strc_def;
    descr="roome";
    o->roome.id=id;
    o->roome.name=descr;
    o->roome.xn.name="north";
    o->roome.xn.locid=1;
    o->hall.id=2;
    o->hall.name="hall";
    ext_set_name(&o->hall.xs,"south");
    o->hall.xs.locid=1;
    o->me.name="me";
    o->me.location=&o->roome;
    printf("%d\n",o->me.location->id);
    printf("%s\n",o->me.location->name);
    printf("%s\n",o->me.location->xn.name);
    entity_move_to(&o->me,&o->hall);
    printf("%d\n",o->me.location->id);
    printf("%s\n",o->me.location->name);
    printf("%s\n",ext_get_name(&o->me.location->xs));
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
