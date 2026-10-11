#ifndef OV01_022051EC_PRIVATE_H
#define OV01_022051EC_PRIVATE_H
#include "global.h"
typedef struct Vec3 {
    s32 x, y, z;
} Vec3;
typedef struct SpawnArgs {
    void *ctx;
    void *renderer;
} SpawnArgs;
extern void *FieldEffectManager_HeapAllocAndFill(u32, u32, u32, u32);
extern void FieldEffectManager_HeapFree(void *);
extern void HeapExp_FndInitAllocator(void *, u32, u32);
extern void Field3dModel_LoadFromFilesystem(void *, u32, u32, u32);
extern void Field3dModelAnimation_LoadFromFilesystem(void *, void *, u32, u32, u32, void *);
extern void Field3dObject_InitFromModel(void *, void *);
extern void Field3dObject_AddAnimation(void *, void *);
extern void Field3dModelAnimation_Unload(void *, void *);
extern void Field3dModel_Unload(void *);
extern void Field3dObject_SetActiveFlag(void *, BOOL);
extern void *FieldEffectManager_GetRendererData(void *, u32);
extern void FieldEffectManager_CreateEffect(void *, const void *, void *, u32, SpawnArgs *, u32);
extern const u8 ov01_022096CC[];
extern void *FieldEffect_GetState(void *);
extern void *FieldEffect_GetUserData(void *);
extern void FieldEffect_GetPosition(void *, Vec3 *);
extern void Field3dObject_SetPosEx(void *, s32, s32, s32);
extern void PlaySE(u32);
extern void Field3dObject_Draw(void *);
extern BOOL Field3dModelAnimation_FrameAdvanceAndCheck(void *, s32);
extern void Field3dModelAnimation_FrameSet(void *, s32);
void *ov01_022051EC(u32);
void ov01_02205208(void *);
void ov01_02205218(void *);
void ov01_022052A4(void *);
void ov01_022052C4(void *, void *);
void *ov01_022052F4(void *);
BOOL ov01_02205300(void *, void *);
void ov01_02205350(void);
void ov01_02205354(void *, void *);
void ov01_02205364(void *, void *);
BOOL ov01_02205388(void *, u32);
void ov01_022053C4(void *, u32, s32);
#endif
