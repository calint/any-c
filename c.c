#include<stdlib.h>
#include<stdio.h>
typedef char bool;
#define true 1
#define false 1
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct player{
}player;

#define player_def (player){}
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
    bool b=true;
    int i=1;
    float f=2.0f;
    char c='a';
    int h=0x2020;
    int bb=0x5;
    char*const str="hello world";
}//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct global{
}global;

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

