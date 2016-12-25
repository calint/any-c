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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int id;
    float f;
}entity;
#define entity_def (entity){0,2.2f}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
    printf("print entity: %d %f\n",o->id,o->f);
}
inline static void entity__init(entity*o){
    o->id=1;
    printf("init entity %d\n",o->id);
}
inline static void entity__free(entity*o){
    printf("free entity %d\n",o->id);
}
inline static void entity_init(entity*o){
    entity__init(o);
}
inline static void entity_free(entity*o){
    entity__free(o);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entity e;
}global;
#define global_def (global){entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    entity_print(&o->e);
}
inline static void global_init(global*o){
    entity_init(&o->e);
}
inline static void global_free(global*o){
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
