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
#define player_def (player){2,}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void player_print(player*o){
	printf(" player: %d\n",o->c);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct global{}global;
#define global_def (global){}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void global_main(global*o,int argc,char** arga){
	player p1=player_def;
	player p2=player_def;
	player_print((player*)&p1);
	player_print((player*)&p2);
	p1.c=1;
	p2.c=p1.c;
	player_print((player*)&p1);
	player_print((player*)&p2);
	p2.c=4;
	player_print((player*)&p1);
	player_print((player*)&p2);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char** a){global_main(0,c,a);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
