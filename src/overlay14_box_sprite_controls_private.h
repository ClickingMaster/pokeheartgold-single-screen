#ifndef POKEHEARTGOLD_OVERLAY14_BOX_SPRITE_CONTROLS_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_SPRITE_CONTROLS_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct SpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    s32 drawPriority;
    s32 pal;
    s32 vram;
    s32 resIdList[6];
    s32 bgPriority;
    s32 vramTransfer;
} SpriteTemplate;

extern void Heap_Free(void *);
extern void *Heap_AllocAtEnd(u32, u32);
extern void *AllocAtEndAndReadWholeNarcMemberByIdPair(u32, u32, u32);
extern u32 PCStorage_IsBonusWallpaperUnlocked(void *, u32);
extern void TextOBJ_SetPosition(void *, s32, s32);
extern void TextOBJ_SetSpritesDrawFlag(void *, u32);

extern void ManagedSprite_SetPositionXY(void *, s16, s16);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void ManagedSprite_SetDrawFlag(void *, u32);
extern void ManagedSprite_SetPriority(void *, u32);
extern u32 ManagedSprite_GetPriority(void *);
extern u32 ManagedSprite_GetDrawPriority(void *);

extern void SpriteSystem_LoadCharResObj(void *, void *, u32, u32, u32, u32, u32);
extern void SpriteSystem_LoadPlttResObj(void *, void *, u32, u32, u32, u32, u32, u32);
extern void SpriteSystem_LoadCellResObj(void *, void *, u32, u32, u32, u32);
extern void SpriteSystem_LoadAnimResObj(void *, void *, u32, u32, u32, u32);
extern void *SpriteSystem_NewSprite(void *, void *, const SpriteTemplate *);

extern u32 PCBox_GetWallpaperGfxIndex(void *, u32);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_SetSpriteDrawPriority(void *, u32, u32);
extern void PCBox_LoadSpriteCharsMain(void *, u32, void *, u32);
extern void PCBox_LoadTrayPageThumbnails(void *);

extern const s8 sPCBoxOutlineOffsetsX[];
extern const s8 sPCBoxOutlineOffsetsY[];
extern const u8 ov14_021F8068[];
extern const SpriteTemplate ov14_021F8140;
extern const SpriteTemplate ov14_021F8174;

void PCBox_SetSelectedIconOutlineVisible(void *p, u32 a1);
void PCBox_UpdateSelectedIconOutline(void *p);
void PCBox_LoadTrayBoxResources(void *p);
void PCBox_CreateTrayBoxSprites(void *p);
void PCBox_LoadWallpaperListResources(void *p);
void PCBox_CreateWallpaperListSprites(void *p);
void PCBox_SetBoxArrowsVisible(void *p, u32 a1);
void ov14_021F4410(void *q);
void PCBox_SetupWallpaperMenuSprites(void *p);
void PCBox_MoveWallpaperMenuSprites(void *q, s32 dy);
void PCBox_UpdateCurrentWallpaperMarker(void *p);
void PCBox_UpdateWallpaperCursor(void *p);
void PCBox_LoadWallpaperThumbnails(void *p);
void PCBox_ColorWallpaperThumbnail(void *p, u8 *buf, u32 wallpaper, u32 match, u32 size);
void ov14_021F46F4(void *q);
void PCBox_SetupBoxTraySprites(void *p);
void PCBox_MoveBoxTraySprites(void *p, s32 dy);
void PCBox_UpdateCurrentBoxMarker(void *p);
void PCBox_UpdateTrayCursor(void *p);

#endif
