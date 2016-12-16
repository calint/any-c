#include<stdlib.h>
#include<stdio.h>
typedef char bool;
#define true 1
#define false 1
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct player{
    int i;
    float f;
    char c;
}player;
#define player_def (player){1,2.0f,'a',}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void player_init(player*o){
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static player*player_alloc(){
	player*p=malloc(sizeof(player));
	*p=player_def;
	player_init(p);
	return p;
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void player_free(player*o){
	free(o);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void player_main(player*o){
	o->i=2;
	o->f=3.0f;
	o->c='a';
}//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct global{}global;
#define global_def (global){}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void global_init(global*o){
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static global*global_alloc(){
	global*p=malloc(sizeof(global));
	*p=global_def;
	global_init(p);
	return p;
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void global_free(global*o){
	free(o);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void global_main(global*o,int c,char** a){
	puts("hello world");
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -

int main(int c,char** a){
	global_main(0,c,a);
}
