#ifdef __WIN32__
#include <windows.h>

extern "C" void __declspec( dllexport ) onGameStarted() {
    MessageBoxA(
        NULL,
        "GameStarted", // box text
        "on",          // box name
        0
    );
}

extern "C" void __declspec( dllexport ) onHosted() {
    MessageBoxA(
        NULL,
        "Hosted", // box text
        "on",     // box name
        0
    );
}

extern "C" void __declspec( dllexport ) onConnection() {
    MessageBoxA(
        NULL,
        "Connection", // box text
        "on",         // box name
        0
    );
}

#else
#error Only Windows is supported
#endif

// i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -shared test.cpp -o test.dll
