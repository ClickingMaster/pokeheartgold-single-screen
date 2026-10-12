#ifndef POKEHEARTGOLD_OVERLAY02_ROAMER_POKECENTER_R14_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY02_ROAMER_POKECENTER_R14_PRIVATE_H
#include <nnsys.h>

#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
void HeapExp_FndInitAllocator(void *, u32, u32);
void *AllocAtEndAndReadWholeNarcMemberByIdPair(u32, u32, u32);
void Field3dModel_LoadFromData(void *, void *);
void Field3dModel_UnloadKeepData(void *);
void FieldEffectManager_HeapFree(void *);
void Field3dObject_InitFromModel(void *, void *);
void Field3dModelAnimation_LoadFromFilesystem(void *, void *, u32, u32, u32, void *);
void Field3dObject_AddAnimation(void *, void *);
void Field3dModelAnimation_Unload(void *, void *);
void MapObject_SetVisible(void *, u32);
void sub_0205F484(void *);
void ov02_0224B784(void *);
void *FieldEffect_GetState(void *);
void FieldEffect_Destroy(void *);
void *Save_Roamers_Get(void *);
u8 GetRoamerIsActiveByIndex(void *, u8);
u32 Roamer_GetLocation(void *, u8);
u32 GetRoamMapByLocationIdx(u32);
void RoamerLocationUpdateRand(void *, u8);
void *Roamers_GetRoamMonStats(void *, u8);
u32 GetRoamerData(void *, u32);
void *RoamerSave_GetRepelAddr(void *);
void *Party_GetMonByIndex(void *, u32);
u32 GetMonData(void *, u32, void *);
u32 SpeciesToRoamerIdx(u16);
void RoamerMon_Init(void **);
void *Save_VarsFlags_Get(void *);
void sub_02066BE8(void *, u32, u32);
void SetRoamerData(void *, u32, u32);
u32 LCRandom(void);
void StartMapSceneScript(void *, u32, void *);
void *Heap_AllocAtEnd(u32, u32);
void GF_AssertFail(void);
BOOL FieldSystem_FindMapProp(void *, int, void **, int *);
u32 MapMatrix_GetWidth(void *);
void Terrain_GetChunkOrigin(int, int, VecFx32 *);
void MapProp_GetTranslation(VecFx32 *, void *);
void TaskManager_Call(void *, void *, void *);
BOOL PokecenterAnimRun(void *);
BOOL ElevatorAnimRun(void *);
BOOL FieldSystem_FindMapPropOfModels(void *, int *, u32, void **, int *);
void *AreaDataManager_GetMapPropTexture(void *);
void *MapProp_GetRenderSurface(void *);
void *MapProp_GetResModel(void *);
void MapPropOneShotAnimationManager_LoadPropAnimations(void *, void *, u32, u32, void *, void *, void *, u32, u32, u32);
extern volatile const int ov02_022536E8[2];
void ov02_0224B88C(void *);
void ov02_0224B90C(void *);
BOOL ov02_0224B938(void *);
BOOL ov02_0224B964(void *);
void ov02_BattleExit_HandleRoamerAction(void *, void *);
void ov02_RepelActiveRoamersFromMapNo(void *, u32);
void *ov02_0224BAA8(void *, u32);
BOOL PlayerStepEvent_RepelCounterDecrement(void *, void *);
void PokecenterAnimCreate(void *, u8);
void ElevatorAnimCreate(void *, u8, u8);
void ov02_0224BF58(void *, u8);
#endif
