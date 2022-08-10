#pragma once

#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

#include <string>

typedef void modFunction_t( void );

extern size_t     onHostedFunctionCount;
extern uintptr_t* onHostedfunctionAddresses;
extern size_t     onConnectionFunctionCount;
extern uintptr_t* onConnectionfunctionAddresses;

struct struct0 {
    int32_t port;
    char    pad1[ 4 ];
    int32_t f2;
};