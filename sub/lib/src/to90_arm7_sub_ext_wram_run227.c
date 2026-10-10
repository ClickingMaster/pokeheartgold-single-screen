#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35551Station {
    u16 field[14];
} W35551Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35551Station **)(ROOT + 0x31c))[0])
u16 CAM_GetFrameCount(u32 index);

u16 CAM_GetFrameCount(u32 index) {
    return STATION[index].field[11];
}
