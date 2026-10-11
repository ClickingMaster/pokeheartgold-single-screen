#ifndef OVERLAY102_SCROLL_UI_R37_PRIVATE_H
#define OVERLAY102_SCROLL_UI_R37_PRIVATE_H

#include "global.h"

void ToggleBgLayer(u32 layer, u32 enabled);
void Sprite_SetDrawFlag(void *sprite, int enabled);
void Sprite_SetMatrix(void *sprite, const VecFx32 *matrix);
void Sprite_SetAnimCtrlSeq(void *sprite, int sequence);
void FillWindowPixelRect(void *window, u32 color, u16 x, u16 y, u16 width, u16 height);
BOOL EasyChatApp_CanScrollWordListUp(u8 *work);
BOOL EasyChatApp_CanScrollWordListDown(u8 *work);
BOOL ov102_021EC3AC(u8 *work);
BOOL ov102_021EC2EC(u8 *work);

BOOL ov102_021EBEC8(u8 *p);
void ov102_021EBEF4(u8 *p, int enabled);
void ov102_021EBF38(u8 *p, u32 index);
BOOL ov102_021EC05C(u8 *p);
void ov102_021EC090(u8 *p, int amount);

#endif
