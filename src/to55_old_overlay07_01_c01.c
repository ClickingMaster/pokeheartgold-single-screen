#include "to55_old_overlay07_private.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

u16 ov07_0221FA1C(void *, int);
void *ov07_0221FA38(void *, int);
void ov07_02232508(void *);
void *ov07_02232A54(void *);

u16 ov07_0221FA1C(void *p, int i) {
    return *(u16 *)((u8 *)PTR_AT(p, 0xc0) + 0xd8 + i * 2);
}
