#ifndef TO55_T14_PRIVATE_H
#define TO55_T14_PRIVATE_H

#include "global.h"

typedef struct Ov07CallbackWork {
    void *owner;
    void *resource;
    s32 state;
    void *picture;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1C;
    s32 field20;
    s32 field24;
    s32 field28;
    s32 field2C;
    u32 field30;
} Ov07CallbackWork;

typedef struct Ov07FadeWork {
    void *owner;
    s32 state;
    s32 timer;
    s16 transition;
} Ov07FadeWork;

void *ov07_022324D8(void *, u32);
void ov07_02230170(Ov07CallbackWork *);
void ov07_022301C4(Ov07CallbackWork *);
void ov07_022302A8(void *, Ov07CallbackWork *);
void ov07_022303A4(void *);
void ov07_0223049C(void *, Ov07FadeWork *);
void ov07_0221C410(void *, void *, void *);
void ov07_0221C448(void *, void *);
void *ov07_0221C470(void *);
void *ov07_0221FA48(void *, void *);
int ov07_0221FAC8(void *, void *);
u32 ov07_0221FAEC(void *, int);
u32 ov07_0221FB04(void *, int);
int ov07_0221FAE8(void *);
void *ov07_0221FAF8(void *, int);
void *ov07_0221FA78(void *);
u32 ov07_0221E6C8(void *);
void *ov07_0221BFD0(void *);
int ov07_02231924(void *, void *);
void *ov07_0221C4A0(void *);
void *ov07_02222C84(void *);
void ov07_02222C98(void *);
void ov07_02222C60(void *);
u32 ov07_02222D88(u16, u16);
void *ov07_02222D90(void *);
void *ov07_02222BE4(void *, u32, void *);
BOOL ov07_022222B4(void *);
void ov07_02222268(void *, int, int, int, int, int);
void ov07_02231EC0(void *, int, int);
int Pokepic_GetAttr(void *, int);
void Pokepic_SetAttr(void *, int, int);
void SetBgPriority(u8, u8);
void BG_ClearCharDataRange(u8, u32, u32, void *);
void BgClearTilemapBufferAndCommit(void *, u8);
void Heap_Free(void *);
void PaletteData_BeginPaletteFade(void *, int, u16, int, int, int, u16);
u32 PaletteData_GetSelectedBuffersBitmask(void *);

#endif
