#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int DWCi_BACKUPlInit(void *);
extern int DWCi_BACKUPlRead(void *);
extern int DWCi_BACKUPlWriteAll(void *);
extern int checkAp(void *);
extern void DWCi_BACKUPlSetWiFi(void *);
extern void init(void *);
extern void initPage(void *, int);
extern u32 MATH_CalcCRC16(const MATHCRC16Table *, const void *, u32);

typedef struct {
    u8 data[254];
    u16 crc;
} Page;

int DWC_BM_Init(u8 *);

int DWC_BM_Init(u8 *p) {
    int valid[4];
    int i;
    u8 *crc, *ap;
    int changed;
    MI_CpuFill8(p, 0, 0x700);
    if (!DWCi_BACKUPlInit(p)) {
        return -10001;
    }
    MATHi_CRC16InitTableRev((MATHCRC16Table *)(p + 0x500), 0xa001);
    if (!DWCi_BACKUPlRead(p)) {
        return -10001;
    }
    MI_CpuFill8(valid, 0, 16);
    crc = p;
    ap = p;
    for (i = 0; i < 3; i++, crc += 256, ap += 256) {
        u32 sum = MATH_CalcCRC16((MATHCRC16Table *)(p + 0x500), crc, 254);
        if (sum == ((Page *)p)[i].crc) {
            if (checkAp(ap)) {
                valid[i] = 1;
            }
        }
    }
    {
        u32 sum = MATH_CalcCRC16((MATHCRC16Table *)(p + 0x500), p + 768, 254);
        if (sum == ((Page *)p)[3].crc) {
            valid[3] = 1;
        }
    }
    if (valid[0] && valid[1] && valid[2] && valid[3]) {
        DWCi_BACKUPlSetWiFi(p + 240);
        return 0;
    }
    if (!valid[0] && !valid[1] && !valid[2] && !valid[3]) {
        init(p);
        return DWCi_BACKUPlWriteAll(p) ? 0 : -10000;
    }
    if (!(valid[0] && valid[1]) && !(valid[2] && valid[3])) {
        init(p);
        return DWCi_BACKUPlWriteAll(p) ? 0 : -10000;
    }
    if (!valid[0] && !valid[1]) {
        init(p);
        return DWCi_BACKUPlWriteAll(p) ? -10003 : -10000;
    }
    if (!valid[0]) {
        initPage(p, 0);
        MI_CpuCopy8(p + 496, p + 240, 14);
        p[239] = p[495];
    } else if (!valid[1]) {
        initPage(p, 1);
        MI_CpuCopy8(p + 240, p + 496, 14);
        p[495] = p[239];
    }
    DWCi_BACKUPlSetWiFi(p + 240);
    if (!valid[2]) {
        initPage(p, 2);
    }
    if (!valid[3]) {
        MIi_CpuClear16(0, (u16 *)(p + 768), 256);
    }
    changed = 0;
    for (i = 0; i < 3; i++) {
        if (!valid[i] && (p[239] & (1 << i))) {
            p[239] &= ~(1 << i);
            changed = 1;
            p[495] = p[239];
        }
    }
    if (!DWCi_BACKUPlWriteAll(p)) {
        return -10000;
    }
    return changed ? -10002 : 0;
}
