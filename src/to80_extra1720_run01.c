#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0xe7];
    u8 status;
    u8 q[7];
    u8 mask;
    u8 r[14];
    u16 crc;
} Page;

typedef struct {
    Page page[4];
    u8 pad[0xf8];
    u8 crcTable[512];
} Work;

extern Work *ov13_0224F5A4;
extern void *ov13_0224128C(u32, u32);
extern void ov13_022412B0(void *);
extern void DWCi_BACKUPlWritePage(void *, int *, void *);
extern u16 MATH_CalcCRC16(const void *, const void *, u32);
void ov13_0223A8F0(int);

static inline int One(void) {
    return 1;
}

void ov13_0223A8F0(int index) {
    int changed[4];
    void *temp;
    int status = ov13_0224F5A4->page[index].status;
    int active = 0, i, offset;
    int value = 1 << index;
    MI_CpuFill8(changed, 0, 16);
    changed[index] = 1;
    if (index <= 2) {
        if (ov13_0224F5A4->page[0].mask & value) {
            active = 1;
        }
        if (status == 255 && active) {
            ov13_0224F5A4->page[0].mask &= ~value;
            ov13_0224F5A4->page[1].mask &= ~value;
            changed[0] = changed[1] = 1;
        } else if (status != 255 && !active) {
            ov13_0224F5A4->page[0].mask |= value;
            ov13_0224F5A4->page[1].mask |= value;
            changed[0] = changed[1] = 1;
        }
    }
    i = 0;
    offset = 0;
    for (; i < 4; i++, offset += 256) {
        if (changed[i]) {
            ov13_0224F5A4->page[i].crc = MATH_CalcCRC16(ov13_0224F5A4->crcTable, (u8 *)ov13_0224F5A4 + offset, 254);
        }
    }
    temp = ov13_0224128C(256, 32);
    DWCi_BACKUPlWritePage(ov13_0224F5A4, changed, temp);
    ov13_022412B0(&temp);
}
