#include "global.h"
extern void ov91_0225EED0(void *, u32, void *, u16, u16);
void ov91_0225D380(u8 *, u16);
void ov91_0225D380(u8 *p, u16 value) {
    u16 old = *(u16 *)(p + 0x8780);
    *(u16 *)(p + 0x8782) = old;
    *(u16 *)(p + 0x8780) = value;
    ov91_0225EED0(p + 0x8730, *(u32 *)(p + 0x877c), p + 0x8784,
                    *(u16 *)(p + 0x8782), *(u16 *)(p + 0x8780));
}
