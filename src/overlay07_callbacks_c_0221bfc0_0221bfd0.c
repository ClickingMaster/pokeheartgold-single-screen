#include "overlay07_callbacks_w01_private.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void *ov07_0221BFC0(void *p) { if (p == NULL) GF_AssertFail(); return PTR_AT(p, 8); }
void *ov07_0221BFD0(void *p) { if (p == NULL) GF_AssertFail(); return PTR_AT(p, 0); }
