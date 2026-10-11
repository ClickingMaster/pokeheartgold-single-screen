#ifndef TO46_OVERLAY_14_RESIDUAL_32_PRIVATE_H
#define TO46_OVERLAY_14_RESIDUAL_32_PRIVATE_H

#include "global.h"

void PlaySE(u32);
void GridInputHandler_SetNextInput(void *, u32);
void *GridInputHandler_GetDpadBox(void *, u32);
void DpadMenuBox_GetPosition(void *, u8 *, u8 *);
void ManagedSprite_SetPositionXY(void *, s32, s32);
void GridInputHandler_SetButtonInputMode(void *, u32);

void ov14_021F3044(void *);
void ov14_021E7ED0(void *);
void PCBox_SlideInPartyPanel(void *);
u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
u32 ov14_021E9518(void *);
void PCBox_AllocMoveWork(void *);
void PCBox_AnimateMoveToPartyEnd(void *);
void PCBox_ApplyMonMove(void *);
void PCBox_FreeMoveWork(void *);
void PCBox_ClearMonInfo(void *);
void PCBox_SlideOutPartyPanel(void *);
void ov14_021E8328(void *);
void ov14_021E95B4(void *);
void ov14_021F6678(void *, u32);
void PCBox_PrintMonActionMessage(void *, u32, u32, u32);

void ov14_021EDE38(void *);
void ov14_021EDE70(void *);
void ov14_021EDE88(void *);
void ov14_021EDF08(void *);
u32 ov14_021EDF28(void *);

#endif
