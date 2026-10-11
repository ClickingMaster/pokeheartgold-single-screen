#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Elem {
    u32 a, b;
};

struct Tag {};

struct Vec8 {
    Elem *ptr;
    u32 size, cap;
};

struct Obj {
    u8 pad[0x50];
    Vec8 vec;
};

extern "C" int ov45_0223AE84(Obj *o, const void *data, u32 len);
extern "C" void ov45_0223ACF0(Vec8 *v, const Elem *first, const Elem *last, Tag);
extern "C" int ov45_0223ACA4(Obj *o, const void *data, u32 len);

extern "C" int ov45_0223ACA4(Obj *o, const void *data, u32 len) {
    ov45_0223AE84(o, data, len);
    const Elem *first = (const Elem *)data;
    ov45_0223ACF0(&o->vec, first, first + len / sizeof(Elem), Tag());
    return 1;
}
