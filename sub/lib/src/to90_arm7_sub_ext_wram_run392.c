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
extern void SetManCtrlFrame(void *);
extern u32 WCalcManRate(void);
extern u32 SetSSIDElement(void *);
extern u32 SetSupRateSet(void *);
extern u32 CAM_AllocateAID(u32);
extern u32 WL_ReadByte(void *);
extern void WSetMacAdrs1(void *, void *);
extern void DMA_Write(void *, void *, u32);
extern void WL_WriteByte(void *, u32);
extern void *CAM_GetMacAdrs(u32);
void TxqEndData(void *arg, u32 next);

typedef struct {
    u16 pad0 : 2;
    u16 type : 2;
    u16 pad1 : 3;
    u16 bit7 : 1;
    u16 bit8 : 1;
    u16 pad2 : 4;
    u16 bit13 : 1;
    u16 bit14 : 1;
    u16 pad3 : 1;
} W35675Flags;

void TxqEndData(void *arg, u32 next) {
    u8 *frame = (u8 *)arg;
    u8 *root = ROOT, *heap = root + 0x194, *state = root + 0x344, *buf = frame - 16;
    CAM_DecFrameCount(frame);
    if (!(H(frame, 8) & 2)) {
        W(root, 0x53c)
        ++;
        if (((volatile W35675Flags *)(frame + 0x14))->bit8) {
            if (H(frame, 0x24) & 1) {
                W(root, 0x550)
                ++;
            } else {
                W(root, 0x54c)
                ++;
            }
        } else {
            if (H(frame, 0x18) & 1) {
                W(root, 0x550)
                ++;
            } else {
                W(root, 0x54c)
                ++;
            }
        }
    } else {
        W(root, 0x540)
        ++;
    }
    if (((volatile W35675Flags *)(frame + 0x14))->bit14) {
        W(root, 0x554)
        ++;
    }
    IssueMaDataConfirm(heap, buf);
    H(ROOT, 0x42c) = 0;
    if (CAM_GetPowerMgtMode(H(frame, 2)) != 0 && !(H(frame, 0x14) & 0x2000)) {
        CAM_SetDoze(H(frame, 2));
    }
    if (next) {
        if (H(heap, 8)) {
            TxqPri(0);
        } else if ((u16)(H(state, 12) + 0xfffe) <= 1 && H(state, 8) == 0x40 && H(state, 14) && CAM_GetFrameCount(H(state, 0x88)) == 0 && H(state, 0x8e) == 0) {
            WSetPowerState(1);
        }
    }
}
