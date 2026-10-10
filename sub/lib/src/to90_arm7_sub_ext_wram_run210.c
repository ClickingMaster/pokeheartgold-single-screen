#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35534Station {
    u16 field[14];
} W35534Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35534Station **)(ROOT + 0x31c))[0])
u16 CAM_GetStaState(u32);
void CAM_SetDoze(u32 index);

void CAM_SetDoze(u32 index) {
    if (CAM_GetStaState(index) == 64) {
        HALF(0x530) &= ~(1U << index);
    }
}
