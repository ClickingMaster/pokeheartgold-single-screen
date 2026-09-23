#include "global.h"
extern void ov112_021F2204(void *, int, int);
extern void ov112_021F1F3C(void *, int);
extern void ov112_021F1624(void *, int, int);
extern void ov112_021F22B0(void *, int, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetAnim(void *, int);
void ov112_021F2874(void *);
void ov112_021F2874(void *s) {
    u8 *entry = *(u8 **)((u8 *)s + 0xc0 + (*(u8 *)((u8 *)s + 0x13d) * 4));
    int y = entry[0x84];
    int delta = entry[0x77];
    int value = (y - 0x12) * 3 + 0x33;
    if (delta < 0xc8) {
        if (delta >= 0x3c) value++;
        else if (delta >= 0) value += 2;
    }
    if (value == 0x35) value = 0x34;
    ov112_021F2204(s, 0, 1);
    ov112_021F22B0(*(void **)((u8 *)s + 0x84), *(u32 *)((u8 *)s + 0xc) & 0xff, 1);
    ov112_021F1624(s, value, 0);
}

void ov112_021F28DC(void *);
void ov112_021F28DC(void *s) {
    ov112_021F2204(s, 0, 1);
    ov112_021F22B0(*(void **)((u8 *)s + 0x84), *(u32 *)((u8 *)s + 0xc) & 0xff, 3);
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)s + 0x8c), 1);
    ManagedSprite_SetAnim(*(void **)((u8 *)s + 0x8c), 8);
    ov112_021F1624(s, 0x42, 0);
}

void ov112_021F2920(void *);
void ov112_021F2920(void *s) {
    ov112_021F2204(s, 1, 1);
    ov112_021F1F3C((u8 *)s + 0xac, 3);
    ov112_021F1624(s, 0x43, 0);
}
