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
extern int __VENEER_OS_SendMessage(void*,void*,u32),CheckEnableChannel(u32),FLASH_VerifyCheckSum(u32);extern u32 WL_ReadByte(const void*);
extern void WSetBssid(const void*),WSetSsid(u16,const void*),WSetBeaconPeriod(u16),WSetChannel(u16,u16),WSetRateSet(const void*),WSetPreambleType(u16),WSetDTIMPeriod(u16),WInitGameInfo(u16,const void*),WStart(void);
extern void MLME_AuthTask(void),MLME_AssTask(void),MLME_ReAssTask(void),MLME_MeasChannelTask(void);
extern void *MakeDeAuthFrame(const void*,u16,u32),*MakeDisAssFrame(const void*,u16);extern void CAM_AddBcFrame(void*,void*),TxqPri(u32),DeleteTxFrameByAdrs(const void*),TxManCtrlFrame(void*);extern u8 BC_ADRS[];
int MLME_MeasChanReqCmd(void*req,void*cfm);
// @fn MLME_MeasChanReqCmd
int MLME_MeasChanReqCmd(void*req,void*cfm){u8*g=G,*state=g+0x404;u32 i;H(cfm,2)=18;if(H(G,0x34c)!=32)return 1;if(H(req,18)>3)return 5;if(H(req,20)>63)return 5;if(!H(req,22))return 5;if(H(req,22)>1000)return 5;for(i=0;i<16;i++){u32 ch=WL_ReadByte((u8*)req+24+i);if(!ch)break;if(!CheckEnableChannel(ch))return 5;}if(!i)return 5;P(state,24)=req;P(state,28)=cfm;H(state,0)=128;H(cfm,4)=128;MLME_MeasChannelTask();return 128;}
