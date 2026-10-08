#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u32 x : 12;
    u32 y : 12;
    u32 touch : 1;
    u32 validity : 2;
    u32 dummy : 5;
} TPData;
extern int SPIi_SetEntry(u32, u32, u16, ...);
extern void SPIi_ReturnResult(u16, u16);
void TpVAlarmHandler(void *arg);
// @fn TpVAlarmHandler
void TpVAlarmHandler(void *arg)
{
    TPData data;
    if (SPIi_SetEntry(0, 0x10, 1, arg) == 0) {
        data.validity = 3;
        *(u16 *)0x027fffaa = ((u16 *)&data)[0];
        *(u16 *)0x027fffac = ((u16 *)&data)[1];
        SPIi_ReturnResult(0x10, (u32)arg & 0xff);
    }
}
