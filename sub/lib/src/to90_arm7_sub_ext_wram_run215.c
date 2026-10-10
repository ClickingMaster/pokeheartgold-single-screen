#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35539Station {
    u16 field[14];
} W35539Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35539Station **)(ROOT + 0x31c))[0])
void CAM_SetAuthSeed(u32 index, u16 value);

void CAM_SetAuthSeed(u32 index, u16 value) {
    STATION[index].field[7] = value;
}
