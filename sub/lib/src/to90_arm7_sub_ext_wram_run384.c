#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define G       (*(u8 *volatile *)0x0380FFF4)
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define V(n)    (*(volatile u16 *)(n))
#define P(p, n) (*(u8 **)((u8 *)(p) + (n)))
void WSetMacAdrs1(void *, const void *);
void WSetStaState(u32);
int FLASH_VerifyCheckSum(u32);
void WStop(void);
void WlessLibReboot(void);
void WInitCounter(void);
void WUpdateCounter(void);
void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
void __VENEER_MIi_CpuCopy32(const void *, void *, u32);
u32 WL_ReadByte(const void *);
void WL_WriteByte(void *, u32);
u32 BBP_Read(u32);
u32 CalcBbpCRC(void);
void WSetChannel(u32, u32);
void WStart(void);
void WSetForcePowerState(u32);
void CAM_IncFrameCount(void *);
void MoveHeapBuf(void *, void *, void *);
void TxqPri(u32);
void ClearTxKeyData(void);
void ClearTxMp(void);
void ClearTxData(void);
void SendMessageToWmDirect(void *, void *);
u32 CAM_Search(const void *);
u32 CAM_GetStaState(u32);
u32 CAM_GetTxRate(u32);
void CAM_AddBcFrame(void *, void *);
void WSetMacAdrs3(void *, const void *, const void *, const void *);
extern const u8 MPKEY_ADRS[];
void DMA_Write(void *, const void *, u32);
void DMA_WriteHeaderData(void *, const void *, const void *, u32);
void DMA_WepWriteHeaderData(void *, const void *, const void *, u32);
u32 __VENEER_OS_DisableIrqMask(u32);
void __VENEER_OS_EnableIrqMask(u32);
void WSetKSID(u32);
void *GetHeapBufNextAdrs(void *);
int CheckFrameTimeout(u16 *, u16 *);
u32 CAM_IsActive(u32);
void CAM_DecFrameCount(void *);
void IssueMaDataConfirm(void *, void *);
void WSetPowerState(u32);
u32 CAM_GetFrameCount(u32);
void CopyTxFrmToMacBuf(void *, void *);
int MA_KeyDataReqCmd(u16 *a, u16 *o);

int MA_KeyDataReqCmd(u16 *a, u16 *o) {
    u8 *c, *q;
    u32 flags;
    u8 *b;
    u32 offset, i = 1;
    u16 *slots;
    u32 irq;
    u8 *p = G, *tail;
    flags = H(p, 0x690);
    c = p + 0x31c;
    o[1] = i;
    q = p + 0x42c;
    if (H(c, 0x12) != 2) {
        return 11;
    }
    if (a[8] > 0x204) {
        return 5;
    }
    if (H(q, 0x50) == 0) {
        i = 0;
    }
    slots = (u16 *)(q + 0x50);
    offset = i * 20;
    if (H(slots, offset)) {
        return 8;
    }
    if (V(0x04808094) & 0x8000) {
        return 8;
    }
    b = P(q, offset + 0x58);
    H(b, 0) = 0;
    H(b, 4) = 0;
    H(b, 8) = 20;
    H(b, 10) = a[8] + 30;
    H(b, 12) = 0x118;
    WSetMacAdrs3(b + 16, p + 0x3a8, c + 8, MPKEY_ADRS);
    H(b, 0x24) = a[9];
    if (a[8]) {
        if (i == 0) {
            WUpdateCounter();
        }
        DMA_Write(b + 0x26, P(a, 0x14), a[8]);
    }
    if (flags & 4) {
        {
            u32 length = a[8] + 2;
            u8 *payload = b + 0x24;
            tail = (u8 *)(((u32)(payload + length) + 3) & ~3u);
        }
        H(tail, 0) = 0xb6b8;
        H(tail, 2) = 0x1d46;
    }
    irq = __VENEER_OS_DisableIrqMask(0x1000000);
    H(slots, offset) = 2;
    q += 0x52;
    H(q, offset)
    ++;
    V(0x04808094) = (((u32)b & 0x3fff) >> 1) | 0x8000;
    {
        u32 k = ((u32)H(c, 0x1e) << 24) >> 31;
        if (!k) {
            WSetKSID(k);
        }
    }
    __VENEER_OS_EnableIrqMask(irq);
    return 0;
}
