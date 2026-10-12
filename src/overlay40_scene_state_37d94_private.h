#ifndef TO45_OV40_R31_R4_PRIVATE_H
#define TO45_OV40_R31_R4_PRIVATE_H

#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))

void InitWindow(void *);
void AddWindowParameterized(void *, void *, int, int, int, int, int, int, u16);
void FillWindowPixelBuffer(void *, int);
void *NewString_ReadMsgData(void *, int);
int VsRecorder_GetCenteredTextX(void *, void *);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void String_Delete(void *);
void ScheduleWindowCopyToVram(void *);
void ClearWindowTilemapAndCopyToVram(void *);
void RemoveWindow(void *);
BOOL VsRecorder_IsNetIdle(void *);
void OrbitCursor_SetVisible(void *, int);
void OrbitCursor_SetActive(void *, int);
void OrbitCursor_SetRadius(void *, int, int);
void ov40_02237008(void *);
void ov40_02236FE0(void *);
void BgClearTilemapBufferAndCommit(void *, int);
void VsRecorder_ApplyBlend(void *, void *, int, int, int, int, int);
void VsRecorder_StepFade(void *, int);
BOOL VsRecorder_StepBlend(void *, void *, int, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void PaletteData_BlendPalettes(void *, int, int, u8, u16);
void VsRecorder_PlaceOrbitCursor(void *, void *, int, s16);
void VsRecorder_ShowTopMessage(void *, int);
void PlaySE(int);
void StopSE(int, int);
void *VsRecorder_GetNet(void *);
int GlobalTerminalNet_RequestBoxDownload(void *, void *);
BOOL GlobalTerminalNet_GetError(void *, void **);
void VsRecorder_HideTopMessage(void *);
void VsRecorder_ShowMessageScreen(void *, int, u32, u32);
void VsRecorder_SetVBlankPaused(void *, int);
void ov40_02236EB4(void *);
BOOL System_GetTouchNew(void);
void ov40_0222C710(void *, int);
void ov40_02237030(void *, int);
void ov40_02236F38(void *);
void VsRecorder_SetSubState(void *, int);
void ov40_02237C9C(void *);
void ov40_02238290(void *);
void VsRecorder_CreateTouchScrollSpritesWithAnim(void *, void *, int);
void VsRecorder_ClearListWindow(void *);
void VsRecorder_InitPairList(void *, void *, void *, void *);
void VsRecorder_TouchScrollSetEnabled(void *, void *);
void VsRecorder_CreateListArrows(void *, void *, int);
void VsRecorder_SetListArrowsY(void *, int, int);
void ov40_02237AC0(void *);

typedef struct Copy44 {
    u32 words[11];
} Copy44;

typedef struct Ov40Inner {
    u8 pad_0000[0x1A4];
    u8 transA[4];
    u8 transB[4];
    u32 blend;
    u8 pad_01B0[0x1D8];
    u32 count;
    u8 pad_038C[0x2A80];
    u8 strings[0xA0];
    Copy44 copied;
} Ov40Inner;

extern const Copy44 ov40_02245310;
extern const u8 ov40_02245268[][4];

void ov40_02237C9C(void *);
void ov40_02237D6C(void *);
int ov40_02237D94(void *);

#endif
