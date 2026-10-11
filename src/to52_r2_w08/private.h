#ifndef W08_OV80_PRIVATE_H
#define W08_OV80_PRIVATE_H
#include "global.h"
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
typedef struct W08Slot {
    u16 value[4];
} W08Slot;
typedef struct W08Work {
    u8 pad00[0x10];
    u8 mode;
    u8 index;
    u8 flag;
    u8 pad13[0x15];
    void *party;
    u8 pad2c[4];
    u16 values[14];
    u8 pad4c[0x324];
    u8 bytes370[4];
    u8 bytes374[4];
    u8 bytes378[4];
    u8 bytes37c[4];
    u8 pad380[0x14];
    W08Slot slots[4];
} W08Work;
void *ov80_02237B58(u8, int);
int ov80_02237D8C(u8);
void FrontierTrainer_GenerateMons(void *, u16, u16, void *, void *, void *, void *, u8);
void ov80_02237E30(void *);
BOOL Comm_GetLocalNetId(void);
int ov80_02237B24(u8, int);
int Party_GetCount(void *);
void *Party_GetMonByIndex(void *, int);
u32 GetMonData(void *, int, void *);
void ov80_02237D9C(void *);
void ov80_02232F08(void *);
void ov80_02232F60(void *);
int ov80_02233020(void *);
#endif
