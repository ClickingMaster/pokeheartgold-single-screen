#ifndef POKEHEARTGOLD_OVERLAY14_BOX_TRANSITION_HANDLERS_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_TRANSITION_HANDLERS_PRIVATE_H
#include "global.h"
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void PCBox_SetSlotIconTranslucent(void *, u32, u32);
extern u32 PCBox_PartyHasOtherUsableMon(void *, u32);
extern void PlaySE(u32);
extern void PCBox_PrintReleaseMessage(void *, u32, u32);
extern void PCBox_PrintMonActionMessage(void *, u32, u32, u32);
extern void *Party_GetMonByIndex(void *, u32);
extern u32 GetMonData(void *, u32, void *);
extern u32 ItemIdIsMail(u32);
extern void PCBox_SetBoxArrowsVisible(void *, u32);
extern void *PCBox_GetSlotBoxMon(void *, u32, u32);
extern u32 GetBoxMonData(void *, u32, void *);
extern u32 PCBox_ShowYesNo(void *, u32);
extern void PCBox_RunReleaseMoveCheck(void *);
extern u32 PCBox_UpdateReleaseShrink(void *);
extern void PCBox_EndReleaseShrink(void *);
extern void PCBox_FreeReleaseMoveCheck(void *);
extern void PCBox_DeleteSlotMon(void *, u32, u32);
extern void PCBox_CloseMessageWindow(void *, u32);
extern void PCBox_ClearMonInfo(void *);
extern void PCBox_DrawBoxThumbnail(void *, u32);
extern void PCBox_RefreshTrayBoxThumbnail(void *, u32);
extern void PCBox_SlideInPartyButton(void *);
extern void ov14_021E82A8(void *);
extern void ov14_021E8328(void *);
extern void GridInputHandler_SetNextInput(void *, u32);
extern void GridInputHandler_SetButtonInputMode(void *, u32);
extern void PCBox_AllocMoveWork(void *);
extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern void PCBox_FreeMoveWork(void *);
extern void PCBox_DrawTrayBoxNameAndCount(void *);
extern void PCBox_ShowSlotMonInfo(void *, u32);
extern u32 PCBox_UpdateReleaseGrow(void *);
extern void PCBox_ResetSpriteAffine(void *);
extern void PCBox_CreateSelectedIconOutline(void *);
extern void ov14_021E8824(void *);
extern void PCBox_SetSelectedIconOutlineVisible(void *, u32);
extern void ov14_021E9450(void);
extern void ov14_021E9194(void);
u32 PCBox_State_TryRelease(void *);
u32 PCBox_State_ReleaseMoveCheck(void *);
u32 PCBox_State_PrintReleased(void *);
u32 PCBox_State_PrintByeBye(void *);
u32 PCBox_State_RemoveReleasedMon(void *);
u32 ov14_021EC23C(void *);
u32 PCBox_State_ReleaseCameBack(void *);
u32 PCBox_State_PrintWorried(void *);
u32 ov14_021EC300(void *);
#endif
