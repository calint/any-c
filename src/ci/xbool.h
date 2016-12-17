#pragma once
#include"../lib.h"
#include "codeblk.h"
#include "toc.h"
typedef struct xbool{
	xexpr super;

	// element
	bool lh_negate;

	struct xexpr*lh;

	char op;

	bool rh_negate;

	struct xexpr*rh;


	// list
	str bool_op_list;

	dynp/*owns &expr_bool*/bool_list;

	bool is_encapsulated;

	bool is_negated;

}xbool;

inline static void _xbool_compile_(const xexpr*oo,toc*tc){
	xbool*o=(xbool*)oo;

	if(o->bool_list.count){
		if(o->is_negated){
			printf("!");
		}
		if(o->is_encapsulated){
			printf("(");
		}
		for(unsigned i=0;i<o->bool_list.count;i++){
			xbool*b=(xbool*)dynp_get(&o->bool_list,i);
			char op=str_get(&o->bool_op_list,i);
			if(op){
				if(op=='&'){
					printf(" && ");
				}else if(op=='|'){
					printf(" || ");
				}else{
					printf("<file> <line:col> unknown op '%d' '%c'\n",op,op);
					exit(1);
				}
			}
			_xbool_compile_((xexpr*)b,tc);
			// ...
		}
		if(o->is_encapsulated){
			printf(")");
		}
		return;
	}

	if(o->is_encapsulated){
		printf("(");
	}

	if(o->lh_negate)
		printf("!");

	o->lh->compile(o->lh,tc);

	if(!o->rh)
		return;

	if(o->op=='='){
		printf("==");
	}else if(o->op=='!'){
		printf("!=");
	}else if(o->op=='>'){
		printf(">");
	}else if(o->op=='g'){
		printf(">=");
	}else if(o->op=='<'){
		printf("<");
	}else if(o->op=='l'){
		printf("<=");
	}else{
		printf("<file> <line:col> unknown op '%d' ')'\n",o->op);
		exit(1);
	}

	if(o->rh_negate)
		printf("!");

	o->rh->compile(o->rh,tc);

	if(o->is_encapsulated){
		printf(")");
	}
}

#define xbool_def (xbool){{_xbool_compile_,NULL,str_def,0},\
	false,NULL,0,false,NULL,\
	str_def,\
	dynp_def,\
	false,false}

inline static void xbool_parse(xbool*o,
		const char**pp,toc*tc){

	o->super.type=str_from_string("bool");

	token_skip_empty_space(pp);

	bool neg=false;
	if(**pp=='!'){// if(!ok){}
		(*pp)++;
		neg=true;
	}

	if(**pp!='('){//   keybits==1 && ok || (a&b!=0)
		o->is_encapsulated=false;

		if(!neg && **pp=='!'){// if(!ok){}
			(*pp)++;
			o->lh_negate=true;
		}else if(neg && **pp=='!'){// if(!!ok){}
			printf("<file> <line:col> did not expect !!\n");
			exit(1);
		}else{
			o->lh_negate=neg;
		}


		o->lh=toc_read_next_xexpr(pp,tc);

		if(**pp=='='){
			(*pp)++;
			if(**pp!='='){
				printf("<file> <line:col> expected '=='\n");
				exit(1);
			}
			o->op='=';
			(*pp)++;
		}else if(**pp=='!'){
			(*pp)++;
			if(**pp!='='){
				printf("<file> <line:col> expected '!='\n");
				exit(1);
			}
			o->op='!';
			(*pp)++;
		}else if(**pp=='>'){
			(*pp)++;
			if(**pp=='='){
				(*pp)++;
				o->op='g';
			}else{
				o->op='>';
			}
		}else if(**pp=='<'){
			(*pp)++;
			if(**pp=='='){
				(*pp)++;
				o->op='l';
			}else{
				o->op='<';
			}
		}else{// if(active)
//			o->op='.';
			return;
		}

		if(**pp=='!'){// if(!ok){}
			(*pp)++;
			o->rh_negate=true;
		}

		o->rh=toc_read_next_xexpr(pp,tc);

		if(**pp==')'){
			return;
		}

		//? keybits==1 && ok
		if(**pp=='&'){
			(*pp)++;
			if(**pp=='&'){
				(*pp)--;
				return;
			}else{
				printf("<file> <line:col> expected &&\n");
				exit(1);
			}
		}else if(**pp=='|'){
			(*pp)++;
			if(**pp=='|'){
				(*pp)--;
				printf("<file> <line:col> expected ||\n");
				exit(1);
			}
		}else{
			printf("<file> <line:col> expected && or ||\n");
			exit(1);
		}
	}

	// example (a==b && c==d)
	o->is_encapsulated=true;
	o->is_negated=neg;
	(*pp)++;
	str_add(&o->bool_op_list,0);
	while(1){
		xbool*e=malloc(sizeof(xbool));
		*e=xbool_def;
		xbool_parse(e,pp,tc);
		dynp_add(&o->bool_list,e);
		token_skip_empty_space(pp);
		if(**pp==')'){
			(*pp)++;
			return;
		}
		if(**pp=='&'){
			(*pp)++;
			if(**pp!='&'){
				printf("<file> <line:col> expected && \n");
				exit(1);
			}
			(*pp)++;
			str_add(&o->bool_op_list,'&');
		}else if(**pp=='|'){
			(*pp)++;
			if(**pp!='|'){
				printf("<file> <line:col> expected || \n");
				exit(1);
			}
			(*pp)++;
			str_add(&o->bool_op_list,'|');
		}else{
			printf("<file> <line:col> expected && or ||\n");
			exit(1);
		}

	}
}
