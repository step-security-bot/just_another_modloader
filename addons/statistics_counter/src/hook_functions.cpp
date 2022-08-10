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

int32_t hook::fun_1160f70( struct s0* ecx, int32_t a2, struct s4* a3 ) {

    return ( original::fun_1160f70( ecx, a2, a3 ) );
    // struct s5* eax4;
    // struct s1* ecx5;
    // int32_t ecx6;
    // int8_t al7;
    // int32_t eax8;
    // int8_t bl9;
    // struct s2* ecx10;
    // struct s0* esi11;
    // uint32_t edi12;
    // uint32_t eax13;
    // struct s3* eax14;
    // int32_t tmp32_15;
    // int1_t zf16;
    // struct s5* edx17;

    // eax4 = g0;
    // g0 = (struct s5*)((int32_t)__zero_stack_offset() - 4 - 12);
    // ecx5 = ecx->f28;
    // ecx6 = ecx5->f24;
    // al7 = fun_1173f30(ecx6);
    // eax8 = 0;
    // if (!al7 || 1) {
    //     bl9 = 0;
    // } else {
    //     ecx10 = *(struct s2**)(&ecx->pad28 + 12);
    //     esi11 = (struct s0*)&ecx->pad28;
    //     edi12 = esi11->f44;
    //     esi11->f48 = a3->f4;
    //     eax13 = a3->f0;
    //     esi11->f44 = eax13;
    //     if (eax13 & 0x8000000) {
    //         esi11->f48->f4 = esi11->f48->f4 + 1;
    //     }
    //     if (edi12 & 0x8000000 && (ecx10->f4 = ecx10->f4 - 1, !ecx10->f4)) {
    //         eax14 = ecx10->f0;
    //         eax14->f4();
    //     }
    //     eax8 = 0;
    //     bl9 = 1;
    // }
    // if (!1 && (tmp32_15 = g4 - 1, g4 = tmp32_15, zf16 = g4 == 0, zf16)) {
    //     edx17 = g0;
    //     edx17->f4();
    // }
    // *(int8_t*)&eax8 = bl9;
    // g0 = eax4;
    // return eax8;
}

void hook::portFunction( void ) {
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

    MessageBoxA(
        NULL,
        "test", // box text
        "on",   // box name
        0
    );

    return ( original::portFunction() );
}