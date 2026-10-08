#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
struct Elem { u32 a, b; };
struct Alloc { void destroy(Elem *, u32) {} };
struct Vec8 { Elem *ptr; u32 size, cap; };
struct Obj { u8 pad[0x50]; Vec8 vec; };
extern "C" int ov45_0223AE84(Obj *o, const void *data, u32 len);
// @fn ov45_0223AE84
extern "C" int ov45_0223AE84(Obj *o, const void *data, u32 len) {
    if (len % 8) return 0;
    if (len != 0) return 1;
    u32 n = o->vec.size;
    Alloc a = Alloc();
    a.destroy(o->vec.ptr, n);
    o->vec.size -= n;
    return 1;
}
