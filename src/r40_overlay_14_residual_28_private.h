#ifndef OVERLAY_14_RESIDUAL_28_R40_PRIVATE_H
#define OVERLAY_14_RESIDUAL_28_R40_PRIVATE_H
#include "global.h"
typedef struct Ov14R28Data {
    u8 pad00[0x2c];
    u32 unk2c;
    u8 pad30[4];
    void *unk34;
} Ov14R28Data;
void PCBox_DrawBoxMenu(Ov14R28Data *data);
void PCBox_SlideInBoxMenu(void *obj);
void PCBox_WaitForUpdateFunc(Ov14R28Data *data, void (*func)(void), u32 value);
void PCBox_UpdateBoxMenuSlide(void);
void PCBox_CreateGridInput(Ov14R28Data *data, u32 value, u32 state);
void PCBox_SetSpriteAnim(void *obj, u32 value, u32 param);
void PCBox_PrintMenuPrompt(Ov14R28Data *data, u32 value, u32 param);
void PCBox_SetSpriteVisible(void *obj, u32 value, u32 param);
void PCBox_CloseMessageWindow(void *obj, u32 value);
void ov14_021F10DC(Ov14R28Data *data, u32 value);
void PCBox_SlideOutBoxMenu(void *obj);
void ov14_021ED62C(Ov14R28Data *data);
u32 ov14_021ED650(Ov14R28Data *data);
void ov14_021ED684(Ov14R28Data *data);
void ov14_021ED6A4(Ov14R28Data *data);
#endif
