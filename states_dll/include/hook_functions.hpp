#pragma once

#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

#include <string>

#include <main.hpp>

namespace hook {

void portFunction( struct struct0* _argument );

}