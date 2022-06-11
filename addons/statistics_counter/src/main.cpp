#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

#include <string>

#include <mingw-std-threads/mingw.thread.h>
#include <stdfunc.hpp>

extern "C" void __declspec( dllexport ) onGameStarted( void ) {
    std::thread (
        []( void ) {
            uintptr_t l_testAddress;
            const uintptr_t l_moduleAddress = getModule( "th155_beta.exe" );
            uintptr_t l_firstPlayerHealth;
            uintptr_t l_secondPlayerHealth;

            AllocConsole();
            freopen_s( (FILE**)stdout, "CONOUT$", "w", stdout );

            do {
                try {
                    l_firstPlayerHealth = getAddress(
                        l_moduleAddress + 0x004DB6A8,
                        // {
                        //     0x98, 0x620, 0x50, 0x4, 0x18, 0x10
                        // },
                        {
                            0x10, 0x18, 0x4, 0x50, 0x620, 0x98
                        },
                        true
                    );
                    l_secondPlayerHealth = getAddress(
                        l_moduleAddress + 0x004DB6A8,
                        // {
                        //     0x98, 0xC, 0x60, 0xC, 0x18, 0x10
                        // },
                        {
                            0x10, 0x18, 0xC, 0x60, 0xC, 0x98
                        },
                        true
                    );

                    l_testAddress = read< uintptr_t >( l_firstPlayerHealth, true );

                    Sleep( 500 );

                } catch ( ... ) {
                    l_testAddress = (uintptr_t)NULL;
                }
            } while ( !l_testAddress );

            MessageBoxA(
                NULL,
                "Counter", // box text
                "on",      // box name
                0
            );
        }
    ).detach();
}

// i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -I "../include" -shared *.cpp -o counter.dll
