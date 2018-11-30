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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - material
typedef struct material{
    int id;
}material;
#define material_def (material){1}
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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void materials__init(materials*o){
    o->m1.id=1;
    o->m2.id=2;
}
inline static material*materials_get(materials*o,strc nm){
    if(nm=="mat1"){
        return &o->m1;
    }
;
    if(nm=="mat2"){
        return &o->m2;
    }
;
}
inline static void materials_init(materials*o){
    materials__init(o);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- -  glob
typedef struct glob{
    struct glob*parent;
    material*material;
    int id;
}glob;
#define glob_def (glob){null,null,1}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void glob_print(glob*o){
    printf("glob %d {material ",o->id);
    if(o->material!=0){
        printf("%d",o->material->id);
    }
    else {
        printf("n/a");
    }
;
    printf("}\n");
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - globs
typedef struct globs{
    glob e1;
    glob e2;
}globs;
#define globs_def (globs){glob_def,glob_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static glob*globs_get(globs*o,strc nm){
    return &o->e1;
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    globs globs;
    materials materials;
}global;
#define global_def (global){globs_def,materials_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    printf("copy of\n");
    glob g1=*globs_get(&o->globs,"a");
    glob_print(&g1);
    g1.material=materials_get(&o->materials,"mat2");
    glob_print(&g1);
    glob g2=*globs_get(&o->globs,"a");
    glob_print(&g2);
    printf("reference to\n");
    glob*g3=globs_get(&o->globs,"a");
    glob_print(g3);
    g3->material=materials_get(&o->materials,"mat2");
    glob_print(g3);
    printf("reference to\n");
    glob*g4=globs_get(&o->globs,"a");
    glob_print(g4);
    printf("copy of\n");
    glob g5=*globs_get(&o->globs,"a");
    glob_print(g4);
}
inline static void global_init(global*o){
    materials_init(&o->materials);
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
