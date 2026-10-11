#ifndef POKEHEARTGOLD_OVERLAY14_BOX_INPUT_STATE_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_INPUT_STATE_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

extern void GridInputHandler_SetButtonInputMode(void *, u32);
extern void GridInputHandler_SetNextInput(void *, u32);
extern u32 GridInputHandler_GetNextInput(void *);
extern void GridInputHandler_SetNextInput(void *, u32);
extern void *Party_GetMonByIndex(void *, u32);
extern u32 GetMonData(void *, u32, void *);
extern u32 ItemIdIsMail(u16);

extern void PlaySE(u32);
extern u32 PCBox_GetSlotMonData(void *, u32, u32, u32);
extern u32 PCBox_PartyHasOtherUsableMon(void *, u32);
extern void PCBox_ApplyMonMove(void *);
extern void PCBox_RevertShayminFormOnMove(void *, u32, u32);
extern void PCBox_ShowSlotMonInfo(void *, u32);
extern void PCBox_ClearMonInfo(void *);
extern void PCBox_ClearMonInfoAndSelection(void *);
extern void ov14_021E7E98(void *);
extern void ov14_021E7EC0(void *);
extern void PCBox_SlideInPartyPanel(void *);
extern void PCBox_SlideOutPartyPanel(void *);
extern void ov14_021E81A8(void *, u32);
extern void PCBox_SlideInPartyButton(void *);
extern void ov14_021E82A8(void *);
extern void ov14_021E8328(void *);
extern void PCBox_SlideInSummaryButton(void *);
extern void ov14_021E94BC(void *);
extern u32 ov14_021E9518(void *);
extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern void PCBox_FreeMoveWork(void *);
extern u32 ov14_021F0CD8(void *);
extern u32 PCBox_OpenBoxTray(void *, u32);
extern void ov14_021F1580(void *, u32);
extern void ov14_021F2FDC(void *);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void PCBox_SetBoxArrowsVisible(void *, u32);
extern void PCBox_DrawPartyButton(void *, u32);
extern void ov14_021F5E94(void *, u32);
extern void PCBox_DrawReturnButton(void *, u32);
extern void PCBox_DrawExitButton(void *, u32);
extern void PCBox_DrawTrayBoxNameAndCount(void *);
extern void ov14_021F63A8(void *);
extern void ov14_021F63B8(void *);
extern void ov14_021F63F0(void *);
extern void PCBox_DrawSummaryButton(void *, u32);
extern void PCBox_CloseMessageWindow(void *, u32);
extern void ov14_021F6688(void *, u32);
extern void PCBox_PrintReleaseMessage(void *, u32, u32);
extern void PCBox_PrintMonActionMessage(void *, u32, u32, u32);
extern void PCBox_CreateGridInput(void *, u32, u32);

u32 ov14_021EC354(void *);
u32 ov14_021EC3A8(void *);
u32 ov14_021EC710(void *);
u32 ov14_021EC730(void *);
u32 ov14_021EC7D4(void *);
u32 ov14_021EC854(void *);
u32 ov14_021ECDA8(void *);
u32 ov14_021ED1AC(void *);
u32 ov14_021ED1E8(void *);
u32 ov14_021ED258(void *);

#endif
