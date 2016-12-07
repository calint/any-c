#include<stdlib.h>
#include<stdio.h>

#define class_depth_cap 8
#define object_count_cap 1024
#define object_parts_cap 1

struct vec4{float x,y,z,w;};

struct part{};

struct object{
	struct part parts[object_parts_cap];
	char class[class_depth_cap];
	char*alloc_byte_ptr;
	struct object*parent;
};

struct{
	struct object table[object_count_cap];
	struct object*table_end;
	struct object*table_itr;

	char alloc[object_count_cap];
	char*alloc_end;
	char*alloc_itr;

}objects;

struct part_name{
	struct part part;
	const char*name;
};

struct ninja{
	struct object object;
};

inline static void objects_init(){
	objects.alloc_itr=objects.alloc;
	objects.alloc_end=objects.alloc+sizeof(objects.alloc);
	objects.table_itr=objects.table;
	objects.table_end=objects.table+sizeof(objects.table);
}

inline static int _try_alloc(char*b){
	if(*b&1)return 0;
	*b=1;
	return 1;
}

inline static struct object*object_alloc(){
	int loops=2;
	while(loops--){
		while(1){
			if(objects.alloc_itr==objects.alloc_end)
				break;
			if(!_try_alloc(objects.alloc_itr)){
				objects.table_itr++;
				objects.alloc_itr++;
				continue;
			}
			struct object*o=objects.table_itr;
			objects.table_itr++;
			objects.alloc_itr++;
			return o;
		}
	}

}
inline static void object_free(struct object*o){
	o->alloc_byte_ptr=2;
}

inline static void run(){
	puts("run");
}





int main(int c,char**a){while(c--)puts(*a++);run();}
