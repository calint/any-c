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


sample source
```
entity{
	id=0
	print{
		p("entity %d\n",id)
	}
}

global{
	main{
		var i=0
		loop{
			if i++==3 break
			entity e
			e.id=i
			e.print()
		}
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
#define entity_def (entity){0}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
    printf("entity %d\n",o->id);
}

//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{}global;
#define global_def (global){}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
    int i=0;
    while(1){
        if (i++==3) {
            break;
        }
        entity e=entity_def;
        e.id=i;
        entity_print((entity*)&e);
    }

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
entity 1
entity 2
entity 3
```


source size
```
cat src/ci/* | wc
   2500    4074   58911
cat src/ci/* | gzip | wc
     44     247   11741
```

