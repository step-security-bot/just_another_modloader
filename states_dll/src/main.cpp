#if defined( _WIN32 )

#include <windows.h>

#else // _WIN32

#error Only Windows is supported

#endif // _WIN32

#include <string>

// Copyright (c) 2016, Mega Limited
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <mingw.thread.h>

#include <stdfunc.h>
#include <main.hpp>

size_t     g_onHostedFunctionCount;
uintptr_t* g_onHostedfunctionAddresses;
size_t     g_onConnectionFunctionCount;
uintptr_t* g_onConnectionfunctionAddresses;

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
        { 0x0 },
        1,
        true
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
    g_onHostedFunctionCount     = _functionCount;
    g_onHostedfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );
}

extern "C" void __declspec( dllexport ) onConnection(
    size_t _functionCount,
    uintptr_t* _functionAddresses
) {
    g_onConnectionFunctionCount     = _functionCount;
    g_onConnectionfunctionAddresses = _functionAddresses;
    const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );
}
