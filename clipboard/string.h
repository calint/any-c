#pragma once
#include<stdio.h>
#include<stdlib.h>

//----------------------------------------------------------------------config
typedef unsigned arrayix;
#define nullptr 0

#define string_initial_capacity 8
#define string_bounds_check 1

//------------------------------------------------------------------------ def

typedef struct string{
	char *data;
	unsigned count;
	unsigned cap;
}string;
static string const string_init={nullptr,0,0};
#define string_def {nullptr,0,0}
//--------------------------------------------------------------------- private

inline static void _string_insure_free_capcity(string*o,arrayix n){
	const unsigned rem=o->cap-o->count;
	if(rem>=n)
		return;
	if(o->data){
		unsigned new_cap=o->cap*2;
		char *new_data=realloc(o->data,sizeof(char)*new_cap);
		if(!new_data){
			fprintf(stderr,"\nout-of-memory");
			fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
			exit(-1);
		}
		if(new_data!=o->data){
			o->data=new_data;
		}
		o->cap=new_cap;
		return;
	}
	o->cap=string_initial_capacity;
	o->data=malloc(sizeof(char)*o->cap);
	if(!o->data){
		fprintf(stderr,"\nout-of-memory");
		fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
		exit(-1);
	}
}

//---------------------------------------------------------------------- public

inline static void string_add(string*this,char o){
	_string_insure_free_capcity(this,1);
	*(this->data+this->count++)=o;
}

//-----------------------------------------------------------------------------

inline static char string_get(string*this,arrayix index){
#ifdef string_bounds_check
	if(index>=this->cap){
		fprintf(stderr,"\nindex-out-of-bounds");
		fprintf(stderr,"\t%s\n\n%d  index: %u    capacity: %u\n",
				__FILE__,__LINE__,index,this->cap);
		exit(-1);
	}
#endif
	char p=*(this->data+index);
	return p;
}

//-----------------------------------------------------------------------------

inline static char string_get_last(string*this){
	char p=*(this->data+this->count-1);
	return p;
}

//-----------------------------------------------------------------------------

inline static size_t string_size_in_bytes(string*this){
	return this->count*sizeof(char);
}

//-----------------------------------------------------------------------------

inline static void string_free(string*this){
	if(!this->data)
		return;
	free(this->data);
}

//-----------------------------------------------------------------------------

inline static void string_add_list(string*this,/*copies*/const char*str,int n){
	//? optimize
	const char*p=str;
	while(n--){
		_string_insure_free_capcity(this,1);
		*(this->data+this->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void string_add_string(string*this,/*copies*/const char*str){
	//? optimize
	const char*p=str;
	while(*p){
		_string_insure_free_capcity(this,1);
		*(this->data+this->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void string_write_to_fd(string*this,int fd){
	if(!this->data)
		return;
	write(fd,this->data,this->count);
}

//-----------------------------------------------------------------------------

