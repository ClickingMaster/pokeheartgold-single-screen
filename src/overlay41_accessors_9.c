#include "overlay41_accessors_private.h"











typedef struct { void *obj; s32 state; void *arg; void (*cb)(void *, void *); } Ov41Cb;

void DressUpButton_MoveTextBy(void *p) { DressUpButton_MoveBy(p); TextOBJ_UpdatePosition(*(void **)((u8 *)p+0x10)); }
