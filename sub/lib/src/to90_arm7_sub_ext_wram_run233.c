#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define G (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define V(a) (*(volatile u16*)(a))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),WStop(void),WInitCounter(void),WSetPowerMgtMode(u16),WSetForcePowerState(u16),WSetPowerState(u16),WSetStaState(u16),WClearAids(void);
extern void CAM_SetPowerMgtMode(u16,u16),CAM_SetStaState(u16,u16),WSetMacAdrs1(void*,const void*),MoveHeapBuf(void*,void*,void*),DeleteHeapBuf(void*,void*),AddTask(u32,u32);
extern int __VENEER_OS_SendMessage(void*,void*,u32),CheckEnableChannel(u16),FLASH_VerifyCheckSum(u32);extern u32 WL_ReadByte(const void*);
extern void WSetBssid(const void*),WSetSsid(u16,const void*),WSetBeaconPeriod(u16),WSetChannel(u16,u16),WSetRateSet(const void*),WSetPreambleType(u16),WSetDTIMPeriod(u16),WInitGameInfo(u16,const void*),WStart(void);
extern void MLME_AuthTask(void),MLME_AssTask(void),MLME_ReAssTask(void),MLME_MeasChannelTask(void);
extern void *MakeDeAuthFrame(const void*,u16,u32),*MakeDisAssFrame(const void*,u16);extern void CAM_AddBcFrame(void*,void*),TxqPri(u32),DeleteTxFrameByAdrs(const void*),TxManCtrlFrame(void*);extern u8 BC_ADRS[];
void WaitLoop_ClrAid(void);
// @fn WaitLoop_ClrAid
void WaitLoop_ClrAid(void){u16 old;u32 wait=1;while(wait){old=V(0x04000208);u32 x;V(0x04000208)=0;x=V(0x0480819c)&3;if(x!=3){x=V(0x04808214);if(x!=5&&x!=7&&x!=8){V(0x04808028)=0;wait=0;}}(void)V(0x04000208);V(0x04000208)=old;}}
