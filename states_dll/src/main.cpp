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

void handleStates( void ) {
    uintptr_t l_address = getAddress(
        getModule( "th155_beta.exe" ) + 0x004DA638,
        { 0x0 }
    );

    std::string l_consoleString;

    const char l_assetExtension[] = ".pat";

    for ( ;; ) {
        l_consoleString = (char*)l_address;

        l_consoleString.erase(
            l_consoleString.find( l_assetExtension ) + sizeof( l_assetExtension ) - 1
        );

        // if ( l_consoleString == "" ) {
        // }

        Sleep( 500 );
    }
}

extern "C" void __declspec( dllexport ) onGameStarted( size_t _functionCount, uintptr_t* _functionAddresses ) {
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );

    waitForWindow( l_moduleAddress );

    for ( uint32_t _functionIndex = 0; _functionIndex < _functionCount; _functionIndex++ ) {
        ( (modFunction_t*)_functionAddresses[ _functionIndex ] )();
    }

    handleStates();
}

extern "C" void __declspec( dllexport ) onHosted( size_t _functionCount, uintptr_t* _functionAddresses ) {
    onHostedFunctionCount     = _functionCount;
    onHostedfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );
}

extern "C" void __declspec( dllexport ) onConnection( size_t _functionCount, uintptr_t* _functionAddresses ) {
    onConnectionFunctionCount     = _functionCount;
    onConnectionfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );
}
