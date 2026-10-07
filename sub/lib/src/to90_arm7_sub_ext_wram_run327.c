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
int DEV_RebootReqCmd(u16*a,u16*o);
// @fn DEV_RebootReqCmd
int DEV_RebootReqCmd(u16*a,u16*o){o[1]=1;if(H(G,0x34c)>=32)WStop();WlessLibReboot();return 0;}
