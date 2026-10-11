#include "overlay41_accessors_private.h"











typedef struct { void *obj; s32 state; void *arg; void (*cb)(void *, void *); } Ov41Cb;

void DressUpButton_PlaySE(void *unused, u32 se, s32 a, s32 b) { if(b==a) PlaySE((u16)se); }
