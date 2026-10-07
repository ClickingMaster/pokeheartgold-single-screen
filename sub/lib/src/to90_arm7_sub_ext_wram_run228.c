#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35552Station {u16 field[14];} W35552Station;
#define ROOT (*(u8 **)0x0380FFF4)
#define HALF(off) (*(volatile u16 *)(ROOT+(off)))
#define STATION (((W35552Station **)(ROOT+0x31c))[0])
u16 CAM_GetStaState(u32);u16 CAM_GetAID(u32);void CAM_ClrTIMElementBitmap(u32);u32 __VENEER_OS_DisableIrqMask(u32);u32 __VENEER_OS_EnableIrqMask(u32);void WEnableTmpttPowerSave(void);void WDisableTmpttPowerSave(void);void DeleteTxFrames(u32);u8 WL_ReadByte(u8*);void WL_WriteByte(u8*,u8);
void CAM_SetTIMElementBitmap(u32 index);
// @fn CAM_SetTIMElementBitmap
void CAM_SetTIMElementBitmap(u32 index){u8*tim;u32 old,aid;u8 value;if(CAM_GetStaState(index)!=64)return;if(HALF(0x534)&(1U<<index))return;tim=(u8*)0x0480425C+HALF(0x3d8);old=__VENEER_OS_DisableIrqMask(0x1000000);if(index==0){value=WL_ReadByte(tim+4);WL_WriteByte(tim+4,(u8)(value|1));}else{aid=CAM_GetAID(index);tim=tim+5+(aid>>3);value=WL_ReadByte(tim);WL_WriteByte(tim,(u8)(value|(1U<<(aid&7))));}__VENEER_OS_EnableIrqMask(old);}
