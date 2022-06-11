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

void hook::portFunction( struct struct0* _argument ) {
    // Fetching port
    // int32_t l_port = _argument->port;

    // Waiting for "Character select" screen
    // std::thread ( []() {
    //     const uintptr_t   l_magicAddress  = 0x004DA638;
    //     const uintptr_t   l_magicOffset   = 0x00;
    //     const std::string l_magicString   = "AnimationSet2D::Load data/system/select/bgm_name/bgm_name.pat";
    //     uintptr_t         l_consoleStdout = getAddress(
    //         getModule( "th155_beta.exe" ) +
    //         l_magicAddress,
    //         { l_magicOffset}
    //     );
    //     std::string       l_consoleOutput;

    //     do {
    //         l_consoleOutput = read< std::string >( l_consoleStdout );

    //         for (
    //             uint16_t _symbolIndex = 0;
    //             _symbolIndex < l_consoleOutput.length();
    //             _symbolIndex++
    //         ) {
    //             if ( l_consoleOutput[ _symbolIndex ] == '\n' ) {
    //                 l_consoleOutput.resize( _symbolIndex );

    //                 break;
    //             }
    //         }

    //         Sleep( 500 );
    //     } while ( l_consoleOutput != std::string( l_magicString ) );
    // } ).detach();

    for ( uint32_t _functionIndex = 0; _functionIndex < onHostedFunctionCount; _functionIndex++ ) {
        MessageBoxA(
            NULL,
            (
                "0x" +
                numberToHexString(
                    (int)onHostedfunctionAddresses[ _functionIndex ]
                )
            ).c_str(), // box text
            "on",      // box name
            0
        );

        ( (modFunction_t*)onHostedfunctionAddresses[ _functionIndex ] )();
    }

    return ( original::portFunction( _argument ) );
}