# any-c
```
simple c++ like language that compiles to c

  has
    : structs with functions
    : compile time type checking
    : deduced variable types
    : terse syntax
```


sample source
```
entity{
	var i=1
	var f=2.2
	print{
		printf("entity: %d %f\n",i,f)
	}
}

global{
	entity e1
	entity e2
	main{
		e1.print()
		e2.print()
		e1.i=2
		e1.print()
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
#define false 1
#define cstr_def ""
#define bool_def false
#define char_def 0
#define int_def 0
#define float_def 0.0f
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  entity
typedef struct entity{
    int i;
    float f;
}entity;
#define entity_def (entity){1,2.2f}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void entity_print(entity*o){
	printf("entity: %d %f\n",o->i,o->f);
}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - --  global
typedef struct global{
    entity e1;
    entity e2;
}global;
#define global_def (global){entity_def,entity_def}
//--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - funcs
inline static void global_main(global*o){
	entity_print((entity*)&o->e1);
	entity_print((entity*)&o->e2);
	o->e1.i=2;
	entity_print((entity*)&o->e1);
}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
int main(int c,char**a){global g=global_def;global_main(&g);}
//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -
```


outputs
```
entity: 1 2.200000
entity: 1 2.200000
entity: 2 2.200000
```

```

source size
cat src/ci/* | wc
   2103    3268   46387
cat src/ci/* | gzip | wc
     35     192    9311
```

