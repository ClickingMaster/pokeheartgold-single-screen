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
extern int __VENEER_OS_SendMessage(void*,void*,u32),CheckEnableChannel(u32);extern u32 FLASH_VerifyCheckSum(u32*);extern u32 WL_ReadByte(const void*);
extern void WSetBssid(const void*),WSetSsid(u16,const void*),WSetBeaconPeriod(u16),WSetChannel(u16,u16),WSetRateSet(const void*),WSetPreambleType(u16),WSetDTIMPeriod(u16),WInitGameInfo(u16,const void*),WStart(void);
extern void MLME_AuthTask(void),MLME_AssTask(void),MLME_ReAssTask(void),MLME_MeasChannelTask(void);
extern void *MakeDeAuthFrame(const void*,u16,u32),*MakeDisAssFrame(const void*,u16);extern void CAM_AddBcFrame(void*,void*),TxqPri(u32),DeleteTxFrameByAdrs(const void*),TxManCtrlFrame(void*);extern u8 BC_ADRS[];
int MLME_JoinReqCmd(void*req,void*cfm);
typedef struct{u16 pad[4];u16 state;u16 tail[78];}w35571_Config;
// @fn MLME_JoinReqCmd
int MLME_JoinReqCmd(void*req,void*cfm){u8*g=G,*state=g+0x404;w35571_Config*config=(w35571_Config*)(g+0x344);u32 mode;u32 a,b;H(cfm,2)=5;mode=H(g,0x32e);if(mode!=3&&mode!=2)return 11;if(config->state<32)return 1;WSetStaState(32);if(H(req,0x18)&1)return 5;if(!H(req,0x1e))return 5;if(H(req,0x1e)>32)return 5;if(H(req,0x46)<10)return 5;if(H(req,0x46)>1000)return 5;if(H(req,0x48)>255)return 5;if(H(req,0x4a)&0xfff0)return 5;if(!CheckEnableChannel(H(req,0x4a)))return 5;a=H(req,0x42);if(a&0xfffff000)return 5;b=H(req,0x44);if(b&0xfffff000)return 5;if(!a)return 5;if(!(b|a))return 5;if(H(req,0x10)>2000)return 5;if(FLASH_VerifyCheckSum(0))return 14;if(H(req,0x40)&32)WSetPreambleType(1);else WSetPreambleType(0);WSetBssid((u8*)req+0x18);WSetSsid(H(req,0x1e),(u8*)req+0x20);WSetBeaconPeriod(H(req,0x46));WSetChannel(H(req,0x4a),0);WSetRateSet((u8*)req+0x42);P(state,24)=req;P(state,28)=cfm;H(state,0)=32;AddTask(2,1);return 128;}
