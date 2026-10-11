#include "overlay41_accessors_private.h"











typedef struct { void *obj; s32 state; void *arg; void (*cb)(void *, void *); } Ov41Cb;

void ov41_0224B848(void *p, void *q) { DressUpCanvas_SetBackdrop((u8 *)p+0x190, *(void **)((u8 *)q+0x5C), *(void **)((u8 *)q+0x68)); }

void ov41_0224B85C(void *p) { DressUpCanvas_ClearObjects((u8 *)p+0x190); DressUpCanvas_Destroy((u8 *)p+0x190); }
