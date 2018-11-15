# any-c
```
experimental simple language that compiles to c

  has
    : structs with functions
    : static type checking
    : deduced variable types
    : mutable references
    : init/free framework
    : terse syntax
```


source size
```
cat src/ci/* src/lib/* src/lib.h main.c | wc
   3617    5907   86651
cat src/ci/* src/lib/* src/lib.h main.c | gzip | wc
     68     332   16386
```



sample source
```
entity{
	id=1	
	print{
		p("entity %d\n",id)
	}
}

global{
	main{
		entity e1
		entity&e2=e1
		e1.print()
		e2.print()
		e2.id=2
		e1.print()
		e2.print()		
	}
}
```


compiles to
```
//--- - - -------------------  - -- - - - - - - -- - - - -- - -  tidy salami
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
#include<stdlib.h>
#include<stdio.h>
typedef const char*strc;
typedef char bool;
#define true 1
#define false 0
#define strc_def ""
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
inline static void entity_print(entity*o){
    printf("entity %d\n",o->id);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    entity e1=entity_def;
    entity*e2=&e1;
    entity_print(&e1);
    entity_print(e2);
    e2->id=2;
    entity_print(&e1);
    entity_print(e2);
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
```


outputs
```
entity 1
entity 1
entity 2
entity 2
```
