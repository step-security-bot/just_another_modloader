#pragma once

#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

#include <string>
#include <stdio.h>

#include <detours.h>

#define RegisterHook( _originalFunction, _hookedFunction, _functionName )\
    DetourTransactionBegin();\
    DetourUpdateThread( GetCurrentThread() );\
    DetourAttach(\
        &_originalFunction,\
        &_hookedFunction\
    );\
    ( (\
        DetourTransactionCommit() != NO_ERROR\
    ) ? (\
        printf( "\nError {}\n", _functionName )\
    ) : (\
        printf( "\nHooked: {}\n", _functionName )\
    ) );

#define RemoveHook( _originalFunction, _hookedFunction, _functionName )\
    RegisterHook( _originalFunction, _hookedFunction, _functionName )

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