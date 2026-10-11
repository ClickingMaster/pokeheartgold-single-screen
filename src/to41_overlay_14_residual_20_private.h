#ifndef TO40_OPUS_R2_OV14_R20_PRIVATE_H
#define TO40_OPUS_R2_OV14_R20_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

extern u32 BgCellManager_UpdateCellMove(void *, u32);
extern void FillBgTilemapRect(void *, u32, u32, u32, u32, u32, u32, u32);
extern void ScheduleBgTilemapBufferTransfer(void *, u32);
extern void ManagedSprite_GetPositionXY(void *, s32 *, s32 *);

extern void PCBox_MoveWallpaperMenuSprites(void *, u32);
extern void PCBox_SlideInChangeBoxButton(void *);
extern void PCBox_SlideInSummaryButton(void *);
extern u32 PCBox_UpdateActionMenuSlide(void *);
extern u32 ov14_021E813C(void *);

extern u32 IsPaletteFadeFinished(void);
extern u32 PCBox_UpdateCursorSlide(void *);
extern u32 ov14_021E9434(void *);
extern u32 PCBox_IsSpriteAnimated(void *, u32);
extern void ov14_021E7AD4(void *, u32, u32, u32);
extern u32 ov14_021E7B8C(void *);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void ManagedSprite_SetPositionXY(void *, s32, s32);
extern void PCBox_SetIconLayer(void *, u32, u32);
extern void PCBox_SetupMonMove(void *, u32, u32);
extern u32 PCBox_GetMoveEntryTarget(void *, u32);
extern u32 PCBox_AnimateMonMove(void *);
extern void PCBox_ApplyMoveEntryIcons(void *, void *);
extern u32 PCBox_UpdatePartyPanelSlide(void *);
extern void ov14_021E7FEC(void *);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void PCBox_SaveMenuInputState(void *);
extern void SysTask_Destroy(void *);
extern void PCBox_FreeGridInput(void *);
extern void PCBox_DestroyYesNoPrompt(void *);
extern void PCBox_FreePanels(void *);
extern void ov14_021F29AC(void *);
extern void PCBox_RemoveWindows(void *);
extern void PCBox_DestroyMsgResources(void *);
extern void PCBox_FreePaletteData(void *);
extern void PCBox_FreeBgLayers(void *);
extern void NARC_Delete(void *);
extern void GF_TouchpadStopSampling(void);
extern void GfGfx_EngineASetPlanes(u32);
extern void GfGfx_EngineBSetPlanes(u32);
extern void Heap_Free(void *);
extern void Heap_Destroy(u32);
extern void ov14_021F391C(void *, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PlaySE(u16);
u32 ov14_021EA928(void *);

extern void PCBox_ShowHeldItemIconOutline(void *);
extern void ov14_021E7AE4(void *, u32, u32);
extern u32 ov14_021E7B98(void *);

typedef u32 (*Ov14StateFunc)(void *);
extern Ov14StateFunc const sPCBoxStateTable[];

#endif
