#ifndef TO50_OV15_R11_PRIVATE_H
#define TO50_OV15_R11_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void BagApp_HideCursor(void *);
extern u32 Pocket_GetQuantity(void *, u32, u16, u32);
extern void BagApp_PrintTrashConfirmMessage(void *);
extern void BagApp_ShowCancelButton(void *, s32);
extern void BagApp_LoadSubScreenLayout(void *, s32, s32, s32);
extern void BagApp_ShowQuantityButtons(void *, s32, u16);
extern void BagApp_PrintQuantityDigits(void *, s32);
extern void BagApp_PrintTrashLabel(void *);
extern void BagApp_PrintCancelButton(void *, s32);
s32 BagApp_ItemAction_Trash(void *);
#endif
