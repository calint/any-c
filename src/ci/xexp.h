#pragma once
#include"../lib.h"

struct toc;
typedef struct xexp{
	void (*compile)(const struct xexp*,struct toc*);
	void (*free)(struct xexp*);
	void (*print_source)(struct xexp*);

	strc type;
	token token;
	char bits; //  1 is encapsulated   4 is in parenthesis
	           //  8 is negated
	bool is_ref;
}xexp;

#define xexp_def (xexp){NULL,NULL,NULL,strc_def,token_def,0,false}

inline static bool xexp_is_encapsulated(const xexp*o){return(o->bits&1)==1;}
inline static void xexp_set_is_encapsulated(xexp*o,const bool b){
	if(b) o->bits|=1;
	else o->bits&=~1;
}
inline static bool xexp_is_parenthesis(const xexp*o){return(o->bits&4)==4;}
inline static void xexp_set_is_parenthesis(xexp*o,const bool b){
	if(b) o->bits|=4;
	else o->bits&=~4;
}

inline static bool xexp_is_negated(const xexp*o){return(o->bits&8)==8;}
inline static void xexp_set_is_negated(xexp*o,const bool b){
	if(b) o->bits|=8;
	else o->bits&=~8;
}
