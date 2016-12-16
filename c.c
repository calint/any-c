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
    int i;
    float f;
    bool b;
    const char* s;
    char ch;
}player;
#define player_def (player){1,2.2f,true,"hello",'.',}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void player_print(player*o){
	printf(" %f %d %d %s %c \n",o->f,o->i,o->b,o->s,o->ch);
}//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
typedef struct global{}global;
#define global_def (global){}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
inline static void global_main(global*o,int c,char** a){
	player p=player_def;
	player_print((player*)&p);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char** a){global_main(0,c,a);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
