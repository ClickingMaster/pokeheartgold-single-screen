#include "overlay07_callbacks_w01_private.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

int ov07_0221E868(void *p) { U16_AT(p, 0x44) |= 2; return 0; }
