#ifndef OVERLAY_80_022310C4_STATE_H
#define OVERLAY_80_022310C4_STATE_H

#include "global.h"

// 0xD98-byte trainer-house / frontier run state. Pointed to by ov80_0223DD44.
typedef struct TrHouse {
    u32 heapId;                      // 0x000
    u8 kind;                         // 0x004
    u8 idx;                          // 0x005
    u8 unk6;                         // 0x006
    u8 unk7;                         // 0x007
    u16 unk8;                        // 0x008
    u16 unkA;                        // 0x00A
    u32 unkC;                        // 0x00C
    u32 unk10;                       // 0x010
    u8 pad14[0x018 - 0x014];         // 0x014
    u16 unk18[0x14];                 // 0x018
    u8 pad40[0x260 - 0x040];         // 0x040
    u8 unk260[4];                    // 0x260
    void *party;                     // 0x264
    u16 unk268[0x14];                // 0x268
    u8 pad290[0x6F2 - 0x290];        // 0x290
    u16 unk6F2;                      // 0x6F2
    u8 unk6F4;                       // 0x6F4
    u8 unk6F5;                       // 0x6F5
    u8 pad6F6[2];                    // 0x6F6
    void *unk6F8;                    // 0x6F8
    void *save;                      // 0x6FC
    u8 pad700[4];                    // 0x700
    u8 unk704[4][9];                 // 0x704
    u16 unk728[(0xD8C - 0x728) / 2]; // 0x728
    void *unkD8C;                    // 0xD8C
    u8 padD90[0xD98 - 0xD90];        // 0xD90
} TrHouse;

typedef struct TrHouseSrc {
    u8 pad0[6]; // 0x00
    u16 unk6;   // 0x06
} TrHouseSrc;

typedef struct TrHouseWork {
    u32 unk0;             // 0x00
    u16 unk4;             // 0x04
    u16 unk6;             // 0x06
    u8 pad8[0x30 - 0x08]; // 0x08
} TrHouseWork;

#endif // OVERLAY_80_022310C4_STATE_H
