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
u32 CAM_Search(const void*);u32 CAM_GetStaState(u32);u32 CAM_GetTxRate(u32);void CAM_AddBcFrame(void*,void*);void WSetMacAdrs3(void*,const void*,const void*,const void*);extern const u8 MPKEY_ADRS[];void DMA_Write(void*,const void*,u32);void DMA_WriteHeaderData(void*,const void*,const void*,u32);void DMA_WepWriteHeaderData(void*,const void*,const void*,u32);u32 __VENEER_OS_DisableIrqMask(u32);void __VENEER_OS_EnableIrqMask(u32);void WSetKSID(u32);void*GetHeapBufNextAdrs(void*);int CheckFrameTimeout(u16*,u16*);u32 CAM_IsActive(u32);void CAM_DecFrameCount(void*);void IssueMaDataConfirm(void*,void*);void WSetPowerState(u32);u32 CAM_GetFrameCount(u32);void CopyTxFrmToMacBuf(void*,void*);
int MA_DataReqCmd(u16*a,u16*o);
// @fn MA_DataReqCmd
int MA_DataReqCmd(u16*a,u16*o){u8*p=G,*s=p+0x344,*c=p+0x31c;u32 id;if(a[11]>0x5e4)return 5;if(H(c,0x12)==1){id=CAM_Search(a+20);if(id==255||CAM_GetStaState(id)!=64)return 10;}else id=H(s,0x88);a[9]=id;a[10]=(u32)P(G,0x3ec);if(a[15]&255){a[16]=a[15];a[15]=0;}else a[16]=CAM_GetTxRate(id);if(a[11]==0){a[18]=(H(s,0x8a)|64)&~0x4000;a[17]=28;}else{a[18]=H(s,0x8a);if(H(c,0x18)==0)a[17]=a[11]+28;else a[17]=a[11]+36;}switch(H(c,0x12)){case 1:WSetMacAdrs1(a+26,a+23);WSetMacAdrs1(a+23,s+0x64);if(id==0){CAM_AddBcFrame(p+0x200,a);{u8*g=G;if((H(g,0x52e)&~H(g,0x532))==0)TxqPri(2);}}else{CAM_IncFrameCount(a+8);MoveHeapBuf(p+0x200,p+0x194,a);TxqPri(0);}break;case 2:case 3:WSetMacAdrs1(a+26,a+20);WSetMacAdrs1(a+20,s+0x64);CAM_IncFrameCount(a+8);MoveHeapBuf(p+0x200,p+0x194,a);TxqPri(0);break;}return 0x81;}
