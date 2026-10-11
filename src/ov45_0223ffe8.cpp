#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Vec {
    u32 *ptr;
    u32 size, cap;
};

struct Tag {};

extern "C" void *memmove(void *, const void *, u32);
extern "C" u32 ov45_0223EA34(Vec *v, u32 n);
extern "C" void ov45_022421E4(Vec *v, u32 n, u32 cap);

struct Rec {
    u32 recommend(Vec *v, u32 n) {
        return ov45_0223EA34(v, n);
    }
};

inline void Copy(u32 *f, u32 *l, u32 *d) {
    memmove(d, f, (l - f) * sizeof(u32));
}

extern "C" void ov45_0223FFE8(Vec *v, u32 *first, u32 *last);
#pragma opt_common_subs off

extern "C" void ov45_0223FFE8(Vec *v, u32 *first, u32 *last) {
    u32 n = last - first;
    u32 cap = v->cap;
    if (cap < n) {
        Rec r = Rec();
        ov45_022421E4(v, n, r.recommend(v, n - cap));
    }
    Copy(first, last, v->ptr);
    v->size = n;
}

#pragma opt_common_subs reset
