#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35540Station {
    u16 field[14];
} W35540Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35540Station **)(ROOT + 0x31c))[0])
void CAM_UpdateLifeTime(u32 index);

void CAM_UpdateLifeTime(u32 index) {
    W35540Station *s = &STATION[index];
    s->field[12] = s->field[13];
}
