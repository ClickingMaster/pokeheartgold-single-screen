#ifndef OV14_R12_PRIVATE_H
#define OV14_R12_PRIVATE_H
#include "global.h"
#define P8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define P32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PP(p, o)  (*(void **)((u8 *)(p) + (o)))
extern void BgCellManager_DrawCellAt(void *, u32, u32, u32);
extern void BgCellManager_StartCellMove(void *, s32, s32, s32, u8);
extern void BgCellManager_GetCellPosition(void *, u32, s8 *, s8 *);
extern u32 BgCellManager_UpdateCellMove(void *, u32);
extern void FillBgTilemapRect(void *, u32, u32, u32, u32, u32, u32, u32);
extern void PCBox_DrawMarkingMenuEntry(void *, u32);
extern void PCBox_UpdatePartyIconPositions(void *);
void PCBox_ToggleMarking(void *, u32);
void PCBox_SlideInMarkingMenu(void *);
void PCBox_SlideOutMarkingMenu(void *);
void ov14_021E7E78(void *);
void ov14_021E7E88(void *);
void ov14_021E7E98(void *);
void ov14_021E7EC0(void *);
void ov14_021E7ED0(void *);
void PCBox_SlideInPartyPanel(void *);
void PCBox_SlideOutPartyPanel(void *);
void ov14_021E7FB8(void *);
void ov14_021E7FEC(void *);
void ov14_021E8020(void *);
void ov14_021E8048(void *);
void ov14_021E805C(void *);
void ov14_021E8094(void *);
u32 PCBox_UpdatePartyPanelSlide(void *);
u32 ov14_021E813C(void *);
#endif
