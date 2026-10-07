#include "overlay07_callbacks_w01_private.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0221C42C(void *owner, void *func, void *arg, u32 value) { ov07_0221BE68(2, owner, func, arg, value); }
void ov07_0221C448(void *owner, void *task) { ov07_0221BEA4(1, owner, task); }
void ov07_0221C458(void *owner, void *task) { ov07_0221BEA4(2, owner, task); }
u16 ov07_0221C468(void *p) { return U16_AT(PTR_AT(p, 0xc0), 0x14); }
u16 ov07_0221C470(void *p) { return U16_AT(PTR_AT(p, 0xc0), 0x16); }
void *ov07_0221C478(void *p) {
    void *q = PTR_AT(p, 0xc0);
    return *(void **)((u8 *)q + 0x1c + U32_AT(q, 0x18) * 4);
}
void *ov07_0221C488(void *p, int i) { return *(void **)((u8 *)PTR_AT(p, 0xc0) + 0x1c + i * 4); }
void *ov07_0221C494(void *p, int i) { return *(void **)((u8 *)PTR_AT(p, 0xc0) + 0x5c + i * 4); }
void *ov07_0221C4A0(void *p) { return PTR_AT(p, 0xc4); }
u32 ov07_0221C4A8(void *p, int i) { if (i >= 10) GF_AssertFail(); return *(u32 *)((u8 *)p + 0x94 + i * 4); }
