#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define P(p,o) (*(u8 **)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16 *)(a))
void *AllocateHeapBuf(void *,u32);void SetFatalErr(u32);void InitManHeader(void*,const void*);u32 SetSupRateSet(void*);u32 SetSSIDElement(void*);u32 WL_ReadByte(const void*);void WL_WriteByte(void*,u8);int IsEnableManagement(void);int MatchMacAdrs(const void*,const void*);u8 *GetHeapBufNextAdrs(void*);u32 CAM_AllocateAID(u32);const void*CAM_GetMacAdrs(u32);u32 CAM_Search(const void*);u32 CAM_GetStaState(u32);void CAM_SetStaState(u16,u32);void CAM_UpdateLifeTime(u32);u32 CAM_GetAID(u32);void CAM_SetPowerMgtMode(u16,u32);void CAM_SetCapaInfo(u32,u32);void CAM_SetSupRate(u32,u32);void DeleteTxFrames(u32);void MLME_IssueDisAssIndication(const void*,u32);void MLME_IssueDeAuthIndication(const void*,u32);void WSetStaState(u32);void WClearAids(void);u8*MakeDisAssFrame(const void*,u32);u8*MakeDeAuthFrame(const void*,u32,u32);u8*MakeAssResFrame(u32,u32,const void*);u8*MakeProbeResFrame(const void*);int IsExistManFrame(const void*,u32);void TxManCtrlFrame(void*);void __VENEER_MIi_CpuClear16(u32,void*,u32);void __VENEER_MIi_CpuClear32(u32,void*,u32);void __VENEER_MIi_CpuCopy16(const void*,void*,u32);void SendMessageToWmDirect(void*,void*);u32 WCalcManRate(void);void WSetMacAdrs2(void*,const void*,const void*);void WSetMacAdrs3(void*,const void*,const void*,const void*);void ReleaseHeapBuf(void*,void*);
u8*MakeAuthFrame(const void*addr,u16 length,u32 test);
u8*MakeAuthFrame(const void*addr,u16 length,u32 test){u8*p;if(test&&!IsEnableManagement())return 0;p=AllocateHeapBuf(BASE+0x188,length+61);if(!p){SetFatalErr(2);return p;}H(p,12)=65535;InitManHeader(p+16,addr);if(length){WL_WriteByte(p+0x42,16);WL_WriteByte(p+0x43,length);length+=2;}H(p,0x16)=length+6;H(p,0x22)=H(p,0x16)+28;H(p,0x24)=0xb0;return p+16;}
