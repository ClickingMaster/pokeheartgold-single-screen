#pragma force_active on
#pragma dollar_identifiers on
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
extern const u8 wlVersion$4017[];
#pragma define_section wlver ".rodata"
int DEV_GetVerInfoReqCmd(u16 *a, u16 *o);

int DEV_GetVerInfoReqCmd(u16 *a, u16 *o) {
    u8 *p;
    o[1] = 9;
    __VENEER_MIi_CpuCopy16(wlVersion$4017, o + 3, 8);
    o[7] = V(0x04808000);
    if (!(H(G, 0x690) & 0x8000)) {
        o[8] = 0x6d;
        o[9] = 0x933d;
    } else {
        o[8] = BBP_Read(0);
        o[9] = CalcBbpCRC();
    }
    p = G;
    if (H(p, 0x690) & 0x4000) {
        o[10] = H(p, 0x5f8);
    } else {
        o[10] = 2;
    }
    return 0;
}
