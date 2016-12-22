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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  buffer
typedef struct buffer{}buffer;
#define buffer_def (buffer){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - - material
typedef struct material{}material;
#define material_def (material){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - render_range
typedef struct render_range{
    buffer buffer;
    int from;
    int to;
    material m;
}render_range;
#define render_range_def (render_range){buffer_def,int_def,int_def,material_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void render_range_draw(render_range*o){
	printf("render_range\n");
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
	render_range rr=render_range_def;
	render_range_draw((render_range*)&rr);
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
