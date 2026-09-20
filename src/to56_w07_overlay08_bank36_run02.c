#include "global.h"

#include "constants/heap.h"

#include "battle/battle_finger.h"
#include "battle/battle_system.h"

#include "sprite_system.h"
extern const s32 ov08_02225D2C[];
extern const s32 ov08_02225D30[];
void ov08_0222419C(u8 *);
void ov08_022241F4(u8 *);
void ov08_0222421C(u8 *, int);
void ov08_0222458C(u16 *, const u16 *, u16, u16, u8, u8);

typedef struct Ov08StateTail {
    u8 pad[0x113e];
    u8 a, b, c;
    u8 low : 4;
    u8 high : 4;
} Ov08StateTail;
void ov08_02224938(Ov08StateTail *, u8, u8);

void ov08_0222458C(u16 *dst, const u16 *src, u16 x, u16 y, u8 width, u8 height) {
    u16 i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            dst[i * width + j] = src[(y + i) * 32 + j + x];
        }
    }
}
void *ov08_022245DC(u8 *work, u32 type, u32 i);
void *ov08_022245DC(u8 *work, u32 type, u32 i) {
    switch (type) {
    case 0:
    case 1:
    case 2:
    case 3:
        return work + 0x328 + i * 0x120;
    case 4:
        return work + 0x688 + i * 0x104;
    case 5:
    case 14:
    case 16:
        return work + 0xA98 + i * 0x32;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return work + 0xB2E + i * 0xC0;
    case 12:
        return work + 0xE2E + i * 0x32;
    case 13:
        return work + 0xEF6 + i * 0x32;
    case 15:
        return work + 0x688 + ((i << 6) + i) * 4;
    default:
        return NULL;
    }
}
u16 ov08_02224684(u8 *work, u32 type, u32 a2, u32 a3);
u16 ov08_02224684(u8 *work, u32 type, u32 a2, u32 a3) {
    if (a2 == 3) {
        return 5;
    }
    switch (type) {
    case 0:
    case 1:
    case 2:
    case 3:
        return 0;
    case 4:
        return 3;
    case 5:
    case 12:
    case 13:
    case 14:
    case 16:
        return 2;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return (u16)(work[0x114d] + 8);
    case 15:
        if (a3 == 2) {
            return (u16)(work[0x114d] + 8);
        }
        return 1;
    default:
        return 0;
    }
}
void ov08_022246F4(u8 *work, u8 *dst, u32 which, u32 idx);
void ov08_022246F4(u8 *work, u8 *dst, u32 which, u32 idx) {
    u16 *src;
    u16(*d)[16] = (u16(*)[16])dst;
    u16 i, j;
    if (which == 0) {
        src = (u16 *)(work + 0xfbe + idx * 32);
    } else if (which == 1) {
        src = (u16 *)(work + 0x101e + idx * 32);
    } else if (which == 2) {
        src = (u16 *)(work + 0x107e + idx * 32);
    } else if (which == 3) {
        src = (u16 *)(work + 0x10de + idx * 32);
    } else {
        return;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            d[i][j + 6] = src[i * 4 + j];
        }
    }
}
