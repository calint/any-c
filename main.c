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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    int id=0b11;
    strc descr=strc_def;
    descr="roome";
    location roome=location_def;
    roome.id=id;
    roome.name=descr;
    roome.xn.name="north";
    roome.xn.locid=2;
    location hall=location_def;
    hall.id=2;
    hall.name="hall";
    hall.xs.name="south";
    hall.xs.locid=1;
    entity me=entity_def;
    me.name="me";
    me.location=&roome;
    printf("%d\n",me.location->id);
    printf("%s\n",me.location->name);
    printf("%s\n",me.location->xn.name);
    me.location=&hall;
    printf("%d\n",me.location->id);
    printf("%s\n",me.location->name);
    printf("%s\n",me.location->xn.name);
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
