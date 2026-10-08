#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u64 pattern;
    u16 length;
    u16 speed;
} BlinkPattern;
typedef struct {
    u32 count;
    int pattern;
} BlinkCounter;
extern BlinkCounter PMi_BlinkCounter;
extern int PMi_LEDStatus;
extern BlinkPattern PMi_BlinkPatternData[];
extern int SPIi_SetEntry(u32, u32, u16, ...);
extern void PM_SetLEDPattern(u32);
void PM_SelfBlinkProc(void);
// @fn PM_SelfBlinkProc
void PM_SelfBlinkProc(void)
{
    int pat = PMi_BlinkCounter.pattern;
    BlinkPattern *d;
    u32 status;
    if (pat == 0) {
        if (SPIi_SetEntry(3, 0x66, 1, 1))
            PM_SetLEDPattern(1);
        return;
    }
    if (pat < 4) {
        if (pat != PMi_LEDStatus)
            SPIi_SetEntry(3, 0x66, 1, pat);
        return;
    }
    d = &PMi_BlinkPatternData[pat - 4];
    if (d->pattern & (0x8000000000000000ULL >> (PMi_BlinkCounter.count / d->speed)))
        status = 1;
    else
        status = 2;
    if (++PMi_BlinkCounter.count >= d->length * d->speed)
        PMi_BlinkCounter.count = 0;
    if (status != PMi_LEDStatus)
        SPIi_SetEntry(3, 0x66, 1, status);
}
