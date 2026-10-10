#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define G       (*(u8 *volatile *)0x0380fff4)
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define P(p, n) (*(u8 **)((u8 *)(p) + (n)))
#define V(n)    (*(volatile u16 *)(n))
u32 FLASH_VerifyCheckSum(u32 *);
void FLASH_DirectRead(u32, u32, void *);
u32 BBP_Read(u32);
int BBP_Write(u32, u32);
void WWaitus(u32);
void WSetChannel(u32, u32);
void WStart(void);
void WStop(void);
void __VENEER_MIi_CpuClear16(u32, void *, u32);
void ClearPeriodicTimeOut(void);
int DEV_TestSignalReqCmd(u16 *, u16 *);

int DEV_TestSignalReqCmd(u16 *a, u16 *o) {
    u8 *g = G, *s = g + 0x344;
    u32 raw, flash;
    o[1] = 1;
    if ((H(s, 8) & 0xf0) != 16) {
        return 1;
    }
    if (a[8] > 1) {
        return 5;
    }
    if (a[10] != 10 && a[10] != 20) {
        return 5;
    }
    if (a[9] > 4) {
        return 5;
    }
    if (FLASH_VerifyCheckSum(0)) {
        return 14;
    }
    switch (a[8]) {
    case 0:
        goto stop;
    case 1:
        break;
    default:
        goto done;
    }
    if (H(s, 8) != 16) {
        return 1;
    }
    raw = 0;
    FLASH_DirectRead(0x65, 1, &raw);
    flash = raw;
    if (flash != BBP_Read(1)) {
        BBP_Write(1, flash);
        WWaitus(5000);
    }
    switch (a[9]) {
    case 0:
    case 1:
    case 2:
        H(s, 8) = 17;
        WSetChannel(a[11], 1);
        V(0x04808040) = 0x8000;
        WWaitus(1500);
        H(s, 0x16) = a[10];
        raw = BBP_Read(2);
        if (a[9] <= 1) {
            raw |= 16;
        }
        V(0x048081a4) = a[10];
        if (a[9] == 1) {
            raw |= 32;
            V(0x048081a2) = 3;
        } else {
            V(0x048081a2) = a[9];
        }
        BBP_Write(2, raw);
        V(0x048081a0) = 0x823;
        break;
    case 3:
    case 4: {
        volatile u16 *buf = (volatile u16 *)0x04804000, *dest;
        u32 i;
        s = G + 0x344;
        WStart();
        WStop();
        H(s, 0xac) = BBP_Read(6);
        if (a[9] == 4) {
            BBP_Write(6, 0);
        }
        __VENEER_MIi_CpuClear16(0, (void *)buf, 12);
        buf[4] = 20;
        buf[5] = 2000;
        dest = buf + 6;
        for (i = 0; i < 2028; i += 2) {
            *dest++ = 0x5555;
        }
        buf[6] = 8;
        V(0x04808194) = 6;
        H(s, 8) = 18;
        H(s, 0x18) = 1;
        WSetChannel(a[11], 1);
        V(0x04808040) = 0x8000;
        WWaitus(1500);
        V(0x04808012) = 2;
        V(0x04808004) = 1;
        V(0x048080ae) = 1;
        V(0x048080a0) = (u16)(((u32)buf & 0x3fff) >> 1) | 0x8000;
        break;
    }
    }
    goto done;
stop:
    if (H(s, 8) == 17) {
        ClearPeriodicTimeOut();
        V(0x048081a0) = 0;
        V(0x048081a2) = 1;
        V(0x04808040) = 0;
        raw = BBP_Read(2) & ~0x30u;
        BBP_Write(2, raw);
    } else if (H(s, 8) == 18) {
        H(s, 0x18) = 0;
        while (V(0x04808004)) {}
        BBP_Write(6, H(s, 0xac));
    } else {
        return 1;
    }
    H(s, 8) = 16;
done:
    return 0;
}
