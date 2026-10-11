#ifndef OVERLAY_14_STATE_INPUT_HANDLERS_6B8C_PRIVATE_H
#define OVERLAY_14_STATE_INPUT_HANDLERS_6B8C_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

#define WORK(p)   PTR_AT((p), 0x34)
#define GRID(p)   PTR_AT(WORK(p), 0x2c)
#define UNK2F0(p) PTR_AT(WORK(p), 0x2f0)

extern u32 gSystem[];

extern void *Heap_Alloc(u32, u32);
extern void GridInputHandler_SetNextInput(void *, u32);
extern u32 GridInputHandler_GetNextInput(void *);
extern u32 GridInputHandler_HandleInput_AllowHold(void *);
extern u32 GridInputHandler_GetUnk0F(void *);
extern void GridInputHandler_SetNextLastUnk0FInputs(void *, u32, u32, u32);
extern void GridInputHandler_SetAllEnabled(void *);
extern void GridInputHandler_ClearEnabledFlag(void *, u32);
extern void *GridInputHandler_GetDpadBox(void *, u32);
extern void DpadMenuBox_GetPosition(const void *, u8 *, u8 *);
extern void ManagedSprite_SetPositionXY(void *, s32, s32);
extern u32 TouchscreenHitbox_TouchNewIsIn(const void *);

extern const u8 ov14_021F86C8[];

extern void PCBox_SetPendingUpdateFunc(void *, void *);
extern void PCBox_UpdateCursorSlide(void *);
extern void ov14_021EA180(void *);
extern u32 PCBox_IsActionMenuOpen(void *);
extern u32 PCBox_IsChangeBoxButtonShown(void *);
extern u32 PCBox_IsSummaryButtonShown(void *);
extern void PCBox_ClearMonInfoAndSelection(void *);
extern void PCBox_PlaceCursorOnGridInput(void *, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_LoadTrayPageThumbnails(void *);
extern void PCBox_UpdateTrayCursor(void *);
extern void PCBox_UpdateCurrentBoxMarker(void *);
extern void PCBox_DrawTrayBoxNameAndCount(void *);
extern void PCBox_LoadWallpaperThumbnails(void *);
extern void PCBox_UpdateCurrentWallpaperMarker(void *);
extern void PCBox_UpdateWallpaperCursor(void *);
extern void PCBox_DrawWallpaperName(void *);

void PCBox_StartCursorSlide(void *q, u32 a1, u32 a2);

void ov14_021F6B8C(void *p, u32 a1, u32 a2);
void ov14_021F6BA4(void *p, u32 a1);
u32 PCBox_GetDepositMenuInput(void *p);
void ov14_021F6C3C(void *p, u32 a1, u32 a2);
void ov14_021F6C94(void *p, u32 a1);
void ov14_021F6D14(void *p, u32 a1, u32 a2);
void ov14_021F6E68(void *p, u32 a1);
u32 PCBox_GetWithdrawMenuInput(void *p);
void ov14_021F6F08(void *p, u32 a1, u32 a2);
void ov14_021F6F70(void *p, u32 a1);
u32 PCBox_GetMoveModeMenuInput(void *p);
void ov14_021F7010(void *p, u32 a1, u32 a2);
void ov14_021F708C(void *p, u32 a1, u32 a2);
void ov14_021F70A4(void *p, u32 a1);
u32 PCBox_GetBoxTrayInput(void *p);
void ov14_021F70F4(void *p, u32 a1);
void ov14_021F7184(void *p, u32 a1, u32 a2);
void ov14_021F7308(void *p, u32 a1);
u32 ov14_021F7340(void *p);
u32 PCBox_GetMoveModePartyMenuInput(void *p);
void ov14_021F73E4(void *p, u32 a1, u32 a2);
void ov14_021F7444(void *p, u32 a1, u32 a2);
u32 PCBox_GetItemModeMenuInput(void *p);
void ov14_021F7528(void *p, u32 a1, u32 a2);
void ov14_021F7594(void *p, u32 a1, u32 a2);
void ov14_021F75AC(void *p, u32 a1);
u32 PCBox_GetItemModePartyMenuInput(void *p);
void ov14_021F7620(void *p, u32 a1, u32 a2);
void ov14_021F7670(void *p, u32 a1, u32 a2);
void ov14_021F7688(void *p, u32 a1);
void ov14_021F7700(void *p, u32 a1, u32 a2);
void ov14_021F7854(void *p, u32 a1);
void ov14_021F7878(void *p, u32 a1);
void ov14_021F78F8(void *p, u32 a1, u32 a2);
void ov14_021F7A2C(void *p, u32 a1);
u32 ov14_021F7A50(void *p);
u32 PCBox_IsSummaryShortcutPressed(void *p);

#endif
