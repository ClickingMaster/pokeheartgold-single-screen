#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35533Station {
    u16 field[14];
} W35533Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35533Station **)(ROOT + 0x31c))[0])
void CAM_SetPowerMgtMode(u32 index, u16 mode);

void CAM_SetPowerMgtMode(u32 index, u16 mode) {
    u8 *w = ROOT;
    volatile u16 *p = (volatile u16 *)(w + 0x52c);
    u32 awake;
    p[1] = (p[1] & ~(1U << index)) | (mode << index);
    awake = p[3];
    if (p[1] & ~awake) {
        *(volatile u16 *)0x048080AC = 8;
    } else {
        *(volatile u16 *)0x048080AE = 8;
    }
}
