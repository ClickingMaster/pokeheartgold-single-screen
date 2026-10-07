#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define G (*(u8 *volatile *)0x0380FFF4)
#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
#define V(n) (*(volatile u16 *)(n))
#define P(p,n) (*(u8 **)((u8 *)(p)+(n)))
void WSetMacAdrs1(void*,const void*);void WSetStaState(u32);int FLASH_VerifyCheckSum(u32);void WStop(void);void WlessLibReboot(void);void WInitCounter(void);void WUpdateCounter(void);void __VENEER_MIi_CpuCopy16(const void*,void*,u32);void __VENEER_MIi_CpuCopy32(const void*,void*,u32);u32 WL_ReadByte(const void*);void WL_WriteByte(void*,u32);u32 BBP_Read(u32);u32 CalcBbpCRC(void);void WSetChannel(u32,u32);void WStart(void);void WSetForcePowerState(u32);void CAM_IncFrameCount(void*);void MoveHeapBuf(void*,void*,void*);void TxqPri(u32);void ClearTxKeyData(void);void ClearTxMp(void);void ClearTxData(void);void SendMessageToWmDirect(void*,void*);
int PARAMGET_GameInfoReqCmd(u16*a,u16*o);
// @fn PARAMGET_GameInfoReqCmd
int PARAMGET_GameInfoReqCmd(u16*a,u16*o){u8 *p,*s,*d;u32 i;if(o[1]>1)o[3]=H(G,0x3e4);p=G;if((int)H(p,0x3e4)>((int)o[1]-2)*2)return 4;if(o[3]){if(H(p,0x3e6)&1){s=P(p,0x3e0);d=(u8 *)(o+4);s++;for(i=0;i<o[3];i++){WL_WriteByte(d,WL_ReadByte(s));d++;s++;}}else __VENEER_MIi_CpuCopy16(P(p,0x3e0),o+4,o[3]+1);}o[1]=((int)o[3]+1)/2+2;return 0;}
