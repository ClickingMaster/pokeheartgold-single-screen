#ifndef TO54_W11_PRIVATE_H
#define TO54_W11_PRIVATE_H
#include "global.h"

typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct Pokepic Pokepic;

void *ov07_022324D8(void *, u32);
void ov07_02231FE4(void *, void *);
void ov07_0221C410(void *, void (*)(void), void *);
void ov07_0221C448(void *, void *);
void ov07_0221F9E8(void *, void *);
void ov07_0222A8D8(ManagedSprite *);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_SetFlipMode(ManagedSprite *, int);
void ManagedSprite_OffsetPositionXY(ManagedSprite *, int, int);
void Sprite_DeleteAndFreeResources(ManagedSprite *);
void SpriteSystem_DrawSprites(SpriteSystem *);
void Heap_Free(void *);
ManagedSprite *SpriteSystem_NewSprite(void *, void *, void *);

void ov07_02232508(void *);
BOOL ov07_0222260C(void *);
void ov07_02222644(void *, s32 *, s32 *);
void ov07_022226FC(ManagedSprite *, s32, s32, u32, u32);
void ManagedSprite_SetAffineScale(ManagedSprite *, float, float);
void ManagedSprite_TickFrame(ManagedSprite *);
void ManagedSprite_SetAffineOverwriteMode(ManagedSprite *, int);
void ManagedSprite_SetPriority(ManagedSprite *, int);
void ManagedSprite_SetDrawPriority(ManagedSprite *, int);
void ov07_02222590(void *, int, int, int, int, int, int);
u32 ov07_0221C470(void *);
Pokepic *ov07_0221FA48(void *, u32);
u32 ov07_0221FAA0(void *, u32);
ManagedSprite *ov07_0221C4E8(void *, int);
u32 ov07_0221FAE8(void *);
u32 ov07_0221FA04(void *, u32);
u32 Pokepic_GetAttr(Pokepic *, int);
void ov07_02222268(void *, s32, s32, s32, s32, s32);
BOOL ov07_022222B4(void *);
void Pokepic_SetAttr(Pokepic *, int, int);
void ov07_0221C7B8(void *, void *, int);
void *ov07_0221BFD0(void *);
void ov07_0223494C(void *, void *);
void ov07_0223475C(void *, void *);
u32 ov07_0221C468(void *);
void ov07_02231A50(void *, u32, s16 *);
u32 ov07_0223192C(void *, u32);
void *ov07_0221C4A8(void *, int);
void ov07_02232020(void *, void *, s16 *, void *);
void *Sprite_GetPaletteProxy(void *);
u32 ObjPlttTransfer_GetPaletteVramOffset(void *, int);
BOOL ov07_02222EE8(void *);
void ov07_02222EF8(void *);
void *ov07_02222F10(void *, void *, int, int, int, int, int, int, int, int, int);
#endif
