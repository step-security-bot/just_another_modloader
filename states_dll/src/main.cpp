#if defined( _WIN32 )

#include <windows.h>

#else // _WIN32

#error Only Windows is supported

#endif // _WIN32

#include <string>

#include <mingw-std-threads/mingw.thread.h>
#include <stdfunc.h>
#include <main.hpp>

size_t     onHostedFunctionCount;
uintptr_t* onHostedfunctionAddresses;
size_t     onConnectionFunctionCount;
uintptr_t* onConnectionfunctionAddresses;

void waitForWindow( uintptr_t _moduleAddress ) {
    const uintptr_t l_magicAddress = 0x0049AFC8;
    uintptr_t l_testAddress;

    do {
        try {
            l_testAddress = (uintptr_t)read( _moduleAddress + l_magicAddress, true );

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

        print( l_consoleString.c_str(), l_consoleString.length() );

        // if ( l_consoleString == "" ) {
        // }

        Sleep( 500 );
    }
}

extern "C" void __declspec( dllexport ) onGameStarted(
    size_t _functionCount,
    uintptr_t* _functionAddresses
) {
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );

    waitForWindow( l_moduleAddress );

    for (
        uint32_t _functionIndex = 0;
        _functionIndex < _functionCount;
        _functionIndex++
    ) {
        ( (modFunction_t*)_functionAddresses[ _functionIndex ] )();
    }

    handleStates();
}

extern "C" void __declspec( dllexport ) onHosted(
    size_t _functionCount,
    uintptr_t* _functionAddresses
) {
    onHostedFunctionCount     = _functionCount;
    onHostedfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );
}

extern "C" void __declspec( dllexport ) onConnection(
    size_t _functionCount,
    uintptr_t* _functionAddresses
) {
    onConnectionFunctionCount     = _functionCount;
    onConnectionfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );
}
