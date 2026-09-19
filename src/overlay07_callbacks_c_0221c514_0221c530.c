#include "overlay07_callbacks_w01_private.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void *ov07_0221C514(void *p) { if (p == NULL) GF_AssertFail(); return PTR_AT(p, 0x138); }
void *ov07_0221C528(void *p) { return PTR_AT(p, 0xcc); }
void *ov07_0221C530(void *p) { return PTR_AT(PTR_AT(p, 0xc0), 0xac); }
