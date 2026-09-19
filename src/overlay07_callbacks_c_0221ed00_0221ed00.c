#include "overlay07_callbacks_w01_private.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

int ov07_0221ED00(void *p) { ov07_0221DD14((u8 *)PTR_AT(p, 0x48) + 0x180, 5); return 0; }
