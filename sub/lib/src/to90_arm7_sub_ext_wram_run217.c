#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35541Station {u16 field[14];} W35541Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35541Station **)(ROOT+0x31c))[0])
u16 CAM_GetStaState(u32);u16 CAM_GetAID(u32);void CAM_ClrTIMElementBitmap(u32);u32 __VENEER_OS_DisableIrqMask(u32);u32 __VENEER_OS_EnableIrqMask(u32);void WEnableTmpttPowerSave(void);void WDisableTmpttPowerSave(void);void DeleteTxFrames(u32);u8 WL_ReadByte(u8*);void WL_WriteByte(u8*,u8);
u32 CAM_AllocateAID(u32 index);
// @fn CAM_AllocateAID
u32 CAM_AllocateAID(u32 index){volatile u16*c=(volatile u16*)(ROOT+0x52c);u32 aid,old=__VENEER_OS_DisableIrqMask(0x1000000),bit,bitmap;for(aid=1,bit=2;aid<16;aid++,bit<<=1){bitmap=c[7];if(!(bitmap&bit)){c[7]=bitmap|bit;c[6]++;if(c[6]==1)WEnableTmpttPowerSave();STATION[index].field[1]=aid;__VENEER_OS_EnableIrqMask(old);return aid;}}__VENEER_OS_EnableIrqMask(old);return 0;}
