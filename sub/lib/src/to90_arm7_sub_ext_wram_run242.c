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
extern int CheckEnableChannel(u32);
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
int MLME_ScanReqCmd(void *req, void *cfm);

int MLME_ScanReqCmd(void *req, void *cfm) {
    u8 *g = G, *state = g + 0x404;
    u32 mode;
    u32 i;
    H(state, 4) = H(cfm, 2) - 3;
    H(cfm, 2) = 3;
    mode = H(g, 0x32e);
    if (mode != 1 && mode != 3 && mode != 2) {
        return 11;
    }
    if (H(G, 0x34c) < 32) {
        return 1;
    }
    if (H(req, 0x16) > 32) {
        return 5;
    }
    if (H(req, 0x38) > 1) {
        return 5;
    }
    if (!WL_ReadByte((u8 *)req + 0x3a)) {
        return 5;
    }
    if (H(req, 0x4a) > 1000) {
        return 5;
    }
    if (H(req, 0x4a) < 10) {
        return 5;
    }
    if (H(req, 0x4c) > 16) {
        return 5;
    }
    for (i = 0; i < 16; i++) {
        u32 ch = WL_ReadByte((u8 *)req + 0x3a + i);
        if (!ch) {
            break;
        }
        if (!CheckEnableChannel(ch)) {
            return 5;
        }
    }
    WSetBssid((u8 *)req + 16);
    WSetSsid(H(req, 0x16), (u8 *)req + 0x18);
    P(state, 24) = req;
    P(state, 28) = cfm;
    H(state, 0) = 16;
    AddTask(2, 0);
    return 128;
}
