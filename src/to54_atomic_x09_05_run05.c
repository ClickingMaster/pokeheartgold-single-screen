#include "global.h"
#pragma require_prototypes off

#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(u8 **)((u8 *)(p) + (o)))

extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetAnim(void *, int);
extern void ov112_021F1624(void *, int, int);
extern void ov112_021F2204(void *, int, int);
extern void ov112_021F22B0(void *, int, int);
extern void ov112_021F1EFC(void *, int);
extern void GF_AssertFail(void);

void ov112_021F257C(u8 *p) {
    u8 *slot = p + p[0x13D] * 4;
    u8 *entry = PTR(slot, 0xC0);
    int row = ((int)entry[0x84] - 1) / 2;
    int base = row * 5;
    int diff;
    int code;
    ov112_021F2204(p, 0, 1);
    ov112_021F2204(p, 1, 0);
    diff = U16(entry, 0x78) - U16(entry, 0x7A);
    if (diff < 0) {
        diff = -diff;
    }
    if (entry[0x84] % 2 == 1) {
        code = diff >= 100 ? 1 : 2;
    } else {
        code = diff >= 100 ? 4 : 3;
    }
    ov112_021F1624(p, base, base + code);
}

void ov112_021F25FC(u8 *p) {
    u8 *slot = p + p[0x13D] * 4;
    u8 *entry = PTR(slot, 0xC0);
    int code;
    ov112_021F2204(p, 0, 1);
    ManagedSprite_SetDrawFlag(PTR(p, 0x84), 1);
    ManagedSprite_SetAnim(PTR(p, 0x84), 0xB);
    if (p[0x13F] != 0 && p[0x13F] <= p[0x13D]) {
        code = 0x1B;
    } else if (U16(entry, 0x78) >= 4000) {
        code = 0x1C;
    } else if (U16(entry, 0x78) >= 3000) {
        code = 0x1D;
    } else if (U16(entry, 0x78) >= 2000) {
        code = 0x1E;
    } else if (U16(entry, 0x78) >= 1000) {
        code = 0x1F;
    } else {
        code = 0x20;
    }
    ov112_021F1624(p, 0x1A, code);
}

void ov112_021F2688(u8 *p) {
    u8 *slot = p + p[0x13D] * 4;
    u8 *entry = PTR(slot, 0xC0);
    int code;
    ManagedSprite_SetDrawFlag(PTR(p, 0x84), 1);
    ManagedSprite_SetAnim(PTR(p, 0x84), 0xD);
    if (U16(entry, 0xA) != 0) {
        code = 0x21;
        ov112_021F2204(p, 0, 1);
    } else {
        code = 0x22;
        ov112_021F22B0(PTR(p, 0x80), (u8)U32(p, 0xC), 3);
    }
    ov112_021F1624(p, code, 0);
}

void ov112_021F26E8(u8 *p) {
    u8 *slot = p + p[0x13D] * 4;
    u8 *entry = PTR(slot, 0xC0);
    int code;
    ov112_021F2204(p, 0, 1);
    ov112_021F2204(p, 1, 0);
    if (p[0x140] != 0 && p[0x140] <= p[0x13D]) {
        code = 0x24;
    } else if (U16(entry, 0x78) >= 4000) {
        code = 0x25;
    } else if (U16(entry, 0x78) >= 3000) {
        code = 0x26;
    } else if (U16(entry, 0x78) >= 2000) {
        code = 0x27;
    } else if (U16(entry, 0x78) >= 1000) {
        code = 0x28;
    } else {
        code = 0x29;
    }
    ov112_021F1624(p, 0x23, code);
    ov112_021F24C4(p);
}

void ov112_021F2768(u8 *p) {
    u8 *slot = p + p[0x13D] * 4;
    u8 *entry = PTR(slot, 0xC0);
    int code;
    if (U16(entry, 0xA) != 0) {
        code = 0x2A;
        ov112_021F2204(p, 0, 1);
    } else {
        code = 0x2B;
        ov112_021F22B0(PTR(p, 0x80), (u8)U32(p, 0xC), 3);
    }
    ov112_021F2204(p, 1, 0);
    ov112_021F1624(p, code, 0);
}
