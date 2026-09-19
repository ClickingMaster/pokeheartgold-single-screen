#ifndef TO52_OVERLAY41_R26_H
#define TO52_OVERLAY41_R26_H

#include "global.h"

void MI_CpuFill8(void *, u8, u32);
void YesNoPrompt_Reset(void *);
void YesNoPrompt_InitFromTemplate(void *, void *);
void ov41_02247D34(void *, int);
int YesNoPrompt_HandleInput(void *);
int YesNoPrompt_IsInTouchMode(void *);
void *ov41_022499F0(void *, void *, int);
void ov41_02249A50(void *, void *);

void ov41_02247D64(u8 *);
int ov41_02247DF8(u8 *);
void ov41_02247E34(u32, u32, u32, u32, u32, u32 *, u32 *);
void ov41_02247F3C(u8 *, const u32 *);

#endif
