#ifndef W16_RENDER_PRIVATE_H
#define W16_RENDER_PRIVATE_H
#include "global.h"
typedef struct RenderPokemonPicState {
    void *spriteList;
    u8 filler_004[0x160];
    void *managedSprite;
    void *bgConfig;
    u8 layer;
    u8 x;
    u8 y;
    u8 state;
} RenderPokemonPicState;
void FillBgTilemapRect(void *, u32, u16, u32, u32, u32, u32, u32);
void ScheduleBgTilemapBufferTransfer(void *, u32);
void sub_0200F82C(RenderPokemonPicState *, u8, u16);
void sub_0200F9DC(RenderPokemonPicState *);
#endif
typedef struct W16Rect { int x,y,w,h; } W16Rect;
typedef struct W16PicTemplate { u16 narcID,charDataID,palDataID,species; u8 animated,pad[3]; u32 personality; } W16PicTemplate;
extern const W16Rect _020F5C40;
extern const W16Rect _020F5C50;
void *Heap_Alloc(u32,u32);
void Heap_Free(void *);
void sub_020143E0(u32,s32,u32,W16Rect *,void *);
void *sub_02014450(u32,s32,u32);
void *SpriteResourceCollection_Find(void *,int);
void *SpriteTransfer_GetCharProxy(void *);
void *SpriteTransfer_GetPaletteProxy(void *,void *);
void DC_FlushRange(const void *,u32);
void GX_LoadOBJ(const void *,u32,u32);
void GX_LoadOBJPltt(const void *,u32,u32);
void sub_0200F748(void *, W16PicTemplate *);
