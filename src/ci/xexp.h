#pragma once
#include"../lib.h"

struct toc;
typedef struct xexp{
	void (*compile)(const struct xexp*,struct toc*);
	void (*free)(struct xexp*);
	cstr type;
	token token;
	char bits; //  1 is encapsulated     2 is block { }
}xexp;

#define xexp_def (xexp){NULL,NULL,cstr_def,token_def,0}

inline static int xexpr_is_empty(const xexp*o){return o->compile==NULL;}
inline static bool xexpr_is_encapsulated(const xexp*o){return(o->bits&1)==1;}
inline static void xexpr_set_is_encapsulated(xexp*o,const bool b){
	if(b)
		o->bits|=1;
	else
		o->bits&=~1;
}
inline static bool xexpr_is_block(const xexp*o){return(o->bits&2)==2;}
inline static void xexpr_set_is_block(xexp*o,const bool b){
	if(b)
		o->bits|=2;
	else
		o->bits&=~2;
}
