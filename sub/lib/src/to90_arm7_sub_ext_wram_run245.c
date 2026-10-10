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
int MLME_AssReqCmd(void *req, void *cfm);

typedef struct {
    u16 pad[4];
    u16 state;
    u16 tail[78];
} w35574_Config;

int MLME_AssReqCmd(void *req, void *cfm) {
    u8 *g = G;
    w35574_Config *config = (w35574_Config *)(g + 0x344);
    u8 *state = g + 0x404;
    u32 mode;
    H(cfm, 2) = 3;
    mode = H(g, 0x32e);
    if (mode != 3 && mode != 2) {
        return 11;
    }
    if (config->state < 48) {
        return 1;
    }
    if (H(req, 16) & 1) {
        return 5;
    }
    if (!H(req, 22)) {
        return 5;
    }
    if (H(req, 22) > 255) {
        return 5;
    }
    if (H(req, 24) > 2000) {
        return 5;
    }
    if (H(req, 24) < 10) {
        return 5;
    }
    WSetStaState(48);
    WClearAids();
    H(config, 0x70) = H(req, 22);
    H(config, 0x72) = H(req, 22);
    P(state, 24) = req;
    P(state, 28) = cfm;
    H(state, 0) = 80;
    MLME_AssTask();
    return 128;
}
