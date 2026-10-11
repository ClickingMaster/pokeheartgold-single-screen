#include "overlay41_accessors_private.h"











typedef struct { void *obj; s32 state; void *arg; void (*cb)(void *, void *); } Ov41Cb;

void DressUp_UpdateBackdropAnim(void *p) { DressUpBgScreen_UpdateAltAnim((u8 *)p+4,1); }

void DressUp_StopBackdropAnim(void *p) { DressUpBgScreen_StopAltAnim((u8 *)p+4); memset(p,0,0x1C); }
