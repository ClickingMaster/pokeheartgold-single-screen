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
extern void sub_0203A914(void);
extern void GF_AssertFail(void);
extern void Save_Cancel(void *);
extern void sub_0201A738(u32);
extern void PlaySE(u32);
extern void sub_020394F4(void);
extern void UnloadOVY38(void);
extern void UnloadDwcOverlay(void);
extern void *Heap_Alloc(u32, u32);
extern void ov74_02229E28(void);
extern void *ov74_02233B04(u32);
extern void *ov74_022358C8(u32);

extern void ov74_0222BED8(void *);
extern void ov74_0222BEF0(void *);
extern Ov74WmState *ov74_0223105C(void);
extern void *ov74_02231054(void);

extern void *ov74_0223D0A4;
extern void *ov74_0223D0A8;
extern u8 ov74_0223C920[];
extern u8 ov74_0223D0C4[];
extern u32 ov74_0223D33C[];
extern u32 ov74_0223D450;
extern u32 ov74_0223D454[];
extern void *ov74_0223E2FC;
extern u32 ov74_0223E304[];

void *ov74_0222CD7C(void);
void *ov74_02229DD4(void);
void *ov74_02231148(void);
void *ov74_02231184(void);
void *ov74_022311DC(void);
void *ov74_022311E8(void);
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
extern void sub_0200FBF4(u32, u32);
extern u32 ov74_0223D454[];
extern BOOL IsPaletteFadeFinished(void);
extern void ov74_02233DBC(int);
extern void ov74_02233DBC(int);
extern void *sPmAgbCartridgeSpec;
extern u64 OS_GetTick(void);
extern u32 ov74_0223127C(u32, u32, u32 *);
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
extern void ov74_02230030(void);
extern void ov74_02230070(void);
extern void ov74_02230138(void);
extern void ov74_02230404(void);
extern void ov74_02230590(void);
extern void ov74_02230964(void);
extern void ov74_02230988(void);
extern void ov74_022309AC(void);
extern void ov74_022309F0(void);

extern int WM_MeasureChannel(void *, u32, u32, u16, u32);
extern void ov74_022300A8(void);
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
