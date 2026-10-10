#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT    (*(u8 *volatile *)0x0380FFF4)
#define H(p, n) (*(volatile u16 *)((u8 *)(p) + (n)))
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(u8 **)((u8 *)(p) + (n)))
extern void CAM_DecFrameCount(void *);
extern void CAM_IncFrameCount(void *);
extern void IssueMaDataConfirm(void *, void *);
extern void TxqPri(u32);
extern void CAM_SetDoze(u32);
extern void WSetPowerState(u32);
extern void MoveHeapBuf(void *, void *, void *);
extern void TxqEndManCtrl(void *, u32);
extern void CAM_ClrTIMElementBitmap(u32);
extern u32 CAM_GetPowerMgtMode(u32);
extern u32 CAM_GetFrameCount(u32);
extern u32 __VENEER_OS_DisableIrqMask(u32);
extern void __VENEER_OS_EnableIrqMask(u32);
extern void WClearKSID(u32);
extern void WlIntrMpEndTask(void);
extern void ClearQueuedPri(u32);
extern void MessageDeleteTx(u32, u32);
extern void ReleaseHeapBuf(void *, void *);
extern void SetFatalErr(u32);
extern void InitManHeader(void *, void *);
extern void *AllocateHeapBuf(void *, u32);
extern void *GetHeapBufNextAdrs(void *);
extern u32 CAM_Search(void *);
extern u32 CAM_GetStaState(u32);
extern void CAM_SetStaState(u16, u32);
extern void ClearTxKeyData(void);
extern void DeleteTxFrames(u32);
extern const u16 Pri2QBit[];
extern u32 WCalcManRate(void);
extern u32 SetSSIDElement(void *);
extern u32 SetSupRateSet(void *);
extern u32 CAM_AllocateAID(u32);
extern u32 WL_ReadByte(void *);
extern void WSetMacAdrs1(void *, void *);
extern void DMA_Write(void *, void *, u32);
extern void WL_WriteByte(void *, u32);
extern void *CAM_GetMacAdrs(u32);
void SetManCtrlFrame(u8 *frame);

void SetManCtrlFrame(u8 *frame) {
    H(frame, 2) = (u16)CAM_Search(frame + 0x18);
    if (H(frame, 2) == 255) {
        H(frame, 2) = 0;
    }
    H(frame, 4) = (u16)W(ROOT, 0x3ec);
    if (H(frame, 0x14) & 0x4000) {
        H(frame, 0x12) += 8;
    }
    CAM_IncFrameCount(frame);
    {
        u8 *root = ROOT;
        MoveHeapBuf(root + 0x188, root + 0x1a0, frame - 16);
    }
}
