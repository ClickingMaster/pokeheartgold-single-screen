#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222F408(void *data, void *arg) {
    u32 mask = 1 << ov07_02221FF0(arg);
    PaletteData_BlendPalettes(PTR(data, 0xC4), 2, (u16)mask, 8, 0);
}
