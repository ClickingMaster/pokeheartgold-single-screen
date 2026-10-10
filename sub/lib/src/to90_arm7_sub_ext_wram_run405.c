#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT    (*(u8 **)(0x0380fff4))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define IO(a)   (*(volatile u16 *)(a))

extern void FLASH_Read(u32, u32, void *);
extern int WaitLoop_BbpAccess(void);
extern int WaitLoop_RfAccess(void);
extern void __VENEER_OS_CancelAlarm(void *);
extern void OS_CancelAlarm(void *);
extern int __VENEER_OS_IsAlarmAvailable(void);
extern void __VENEER_OS_CreateAlarm(void *);
extern u64 __VENEER_OS_GetTick(void);
typedef void (*Callback)(void *);
extern void __VENEER_OS_SetAlarm(void *, u64, Callback, void *);
extern void __VENEER_OS_SetPeriodicAlarm(void *, u64, u64, Callback, void *);
extern u64 __VENEER__ll_udiv(u64, u64);
extern void AddTask(u32, u32);
int WCheckSSID(u32 size, const u8 *src);

extern u8 WL_ReadByte(const void *ptr);

int WCheckSSID(u32 size, const u8 *src) {
    u8 *p = ROOT;
    u8 *q = p + 0x344;
    u32 n;
    u8 *ssid;
    u8 *mask;
    u32 i;
    if (size > 32) {
        return 0;
    }
    n = H(q, 0x1e);
    if (!n) {
        return 1;
    }
    if (H(p, 0x404) == 0x13) {
        if (size < n) {
            return 0;
        }
        size = n;
    } else if (size != n) {
        return 0;
    }
    ssid = q + 0x20;
    mask = q + 0x40;
    for (i = 0; i < size; i++) {
        u32 m = WL_ReadByte(mask);
        mask++;
        u32 s = WL_ReadByte(src);
        src++;
        u32 e = WL_ReadByte(ssid);
        ssid++;
        if ((s | m) != (e | m)) {
            return 0;
        }
    }
    return 1;
}
