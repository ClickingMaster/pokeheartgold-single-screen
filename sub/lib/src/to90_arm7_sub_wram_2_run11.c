#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern void PM_SetLEDPattern(int),PMi_SetLED(int),PMi_SetControl(u32),PMi_ResetControl(u32),SND_BeginSleep(void);
void PMi_SwitchUtilityProc(u32 command);
// @fn PMi_SwitchUtilityProc
void PMi_SwitchUtilityProc(u32 command){switch(command){case 1:PM_SetLEDPattern(1);PMi_SetLED(1);break;case 2:PM_SetLEDPattern(3);PMi_SetLED(3);break;case 3:PM_SetLEDPattern(2);PMi_SetLED(2);break;case 4:PMi_SetControl(4);break;case 5:PMi_ResetControl(4);break;case 6:PMi_SetControl(8);break;case 7:PMi_ResetControl(8);break;case 8:PMi_SetControl(12);break;case 9:PMi_ResetControl(12);break;case 10:PMi_SetControl(1);break;case 11:PMi_ResetControl(1);break;case 12:PMi_ResetControl(2);break;case 13:PMi_SetControl(2);break;case 15:PMi_ResetControl(64);break;case 14:SND_BeginSleep();PMi_SetControl(64);break;}}
