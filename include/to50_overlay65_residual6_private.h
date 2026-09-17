#ifndef TO50_OV65_R6_PRIVATE_H
#define TO50_OV65_R6_PRIVATE_H
#include "global.h"

#include "system.h"
#include "touchscreen.h"
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef void (*Ov65Callback)(void *);
typedef struct Ov65StateR6 {
    u8 pad0[0x94];
    s32 selection;
    u8 pad98[0xC0];
    void *menu;
    u8 pad15C[0x540];
    u16 enabled[12][8];
    u8 pad75C[0x1AC4];
    Ov65Callback callback;
} Ov65StateR6;
extern void PlaySE(u32);
extern void ov65_0221CB5C(void *);
extern void ov65_0221DD34(s32, void *, s32);
extern void ov65_0221DBF4(void *);
extern void ov65_0221E144(void *);
extern void ov65_0221E9A8(void *);
extern void ov65_0221F3F4(void *);
extern const TouchscreenHitbox ov65_0221FE6C[];
s32 ov65_0221DF1C(void *);
s32 ov65_0221DFB8(void *);
#endif
