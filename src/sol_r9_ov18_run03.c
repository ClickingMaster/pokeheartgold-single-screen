#include "sol_r9_ov18_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))

void PokedexApp_DrawCountNumberAndCopy(void *work, u32 index, u32 value) {
    PokedexApp_DrawCountNumber(work, index, value);
    ScheduleWindowCopyToVram((u8 *)work + 0xc + index * 0x10);
}

void PokedexApp_DrawSeenAndCaughtCounts(void *work) {
    PokedexApp_DrawCountNumberAndCopy(work, U8(work, 0x185d) + 2, U16(work, 0x102c));
    PokedexApp_DrawCountNumberAndCopy(work, U8(work, 0x185d) + 4, U16(work, 0x102e));
    U8(work, 0x185d) ^= 1;
}
