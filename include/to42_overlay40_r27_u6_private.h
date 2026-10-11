#ifndef OV40_R27_U6_PRIVATE_H
#define OV40_R27_U6_PRIVATE_H

#include "global.h"
#include "sys_task_api.h"

#define PTR(p, off)   (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off)  (*(u32 *)((u8 *)(p) + (off)))
#define HALF(p, off)  (*(u16 *)((u8 *)(p) + (off)))
#define SHALF(p, off) (*(s16 *)((u8 *)(p) + (off)))
#define S32(p, off)   (*(s32 *)((u8 *)(p) + (off)))
#define BYTE(p, off)  (*(u8 *)((u8 *)(p) + (off)))

typedef SysTaskFunc Ov40TaskFn;

void *Heap_Alloc(u32 heapId, u32 size);
void Heap_Free(void *ptr);
void MI_CpuFill8(void *dest, u8 value, u32 size);
void PaletteData_BlendPalettes(void *data, int buffer, u16 selected, u8 amount, u16 color);
void SetBgPriority(u8 bgId, u16 priority);
void BgClearTilemapBufferAndCommit(void *bgConfig, u32 layer);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *narc, u32 memberNo, void *bgConfig, u32 layer, u32 tileStart, u32 szByte, u32 isCompressed, u32 heapId);
void GfGfx_EngineATogglePlanes(int planes, int enable);
void GfGfx_EngineBTogglePlanes(int planes, int enable);
void ManagedSprite_SetAnim(void *sprite, int anim);
void ManagedSprite_SetDrawFlag(void *sprite, BOOL draw);
void ManagedSprite_SetPaletteOverrideOffset(void *sprite, int offset);
void TextOBJ_SetSpritesDrawFlag(void *obj, BOOL draw);
void *TouchHitboxController_Create(const void *hitboxes, int count, const void *callbacks, void *arg, u32 heapId);
void TextOBJ_SetPosition(void *p, int x, int y);
void OrbitCursor_SetTarget(void *p, s16 x, s16 y);
u64 BattleRecordHeader_GetField(void *summary, int field, int index);

void VsRecorder_SetSubState(void *p, int value);
BOOL VsRecorder_MenuEnterChild(void *p);
BOOL VsRecorder_MenuRestore(void *p);
void VsRecorder_MenuRunAction(void *p);
int VsRecorder_HasResumeScreen(void *p);
void VsRecorder_SetSpritePos(void *sprite, s16 x, s16 y);
void VsRecorder_GetSpritePos(void *sprite, s16 *x, s16 *y);
void VsRecorder_SetMenuRowText(void *work, void *entry, int msgId, void *selection);
void VsRecorder_FreeMenuRows(void *p);
void VsRecorder_InitBgBlend(void *a0, void *a1, int a2);
BOOL VsRecorder_StepBlend(void *a0, void *a1, int a2, int a3);
BOOL VsRecorder_StepFade(void *p, int value);
u32 VsRecorder_GetBlendColor(void *p);
void VsRecorder_ReloadSchemePalettes(void *p, u32 value);
void VsRecorder_StartFlipScreens(void *p, int value);
int ov40_02230410(void *p);
void VsRecorder_CreateVideoInfo(void *p, void *a1);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *p, BOOL draw);
void VsRecorder_LoadScreen(void *p, int a1, int a2);
void VsRecorder_SetVBlankPaused(void *p, int value);
void ov40_02232F50(void *p);
void ov40_02232F88(void *p);
void ov40_02233044(void *p);

extern const u8 ov40_02245134[];
extern const u8 ov40_02232ED4[];

void VsRecorder_RowMoveTask(SysTask *task, void *arg);
int VsRecorder_MenuSelect(void *p);
int VsRecorder_MenuBack(void *p);
int VsRecorder_MenuPrevScheme(void *p);
int ov40_02232094(void *p);
int ov40_02232288(void *p);
int ov40_022322E0(void *p);
int ov40_02232470(void *p);

#endif
