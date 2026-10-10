#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35542Station {
    u16 field[14];
} W35542Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35542Station **)(ROOT + 0x31c))[0])
u16 CAM_GetStaState(u32);
u16 CAM_GetAID(u32);
void CAM_ClrTIMElementBitmap(u32);
u32 __VENEER_OS_DisableIrqMask(u32);
u32 __VENEER_OS_EnableIrqMask(u32);
void WEnableTmpttPowerSave(void);
void WDisableTmpttPowerSave(void);
void DeleteTxFrames(u32);
u8 WL_ReadByte(u8 *);
void WL_WriteByte(u8 *, u8);
void CAM_ReleaseAID(u32 index);

void CAM_ReleaseAID(u32 index) {
    u8 *w = ROOT;
    u32 aid;
    CAM_ClrTIMElementBitmap(index);
    aid = CAM_GetAID(index);
    if (aid) {
        STATION[index].field[1] = 0;
        *(volatile u16 *)(w + 0x53a) &= ~(1U << aid);
        (*(volatile u16 *)(w + 0x538))--;
        if (*(volatile u16 *)(w + 0x538) == 0) {
            WDisableTmpttPowerSave();
        }
    }
}
