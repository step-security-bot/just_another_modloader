#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

extern "C" void __declspec( dllexport ) onGameStarted( void ) {
    MessageBoxA(
        NULL,
        "GameStarted", // box text
        "on",          // box name
        0
    );
}

extern "C" void __declspec( dllexport ) onHosted( void ) {
    MessageBoxA(
        NULL,
        "Hosted", // box text
        "on",     // box name
        0
    );
}

extern "C" void __declspec( dllexport ) onConnection( void ) {
    MessageBoxA(
        NULL,
        "Connection", // box text
        "on",         // box name
        0
    );
}

// i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -shared test.cpp -o test.dll
