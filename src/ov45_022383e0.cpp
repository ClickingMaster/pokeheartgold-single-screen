#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void *memmove(void *, const void *, unsigned);

class Alloc {};

class TrueType {};

struct Vec {
    char *ptr;
    u32 size, cap;
};

extern "C" void ov45_02237800(Vec *, u32, Alloc);
extern "C" void ov45_022383E0(Vec *, char *, char *);

inline void CopyRange(char *first, char *last, char *out, TrueType) {
    memmove(out, first, last - first);
}

#pragma opt_common_subs off

extern "C" void ov45_022383E0(Vec *v, char *first, char *last) {
    if (last - first != 0) {
        ov45_02237800(v, last - first, Alloc());
        CopyRange(first, last, v->ptr + v->size, TrueType());
        v->size += last - first;
    }
}

#pragma opt_common_subs reset
