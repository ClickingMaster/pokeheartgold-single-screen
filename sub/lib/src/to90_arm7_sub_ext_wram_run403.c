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
extern u32 WL_ReadByte(const void *);
extern void WSetChannel(u16, u16);
extern void WSetPreambleType(u16);
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
int MLME_StartReqCmd(void *req, void *cfm);

typedef struct {
    u8 prefix[16];
    u16 length;
    u8 ssid[32];
    u16 beacon, dtim, channel, rates[2], gameLength;
} StartRequest;

extern u32 CheckEnableChannel(u32 channel);
extern u32 FLASH_VerifyCheckSum(u32 *out);
extern int WInitGameInfo(u32 length, const void *input);
extern u32 WSetBeaconPeriod(u16 x);
extern u32 WSetBssid(const u16 *p);
extern int WSetDTIMPeriod(u16 x);
extern int WSetRateSet(const u16 *r);
extern u32 WSetSsid(u32 count, const u8 *src);

int MLME_StartReqCmd(void *req, void *cfm) {
    StartRequest *r = req;
    u8 *g = G, *config = g + 0x344, *cam = g + 0x31c;
    H(cfm, 2) = 1;
    if (H(cam, 18) != 1 && H(cam, 18) != 0) {
        return 11;
    }
    if (H(config, 8) != 32) {
        return 1;
    }
    if (r->length > 32) {
        return 5;
    }
    if (!r->length) {
        return 5;
    }
    if (r->beacon < 10) {
        return 5;
    }
    if (r->beacon > 1000) {
        return 5;
    }
    if (!r->dtim) {
        return 5;
    }
    if (r->dtim > 255) {
        return 5;
    }
    if (r->channel & 0xfff0) {
        return 5;
    }
    if (!CheckEnableChannel(r->channel)) {
        return 5;
    }
    if (!r->rates[0]) {
        return 5;
    }
    if (r->rates[0] & 0xfffff000) {
        return 5;
    }
    if (!r->rates[1]) {
        return 5;
    }
    if (r->rates[1] & 0xfffff000) {
        return 5;
    }
    if (r->gameLength > 128) {
        return 5;
    }
    if (FLASH_VerifyCheckSum(0)) {
        return 14;
    }
    if (!H(cam, 18)) {
        WSetBssid((const u16 *)BC_ADRS);
    } else {
        WSetBssid((const u16 *)(cam + 8));
    }
    WSetSsid(r->length, r->ssid);
    WSetBeaconPeriod(r->beacon);
    WSetDTIMPeriod(r->dtim);
    WSetChannel(r->channel, 0);
    WSetRateSet(r->rates);
    WInitGameInfo(r->gameLength, (const u8 *)req + 0x3e);
    H(config, 0xa4) = 0;
    WStart();
    return 0;
}
