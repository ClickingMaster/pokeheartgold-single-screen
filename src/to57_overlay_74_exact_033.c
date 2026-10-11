#include "to57_overlay74_large_private.h"
#include "to57_overlay74_large_prototypes.h"

extern BOOL WindowIsInUse(void *);
extern void EraseFrameAndWindow1(void *, u32);
extern void ClearFrameAndWindow2(void *, u32);
extern void Sprite_Delete(void *);
extern void BG_SetMaskColor(u32, u32);
extern void Heap_FreeExplicit(u32, void *);
extern void Heap_Free(void *);
extern void sub_02037E9C(void);
extern void sub_02034DE0(void);
extern void WirelessStatusIcon_Destroy(void);
extern void GF_AssertFail(void);
extern void Save_Cancel(void *);
extern void sub_0201A738(u32);
extern void PlaySE(u32);
extern void sub_020394F4(void);
extern void UnloadOVY38(void);
extern void UnloadDwcOverlay(void);
extern void *Heap_Alloc(u32, u32);
extern void MysteryGiftApp_SetWaitingIconVisible(void);
extern void *ov74_02233B04(u32);
extern void *ov74_022358C8(u32);

extern void MysteryGiftAnim_ApplyBrightnessMain(void *);
extern void MysteryGiftAnim_ApplyBrightnessSub(void *);
extern Ov74WmState *RangerLink_GetWork(void);
extern void *RangerLink_GetParentParam(void);

extern void *sMysteryGiftCommWork;
extern void *ov74_0223D0A8;
extern u8 gRangerLinkParentParam[];
extern u8 gRangerLinkWork[];
extern u32 ov74_0223D33C[];
extern u32 ov74_0223D450;
extern u32 ov74_0223D454[];
extern void *ov74_0223E2FC;
extern u32 ov74_0223E304[];

void *MysteryGiftApp_GetOverlayManager(void);
void *MysteryGiftComm_GetRecvBuffer(void);
void *RangerLink_GetPeers(void);
void *RangerLink_GetTransfer(void);
void *RangerLink_GetGameInfo(void);
void *RangerLink_GetSsid(void);
void *ov74_02233F4C(void);
void *ov74_02233F5C(void);
void *ov74_02233F84(void);
void *ov74_02235138(u32 size);

extern const u16 _0223B760[][2];
extern int CTRDG_IdentifyAgbBackup(int);
extern void CTRDG_ReadAgbFlash(u16, u32, void *, u32);

extern u8 sPmAgbRomHeader[];
extern void CTRDG_Init(void);
extern BOOL CTRDG_IsAgbCartridge(void);
extern u16 CTRDG_GetAgbMakerCode(void);
extern void CTRDG_Enable(BOOL);
extern void CTRDG_Read32(const void *, void *);
extern BOOL CTRDG_CpuCopy16(const void *, void *, u32);
extern BOOL CTRDG_IsExisting(void);
extern BOOL ov74_0223514C(void);

extern void Heap_Create(u32, u32, u32);
extern void *OverlayManager_CreateAndGetData(void *, u32, u32);
extern void SetMasterBrightnessByColor(u32, u32);
extern u32 ov74_0223D454[];
extern BOOL IsPaletteFadeFinished(void);
extern void ov74_02233DBC(int);
extern void ov74_02233DBC(int);
extern void *sPmAgbCartridgeSpec;
extern u64 OS_GetTick(void);
extern u32 RangerLink_Scramble(u32, u32, u32 *);
extern u16 SVC_GetCRC16(u16, const void *, u32);
extern int WM_Enable(void *);
extern int WM_Disable(void *);
extern int WM_StartParent(void *);
extern int WM_StartScan(void *, u32);
extern int WM_EndScan(void *);
extern int WM_PowerOn(void *);
extern int WM_PowerOff(void *);
extern int WM_Reset(void *);
extern int WM_End(void *);
extern void OS_Terminate(void);
extern void RangerLink_EnableCallback(void);
extern void RangerLink_DisableCallback(void);
extern void RangerLink_StartParentCallback(void);
extern void RangerLink_ScanCallback(void);
extern void RangerLink_EndScanCallback(void);
extern void RangerLink_PowerOnCallback(void);
extern void RangerLink_PowerOffCallback(void);
extern void RangerLink_ResetCallback(void);
extern void RangerLink_EndCallback(void);

extern int WM_MeasureChannel(void *, u32, u32, u16, u32);
extern void RangerLink_MeasureChannelCallback(void);
extern u32 AGB_GetBoxMonData(void *, int, u8 *);
extern int TranslateAgbSpecies(int);
extern u32 CalcLevelBySpeciesAndExp(u16, u32);

void ov74_02235414(void *p, u32 a, u32 b, u32 c, u32 d, u32 e) {
    memset(p, 0, 0x54);
    U32AT(p, 4) = 1;
    U32AT(p, 8) = 1;
    U32AT(p, 0x4c) = -1;
    U32AT(p, 0x10) = a;
    U32AT(p, 0x30) = b;
    U32AT(p, 0x34) = c;
    U32AT(p, 0x38) = d;
    U32AT(p, 0x3c) = e;
    U32AT(p, 0x2c) = 0;
    U32AT(p, 0x40) = 0;
    U32AT(p, 0x44) = 0x1020f;
    U8AT(p, 0x48) = 15;
    U32AT(p, 0) = 0;
    U32AT(p, 0x24) = 0;
    U32AT(p, 0x20) = 0;
    U32AT(p, 0x14) = 0;
    U32AT(p, 0x50) = 0xff;
}

void ov74_02235464(void *p, u32 a, u32 b, u32 c) {
    U32AT(p, 0x18) = a;
    U32AT(p, 0x1c) = b;
    U32AT(p, 0x28) = c;
}

void ov74_0223546C(void *p, u32 a, u32 b) {
    U32AT(p, 0) = a;
    U32AT(p, 0x40) = b;
}

void ov74_02235474(void *p, u32 a, u32 b) {
    U32AT(p, 0x20) = a;
    U32AT(p, 0x24) = b;
}
