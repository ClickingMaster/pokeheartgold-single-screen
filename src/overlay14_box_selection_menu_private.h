#ifndef POKEHEARTGOLD_OVERLAY14_BOX_SELECTION_MENU_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_SELECTION_MENU_PRIVATE_H

#include "global.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern void ov14_021F4BC0(void *);
extern void PCBox_UpdateCurrentBoxMarker(void *);
extern void PCBox_UpdateTrayCursor(void *);
extern void PCBox_DrawTrayBoxNameAndCount(void *);
extern u32 ov14_021F10B4(void *, u32);
extern void PCBox_DrawDepositTitle(void *);
extern void ov14_021E8560(void *);
extern void ov14_021E8588(void *);
extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern void PCBox_CreateGridInput(void *, u32, u32);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_CloseMessageWindow(void *, u32);
extern void PCBox_PrintMonActionMessage(void *, u32, u32, u32);
extern int GridInputHandler_HandleInput_AllowHold(void *);
extern u32 ov14_021F1448(void *, u32);
extern void PlaySE(u32);
extern u32 ov14_021F1504(void *, s32);
extern u32 ov14_021F1540(void *);
extern u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
extern u32 ov14_021F1534(void *, u32);

extern void ov14_021E95C8(void);
extern void ov14_021E9604(void);

u32 ov14_021EE338(void *);
u32 ov14_021EE35C(void *);
u32 ov14_021EE380(void *);
u32 ov14_021EE3C8(void *);
u32 ov14_021EE4AC(void *);

#endif
