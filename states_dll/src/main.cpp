// #define __WIN32__

#ifdef __WIN32__
#include <windows.h>
#include <tlhelp32.h>
#else
#error Only Windows is supported
#endif

#include <cstdarg>
#include <string>
#include <vector>
#include <stdio.h>

#include <mingw-std-threads/mingw.thread.h>
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

struct s0 {
    int32_t port;
    char pad1[ 4 ];
    int32_t f2;
};

void ( __cdecl* originalPortFunction )( struct s0* a1 ) = nullptr;

void hookPortFunction( struct s0* a1 ) {
    // Fetching port
    int32_t port = a1->port;

    // Waiting for console string
    std::thread ( []() {
        Address thcrapOutAddress = getAddress( modulePtr + 0x004DA638, { 0x00 } );
        std::string thcrapOut;

        do {
            thcrapOut = read< std::string >( thcrapOutAddress );

            for ( unsigned short letter = 0; letter < (thcrapOut).size(); letter++ ) {
                if ( (thcrapOut)[ letter ] == '\n' ) {
                    (thcrapOut).resize( letter );

                    break;
                }
            }

            Sleep( 500 );
        } while ( thcrapOut != "AnimationSet2D::Load data/system/select/bgm_name/bgm_name.pat" );
    } ).detach();

    return originalPortFunction( a1 );
}

template< const uint32_t _lengthOfText >
void print( const char (&_text)[ _lengthOfText ] ) {
    //! <b>[print]</b>
    /// Write text to console
    /// @code{.cpp}
    #ifdef _WIN32
    WriteConsoleA(
        GetStdHandle( STD_OUTPUT_HANDLE ), // Console handle
        _text,                             // Pointer to text
        _lengthOfText,                     // Length of text
        NULL,                              // Buffer to write value of characters written
        NULL                               // Reserved. Must be NULL
    );
    #else
    for ( uint32_t _symbolIndex = 0; _symbolIndex < _lengthOfText; _symbolIndex++ ) {
        putc( _text[ _symbolIndex ], stdout );
    }
    #endif
    /// @endcode
    //! <b>[print]</b>
}

template < typename I >
std::string numberToHexString( I w, size_t hex_length = sizeof( I ) << 1 ) {
    const char hex[] = "0123456789ABCDEF";

    std::string rc( hex_length, '0' );

    for (
        size_t i = 0, j = ( hex_length - 1 ) * 4;
        i < hex_length;
        ++i , j -= 4
    ) {
        rc[ i ] = hex[ ( w >> j ) & 0x0f ];
    }

    return ( rc );
}

template < typename T >
T read( const uintptr_t address, const bool memoryCheck = false ) noexcept {
    if ( memoryCheck ) {
        MEMORY_BASIC_INFORMATION MBI;

        VirtualQuery(
            reinterpret_cast< LPCVOID >( address ),
            &MBI,
            sizeof( MBI )
        );

        if ( !( MBI.State & MEM_COMMIT ) ) {
            print( "\nMEM_COMMIT\n" );

            return ( T{} );
        }

        if ( MBI.Protect & ( PAGE_GUARD | PAGE_NOCACHE | PAGE_NOACCESS ) ) {
            print( "\nPAGE_GUARD\n" );

            if (
                !VirtualProtect(
                    MBI.BaseAddress,
                    MBI.RegionSize,
                    PAGE_READWRITE,
                    &MBI.Protect
                )
            ) {
                print( "\nVirtualProtect\n" );

                return ( T{} );
            }

            T returnValue = *reinterpret_cast< T* >( address );
            DWORD dwOldProtect;

            VirtualProtect(
                MBI.BaseAddress,
                MBI.RegionSize,
                MBI.Protect,
                &dwOldProtect
            );

            return ( returnValue );
        }
    }

    return ( *reinterpret_cast< T* >( address ) );
}

uintptr_t getModule( const char* module ) noexcept {
    MODULEENTRY32 moduleEntry{ sizeof( MODULEENTRY32 ) };

    const HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE |
        TH32CS_SNAPMODULE32,
        0
    );

    if ( Module32First( snapshot, &moduleEntry ) ) {
        do {
            if ( !strcmp( module, static_cast< char* >( moduleEntry.szModule ) ) ) {
                CloseHandle( snapshot );

                return ( reinterpret_cast< uintptr_t >( moduleEntry.hModule ) );
            }

        } while ( Module32Next( snapshot, &moduleEntry ) );
    }

    return ( (uintptr_t)nullptr );
}

template< typename T, const size_t _lengthOfText >
uintptr_t getAddress( uintptr_t addr, const T (&vect)[ _lengthOfText ] ) noexcept {
    for ( size_t i = 0; i < _lengthOfText; i++ ) {
        addr = read< uintptr_t >( addr );
        addr += (char)vect[ i ];
    }

    return ( addr );
}

void waitForWindow( uintptr_t moduleAddress ) {
    const uintptr_t magicAddress = 0x0049AFC8;

    uintptr_t testAddress;

    do {
        try {
            testAddress = read< uintptr_t >( moduleAddress + magicAddress );

        } catch ( ... ) {
            testAddress = (uintptr_t)NULL;
        }

        Sleep( 500 );
    } while ( !testAddress );
}

void waitForAssetLoading( uintptr_t moduleAddress ) {
    const uintptr_t magicAddress  = 0x004DA638;
    const uintptr_t magicOffset   = 0x00;
    const std::string magicString = "AnimationSet2D::Load data/actor/common/common.pat";

    uintptr_t consoleStdout = getAddress( moduleAddress + magicAddress, { magicOffset } );
    std::string consoleOutput;

    do {
        consoleOutput = read< std::string >( consoleStdout );

        for (
            uint16_t symbolIndex = 0;
            symbolIndex < consoleOutput.length();
            symbolIndex++
        ) {
            if ( consoleOutput[ symbolIndex ] == '\n' ) {
                consoleOutput.resize( symbolIndex );

                break;
            }
        }

        Sleep( 500 );
    } while ( consoleOutput != std::string( magicString ) );
}

void _onGameStarted( void ) {
    // std::vector< uintptr_t > onGameStartedfunctionAddresses = *( static_cast< std::vector< uintptr_t>* >( _onGameStartedfunctionAddresses ) );

    const uintptr_t moduleAddress = getModule( "th155_beta.exe" );

    // waitForWindow( moduleAddress );

    // waitForAssetLoading( moduleAddress );

    MessageBoxA(
        NULL,
        ( "0x" + numberToHexString( moduleAddress ) ).c_str(), // box text
        "on",                                             // box name
        0
    );
}

extern "C" void __declspec( dllexport ) onGameStarted( size_t count, ... ) {
    va_list argsList;

    va_start( argsList, count );

    MessageBoxA(
        NULL,
        std::to_string(
            static_cast< int >( count )
        ).c_str(), // box text
        "on",      // box name
        0
    );

    std::vector< uintptr_t > onGameStartedfunctionAddresses;

    for ( uint32_t index = 0; index < count; index++ ) {
        uintptr_t argument = va_arg( argsList, uintptr_t );

        onGameStartedfunctionAddresses.push_back( argument );

        MessageBoxA(
            NULL,
            ( "0x" + numberToHexString( argument ) ).c_str(), // box text
            "on",                                             // box name
            0
        );
    }

    va_end( argsList );

    const uintptr_t moduleAddress = getModule( "th155_beta.exe" );

    MessageBoxA(
        NULL,
        ( "0x" + numberToHexString( moduleAddress ) ).c_str(), // box text
        "on",                                             // box name
        0
    );

    // std::thread(
    //     &_onGameStarted
    //     // static_cast< void* >( onGameStartedfunctionAddresses.data() )
    // ).detach();
}

extern "C" void __declspec( dllexport ) onHosted( size_t count, ... ) {
    va_list argsList;

    va_start( argsList, count );

    MessageBoxA(
        NULL,
        std::to_string(
            static_cast< int >( count )
        ).c_str(), // box text
        "on",      // box name
        0
    );

    for ( uint32_t index = 0; index < count; index++ ) {
        MessageBoxA(
            NULL,
            (
                "0x" +
                numberToHexString(
                    *static_cast< int* >(
                        va_arg( argsList, void* )
                    )
                )
            ).c_str(), // box text
            "on",      // box name
            0
        );
    }

    va_end( argsList );

    uintptr_t l_originalFunctionAddress = getAddress(
        getModule( "th155_beta.exe" ) + 0x004DBBAC,
        { 0x10, 0x48, 0x50, 0x34, 0x1C, 0x18, 0x80, 0x28, 0x44, 0xE40 }
    );

    void ( __cdecl* l_originalFunction )( struct s0* a1 ) = ( void ( __cdecl* )( struct s0* a1 ) )( l_originalFunctionAddress );

    RegisterHook( l_originalFunction, hookPortFunction, "Function Hooked!" );
}

extern "C" void __declspec( dllexport ) onConnection( size_t count, ... ) {
    va_list argsList;

    va_start( argsList, count );

    MessageBoxA(
        NULL,
        std::to_string(
            static_cast< int >( count )
        ).c_str(), // box text
        "on",      // box name
        0
    );

    for ( uint32_t index = 0; index < count; index++ ) {
        MessageBoxA(
            NULL,
            (
                "0x" +
                numberToHexString(
                    *static_cast< int* >(
                        va_arg( argsList, void* )
                    )
                )
            ).c_str(), // box text
            "on",      // box name
            0
        );
    }

    va_end( argsList );
}

// i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -I "../include" -shared main.cpp *.o -o states.dll
