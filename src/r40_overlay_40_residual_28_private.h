#ifndef SOL_R40_OVERLAY_40_RESIDUAL_28_PRIVATE_H
#define SOL_R40_OVERLAY_40_RESIDUAL_28_PRIVATE_H

#include "global.h"

void *ov40_0223D540(void *p);
void *GlobalTerminalNet_GetDressUpList(void *p, void *dst, int value);
void GlobalTerminalNet_CopyRequestHeader(const void *src, void *dst);
void *FrontierCard_New(int heapId);
void FrontierCard_Delete(void *p);

void ov40_0223615C(void *p, const u32 *state);
void ov40_02236184(void *p, int index);
void ov40_022361B0(void *p);

#endif
