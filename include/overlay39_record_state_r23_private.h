#ifndef TO57_OV39_R23_PRIVATE_H
#define TO57_OV39_R23_PRIVATE_H
#include "global.h"

typedef struct Ov39AppR23 {
    u8 pad00[8];
    u32 state08;
    u32 state0C;
    u8 pad10[0x2C - 0x10];
    void *unk2C;
    u8 pad30[0x40 - 0x30];
    u32 printer40;
    u8 pad44[0x90 - 0x44];
    u32 unk90;
    u32 substate94;
    s32 timer98;
} Ov39AppR23;

typedef struct Ov39LargeRec {
    u32 word00;
    u32 word04;
    u32 word08;
    u16 half0C;
    u8 pad0E[0x24 - 0x0E];
    u16 arr24[8];
    s8 byte34;
    u8 byte35;
    u8 byte36;
    u8 byte37;
    u8 pad38[4];
    u32 word3C;
    u16 half40;
    u16 half42;
    u16 half44;
    u16 half46;
    u8 bytes48[40];
    u8 byte70;
    u8 byte71;
} Ov39LargeRec;

typedef struct Ov39SmallRec {
    u32 word00;
    u32 word04;
    u32 word08;
    u16 half0C;
    u16 arr0E[8];
    s8 byte1E;
    u8 byte1F;
    u8 byte20;
    u8 byte21;
    u8 pad22[2];
    u32 word24;
    u16 half28;
    u16 half2A;
    u16 half2C;
    u16 half2E;
    u8 bytes30[40];
    u8 byte58;
    u8 byte59;
    u16 tail5A[3];
} Ov39SmallRec;

BOOL ov39_022290A4(Ov39AppR23 *);
void ov39_0222915C(const Ov39LargeRec *, Ov39SmallRec *);
void ov39_02229224(const Ov39SmallRec *, Ov39LargeRec *);

BOOL ov39_02228A70(u32);
void ov39_02228B04(Ov39AppR23 *, void *, int, u32, u32);
void ov00_021EC8D8(void);
void sub_0203946C(void);
void sub_0203A914(void);
u32 sub_0202B994(void *);
#endif
