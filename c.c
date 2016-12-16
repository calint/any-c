#include<stdlib.h>
#include<stdio.h>
typedef char bool;
#define true 1
#define false 1
#define char_def 0
#define int_def 0
#define float_def 0.0f
#define bool_def false
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct player{
    int c;
}player;
#define player_def (player){int_def,}
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
inline static void player_print(player*o){
	printf(" player: %d\n",o->c);
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
	player p1;
	player p2;
	p1.c=1;
	player_print((player*)&p1);
	p2.c=p1.c;
	player_print((player*)&p2);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -

int main(int c,char** a){
	global_main(0,c,a);
}
