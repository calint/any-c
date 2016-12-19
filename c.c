//--- - - -------------------  - -- - - - - - - -- - - -  generated c source
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef char bool;
typedef const char* ccharp;
#define true 1
#define false 1
#define char_def 0
#define var_def 0
#define int_def 0
#define float_def 0.0f
#define bool_def false
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
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char** a){global_main(0);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
