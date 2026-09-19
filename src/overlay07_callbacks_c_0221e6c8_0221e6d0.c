#include "overlay07_callbacks_w01_private.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void *ov07_0221E6C8(void *p) { return PTR_AT(p, 0x1a8); }
int ov07_0221E6D0(void) { return 0; }
