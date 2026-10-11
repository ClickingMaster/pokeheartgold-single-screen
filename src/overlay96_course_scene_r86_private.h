#ifndef POKEHEARTGOLD_OVERLAY96_COURSE_SCENE_R86_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY96_COURSE_SCENE_R86_PRIVATE_H
#include "global.h"

typedef struct GfxBanks40 {
    u32 words[10];
} GfxBanks40;
extern const GfxBanks40 ov96_0221CB9C;
extern u8 gSystem[];
void *PokeathlonCourse_GetGraphicsSystem(void *);
void *PokeathlonCourse_GetHeapAllocPtr4(void *);
void *PokeathlonCourse_GetDataCopyArea(void *);
u16 PokeathlonCourse_GetField5F0_AtIndex(void *, u8);
void PokeathlonCourse_SetField5E0_AtIndex(void *, u8, u16);
void PokeathlonCourse_ResetField3A4(void *);
void PokeathlonCourse_FreePtr4HeapAlloc(void *);
void PokeathlonCourse_SetStateField07(void *, int);
BOOL ov96_021E5F24(void *);
BOOL PokeathlonCourse_RunStartCountdown(void *);
BOOL PokeathlonCourse_RunFinish(void *);
void PokeathlonCourseScoreList_Init(void *);
void PokeathlonCourseScoreList_AddPointsToTotals(void *);
void PokeathlonCourseScoreList_SaveEventPoints(void *, int);
void PokeathlonCourse_AddScoreEntry(void *, void *, int, u16, int, int, int);
void PokeathlonCourseScoreList_Rank(void *, int);
void ov96_0221A56C(void *, int);
void WirelessStatusIcon_Destroy(void);
void Main_SetVBlankIntrCB(void *, void *);
void Main_SetHBlankIntrCB(void *, void *);
void FreeBgTilemapBuffer(void *, int);
void RemoveWindow(void *);
void Heap_Free(void *);
void PokeathlonCourseSpritePool_Free(void *);
void PokeathlonCourseMonObjects_Free(void *);
void PokeathlonCourseSprites_FreeResources(void *);
void OamManager_Free(void);
void ObjCharTransfer_Destroy(void);
void ObjPlttTransfer_Destroy(void);
void PokeathlonPennantCaptureCpu_Delete(void *);
void PokeathlonPennantCaptureDisplay_Delete(void *);
void FontID_Release(int);
void GfGfx_SwapDisplay(void);
void Heap_Destroy(int);
void GfGfx_SetBanks(const GfxBanks40 *);
void PokeathlonCourseSpritePool_LoadChar(void *, int, int, int, int);
void PokeathlonCourseSpritePool_LoadPltt(void *, int, int, int, int, int);
void PokeathlonCourseSpritePool_LoadCell(void *, int, int, int);
void PokeathlonCourseSpritePool_LoadAnim(void *, int, int, int);
void BeginNormalPaletteFade(int, int, int, int, int, int, void *);
BOOL IsPaletteFadeFinished(void);
u8 *ov96_021E8A20(void *ptr);
void ScheduleSetBgPosText(void *, int, int, int);
void PokeathlonPennantCapture_ApplyFrame(void *, int, void *, void *);
void PokeathlonPennantCapture_UpdateObstacleSprites(void *, int, int);
void PokeathlonPennantCapture_UpdateFieldItemSprites(void *, int, int);
void PokeathlonPennantCapture_UpdatePennantSprites(void *, void *, int, int);
void PokeathlonPennantCapture_AnimateBg(void *);
void PokeathlonPennantCaptureDisplay_SetTimer(void *, u16);
void PokeathlonCourse_UpdateTimeUpCountdown(void *, u16);
void GF_AssertFail(void);
void PokeathlonCourseObject_GroundToCenter(void *, int, int, int *, int *);
BOOL PokeathlonCourseObject_IsInTouchRange(void *, int, int, int, int);

int ov96_02204DF8(void *);
int ov96_02204E58(void *);
void ov96_02204F20(void);
void PokeathlonPennantCapture_LoadSpriteResources(void *);
int PokeathlonPennantCapture_State_Play(void *, u8 *);
int PokeathlonPennantCapture_State_End(void *, u8 *);
void PokeathlonPennantCapture_ShowFrame(void *);
BOOL PokeathlonPennantCapture_IsTouchOnOwnMon(u8 *, int, int, int);
void PokeathlonPennantCapture_HostStep(void *);
BOOL PokeathlonPennantCapture_SendInput(void *);

#endif
