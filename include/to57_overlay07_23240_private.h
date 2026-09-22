#ifndef TO57_NEW34_PRIVATE_H
#define TO57_NEW34_PRIVATE_H
#include "global.h"

void *ov07_022324D8(void *, int);
void ov07_02231FE4(void *, void *);
int ov07_0221C4A8(void *, int);
void ov07_02232020(void *, u32, void *, void *);
void ov07_022323FC(void *, u32, void *, void *);
void ov07_0221C410(void *, void *, void *);
struct Ov07Work23240;
void ov07_02223240(void *, struct Ov07Work23240 *);
void ov07_02223350(void *);
int ov07_022227D8(void *);
void ov07_022227A8(void *, int, int, int, int);
void Pokepic_SetAttr(void *, int, int);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ManagedSprite_TickFrame(void *);
void SpriteSystem_DrawSprites(void *);
void BgSetPosTextAndCommit(void *, int, int, int);
void ov07_0221C448(void *, void *);
void Heap_Free(void *);
void ToggleBgLayer(int, int);
struct Ov07FadeWork;
void ov07_022233D8(void *, struct Ov07FadeWork *);
void ov07_022236F0(void *, struct Ov07FadeWork *);
u16 ov07_0221C468(void *);
int ov07_0223192C(void *, u16);
int ov07_0221FB7C(int, int);
int ov07_0221BFD0(void *);
int ov07_0221BFC0(void *);
void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
void PaletteData_LoadNarc(void *, int, int, int, int, int, int);
void BgClearTilemapBufferAndCommit(void *, int);
void ov07_02223550(void *);
void ov07_02223864(void *);

#endif
