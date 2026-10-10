#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define G       (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
#define V(a)    (*(volatile u16 *)(a))
extern void __VENEER_MIi_CpuClear16(u32, void *, u32);
extern void WStop(void);
extern void WInitCounter(void);
extern void WSetPowerMgtMode(u16);
extern void WSetForcePowerState(u16);
extern void WSetPowerState(u16);
extern void WSetStaState(u16);
extern void WClearAids(void);
extern void CAM_SetPowerMgtMode(u16, u16);
extern void CAM_SetStaState(u16, u16);
extern void WSetMacAdrs1(void *, const void *);
extern void MoveHeapBuf(void *, void *, void *);
extern void DeleteHeapBuf(void *, void *);
extern void AddTask(u32, u32);
extern int __VENEER_OS_SendMessage(void *, void *, u32);
extern int CheckEnableChannel(u16);
extern int FLASH_VerifyCheckSum(u32);
extern u32 WL_ReadByte(const void *);
extern void WSetBssid(const void *);
extern void WSetSsid(u16, const void *);
extern void WSetBeaconPeriod(u16);
extern void WSetChannel(u16, u16);
extern void WSetRateSet(const void *);
extern void WSetPreambleType(u16);
extern void WSetDTIMPeriod(u16);
extern void WInitGameInfo(u16, const void *);
extern void WStart(void);
extern void MLME_AuthTask(void);
extern void MLME_AssTask(void);
extern void MLME_ReAssTask(void);
extern void MLME_MeasChannelTask(void);
extern void *MakeDeAuthFrame(const void *, u16, u32);
extern void *MakeDisAssFrame(const void *, u16);
extern void CAM_AddBcFrame(void *, void *);
extern void TxqPri(u32);
extern void DeleteTxFrameByAdrs(const void *);
extern void TxManCtrlFrame(void *);
extern u8 BC_ADRS[];
void SendMessageToWmDirect(void *heap, void *msg);

void SendMessageToWmDirect(void *heap, void *msg) {
    u8 *g = G;
    if (H(g, 0x1fc)) {
        MoveHeapBuf(heap, g + 0x1f4, msg);
        AddTask(2, 0x13);
    } else if (__VENEER_OS_SendMessage(P(g, 0x304), msg, 0)) {
        DeleteHeapBuf(heap, msg);
    } else {
        MoveHeapBuf(heap, G + 0x1f4, msg);
    }
}
