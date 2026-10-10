#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W38865State {
    u8 pad0[8];
    u16 state;
    u8 pad1[20];
    u16 length;
    u8 ssid[32], pad2[82];
    u16 offset;
} W38865State;

typedef struct W38865Work {
    u8 pad0[0x32e];
    u16 mode;
    u8 pad1[20];
    W38865State state;
    u8 pad2[0xd4];
    u8 *tx;
} W38865Work;

#define W38865_ROOT (*(W38865Work *volatile *)0x0380fff4)

u32 WSetSsid(u32, const u8 *);

extern u8 WL_ReadByte(const void *ptr);
extern void WL_WriteByte(void *ptr, u32 value);

u32 WSetSsid(u32 count, const u8 *src) {
    W38865Work *w = W38865_ROOT;
    u8 *s = (u8 *)&w->state;
    u32 i;
    int changed = 0;
    if (count > 32) {
        return 5;
    }
    if (((W38865State *)s)->state == 0x40 && w->mode == 1) {
        if (((W38865State *)s)->length != count) {
            return 6;
        }
        if (((W38865State *)s)->offset) {
            changed = 1;
        }
    }
    for (i = 0; i < count; i++) {
        u32 byte = WL_ReadByte(src);
        WL_WriteByte(s + 32 + i, byte);
        src++;
    }
    for (; i < 32; i++) {
        WL_WriteByte(s + 32 + i, 0);
    }
    ((W38865State *)s)->length = count;
    if (changed) {
        u8 *dest = W38865_ROOT->tx + 0x26 + ((W38865State *)s)->offset;
        s += 32;
        for (i = 0; i < count; i++) {
            WL_WriteByte(dest + i, WL_ReadByte(s + i));
        }
    }
    return 0;
}
