#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void *AllocateHeapBuf(void *, u32);
void SetFatalErr(u32);
void InitManHeader(void *, const void *);
u32 SetSupRateSet(u8 *);
u32 SetSSIDElement(u8 *);
u8 WL_ReadByte(const void *);
void WL_WriteByte(void *, u8);
int IsEnableManagement(void);
int MatchMacAdrs(const void *, const void *);
u8 *GetHeapBufNextAdrs(void *);
u32 CAM_AllocateAID(u32);
const void *CAM_GetMacAdrs(u32);
u32 CAM_Search(const void *);
u32 CAM_GetStaState(u32);
void CAM_SetStaState(u16, u32);
void CAM_UpdateLifeTime(u32);
u32 CAM_GetAID(u32);
void CAM_SetPowerMgtMode(u16, u32);
void CAM_SetCapaInfo(u32, u32);
void CAM_SetSupRate(u32, u32);
void DeleteTxFrames(u32);
void MLME_IssueDisAssIndication(const void *, u32);
void MLME_IssueDeAuthIndication(const void *, u32);
void WSetStaState(u32);
void WClearAids(void);
u8 *MakeDisAssFrame(const void *, u32);
u8 *MakeDeAuthFrame(const void *, u16, u32);
u8 *MakeAssResFrame(u32, u32, const void *);
u8 *MakeProbeResFrame(const void *);
int IsExistManFrame(const void *, u16);
void TxManCtrlFrame(void *);
void __VENEER_MIi_CpuClear16(u32, void *, u32);
void __VENEER_MIi_CpuClear32(u32, void *, u32);
void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
void SendMessageToWmDirect(void *, void *);
u32 WCalcManRate(void);
void WSetMacAdrs2(void *, const void *, const void *);
void WSetMacAdrs3(void *, const void *, const void *, const void *);
void ReleaseHeapBuf(void *, void *);
void ElementChecker(void *);
void RxProbeReqFrame(u8 *frame);

void RxProbeReqFrame(u8 *frame) {
    u32 info[11];
    u8 *reply;
    if (IsExistManFrame(frame + 0x1e, 0x50)) {
        return;
    }
    if (!(H(frame, 0x24) & 1) && !(H(frame, 8) & 0x8000)) {
        return;
    }
    __VENEER_MIi_CpuClear32(0, info, 44);
    P(info, 0) = frame + 0x2c;
    H(info, 8) = H(frame, 6);
    if (!((struct { u16 lower : 1, enabled : 1, upper : 14; } *)(BASE + 0x33a))->enabled) {
        H(info, 12) = 0x800;
    }
    ElementChecker(info);
    if ((H(info, 10) & 1) != 1) {
        return;
    }
    reply = MakeProbeResFrame(frame + 0x1e);
    if (reply) {
        TxManCtrlFrame(reply);
    }
}
