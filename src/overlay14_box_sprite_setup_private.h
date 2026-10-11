#ifndef TO37_SOL_R10_OVERLAY14_RESIDUAL54_PRIVATE_H
#define TO37_SOL_R10_OVERLAY14_RESIDUAL54_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
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

extern void PCBox_GetSlotPosition(u32, s16 *, s16 *, u32);
extern void PCBox_SetHeldItemIconPosition(void *, s32, s32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_SetSpriteDrawPriority(void *, u32, u32);
extern void PCBox_LoadSpriteCharsSub(void *, u32, void *, u32);

extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
extern void ManagedSprite_SetDrawFlag(void *, u32);
extern void ManagedSprite_SetPriority(void *, u32);
extern u32 ManagedSprite_GetPriority(void *);
extern u32 ManagedSprite_GetDrawPriority(void *);
extern u32 ManagedSprite_GetPaletteOverrideOffset(void *);
extern void ManagedSprite_SetPaletteOverride(void *, u32);
extern void *SpriteSystem_NewSprite(void *, void *, const void *);
extern void *SpriteSystem_NewSpriteWithYOffset(void *, void *, const void *, s32);
extern void SpriteSystem_LoadCharResObj(void *, void *, u32, u32, u32, u32, u32);
extern void SpriteSystem_LoadPlttResObj(void *, void *, u32, u32, u32, u32, u32, u32);
extern void SpriteSystem_LoadCellResObj(void *, void *, u32, u32, u32, u32);
extern void SpriteSystem_LoadAnimResObj(void *, void *, u32, u32, u32, u32);
extern void *Sprite_GetImageProxy(void *);
extern void Sprite_SetImageProxy(void *, void *);

extern void BgCellManager_GetCellPosition(void *, u32, s8 *, s8 *);
extern void *TypeIcon_GetNarc(void *);
extern u32 TypeIcon_GetCharFile(void *);
extern u32 TypeIcon_GetPalette(void *);
extern void *GfGfxLoader_GetCharData(void *, u32, u32, void **, u32);
extern void Heap_Free(void *);

extern const s8 sPCBoxOutlineOffsetsX[];
extern const s8 sPCBoxOutlineOffsetsY[];
extern const u8 ov14_021F810C[];
extern const u8 ov14_021F81A8[];
extern const u8 ov14_021F81DC[];
extern const u8 ov14_021F8278[];
extern const u8 ov14_021F82AC[];
extern const u8 ov14_021F82E0[];
extern const u8 ov14_021F8314[];
extern const u8 ov14_021F8348[];
extern const u8 ov14_021F837C[];
extern const u8 ov14_021F83B0[];
extern const u8 ov14_021F83E4[];
extern const u8 ov14_021F844C[];
extern const u8 ov14_021F8480[];

void PCBox_MoveHeldItemIconToSlot(void *, u32, u32);
void ov14_021F39A0(void *, u32, u32);
void PCBox_CreateHeldItemIconOutline(void *);
void PCBox_ShowHeldItemIconOutline(void *);
void PCBox_UpdateHeldItemIconOutline(void *);
void PCBox_UpdateInfoItemIconPosition(void *);
void PCBox_LoadTypeIconResources(void *);
void PCBox_CreateTypeIconSprites(void *);
void PCBox_LoadTypeIcon(void *, u32, u32, u32);
void PCBox_DrawTypeIcons(void *, void *);
void PCBox_LoadUiSpriteResources(void *);
void PCBox_CreateUiSprites(void *);
void PCBox_CreateSelectedIconOutline(void *);

#endif
