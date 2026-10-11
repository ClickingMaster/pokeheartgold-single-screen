#ifndef TO50_OVERLAY14_R20_2_PRIVATE_H
#define TO50_OVERLAY14_R20_2_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov14Motion {
    u8 x;
    u8 y;
    u8 dx;
    u8 dy;
    u32 xNegative : 1;
    u32 yNegative : 1;
    u32 frames : 30;
} Ov14Motion;

extern u32 BgCellManager_UpdateCellMove(void *, u32);
extern void ManagedSprite_GetPositionXY(void *, s32 *, s32 *);
extern void ManagedSprite_SetPositionXY(void *, s32, s32);
extern u32 PCBox_UpdateActionMenuSlide(void *);
extern u32 ov14_021E813C(void *);
extern u32 PCBox_UpdatePartyPanelSlide(void *);
extern void ov14_021F69F0(void *, u32);
extern u32 System_GetTouchHeldCoords(u32 *, u32 *);
extern u32 PCBox_GetTouchedBoxSlot(s16, s16);
extern u32 PCBox_GetTouchedPartySlot(s16, s16, const void *);
extern const u8 ov14_021F7C08[];
extern void ov14_021E6F3C(void *, u32, u32);
extern void PCBox_SetupMonMoveToPartyEnd(void *, u32, u32);
extern void PCBox_SetSelectedIconOutlineVisible(void *, u32);
extern u32 PCBox_GetMoveEntryTarget(void *, u32);
extern void ov14_021E7FEC(void *);
extern void PCBox_SlideInActionMenu(void *);
extern void ov14_021E8824(void *);
extern void ov14_021E7FB8(void *);
extern void PCBox_SlideOutPartyPanel(void *);
extern void ov14_021E7ED0(void *);
extern void ov14_021E8328(void *);
extern void PCBox_GetSlotPosition(u32, s16 *, s16 *, u32);
extern void PCBox_UpdateSelectedIconOutline(void *);
extern u32 PCBox_AnimateMonMove(void *);
extern void PCBox_ApplyMoveEntryIcons(void *, void *);
extern u32 ov14_021E66F4(void *);
extern void PCBox_ClearMonInfo(void *);
extern void PCBox_SlideInPartyPanel(void *);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
u32 PCBox_UpdateDepositDrag(void *);
u32 PCBox_UpdateCursorSlide(void *);
u32 PCBox_UpdateWithdrawDrag(void *);

#endif
