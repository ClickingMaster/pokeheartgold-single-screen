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
int CheckFrameTimeout(u16 *a);

int CheckFrameTimeout(u16 *a) {
    u8 *p = G + 0x344;

    struct Bits {
        u16 pad : 2, type : 2, sub : 4, rest : 8;
    };
    struct Bits *f = (struct Bits *)(a + 10);
    u16 timeout = (u16)(H(p, 0x8c) << 3);
    if (f->type == 0) {
        if (H(p, 12) == 1) {
            u32 n = f->sub;
            if (n == 1 || n == 3 || n == 11) {
                timeout >>= 3;
            }
        }
    } else {
        timeout >>= 3;
    }
    return timeout < (u16)(*(u32 *)(p + 0xa8) - a[2]);
}
