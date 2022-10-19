#pragma once

#if defined( _WIN32 )

#include <windows.h>

#else // _WIN32

#error Only Windows is supported

#endif // _WIN32

extern size_t     onHostedFunctionCount;
extern uintptr_t* onHostedfunctionAddresses;
extern size_t     onConnectionFunctionCount;
extern uintptr_t* onConnectionfunctionAddresses;
