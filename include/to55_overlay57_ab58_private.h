#ifndef TO55_W11_PRIVATE_H
#define TO55_W11_PRIVATE_H

#include "global.h"

typedef struct Ov57BallParams {
    s32 kind;
    s32 heapId;
    s32 callbackId;
    s32 ball;
    s32 species;
    s32 unk14;
    s32 unk18;
    void *spriteSystem;
    void *paletteData;
    void *unused;
} Ov57BallParams;

typedef struct Ov57PicParams {
    s32 kind;
    s32 unused;
    void *mon;
} Ov57PicParams;

typedef struct Ov57WorkW11 {
    u8 pad000[0xDC];
    void *spriteSystem;
    u8 pad0E0[8];
    void *paletteData;
    u8 pad0EC[0xDC];
    void *pokepic;
    u8 pad1CC[0x84];
    void *picWork;
    void *ballWork;
    u8 pad258[0x2C];
    void *anim;
    u8 pad288[0x164];
    s32 capsuleIndex;
    u8 pad3F0[0xC];
    s32 parentState;
    s32 state;
    s32 delay;
    u8 pad40C[0x50];
    void *mon;
} Ov57WorkW11;

void GfGfx_EngineATogglePlanes(u32, BOOL);
void PaletteData_BeginPaletteFade(void *, u32, u16, u16, u8, s8, u8);
u32 PaletteData_GetSelectedBuffersBitmask(void *);
u32 GetMonData(void *, int, void *);
void SetMonData(void *, int, void *);
void *ov07_02232694(s32, Ov57PicParams *);
void ov07_022329B0(void *);
BOOL ov07_02232A04(void *);
void ov07_02232A44(void *);
BOOL ov07_02232A54(void *);
void ov07_02232AB8(void *);
void *ov07_02233DB8(Ov57BallParams *);
BOOL ov07_02233E88(void *);
BOOL ov07_02233EA0(void *);
void ov07_02233ECC(void *);
BOOL Pokepic_IsAnimFinished(void *);
BOOL sub_02017068(void *, int);
void PlaySE(u16);
void ov57_0223857C(u8 *, u8 *);
void ov57_02239728(void *, u32, u32, u32);
void ov57_0223B7C4(void *);
void ov57_0223B948(void *, BOOL);
void ov57_0223B950(void *);
void ov57_0223B9C8(void *);
void ov57_0223BA1C(void *);
BOOL ov57_0223BA40(void *);
BOOL ov57_0223BABC(void *);
void ov57_0223BB38(void *, BOOL);
void ov57_0223BB4C(void *);
void ov57_0223BB5C(void);
int ov57_0223AB58(Ov57WorkW11 *);

#endif
