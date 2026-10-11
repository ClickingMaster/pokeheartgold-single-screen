#ifndef TO50_OV15_R19_PRIVATE_H
#define TO50_OV15_R19_PRIVATE_H
#include "global.h"
void BagApp_SetCancelSpriteVisible(void *, u32);
void BagApp_PrintCancelButton(void *, u32);
void ScheduleWindowCopyToVram(void *);
void ManagedSprite_SetAnimationFrame(void *, u32);
void ManagedSprite_SetAnim(void *, u32);
void BagApp_ClearCancelButtonText(void *);
void BagApp_ShowCancelButton(void *, u32);
int BagApp_StartButtonFlash(void *, u8, u8, u8, void *);
int BagApp_StartButtonAnim(void *, u8, u32, void *);
BOOL ManagedSprite_IsAnimated(void *);
void ManagedSprite_SetPaletteOverride(void *, u32);
int BagApp_State_ButtonFeedback(void *);
#endif
