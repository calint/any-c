#pragma once
#include"../lib.h"
#include "code.h"
#include "toc.h"
typedef struct xbool{
	xexpr super;

	// leaf
	bool lh_negate;
	struct xexpr*lh;
	char op;
	bool rh_negate;
	struct xexpr*rh;

	// list
	str bool_op_list;
	dynp bool_list;
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
					longjmp(_jmpbufenv,1);
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
		longjmp(_jmpbufenv,1);
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

inline static void xbool_parse(xbool*o,toc*tc){
	o->super.type=str_from_string("bool");
	token_skip_empty_space(&tc->srcp);
	bool neg=false;
	if(*tc->srcp=='!'){// if(!ok){}
		tc->srcp++;
		neg=true;
	}

	if(*tc->srcp!='('){//   keybits==1 && ok || (a&b!=0)
		o->is_encapsulated=false;

		if(!neg && *tc->srcp=='!'){// if(!ok){}
			tc->srcp++;
			o->lh_negate=true;
		}else if(neg && *tc->srcp=='!'){// if(!!ok){}
			printf("<file> <line:col> did not expect !!\n");
			longjmp(_jmpbufenv,1);
		}else{
			o->lh_negate=neg;
		}


		o->lh=toc_read_next_xexpr(tc);

		if(*tc->srcp=='='){
			tc->srcp++;
			if(*tc->srcp!='='){
				printf("<file> <line:col> expected '=='\n");
				longjmp(_jmpbufenv,1);
			}
			o->op='=';
			tc->srcp++;
		}else if(*tc->srcp=='!'){
			tc->srcp++;
			if(*tc->srcp!='='){
				printf("<file> <line:col> expected '!='\n");
				longjmp(_jmpbufenv,1);
			}
			o->op='!';
			tc->srcp++;
		}else if(*tc->srcp=='>'){
			tc->srcp++;
			if(*tc->srcp=='='){
				tc->srcp++;
				o->op='g';
			}else{
				o->op='>';
			}
		}else if(*tc->srcp=='<'){
			tc->srcp++;
			if(*tc->srcp=='='){
				tc->srcp++;
				o->op='l';
			}else{
				o->op='<';
			}
		}else{// if(active)
//			o->op='.';
			return;
		}

		if(*tc->srcp=='!'){// if(!ok){}
			tc->srcp++;
			o->rh_negate=true;
		}

		o->rh=toc_read_next_xexpr(tc);

		if(*tc->srcp==')'){
			return;
		}

		//? keybits==1 && ok
		if(*tc->srcp=='&'){
			tc->srcp++;
			if(*tc->srcp=='&'){
				tc->srcp--;
				return;
			}else{
				printf("<file> <line:col> expected &&\n");
				longjmp(_jmpbufenv,1);
			}
		}else if(*tc->srcp=='|'){
			tc->srcp++;
			if(*tc->srcp=='|'){
				tc->srcp--;
				printf("<file> <line:col> expected ||\n");
				longjmp(_jmpbufenv,1);
			}
		}else{
			printf("<file> <line:col> expected && or ||\n");
			longjmp(_jmpbufenv,1);
		}
	}

	// example (a==b && c==d)
	o->is_encapsulated=true;
	o->is_negated=neg;
	tc->srcp++;
	str_add(&o->bool_op_list,0);
	while(1){
		xbool*e=malloc(sizeof(xbool));
		*e=xbool_def;
		xbool_parse(e,tc);
		dynp_add(&o->bool_list,e);
		token_skip_empty_space(&tc->srcp);
		if(*tc->srcp==')'){
			tc->srcp++;
			return;
		}
		if(*tc->srcp=='&'){
			tc->srcp++;
			if(*tc->srcp!='&'){
				printf("<file> <line:col> expected && \n");
				longjmp(_jmpbufenv,1);
			}
			tc->srcp++;
			str_add(&o->bool_op_list,'&');
		}else if(*tc->srcp=='|'){
			tc->srcp++;
			if(*tc->srcp!='|'){
				printf("<file> <line:col> expected || \n");
				longjmp(_jmpbufenv,1);
			}
			tc->srcp++;
			str_add(&o->bool_op_list,'|');
		}else{
			printf("<file> <line:col> expected && or ||\n");
			longjmp(_jmpbufenv,1);
		}

	}
}
