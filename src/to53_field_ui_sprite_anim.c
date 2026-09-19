#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

extern int sub_02086D98(int, int);
extern void ManagedSprite_SetAnim(void *, int);

void sub_02085FFC(void *state);

void sub_02085FFC(void *state) {
    s32 index;
    u32 offset;
    u32 value;
    u32 previous;

    U32_AT(state, 0x268) = 1;
    U8_AT(state, 0x280) = 0;
    if (U32_AT(state, 0x2d4) == 0) {
        U32_AT(state, 0x214) = U32_AT(state, 0x2d0) - 1;
        offset = U32_AT(state, 0x214) * 0x1c;
        value = U32_AT(state, offset + 4);
        U32_AT(state, 0x3ac) = 1;
        U32_AT(state, 0x3b0) = value;
        U32_AT(state, 0x3b4) = 1;
        return;
    }
    index = U32_AT(state, 0x214);
    offset = index * 0x1c;
    U32_AT(state, offset) = 0;
    value = sub_02086D98(U32_AT(state, offset), U32_AT(state, offset + 8));
    ManagedSprite_SetAnim(PTR_AT(state, offset + 0xc), value);
    previous = U32_AT(state, offset + 4);
    if (index > (s32)U32_AT(state, 0x3f0)) {
        offset = (index - 1) * 0x1c;
        value = sub_02086D98(U32_AT(state, offset), U32_AT(state, offset + 8));
        ManagedSprite_SetAnim(PTR_AT(state, offset + 0xc), value);
        value = U32_AT(state, offset + 4);
        if (previous != value) {
            U32_AT(state, 0x3ac) = 1;
            U32_AT(state, 0x3b0) = value;
            U32_AT(state, 0x3b4) = 1;
            return;
        }
        U32_AT(state, 0x3ac) = 2;
        U32_AT(state, 0x3b0) = index - 1;
    }
}
