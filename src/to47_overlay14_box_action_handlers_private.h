#ifndef TO47_OVERLAY14_BOX_ACTION_HANDLERS_PRIVATE_H
#define TO47_OVERLAY14_BOX_ACTION_HANDLERS_PRIVATE_H

#include "global.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

extern void PCBox_ClearMonInfoAndSelection(void *);
extern void ov14_021E8328(void *);
extern void PCBox_SlideOutActionMenu(void *);
extern void ov14_021E884C(void *);
extern u32 ov14_021EA130(void *);
extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern u32 ov14_021F0AD8(void *);
extern u32 ov14_021F0C88(void *);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void PCBox_SetSelectedIconOutlineVisible(void *, u32);
extern void PCBox_DrawReturnButton(void *, u32);

void ov14_021F2818(void *);
void ov14_021F2858(void *);
void ov14_021F2874(void *);

#endif
