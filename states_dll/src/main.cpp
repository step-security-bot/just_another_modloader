#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

#include <string>

#include <mingw-std-threads/mingw.thread.h>
#include <detours.h>
#include <stdfunc.hpp>
#include <main.hpp>
#include <original_functions.hpp>
#include <hook_functions.hpp>

void ( __cdecl* original::portFunction )( struct struct0* _argument ) = nullptr;

size_t     onHostedFunctionCount;
uintptr_t* onHostedfunctionAddresses;
size_t     onConnectionFunctionCount;
uintptr_t* onConnectionfunctionAddresses;

void waitForWindow( uintptr_t _moduleAddress ) {
    const uintptr_t l_magicAddress = 0x0049AFC8;
    uintptr_t l_testAddress;

    do {
        try {
            l_testAddress = read< uintptr_t >( _moduleAddress + l_magicAddress );

        } catch ( ... ) {
            l_testAddress = (uintptr_t)NULL;
        }

        Sleep( 500 );
    } while ( !l_testAddress );
}

extern "C" void __declspec( dllexport ) onGameStarted( size_t _functionCount, uintptr_t* _functionAddresses ) {
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );

    // MessageBoxA(
    //     NULL,
    //     (
    //         "0x" +
    //         numberToHexString( l_moduleAddress )
    //     ).c_str(), // box text
    //     "on",      // box name
    //     0
    // );

    waitForWindow( l_moduleAddress );

    // MessageBoxA(
    //     NULL,
    //     std::to_string(
    //         _functionCount
    //     ).c_str(), // box text
    //     "on",      // box name
    //     0
    // );

    for ( uint32_t _functionIndex = 0; _functionIndex < _functionCount; _functionIndex++ ) {
        // MessageBoxA(
        //     NULL,
        //     (
        //         "0x" +
        //         numberToHexString(
        //             (int)_functionAddresses[ _functionIndex ]
        //         )
        //     ).c_str(), // box text
        //     "on",      // box name
        //     0
        // );

        ( (modFunction_t*)_functionAddresses[ _functionIndex ] )();
    }
}

extern "C" void __declspec( dllexport ) onHosted( size_t _functionCount, uintptr_t* _functionAddresses ) {
    onHostedFunctionCount     = _functionCount;
    onHostedfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );

    // MessageBoxA(
    //     NULL,
    //     (
    //         "0x" +
    //         numberToHexString( l_moduleAddress )
    //     ).c_str(), // box text
    //     "on",      // box name
    //     0
    // );

    waitForWindow( l_moduleAddress );

    // MessageBoxA(
    //     NULL,
    //     std::to_string(
    //         _functionCount
    //     ).c_str(), // box text
    //     "on",      // box name
    //     0
    // );

    // uintptr_t l_originalFunctionAddress = getAddress(
    //     getModule( "th155_beta.exe" ) + 0x004DBBAC,
    //     { 0x10, 0x48, 0x50, 0x34, 0x1C, 0x18, 0x80, 0x28, 0x44, 0xE40 }
    // );

    // void ( __cdecl* l_originalFunction )( struct s0* a1 ) = ( void ( __cdecl* )( struct s0* a1 ) )( l_originalFunctionAddress );

    // RegisterHook( l_originalFunction, hookPortFunction, "Function Hooked!" );
}

extern "C" void __declspec( dllexport ) onConnection( size_t _functionCount, uintptr_t* _functionAddresses ) {
    onConnectionFunctionCount     = _functionCount;
    onConnectionfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );

    // MessageBoxA(
    //     NULL,
    //     (
    //         "0x" +
    //         numberToHexString( l_moduleAddress )
    //     ).c_str(), // box text
    //     "on",      // box name
    //     0
    // );

    waitForWindow( l_moduleAddress );

    // MessageBoxA(
    //     NULL,
    //     std::to_string(
    //         _functionCount
    //     ).c_str(), // box text
    //     "on",      // box name
    //     0
    // );

    // uintptr_t l_originalFunctionAddress = getAddress(
    //     getModule( "th155_beta.exe" ) + 0x004DBBAC,
    //     { 0x10, 0x48, 0x50, 0x34, 0x1C, 0x18, 0x80, 0x28, 0x44, 0xE40 }
    // );

    // void ( __cdecl* l_originalFunction )( struct s0* a1 ) = ( void ( __cdecl* )( struct s0* a1 ) )( l_originalFunctionAddress );

    // RegisterHook( l_originalFunction, hookPortFunction, "Function Hooked!" );
}

// i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -I "../include" -shared *.cpp ../lib/*.o -o states.dll
