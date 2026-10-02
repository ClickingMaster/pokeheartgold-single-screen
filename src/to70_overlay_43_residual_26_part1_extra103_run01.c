#pragma force_active on
#ifndef TO50_OVERLAY43_R26_PART1_PRIVATE_H
#define TO50_OVERLAY43_R26_PART1_PRIVATE_H
#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern u8 gSystem[];
extern const u8 ov43_0222EE84[];
extern const u8 ov43_0222EE42[];

BOOL ov43_0222D230(u8 *, int);
void ov43_0222DB28(void *, void *, int);
void ov43_0222DB94(void *, void *, int);
void ov43_0222AD40(void *, int, int);
void ov43_0222ED18(void *, int);
int TouchscreenHitbox_FindHitboxAtTouchNew(const void *);
void PlaySE(int);

int ov43_0222D24C(void *, void *, void *, int);
int ov43_0222D2FC(void *, void *, void *);
int ov43_0222D3B8(void *, void *, void *, int *);
#endif

#pragma force_active on

int ov43_0222D24C(void *p, void *state, void *work, int input) {
    switch (input) {
    case 0:
        if (!ov43_0222D230(p, S16_AT(p, 0))) {
            break;
        }
        PlaySE(0x5DD);
        return 6;
    case 1:
        PlaySE(0x5DD);
        return 6;
    case 2:
        ov43_0222DB28(p, work, S16_AT(p, 0));
        ov43_0222DB94(p, work, S16_AT(p, 0));
        U32_AT(p, 8) = 2;
        return 3;
    case 3:
        ov43_0222DB28(p, work, S16_AT(p, 0));
        ov43_0222DB94(p, work, S16_AT(p, 0));
        U32_AT(p, 8) = 3;
        return 3;
    case 4:
        U32_AT(p, 8) = 0;
        U16_AT(p, 0xC) = 4;
        ov43_0222AD40(work, 2, 1);
        return 5;
    case 5:
        U32_AT(p, 8) = 1;
        U16_AT(p, 0xC) = 4;
        ov43_0222AD40(work, 3, 1);
        return 5;
    }
    return 2;
}

int ov43_0222D2FC(void *p, void *state, void *work) {
    u32 keys;

    if (U32_AT(gSystem, 0x48) & 0xCF3) {
        U32_AT(state, 0) = 0;
    }
    if (U32_AT(gSystem, 0x48) & 3) {
        return ov43_0222D24C(p, state, work, 1);
    }
    keys = U32_AT(gSystem, 0x4C);
    if ((keys & 0x20) || (U8_AT(state, 0xE) && (keys & 0x200))) {
        ov43_0222ED18(p, -1);
        return ov43_0222D24C(p, state, work, 2);
    }
    if ((keys & 0x10) || (U8_AT(state, 0xE) && (keys & 0x100))) {
        ov43_0222ED18(p, 1);
        return ov43_0222D24C(p, state, work, 3);
    }
    if (U32_AT(gSystem, 0x44) & 0x40) {
        return ov43_0222D24C(p, state, work, 4);
    }
    if (U32_AT(gSystem, 0x44) & 0x80) {
        return ov43_0222D24C(p, state, work, 5);
    }
    return 2;
}

int ov43_0222D3B8(void *p, void *state, void *work, int *handled) {
    int hit;
    s16 delta;

    if (U32_AT(p, 0x10) != 0) {
        hit = TouchscreenHitbox_FindHitboxAtTouchNew(ov43_0222EE84);
    } else {
        hit = TouchscreenHitbox_FindHitboxAtTouchNew(ov43_0222EE42);
    }
    if (hit == -1) {
        return 2;
    }
    if (hit == 0) {
        *handled = 1;
        return ov43_0222D24C(p, state, work, 4);
    }
    if (hit == 1) {
        *handled = 1;
        return ov43_0222D24C(p, state, work, 5);
    }
    if (ov43_0222D230(p, (s16)(hit - 2))) {
        S16_AT(p, 0) = hit - 2;
        *handled = 1;
        return ov43_0222D24C(p, state, work, 1);
    }
    delta = (s16)((hit - 2) - S16_AT(p, 0));
    if (delta == 0) {
        return 2;
    }
    *handled = 1;
    ov43_0222ED18(p, delta);
    if (delta >= 0) {
        return ov43_0222D24C(p, state, work, 3);
    }
    return ov43_0222D24C(p, state, work, 2);
}
