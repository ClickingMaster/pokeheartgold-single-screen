#ifndef TO56_W10_PRIVATE_H
#define TO56_W10_PRIVATE_H
#include "global.h"

typedef struct FadeWork {
    void *owner;
    void *palette;
    int state;
    int count;
    int unk10;
    int unk14;
    int unk18;
    int delay;
} FadeWork;

typedef struct ScaleWork {
    void *owner;
    void *spriteSystem;
    int state;
    s16 x;
    s16 y;
    void *sprite;
    u8 anim[0x24];
    int a;
    int b;
    int c;
    int d;
    int count;
    int mode;
    int flag;
} ScaleWork;

typedef struct MoveWork {
    void *owner;
    int state;
    s16 x;
    s16 y;
    void *pic;
    u8 anim[0x24];
    int a;
    int b;
    int c;
    int d;
    int e;
    int delay;
    int packed;
    int hold;
} MoveWork;

void GF_AssertFail(void);
void Heap_Free(void *);
void *ov07_022324D8(void *, int);
void *ov07_0221FA78(void *);

extern u32 ov07_0221C4A8(void *p, int i);
int ov07_0221E6C8(void *);
void *ov07_0221C410(void *, void *, void *);
void ov07_0221C448(void *, void *);
void *ov07_0221C514(void *);
u16 ov07_0221C468(void *);
u16 ov07_0221C470(void *);
void *ov07_0221FA48(void *, int);
int ov07_0221FAA0(void *, int);
int ov07_0221FAB0(void *);
int ov07_0221FA04(void *, int);
int ov07_0223197C(void *, int);
int ov07_0221C4E8(void *, int);
int ov07_0221FB78(void *, int);
void ov07_02231E08(void *, int, int);
int PaletteData_BeginPaletteFade(void *, int, u16, s8, u8, u8, u16);
void Pokepic_StartPaletteFade(void *, int, int, int, int);
int Pokepic_ResumePaletteFade(void *);
int Pokepic_GetAttr(void *, int);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
void ManagedSprite_SetAffineScale(void *, u32, u32);
void SpriteSystem_DrawSprites(void *);
u32 _fsub(u32, u32);
void ov07_02222508(void *, s16, s16, s16, int);
int ov07_02222558(void *);
void ov07_02222644(void *, u32 *, u32 *);
void ov07_022226FC(void *, s16, s16, int, int);
void ov07_02222590(void *, s16, s16, s16, s16, s16, int);
int ov07_02222768(void *, void *);
int ov07_02222788(void *, void *);
void ov07_022226C4(void *, s16, s16, int, int);
void ov07_02223224(void *);
void ov07_02232020(void *, int, void *, void *);
void ov07_02223E94(void *, void *);
void ov07_02223EB4(void *);
void ov07_02224008(void *);
void ov07_02224348(void *);
void ov07_022245BC(void *);
void ov07_02223F5C(void *, FadeWork *);
void ov07_022241D8(void *, ScaleWork *);
void ov07_0222446C(void *, MoveWork *);
#endif
