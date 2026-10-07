#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define G (*(u8 *volatile *)0x0380fff4)
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define V(a) (*(volatile u16*)(a))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),WStop(void),WInitCounter(void),WSetPowerMgtMode(u16),WSetForcePowerState(u16),WSetPowerState(u16),WSetStaState(u16),WClearAids(void);
extern void CAM_SetPowerMgtMode(u16,u16),CAM_SetStaState(u16,u16),WSetMacAdrs1(void*,const void*),MoveHeapBuf(void*,void*,void*),DeleteHeapBuf(void*,void*),AddTask(u32,u32);
extern int __VENEER_OS_SendMessage(void*,void*,u32),CheckEnableChannel(u32),FLASH_VerifyCheckSum(u32);extern u32 WL_ReadByte(const void*);
extern void WSetBssid(const void*),WSetSsid(u16,const void*),WSetBeaconPeriod(u16),WSetChannel(u16,u16),WSetRateSet(const void*),WSetPreambleType(u16),WSetDTIMPeriod(u16),WInitGameInfo(u16,const void*),WStart(void);
extern void MLME_AuthTask(void),MLME_AssTask(void),MLME_ReAssTask(void),MLME_MeasChannelTask(void);
extern void *MakeDeAuthFrame(const void*,u16,u32),*MakeDisAssFrame(const void*,u16);extern void CAM_AddBcFrame(void*,void*),TxqPri(u32),DeleteTxFrameByAdrs(const void*),TxManCtrlFrame(void*);extern u8 BC_ADRS[];
int MLME_DeAuthReqCmd(void*req,void*cfm);
// @fn MLME_DeAuthReqCmd
int MLME_DeAuthReqCmd(void*req,void*cfm){u8*g=G,*state=g+0x404;u32 mode;u8*current;u8*frame;H(cfm,2)=4;current=G;mode=H(current,0x32e);if(mode!=3&&mode!=2&&mode!=1)return 11;if(H(current,0x34c)<48)return 1;if((u16)(mode+0xfffe)<=1&&(H(req,16)&1))return 5;WSetMacAdrs1((u8*)cfm+6,(u8*)req+16);frame=MakeDeAuthFrame((u8*)cfm+6,H(req,22),0);if(!frame)return 8;P(state,24)=req;P(state,28)=cfm;P(state,4)=frame;H(state,0)=65;if(H(req,16)&1){H(frame,4)=*(u32*)(G+0x3ec);CAM_AddBcFrame(G+0x188,frame-16);g=G;if(!(H(g,0x52e)&~H(g,0x532)))TxqPri(2);}else{DeleteTxFrameByAdrs((u8*)req+16);TxManCtrlFrame(frame);}return 128;}
