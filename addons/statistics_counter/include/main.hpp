#pragma once

#ifdef __WIN32__
#include <windows.h>
#else
#error Only Windows is supported
#endif

#include <string>
#include <stdint.h>

typedef void modFunction_t( void );

struct s1 {
    int8_t pad24[24] ;
    int32_t f24;
};

struct s3 {
    int8_t pad4[4];
    int32_t f4;
};

struct s2 {
    struct s3* f0;
    int32_t f4;
};

struct s0 {
    int8_t pad28[28];
    struct s1* f28;
    int8_t pad44[12];
    uint32_t f44;
    struct s2* f48;
};

struct s4 {
    uint32_t f0;
    struct s2* f4;
};

struct s5 {
    int8_t pad4[4];
    int32_t f4;
};

extern struct s5* g0;

extern int8_t fun_1173f30(int32_t ecx);

extern int32_t g4;