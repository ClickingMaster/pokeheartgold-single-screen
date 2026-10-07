#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 ctrdgi_flash_lock_id[];
extern void StartFlashTimer(int),CheckFlashTimer(void);
extern u8 CTRDGi_ReadSrFunc(const volatile u8*);
#define IRQ (*(vu16*)0x04000208)
#define STATUS (*(vu16*)(ctrdgi_flash_lock_id+2))
#define SAVED (*(u32*)(ctrdgi_flash_lock_id+0x1c))
static inline void UnlockFlash(u8 command){u16 previous=IRQ;IRQ=0;SAVED=previous;*(vu8*)0x0A005555=0xAA;*(vu8*)0x0A002AAA=0x55;*(vu8*)0x0A005555=command;}
static inline void RestoreIRQ(void){u16 previous=IRQ;(void)previous;IRQ=SAVED;}
u16 CTRDGi_ReadFlashID(void);
u16 CTRDGi_ReadFlashID(void){u16 id;UnlockFlash(0x90);RestoreIRQ();StartFlashTimer(0);while(!STATUS)CheckFlashTimer();id=(u16)((u32)CTRDGi_ReadSrFunc((vu8*)0x0A000001)<<8);id|=CTRDGi_ReadSrFunc((vu8*)0x0A000000);UnlockFlash(0xF0);*(vu8*)0x0A005555=0xF0;RestoreIRQ();StartFlashTimer(0);while(!STATUS)CheckFlashTimer();return id;}
