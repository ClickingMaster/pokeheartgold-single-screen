#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
struct Vec { u32 *ptr; u32 size, cap; };
struct Tag {};
extern "C" void ov00_021EC2EC(int, void *, u32);
extern "C" void ov45_0223EA04(Vec *v, u32 n, Tag t);
struct Alloc {
    void release(u32 *p) { if (p) ov00_021EC2EC(0, p, 0); }
};
inline void Allocate(Vec *v, u32 n, Tag t) { ov45_0223EA04(v, n, t); }
extern "C" void ov45_022421E4(Vec *v, u32 size, u32 cap);
// @fn ov45_022421E4
extern "C" void ov45_022421E4(Vec *v, u32 size, u32 cap) {
    u32 n = v->size;
    Alloc a = Alloc();
    v->size -= n;
    if (v->ptr) {
        a.release(v->ptr);
        v->ptr = 0;
        v->cap = 0;
    }
    ov45_0223EA04(v, cap, Tag(Tag()));
}
