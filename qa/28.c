//--- - - -------------------  - -- - - - - - - -- - - - -- - -  tidy salami
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef const char* strc;
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
    printf("entity: %d %f\n",o->id,o->f);
}
inline static void entity__free(entity*o){
    printf("free entity %d\n",o->id);
}
inline static void entity_free(entity*o){
    entity__free(o);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    int i=1;
    while(1){
        entity e=entity_def;
        e.id=i++;
        entity_print(&e);
        if (i<4) {
                        entity_free(&e);
continue;
        }
        else {
                        entity_free(&e);
break;
        }
        entity e2=entity_def;
        e2.id=i++;
        entity_print(&e2);
        entity_free(&e2);        entity_free(&e);    }

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
