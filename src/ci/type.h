#pragma once
#include"../lib.h"
#include "field.h"
#include "func.h"
typedef struct type{
	str name;
	dynp/*owns str*/extends;
	dynp/*owns field*/fields;
	dynp/*owns func*/funcs;
}type;

#define type_def (type){str_def,dynp_def,dynp_def,dynp_def}
