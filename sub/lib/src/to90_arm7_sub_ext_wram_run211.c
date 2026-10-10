#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35535Station {
    u16 field[14];
} W35535Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35535Station **)(ROOT + 0x31c))[0])
void CAM_SetAwake(u32 index);

void CAM_SetAwake(u32 index) {
    HALF(0x530) |= 1U << index;
}
