#pragma once
#include"xexpls.h"

typedef struct xbool{
	xexp super;

	// leaf
	bool lh_negate;
	struct xexpls lh;
	char op;
	bool rh_negate;
	struct xexpls rh;

	// list
	strb bool_op_list;
	ptrs bool_list;
	bool is_encapsulated;
	bool is_negated;
}xbool;

inline static void _xbool_compile_(const xexp*oo,toc*tc){
	xbool*o=(xbool*)oo;

	if(o->bool_list.count){
		if(o->is_negated){
			printf("!");
		}
		if(o->is_encapsulated){
			printf("(");
		}
		for(unsigned i=0;i<o->bool_list.count;i++){
			xbool*b=(xbool*)ptrs_get(&o->bool_list,i);
			char op=strb_get(&o->bool_op_list,i);
			if(op){
				if(op=='&'){
					printf(" && ");
				}else if(op=='|'){
					printf(" || ");
				}else{
					printf("<file> <line:col> unknown op '%d' '%c'\n",op,op);
					longjmp(_jmp_buf,1);
				}
			}
			_xbool_compile_((xexp*)b,tc);
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

	o->lh.super.compile((xexp*)&o->lh,tc);

	if(!o->rh.exps.count)
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
		longjmp(_jmp_buf,1);
	}

	if(o->rh_negate)
		printf("!");

	o->rh.super.compile((xexp*)&o->rh,tc);

	if(o->is_encapsulated){
		printf(")");
	}
}

#define xbool_def (xbool){{_xbool_compile_,NULL,cstr_def,token_def,0,false},\
	false,xexpls_def,0,false,xexpls_def,\
	strb_def,\
	ptrs_def,\
	false,false}

inline static void xbool_parse(xbool*o,toc*tc,token tk){
	o->super.type="bool";
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
			longjmp(_jmp_buf,1);
		}else{
			o->lh_negate=neg;
		}


		xexpls_parse_next(&o->lh,tc,tk);

//		o->lh=toc_read_next_xexpr(tc);

		if(*tc->srcp=='='){
			tc->srcp++;
			if(*tc->srcp!='='){
				printf("<file> <line:col> expected '=='\n");
				longjmp(_jmp_buf,1);
			}
			o->op='=';
			tc->srcp++;
		}else if(*tc->srcp=='!'){
			tc->srcp++;
			if(*tc->srcp!='='){
				printf("<file> <line:col> expected '!='\n");
				longjmp(_jmp_buf,1);
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

		xexpls_parse_next(&o->rh,tc,tk);
//		o->rh=toc_read_next_xexpr(tc);

		if(*tc->srcp==')'){
			return;
		}
//		if(*tc->srcp=='}'){//? optional
//			return;
//		}

		//? keybits==1 && ok
		if(*tc->srcp=='&'){
			tc->srcp++;
			if(*tc->srcp=='&'){
				tc->srcp--;
				return;
			}else{
				printf("<file> <line:col> expected &&\n");
				longjmp(_jmp_buf,1);
			}
		}else if(*tc->srcp=='|'){
			tc->srcp++;
			if(*tc->srcp=='|'){
				tc->srcp--;
				printf("<file> <line:col> expected ||\n");
				longjmp(_jmp_buf,1);
			}
		}else{
			// not encapsulated, could be if b==2 break
//			printf("<file> <line:col> expected && or ||\n");
//			longjmp(_jmpbufenv,1);
			return;
		}
	}

	// example (a==b && c==d)
	o->is_encapsulated=true;
	o->is_negated=neg;
	tc->srcp++;
	strb_add(&o->bool_op_list,0);
	while(1){
		xbool*e=malloc(sizeof(xbool));
		*e=xbool_def;
		xbool_parse(e,tc,tk);
		ptrs_add(&o->bool_list,e);
		token_skip_empty_space(&tc->srcp);
		if(*tc->srcp==')'){
			tc->srcp++;
			return;
		}
		if(*tc->srcp=='&'){
			tc->srcp++;
			if(*tc->srcp!='&'){
				printf("<file> <line:col> expected && \n");
				longjmp(_jmp_buf,1);
			}
			tc->srcp++;
			strb_add(&o->bool_op_list,'&');
		}else if(*tc->srcp=='|'){
			tc->srcp++;
			if(*tc->srcp!='|'){
				printf("<file> <line:col> expected || \n");
				longjmp(_jmp_buf,1);
			}
			tc->srcp++;
			strb_add(&o->bool_op_list,'|');
		}else{
			printf("<file> <line:col> expected && or ||\n");
			longjmp(_jmp_buf,1);
		}

	}
}
