#ifndef POKEHEARTGOLD_OVERLAY14_BOX_NAVIGATION_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_NAVIGATION_PRIVATE_H

#include "global.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_DrawReturnButton(void *, u32);
extern void ov14_021E8328(void *);
extern void PCBox_SlideInPartyButton(void *);
extern void ov14_021E82A8(void *);
extern void PCBox_CreateGridInput(void *, u32, u32);
extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern u32 PCBox_GetWallpaperGfxIndex(void *, u32);
extern void PCBox_SetupWallpaperMenuSprites(void *);
extern void PCBox_UpdateCurrentWallpaperMarker(void *);
extern void PCBox_UpdateWallpaperCursor(void *);
extern void PCBox_DrawWallpaperName(void *);
extern void PCBox_SlideOutBoxMenu(void *);
extern void PCBox_SlideInWallpaperList(void *);
extern void PCBox_DrawChangeWallpaperTitle(void *);
extern void ov14_021E8560(void *);
extern void PCBox_PrintMenuPrompt(void *, u32, u32);
extern int GridInputHandler_HandleInput_AllowHold(void *);
extern void PlaySE(u32);
extern void PCBox_SelectWallpaperInPage(void *, u32);
extern void PCBox_ChangeWallpaperPage(void *, s32);
extern u32 PCBox_ConfirmWallpaperChoice(void *);
extern u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
extern u32 PCBox_FlashPanel(void *, u32, u32);
extern void ov14_021E8588(void *);
extern void PCBox_SlideOutWallpaperList(void *);

extern void ov14_021E95B4(void);
extern void ov14_021E94BC(void);
extern void ov14_021E9660(void);
extern void ov14_021E95C8(void);
extern void ov14_021E9604(void);
extern void ov14_021E96A8(void);

u32 ov14_021ED6D0(void *);
u32 PCBox_State_OpenWallpaperMenu(void *);
u32 ov14_021ED7B8(void *);
u32 PCBox_State_SetupWallpaperMenuInput(void *);
u32 PCBox_State_WallpaperMenuInput(void *);
u32 ov14_021ED920(void *);
u32 ov14_021ED940(void *);

#endif
