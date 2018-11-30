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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    int i=1;
    int j=2;
    int k=3;
    if(i==1){
        printf("true\n");
    }
    else {
        printf("false\n");
    }
;
    if(i==1 && j==2){
        printf("true\n");
    }
    else {
        printf("false\n");
    }
;
    if(!(i==1)){
        printf("true\n");
    }
    else {
        printf("false\n");
    }
;
    if(i==1 && !(j==2 && k==3)){
        printf("true\n");
    }
    else {
        printf("false\n");
    }
;
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
