#ifndef TO55_W08_PRIVATE_H
#define TO55_W08_PRIVATE_H

#include "global.h"

#include "system.h"

#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTRAT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct W08Entry {
    u8 pad_000[0x348];
    u8 field_348;
    u8 pad_349[7];
    u8 field_350;
} W08Entry;

typedef struct W08Flags {
    u8 low : 4;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 high : 2;
} W08Flags;

typedef struct W08Work {
    u8 pad_0000[0xde];
    W08Flags flags;
    u8 pad_00df[0x1];
    u8 copied_entry[0x348];
    u8 field_428;
    u8 pad_0429[0x277b];
    void *save;
    u8 pad_2ba8[0xc];
    W08Entry *entries[3];
    s32 index;
    u8 pad_2bc4[0xc];
    u8 window[0x10];
    u8 pad_2be0[0x54];
    u32 counter;
    u8 pad_2c38[0x18c];
    void *sprite_left;
    void *sprite_right;
    u8 pad_2dcc[0xf7c];
    void (*callback)(void *);
    u32 selection;
    void *waiting_icon;
} W08Work;

void GF_AssertFail(void);
BOOL SaveSubstruct_AssertCRC(int);
void SaveSubstruct_UpdateCRC(int);
void ov74_022360A0(void *);
void ov74_0222E03C(void *);
void Sprite_SetAnimCtrlSeq(void *, int);
void PlaySE(u32);
void sub_02037AC0(u8);
void ov74_0222D9E0(void *, void *, int, int);
void *WaitingIcon_New(void *, u16);
void ov74_0222EB28(void *, int *, int);
void *memcpy(void *, const void *, u32);
void ov74_02229CE0(void *, void *, void *);

void ov74_0222E060(void *);
void ov74_0222E0D4(void *, BOOL, int *);
int ov74_0222E7EC(void *);

#endif
