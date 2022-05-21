// #define __WIN32__

#ifdef __WIN32__
#include <windows.h>
#include <tlhelp32.h>
#else
#error Only Windows is supported
#endif

#include <cstdarg>
#include <string>

#include <mingw-std-threads/mingw.thread.h>
#include <detours.h>
#include <stdfunc.hpp>
#include <main.hpp>
#include <original_functions.hpp>
#include <hook_functions.hpp>

void ( __cdecl* original::portFunction )( struct struct0* _argument ) = nullptr;

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

void waitForAssetLoading( uintptr_t _moduleAddress ) {
    const uintptr_t   l_magicAddress  = 0x004DA638;
    const uintptr_t   l_magicOffset   = 0x00;
    const std::string l_magicString   = "AnimationSet2D::Load data/actor/common/common.pat";
    uintptr_t         l_consoleStdout = getAddress( _moduleAddress + l_magicAddress, { l_magicOffset } );
    std::string       l_consoleOutput;

    do {
        l_consoleOutput = read< std::string >( l_consoleStdout );

        for (
            uint16_t _symbolIndex = 0;
            _symbolIndex < l_consoleOutput.length();
            _symbolIndex++
        ) {
            if ( l_consoleOutput[ _symbolIndex ] == '\n' ) {
                l_consoleOutput.resize( _symbolIndex );

                break;
            }
        }

        Sleep( 500 );
    } while ( l_consoleOutput != std::string( l_magicString ) );
}

extern "C" void __declspec( dllexport ) onGameStarted( size_t _argumentCount, ... ) {
    va_list l_argsList;

    va_start( l_argsList, _argumentCount );

    MessageBoxA(
        NULL,
        std::to_string(
            _argumentCount
        ).c_str(), // box text
        "on",      // box name
        0
    );

    for ( uint32_t _argumentIndex = 0; _argumentIndex < _argumentCount; _argumentIndex++ ) {
        MessageBoxA(
            NULL,
            (
                "0x" +
                numberToHexString(
                    va_arg( l_argsList, int )
                )
            ).c_str(), // box text
            "on",      // box name
            0
        );
    }

    va_end( l_argsList );

    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );

    MessageBoxA(
        NULL,
        (
            "0x" +
            numberToHexString( l_moduleAddress )
        ).c_str(), // box text
        "on",      // box name
        0
    );

    // waitForWindow( l_moduleAddress );

    // waitForAssetLoading( l_moduleAddress );
}

extern "C" void __declspec( dllexport ) onHosted( size_t _argumentCount, ... ) {
    va_list l_argsList;

    va_start( l_argsList, _argumentCount );

    MessageBoxA(
        NULL,
        std::to_string(
            _argumentCount
        ).c_str(), // box text
        "on",      // box name
        0
    );

    for ( uint32_t _argumentIndex = 0; _argumentIndex < _argumentCount; _argumentIndex++ ) {
        MessageBoxA(
            NULL,
            (
                "0x" +
                numberToHexString(
                    va_arg( l_argsList, int )
                )
            ).c_str(), // box text
            "on",      // box name
            0
        );
    }

    va_end( l_argsList );

    // uintptr_t l_originalFunctionAddress = getAddress(
    //     getModule( "th155_beta.exe" ) + 0x004DBBAC,
    //     { 0x10, 0x48, 0x50, 0x34, 0x1C, 0x18, 0x80, 0x28, 0x44, 0xE40 }
    // );

    // void ( __cdecl* l_originalFunction )( struct s0* a1 ) = ( void ( __cdecl* )( struct s0* a1 ) )( l_originalFunctionAddress );

    // RegisterHook( l_originalFunction, hookPortFunction, "Function Hooked!" );
}

extern "C" void __declspec( dllexport ) onConnection( size_t _argumentCount, ... ) {
    va_list l_argsList;

    va_start( l_argsList, _argumentCount );

    MessageBoxA(
        NULL,
        std::to_string(
            _argumentCount
        ).c_str(), // box text
        "on",      // box name
        0
    );

    for ( uint32_t _argumentIndex = 0; _argumentIndex < _argumentCount; _argumentIndex++ ) {
        MessageBoxA(
            NULL,
            (
                "0x" +
                numberToHexString(
                    va_arg( l_argsList, int )
                )
            ).c_str(), // box text
            "on",      // box name
            0
        );
    }

    va_end( l_argsList );
}

// i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -I "../include" -shared *.cpp ../lib/*.o -o states.dll
