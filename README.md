# any-c
```
simple c++ like language that compiles to c

  has
    : structs with functions
    : static type checking
    : deduced variable type
    : mutable references
    : terse syntax
```


source size
```
cat src/ci/* | wc
   2500    4074   58911
cat src/ci/* | gzip | wc
     44     247   11741
```



sample source
```
entity{
	id=1	
	p(){
		p("p entity %d\n",id)
	}
}

global{
	main{
		entity e1
		entity&e2=e1
		e1.p()
		e2.p()
		e2.id=2
		e1.p()
		e2.p()		
	}
}
```


compiles to
```
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
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int id;
}entity;
#define entity_def (entity){1}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_p(entity*o){
    printf("p entity %d\n",o->id);
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    entity e1=entity_def;
    entity*e2=&e1;
    entity_p((entity*)&e1);
    entity_p((entity*)e2);
    e2->id=2;
    entity_p((entity*)&e1);
    entity_p((entity*)e2);
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
```


outputs
```
p entity 1
p entity 1
p entity 2
p entity 2
```
