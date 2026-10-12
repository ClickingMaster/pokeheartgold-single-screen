#ifndef OVERLAY40_TRAINER_CARD_PAGES_H
#define OVERLAY40_TRAINER_CARD_PAGES_H

#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define HALF(p, off) (*(u16 *)((u8 *)(p) + (off)))
#define BYTE(p, off) (*(u8 *)((u8 *)(p) + (off)))

#define BLEND(p, w, a, b) \
    PaletteData_BlendPalettes(PTR(p, 0x28), a, b, (u8)WORD(w, 8), (u16)WORD(p, 0x58))

void PaletteData_BlendPalettes(void *palette, int a, int b, int coeff, int color);
void BgClearTilemapBufferAndCommit(void *bgConfig, int bgId);
void GfGfx_EngineATogglePlanes(int mask, int enable);
void GfGfx_EngineBTogglePlanes(int mask, int enable);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *narc, int memberNo, void *bgConfig, int layer, int tileStart, int szByte, int isCompressed, int heapId);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *narc, int memberNo, void *bgConfig, int layer, int tileStart, int szByte, int isCompressed, int heapId);
void ManagedSprite_SetAnim(void *sprite, int anim);
void TouchHitboxController_Destroy(void *controller);
void BeginNormalPaletteFade(int a, int b, int c, int d, int e, int f, int g);
BOOL IsPaletteFadeFinished(void);
void PlaySE(u16 seqNo);
void StopSE(int seqNo, int a);
BOOL System_GetTouchNew(void);
void Heap_Free(void *ptr);

void *BattleRecord_GetHeader(void);
u64 BattleRecordHeader_GetField(void *a, int b, int c);
void WirelessStatusIcon_ShowOnBothScreens(int a, int b);
int BattleRecord_Exists(void);
void BattleRecord_Delete(void);
void OrbitCursor_SetVisible(void *p, int a);
void OrbitCursor_SetRadius(void *p, int a, int b);
void sub_02087A84(void *p, int a, int b);

int ov39_0222774C(void *p, u32 lo, u32 hi);
int GlobalTerminalNet_GetError(void *p, u32 *out);
u64 GlobalTerminalNet_GetUploadedVideoNumber(void *p);
void GlobalTerminalNet_GetDownloadedVideo(void *p, void *out);

void VsRecorder_SetScreenIfDone(void *p, u32 value, BOOL valid, u32 *out);
void VsRecorder_SetSubState(void *p, int value);
void VsRecorder_SetTextSpriteMsg(void *dst, void *src, int a);
void VsRecorder_RebuildMenuRowsParent(void *p);
void VsRecorder_RebuildMenuRows(void *p);
int VsRecorder_StepBlend(int *a, int *b, int c, int d);
int VsRecorder_StepFade(int *p, int direction);
void VsRecorder_ResetFade(void *p);
void VsRecorder_ClearPaletteBlend(void *p);
void VsRecorder_RemoveTopMessage(void *p);
void VsRecorder_SetResumeScreen(void *p, u32 value);
void VsRecorder_CreateProfile(void *p, void *q);
void VsRecorder_FreeProfile(void *p, void *q);
void VsRecorder_SetProfileDrawFlag(void *p, BOOL draw);
void VsRecorder_ClearListWindow(void *p);
void VsRecorder_ImportBattleRecord(void *p);
void VsRecorder_StartFlipScreens(void *p, int a);
void *VsRecorder_IsFlipScreensDone(void *p);
int VsRecorder_StepRecordSave(void *p, u32 a, u32 b);
void VsRecorder_LoadSavedVideos(void *p);
void VsRecorder_FreeSavedVideos(void *p);
void VsRecorder_CreateVideoInfo(void *p, void *q);
void VsRecorder_FreeVideoInfo(void *p, void *q);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *p, BOOL draw);
void VsRecorder_ResetRecordSave(void *p);
void VsRecorder_PlaceOrbitCursor(void *p, void *q, int a, int b);
void VsRecorder_LoadScreen(void *p, int a, int b);
int ov40_02230410(void *p);
void VsRecorder_SetVBlankPaused(void *p, void *value);
void *VsRecorder_GetNet(void *p);
BOOL VsRecorder_IsNetIdle(void *p);

void ov40_02240910(void *p);
void ov40_02240E28(void *p, int a, int b);
void ov40_02240F24(void *p, int a, int b);
void ov40_02241054(void *p);
void ov40_02241114(void *p);
void ov40_02241A34(void *p);
void ov40_02241A54(void *p, int a);
void ov40_02241AB0(void *p);
void ov40_02241AFC(void *p);
void ov40_02241BDC(void *p);
/* --- round 6 additions --- */
void Sprite_DeleteAndFreeResources(void *sprite);
void ManagedSprite_SetPositionXY(void *sprite, s16 x, s16 y);
void ManagedSprite_SetDrawFlag(void *sprite, int draw);
void TextOBJ_SetSpritesDrawFlag(void *textOBJ, int draw);
void TextOBJ_SetPosition(void *p, s32 a, s32 b);
u32 FrontierCard_GetCountry(void *card);
u32 FrontierCard_GetRegion(void *card);

void VsRecorder_CreateTextSprite(void *dst, void *src, int a);
void VsRecorder_FreeTextSprite(void *p);
void ov40_0222D6EC(void *p, int a);
void VsRecorder_FreeButtonGfx(void *p);
void *VsRecorder_NewButtonSprite(void *p, int a);
void VsRecorder_SetMessageText(void *p, int a);
void VsRecorder_OnTouchConfirm(void *p);
int ov40_0223DB94(void *p);
void ov40_0223DDE8(void *p, u32 a, u32 b);
void ov40_0223DEB8(void *p);
int ov40_0223EBB8(void *p, u32 a);
void ov40_0223EC40(void *p, u32 a);

typedef void (*HitboxCallback)(u32 a, int b, void *p);
extern const u8 ov40_022456B4[];
extern const u8 ov40_02245674[];
extern const u8 ov40_02245654[];
void SetBgPriority(int bgId, int priority);
void *TouchHitboxController_Create(const void *hitboxes, int count, HitboxCallback cb, void *param, int heapId);
void VsRecorder_ClearResumeState(void *p);
void VsRecorder_InitBgBlend(int *a, int *b, int c);
void ov40_0224085C(void *p);
int ov40_02241488(void *p);
void VsRecorder_MenuPop(void *p);
void VsRecorder_ShowTopMessage(void *p, int a);
void VsRecorder_HideTopMessage(void *p);
void VsRecorder_AddGameStatsScore(void *p, int a);
void VsRecorder_ShowMessageScreen(void *p, int a, u32 b, u32 c);
void VsRecorder_PrintVideoNumber(void *w, void *p);
int GlobalTerminalNet_RequestVideoUpload(void *a, void *b);
BOOL BattleRecord_Load(void *a, int b, int *c, void *d, int e);
void *FrontierCard_New(int a);
void FrontierCard_Delete(void *a);
void FrontierCard_InitFromSave(void *a, void *b);
int ov40_022417DC(void *p);
int ov40_022417A4(void *p);
int ov40_02241948(void *p);
void InitWindow(void *window);
void AddWindowParameterized(void *bgConfig, void *window, int bgId, int x, int y, int width, int height, int palette, int baseTile);
void FillWindowPixelBuffer(void *window, int fillValue);
void AddTextPrinterParameterizedWithColor(void *window, int fontId, void *str, int x, int y, int speed, u32 color, void *callback);
void ScheduleWindowCopyToVram(void *window);
void ClearWindowTilemapAndCopyToVram(void *window);
void RemoveWindow(void *window);
void String_Delete(void *str);
void *NewString_ReadMsgData(void *msgData, int msgId);
int VsRecorder_GetCenteredTextX(void *window, void *str);

void ov40_02241C10(u32 a, int b, void *p);
void ov40_02241C70(u32 a, int b, void *p);
void ov40_02241CD8(u32 a, int b, void *p);
void ov40_02241D10(u32 a, int b, void *p);
void ov40_02241E14(u32 a, int b, void *p);
void ov40_02241E40(u32 a, int b, void *p);
void ov40_02241ED4(u32 a, int b, void *p);
void ov40_02241F3C(u32 a, int b, void *p);
void ov40_02241F74(void *p, int a);
void ov40_02241FD0(void *p);
void ov40_02242084(void *p);
void ov40_022420B4(void *p, int a);
void ov40_02242110(void *p);
void ov40_022421FC(void *p);

BOOL ov40_0224222C(void *p);
BOOL ov40_02242378(void *p);
BOOL ov40_02242490(void *p);
BOOL ov40_0224253C(void *p);
int ov40_022425E8(void *p);
BOOL ov40_022428D4(void *p);
BOOL ov40_02242AEC(void *p);
BOOL ov40_02242CFC(void *p);
void ov40_02242DF0(void *p, u32 *flag);
void ov40_02242E14(void *p, u32 *flag);

#endif
