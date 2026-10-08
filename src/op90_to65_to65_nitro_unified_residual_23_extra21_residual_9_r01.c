#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 pad[0xc]; void *preSleep; u8 pad2[8]; void *postSleep; } PMWork;
extern PMWork _021E370C;
void PMi_ExecuteList(void *list);
u32 PMi_SetLCDPower(int a, int b, int c, int d);
u32 PMi_SetLED(int a);
int PM_GetLCDPower(void);
u32 PMi_SendSleepStart(u16 trigger, u16 logic);
void OS_Halt(void);
BOOL CTRDG_IsExisting(void);
static inline void WaitVBlank(volatile u32 *count)
{
    while (*count == OS_GetVBlankCount()) { }
    *count = OS_GetVBlankCount();
}
// @fn PM_GoSleepMode
void PM_GoSleepMode(PMWakeUpTrigger trigger, PMLogic logic, u16 keyPattern)
{
    BOOL powerOff;
    u32 dispMain;
    u32 dispSub;
    u16 param1;
    u16 ime;
    OSIntrMode enabled;
    OSIrqMask mask;
    int lcd;
    PMBackLightSwitch top, bottom;
    volatile u32 count;

    powerOff = FALSE;
    PMi_ExecuteList(_021E370C.preSleep);
    ime = reg_OS_IME;
    reg_OS_IME = 0;
    enabled = OS_DisableInterrupts();
    mask = OS_DisableIrqMask(0x3fffff);
    OS_SetIrqMask((OS_IsTickAvailable() ? 8 : 0) | 0x40000);
    OS_RestoreInterrupts(enabled);
    (void)reg_OS_IME;
    reg_OS_IME = 1;
    if ((trigger & 8) && *(vu16 *)0x27ffc40 == 2) {
        trigger &= ~8;
    }
    if ((trigger & 16) && !CTRDG_IsExisting()) {
        trigger &= ~16;
    }
    dispMain = reg_GX_DISPCNT;
    dispSub = reg_GXS_DB_DISPCNT;
    lcd = PM_GetLCDPower();
    PM_GetBackLight(&top, &bottom);
    PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF);
    count = OS_GetVBlankCount();
    WaitVBlank(&count);
    reg_GX_DISPCNT &= ~0x30000;
    reg_GXS_DB_DISPCNT &= ~0x10000;
    WaitVBlank(&count);
    while (count == OS_GetVBlankCount()) { }
    param1 = (u16)(trigger | (top << 5) | (bottom << 6));
    while (PMi_SendSleepStart(param1, (u16)(logic | keyPattern))) { }
    OS_Halt();
    if ((trigger & 8) && (reg_OS_IF & 0x100000)) {
        powerOff = TRUE;
    }
    if (!powerOff) {
        if (lcd == 1) {
            PMi_SetLCDPower(1, 1, 1, 1);
        } else {
            PMi_SetLED(1);
        }
        reg_GX_DISPCNT = dispMain;
        reg_GXS_DB_DISPCNT = dispSub;
    }
    OS_SpinWait(0x708100);
    OS_DisableInterrupts();
    OS_SetIrqMask(mask);
    OS_RestoreInterrupts(enabled);
    (void)reg_OS_IME;
    reg_OS_IME = ime;
    if (powerOff) {
        PM_ForceToPowerOff();
    }
    PMi_ExecuteList(_021E370C.postSleep);
}
