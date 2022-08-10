#pragma once

#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

#include <string>

#include <main.hpp>

namespace hook {

int32_t fun_1160f70( struct s0* ecx, int32_t a2, struct s4* a3 );

void portFunction( void );

}