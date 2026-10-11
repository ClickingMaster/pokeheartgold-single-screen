#ifndef TO45_OV40_R31_R6_PRIVATE_H
#define TO45_OV40_R31_R6_PRIVATE_H

#include "global.h"

#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov40R31R6Work {
    u8 pad0000[0x1A4];
    u32 fadeA;
    u32 fadeB;
    u8 pad01AC[0x2C60];
    u8 strings[0xA0];
    u8 pad02EAC[0x2C];
    u8 panel[0x8C];
    u32 flag;
} Ov40R31R6Work;

void VsRecorder_ApplyBlend(void *, void *, u32, u32, u32, u32, u32);
u32 VsRecorder_StepBlend(void *, void *, u32, u32);
void VsRecorder_SetVBlankPaused(void *, u32);
void ov40_02237564(void *);
void ov40_02237410(void *);
void ov40_022371A0(void *);
void ov40_02237008(void *);
void ov40_02237C54(void *);
void ov40_02237C9C(void *);
void ov40_02237AC0(void *);
void ov40_02237D6C(void *);
void ov40_02236184(void *, void *);
void ov40_022361B0(void *);
void VsRecorder_FreeTouchScroll(void *);
void VsRecorder_RemoveListWindow(void *);
void VsRecorder_FreeListArrows(void *, void *);
void VsRecorder_CreateProfile(void *, void *);
void VsRecorder_FreeProfile(void *, void *);
void VsRecorder_SetProfileDrawFlag(void *, u32);
void VsRecorder_CreateTouchScrollSpritesWithAnim(void *, void *, u32);
void VsRecorder_InitPairList(void *, void *, void *, void *);
void VsRecorder_TouchScrollSetEnabled(void *, void *);
void VsRecorder_CreateListArrows(void *, void *, u32);
void VsRecorder_SetListArrowsY(void *, u32, u32);
void VsRecorder_PlaceOrbitCursor(void *, void *, u32, s16);
void VsRecorder_UpdateTouchScroll(void *);
void VsRecorder_ListMoveCursor(void *, s16);
void VsRecorder_RedrawPairList(void *, void *, void *);
void ov40_02237B7C(void *, u32);
void VsRecorder_SetSubState(void *, u32);
void BgClearTilemapBufferAndCommit(void *, u32);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, u32, void *, u32, u32, u32, u32, u32);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, u32, void *, u32, u32, u32, u32, u32);
void GfGfx_EngineATogglePlanes(u32, u32);
void GfGfx_EngineBTogglePlanes(u32, u32);
void OrbitCursor_SetVisible(void *, u32);
void OrbitCursor_SetRadius(void *, u32, u32);
void OrbitCursor_MoveTarget(void *, u32, s16);
u32 ov40_02238838(void *);
u32 ov40_02238A50(void *);
u32 ov40_02238BB0(void *);

#endif
