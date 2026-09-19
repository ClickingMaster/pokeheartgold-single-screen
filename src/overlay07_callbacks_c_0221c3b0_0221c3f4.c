#include "overlay07_callbacks_w01_private.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

BOOL ov07_0221C3B0(void *p) { return U32_AT(p, 0x10) == 1; }
BOOL ov07_0221C3C0(void *p) {
    void *q = PTR_AT(p, 0x14);
    if (q == NULL) {
        if (q == NULL) GF_AssertFail();
        return FALSE;
    }
    Heap_Free(q);
    return TRUE;
}
BOOL ov07_0221C3DC(void *p) { if (p == NULL) return FALSE; return U32_AT(p, 0xc) == 1; }
void ov07_0221C3F4(void *owner, void *func, void *arg, u32 value) { ov07_0221BE68(1, owner, func, arg, value); }
