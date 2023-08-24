#ifndef _STDFUNC_H

#define _STDFUNC_H

///////////////
/// @file stdfunc.h
/// @brief Declarations of many useful functions and lambdas related to replace standard if needed.
///////////////
#pragma once

#ifdef _WIN32

#include <windows.h>
// #pragma comment( lib, "user32" )
// #pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "AdvApi32.lib")
#include <tlhelp32.h>

// Copyright (c) Microsoft Corporation. All rights reserved.

// Licensed under the MIT License.
#include <detours.h>

#else

#include <unistd.h>
#include <stdio.h>

#endif // _WIN32

#include <stdint.h>
#include <stdbool.h>

#ifndef INT8_MIN

#define INT8_MIN -128

#endif // INT8_MIN

///////////////
/// @brief Pass from Python, same as NULL.
///////////////
#define Pass (void*)0

///////////////
/// @brief How much memory to allocate.
///////////////
#define HEAP_MEMORY_SIZE 150

///////////////
/// @brief Hex length.
///////////////
#define HEX_LENGTH ( sizeof( int32_t ) << 1 )

///////////////
/// @brief Detouring macro simplified.
///////////////
#define RegisterHook( _originalFunctionAddress, _hookFunctionAddress, _functionName )\
    DetourTransactionBegin();\
    DetourUpdateThread( GetCurrentThread() );\
    DetourAttach(\
        &_originalFunctionAddress,\
        &_hookFunctionAddress\
    );\
    ( (\
        DetourTransactionCommit() != NO_ERROR\
    ) ? (\
        printf( "\nError %s\n", _functionName )\
    ) : (\
        printf( "\nHooked: %s\n", _functionName )\
    ) );

///////////////
/// @brief Un-detouring macro simplified.
///////////////
#define RemoveHook( _originalFunctionAddress, _hookFunctionAddress, _functionName )\
    RegisterHook( _originalFunctionAddress, _hookFunctionAddress, _functionName )

#if defined( __cplusplus )

extern "C" {

#endif // __cplusplus

const char* const boolToString( bool _boolean );

char* numberToHexString( int32_t _number );

uint_fast32_t lengthOfInt( long _number );

uint_fast32_t lengthOfCString( char* _string );

void print( const char* _text, const uint32_t _lengthOfText );

unsigned long Rand( void );

void SRand( unsigned long _seed );

void clearConsole( void );

void Memcpy( void* _destination, const void* _source, size_t _numberOfBytes );

char* Ltoa( unsigned long _number, char* _cString );

uintptr_t getModule( const char* _moduleName );

void* read(
    const uintptr_t _address,
    const bool _memoryCheck
);

uintptr_t getAddress(
    uintptr_t _moduleAddress,
    const uint32_t* _offsets,
    uint32_t _offsetsCount,
    const bool _memoryCheck
);

#if defined( __cplusplus )

}

#endif // __cplusplus

#endif // _STDFUNC_H
