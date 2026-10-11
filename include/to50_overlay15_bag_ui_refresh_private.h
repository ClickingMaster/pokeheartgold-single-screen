#ifndef TO50_OV15_R13_PRIVATE_H
#define TO50_OV15_R13_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void BagApp_CloseItemMenuWindows(void *);
extern void BagApp_RedrawPocket(void *);
extern void BagApp_HideQuantityButtons(void *);
extern void BagApp_PrintCancelButton(void *, s32);
extern void ManagedSprite_SetAnimationFrame(void *, s32);
extern void ManagedSprite_SetAnim(void *, s32);
s32 BagApp_State_CancelTrash(void *);
#endif
