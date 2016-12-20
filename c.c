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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  rrange
typedef struct rrange{
    buffer buffer;
    int from;
    int to;
    material m;
}rrange;
#define rrange_def (rrange){buffer_def,int_def,int_def,material_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void rrange_draw(rrange*o){
	printf("draw_render_range\n");
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - - glo
typedef struct glo{
    buffer vertbuf;
    rrange rendrngs;
}glo;
#define glo_def (glo){buffer_def,rrange_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void glo_draw(glo*o){
	printf("draw_glo\n");
	rrange_draw((rrange*)&o->rendrngs);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    glo gl;
}global;
#define global_def (global){glo_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_draw(global*o){
	printf("draw_global\n");
}
inline static void global_main(global*o){
	global_draw((global*)&o);
	glo_draw((glo*)&o->gl);
	glo g=glo_def;
	glo_draw((glo*)&g);
	rrange_draw((rrange*)&g.rendrngs);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char**a){global_main(0);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
