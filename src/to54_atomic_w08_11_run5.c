#include "global.h"

void ConnectToRanger_FormatNumber5Digits(u16 *dst, int value);

typedef void (*TaskFn)(void *);
extern u8 *ov74_0223D0A4;
void ov74_02229E14(void *);
void *Heap_Alloc(int, u32);
void MI_CpuFill8(void *, u8, u32);
void *SysTask_CreateOnMainQueue(TaskFn, void *, u32);
void ov74_02229CB8(void *);
void sub_0203778C(void *);
void ov74_02229B68(void *arg);

const u16 *PlayerProfile_GetNamePtr(void *);
int PlayerProfile_GetTrainerID_VisibleHalf(void *);
u16 ConnectToRanger_ConvertCharToRanger(u16);
extern u16 gConnectToRanger_PlayerName[];
extern u16 gConnectToRanger_TrainerId[];
void ConnectToRanger_EncodePlayerData(void *);

void Heap_Destroy(int);
void RegisterMainOverlay(const void *, const void *);
void OverlayManager_FreeData(void *);
extern const u32 SDK_OVERLAY_intro_title_ID;
extern const u8 gApplication_TitleScreen[];
int ConnectToRanger_Exit(void *);

void ConnectToRanger_SetTimeout(void *, int);

void *OverlayManager_GetData(void *);
void ov74_022359BC(void);
int WindowIsInUse(void *);
void ClearWindowTilemapAndCopyToVram(void *);
void RemoveWindow(void *);
void FreeBgTilemapBuffer(void *, int);
void Heap_Free(void *);
void ConnectToRanger_FreeGraphics(void *);

void ov74_02231008(void);
int ov74_02230FD4(void);
extern u8 gSystem[];
int ConnectToRanger_CheckTimeoutOrCancel(void *, int *, int, int);

void *ov74_022352D0(void *, int, u32, u32);
void *BgConfig_Alloc(int);
void *OverlayManager_GetArgs(void *);
void *Save_PlayerData_GetProfile(void *);
void *Save_Pokedex_Get(void *);
void *Save_PlayerData_GetOptionsAddr(void *);
void ov74_022352A0(int);
void *Save_MysteryGift_Get(void *);
void Heap_Create(int, int, u32);
void Sound_SetSceneAndPlayBGM(int, int, int);
int ConnectToRanger_Init(void *);

void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
void BgTilemapRectChangePalette(void *, int, int, int, int, int, int);
void BgCommitTilemapBufferToVram(void *, int);
void ConnectToRanger_LoadSubScreenBg(void *);

void ov74_02235414(void *, void *, int, int, int, int);
void ov74_02235464(void *, int, int, void *);
void ov74_02235568(void *, void *, int, int, int);
void EraseFrameAndWindow1(void *, int);
int ConnectToRanger_ShowPokedexRequiredMessage(void *);

void ov74_02229B68(void *arg) {
    if (ov74_0223D0A4 == NULL) {
        ov74_02229E14(arg);
        ov74_0223D0A4 = Heap_Alloc(15, 980);
        MI_CpuFill8(ov74_0223D0A4, 0, 980);
        *(u16 *)(ov74_0223D0A4 + 0x24) = 50;
        *(void **)(ov74_0223D0A4 + 0x1c) = SysTask_CreateOnMainQueue(ov74_02229CB8, NULL, 10);
        *(void **)ov74_0223D0A4 = arg;
        sub_0203778C(ov74_0223D0A4 + 4);
    }
}
