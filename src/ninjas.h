#pragma once
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include"ninja.h"
//----------------------------------------------------------------------config

#define ninjas_initial_capacity 8
#define ninjas_bounds_check 1

//------------------------------------------------------------------------ def

typedef struct _ninjas{
	ninja* *data;
	unsigned count;
	unsigned cap;
}ninjas;
#define ninjas_def (ninjas{0,0,0})

//--------------------------------------------------------------------- private

inline static void _ninjas_insure_free_capcity(ninjas*o,unsigned n){
	const unsigned rem=o->cap-o->count;
	if(rem>=n)
		return;
	if(o->data){
		unsigned new_cap=o->cap*2;
		ninja* *new_data=realloc(o->data,sizeof(ninja*)*new_cap);
		if(!new_data){
			fprintf(stderr,"\nout-of-memory");
			fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
			exit(-1);
		}
		if((ninja*)new_data!=*o->data){
			o->data=new_data;
		}
		o->cap=new_cap;
		return;
	}
	o->cap=ninjas_initial_capacity;
	o->data=malloc(sizeof(ninja)*o->cap);
	if(!o->data){
		fprintf(stderr,"\nout-of-memory");
		fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
		exit(-1);
	}
}

//---------------------------------------------------------------------- public

inline static void ninjas_add(ninjas*o,ninja* obj){
	_ninjas_insure_free_capcity(o,1);
	*(o->data+o->count++)=obj;
}

//-----------------------------------------------------------------------------

inline static ninja* ninjas_get(ninjas*o,unsigned index){
#ifdef ninjas_bounds_check
	if(index>=o->cap){
		fprintf(stderr,"\nindex-out-of-bounds");
		fprintf(stderr,"\t%s\n\n%d  index: %u    capacity: %u\n",
				__FILE__,__LINE__,index,o->cap);
		exit(-1);
	}
#endif
	ninja* p=*(o->data+index);
	return p;
}

//-----------------------------------------------------------------------------

inline static ninja* ninjas_get_last(ninjas*o){
	ninja* p=*(o->data+o->count-1);
	return p;
}

//-----------------------------------------------------------------------------

inline static size_t ninjas_size_in_bytes(ninjas*o){
	return o->count*sizeof(ninja);
}

//-----------------------------------------------------------------------------

inline static void ninjas_free(ninjas*o){
	if(!o->data)
		return;
	free(o->data);
}

//-----------------------------------------------------------------------------

inline static void ninjas_add_list(ninjas*o,
		/*copies*/const ninja**str,int n){
	//? optimize
	const ninja**p=str;
	while(n--){
		_ninjas_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void ninjas_add_string(ninjas*o,
		/*copies*/const ninja**str){
	//? optimize
	const ninja**p=str;
	while(*(char*)p){
		_ninjas_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void ninjas_write_to_fd(ninjas*o,int fd){
	if(!o->data)
		return;
	write(fd,o->data,o->count);
}

//-----------------------------------------------------------------------------

inline static void ninjas_print(ninjas*o){
	printf("ninjas[ data: %p  cap: %u  count: %u ]",
			(void*)o->data,o->cap,o->count);
}

//-----------------------------------------------------------------------------

static ninjas _ninjas;

inline static/*gives*/ninja*ninja_new(ninja*def){
	ninja*o=malloc(sizeof(ninja));
	if(def)
		*o=*def;
	ninjas_add(&_ninjas,o);
	return o;
}


