#pragma once
#include"../lib.h"

typedef struct field{
	str type;
	str name;
	xexpr*initval;
}field;

#define field_def (field){str_def,str_def,NULL}

