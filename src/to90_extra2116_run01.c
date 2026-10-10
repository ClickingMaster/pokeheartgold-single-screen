#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 flag, pad[3];
    s16 *buffer;
} Global;

typedef struct {
    u8 tag, pad;
    u16 length;
    u8 data[256];
} Message;

extern Global ov13_0224F4C4;
extern const u8 ov13_02245320[];
extern void *ov13_0224128C(int, int);
extern void ov13_0222B414(void);
extern void ov13_0222B424(void);
extern int ov13_02222C1C(void (*)(void), void (*)(void));
void ov13_0222B254(void);

void ov13_0222B254(void) {
    Message msg;
    u32 n;
    u16 *dst;
    const u16 *src;
    u16 a, b;
    u16 *base;
    ov13_0224F4C4.buffer = ov13_0224128C(620, 4);
    ov13_0224F4C4.flag = 0;
    MIi_CpuClear16(0, (u16 *)&msg, 260);
    msg.tag = 80;
    msg.length = 12;
    MI_CpuCopy8(ov13_02245320, msg.data, 12);
    ov13_0224F4C4.buffer[0] = 3;
    base = (u16 *)ov13_0224F4C4.buffer;
    dst = base + 1;
    src = (const u16 *)&msg;
    n = 65;
    do {
        a = src[0];
        b = src[1];
        src += 2;
        dst[0] = a;
        dst[1] = b;
        dst += 2;
    } while (--n);
    base[131] = 1;
    ov13_0224F4C4.buffer[132] = -1;
    ov13_0224F4C4.buffer[133] = 1;
    ov13_0224F4C4.buffer[134] = -1;
    ov13_0224F4C4.buffer[135] = -1;
    OS_GetMacAddress((u8 *)ov13_0224F4C4.buffer + 0x110);
    if (ov13_02222C1C(ov13_0222B414, ov13_0222B424)) {
        OS_Terminate();
    }
}
