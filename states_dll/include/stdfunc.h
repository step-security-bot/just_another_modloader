///////////////
/// @file stdfunc.h
/// @brief Declarations of many useful functions and lambdas related to replace standart if needed.
///////////////
#ifndef _STDFUNC_H

#define _STDFUNC_H

#if defined( _WIN32 )

#include <windows.h>
// #pragma comment( lib, "user32" )
// #pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "AdvApi32.lib")
#include <tlhelp32.h>

#else // _WIN32

#include <unistd.h>
#include <stdio.h>

#endif // _WIN32

#include <stdint.h>
#include <stdbool.h>
#include <detours.h>

#ifndef INT8_MIN

#define INT8_MIN -128

#endif // INT8_MIN

///////////////
/// @brief Pass from Python, same as NULL.
///////////////
#define Pass (void*)0

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
/// @brief Undetouring macro simplified.
///////////////
#define RemoveHook( _originalFunctionAddress, _hookFunctionAddress, _functionName )\
    RegisterHook( _originalFunctionAddress, _hookFunctionAddress, _functionName )

const char* const boolToString( bool _boolean ) const;

char* numberToHexString( int32_t _number );

uint32_t* pop(
    uint32_t* _array,
    uint32_t _lengthOfArray,
    const uint32_t _elementToPop
);

uint_fast32_t lengthOfInt( long _number );

uint_fast32_t lengthOfString( char* _string );

void print( const char* _text, const uint32_t _lengthOfText ) const;

int Pow( int32_t _number, uint32_t _exp );

uint32_t Rand( void );

void SRand( unsigned long _seed );

void clearConsole( void );

void* Malloc( uint32_t _numberOfBytes );

void Free( void* _firstbyte );

void Memcpy(
    void* _destination,
    const void* _source,
    size_t _numberOfBytes
);

char* Ltoa( long _number, char* _cString );

int32_t findDuplicate( int32_t* _numbers, uint32_t _lengthOfArray );

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

#endif // _STDFUNC_H
