#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35555Station {
    u16 field[14];
} W35555Station;

#define ROOT      (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT + (off)))
#define STATION   (((W35555Station **)(ROOT + 0x31c))[0])
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
void CAM_Delete(u32 index);

void CAM_Delete(u32 index) {
    DeleteTxFrames(index);
    STATION[index].field[0] = 0;
    HALF(0x52c)
    --;
}
