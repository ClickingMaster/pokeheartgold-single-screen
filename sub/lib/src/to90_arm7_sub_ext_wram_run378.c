#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35556Station {u16 field[14];} W35556Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35556Station **)(ROOT+0x31c))[0])
u16 CAM_GetStaState(u32);u16 CAM_GetAID(u32);void CAM_ClrTIMElementBitmap(u32);u32 __VENEER_OS_DisableIrqMask(u32);u32 __VENEER_OS_EnableIrqMask(u32);void WEnableTmpttPowerSave(void);void WDisableTmpttPowerSave(void);void DeleteTxFrames(u32);u8 WL_ReadByte(u8*);void WL_WriteByte(u8*,u8);
void __VENEER_MIi_CpuClear16(u32,void*,u32);void CAM_InitElement(u32,const u8*);void CAM_SetStaState(u16,u16);extern const u8 BC_ADRS[];
void InitializeCAM(void);
// @fn InitializeCAM
void InitializeCAM(void){W35556Station*s=STATION;u32 count=HALF(0x322),j;__VENEER_MIi_CpuClear16(0,s,count*28);__VENEER_MIi_CpuClear16(0,ROOT+0x52c,16);s[0].field[13]=65535;for(j=1;j<count;j++)s[j].field[13]=65535;CAM_InitElement(0,BC_ADRS);CAM_SetStaState(0,64);}
