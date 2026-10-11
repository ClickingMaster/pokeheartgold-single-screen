#ifndef W13_R5_PRIVATE_H
#define W13_R5_PRIVATE_H
#include "global.h"
void *PCBox_GetSlotMonData(void *, u32, u32, u32);
void PCBox_DrawMarkingMenu(void *);
void PCBox_DrawConfirmCancel(void *);
void PCBox_SlideInMarkingMenu(void *);
void PCBox_UpdateMarkingMenuSlide(void);
void PCBox_WaitForUpdateFunc(void *, void (*)(void), u32);
void PCBox_CreateGridInput(void *, u32, u32);
void PCBox_SetIconsTranslucent(void *, u32, u32);
void PCBox_SetSlotIconTranslucent(void *, u32, u32);
void PCBox_PrintTouchTheMark(void *, u32);
u32 GridInputHandler_HandleInput_AllowHold(void *);
void PlaySE(u32);
void PCBox_ToggleMarking(void *, u32);
void PCBox_SetSlotMonData(void *, u32, u32, u8 *);
void PCBox_DrawMarkings(void *, u32);
u32 ov14_021F23F0(void *, u32, u32);
u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
u32 PCBox_State_MarkingMenuInput(void *);
void PCBox_State_OpenMarkingMenu(void *);
u32 ov14_021EBE2C(void *);
#endif
