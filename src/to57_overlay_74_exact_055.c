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
extern void *PmAgbCartridge_GetSectorData(u32);
extern void *MainMenuUtil_UpdateGiftSprite(u32);

extern void MysteryGiftAnim_ApplyBrightnessMain(void *);
extern void MysteryGiftAnim_ApplyBrightnessSub(void *);
extern Ov74WmState *RangerLink_GetWork(void);
extern void *RangerLink_GetParentParam(void);

extern void *sMysteryGiftCommWork;
extern void *ov74_0223D0A8;
extern u8 gRangerLinkParentParam[];
extern u8 gRangerLinkWork[];
extern u32 sPmAgbSave[];
extern u32 sMainMenuUtilCryptoHeapId;
extern u32 gMainMenuUtilWork[];
extern void *ov74_0223E2FC;
extern u32 ov74_0223E304[];

void *MysteryGiftApp_GetOverlayManager(void);
void *MysteryGiftComm_GetRecvBuffer(void);
void *RangerLink_GetPeers(void);
void *RangerLink_GetTransfer(void);
void *RangerLink_GetGameInfo(void);
void *RangerLink_GetSsid(void);
void *PmAgbCartridge_GetPCStorage(void);
void *PmAgbCartridge_GetTrainerInfo(void);
void *PmAgbCartridge_GetPlayerName(void);
void *MainMenuUtil_CryptoAlloc(u32 size);

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
extern BOOL MainMenuUtil_CheckGiftCartridgeSignature(void);

extern void Heap_Create(u32, u32, u32);
extern void *OverlayManager_CreateAndGetData(void *, u32, u32);
extern void SetMasterBrightnessByColor(u32, u32);
extern u32 gMainMenuUtilWork[];
extern BOOL IsPaletteFadeFinished(void);
extern void PmAgbCartridge_WriteSectorAsync(int);
extern void PmAgbCartridge_WriteSectorAsync(int);
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

u32 ov74_02236F30(u32 value) {
    u32 result;
    if (value == 1) {
        result = 1;
    } else {
        result = 0x1de;
    }
    return (u16)result;
}

u16 ov74_02236F44(u32 language) {
    switch (language) {
    case 1:
    default:
        return 0xEA;
    case 2:
    case 4:
    case 7:
        return 0x1B4;
    case 3:
        return 0x1B7;
    case 5:
        return 0x1B6;
    }
}

u16 ov74_02236F80(u32 language) {
    switch (language) {
    case 1:
    default:
        return 0xEB;
    case 2:
    case 4:
    case 7:
        return 0x1B5;
    case 3:
        return 0x1B8;
    case 5:
        return 0x1B4;
    }
}

BOOL ConvertRSStringToDPStringInternational(const u8 *src, u16 *dst, u32 length, u32 language) {
    BOOL nonJa = (language != 1);
    u32 i;
    for (i = 0; i < (length - 1); i++) {
        if (src[i] == 0xFF) {
            break;
        }
        if (src[i] >= 0xF7) {
            int invalidStrLen = (length - 1 < 10) ? length - 1 : 10;
            int j;
            for (j = 0; j < invalidStrLen; j++) {
                dst[j] = 0x1AC;
            }
            dst[j] = 0xFFFF;
            return FALSE;
        }
        switch (_0223B760[src[i]][nonJa]) {
        case 1:
            dst[i] = ov74_02236F30(language);
            break;
        case 0xEA:
            dst[i] = ov74_02236F44(language);
            break;
        case 0xEB:
            dst[i] = ov74_02236F80(language);
            break;
        default:
            dst[i] = _0223B760[src[i]][nonJa];
            break;
        }
    }
    dst[i] = 0xFFFF;
    return TRUE;
}
