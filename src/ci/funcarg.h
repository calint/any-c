#pragma once
#include"../lib.h"

typedef struct funcarg{
	str type;
	str name;
}funcarg;

#define funcarg_def (funcarg){str_def,str_def}
