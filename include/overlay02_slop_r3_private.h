#ifndef POKEHEARTGOLD_OVERLAY02_SLOP_R3_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY02_SLOP_R3_PRIVATE_H
#define OV02_02248728_OWN_DECLS

// map_prop.h (via field_system.h) declares MapPropManager_LoadOne as void-return, but
// this TU was matched consuming r0 (real return, non-void). Shadow the header decl
// so the int-return extern below stands without an indirect-call cast.
#define MapPropManager_LoadOne MapPropManager_LoadOne_UpstreamVoidDecl

// The map-prop animation/manager API has named declarations published in
// map_prop_animation.h / map_prop.h (both visible here via field_system.h). This TU was
// matched against the untyped `void *` view below, so shadow every upstream decl we
// consume and keep our own -- see the IPA header discipline note in CLAUDE.md.
#define MapPropOneShotAnimationManager_LoadPropAnimations      MapPropOneShotAnimationManager_LoadPropAnimations_UpstreamDecl
#define MapPropOneShotAnimationManager_SetAnimationRenderObj   MapPropOneShotAnimationManager_SetAnimationRenderObj_UpstreamDecl
#define MapPropOneShotAnimationManager_PlayAnimation           MapPropOneShotAnimationManager_PlayAnimation_UpstreamDecl
#define MapPropOneShotAnimationManager_UnloadAnimation         MapPropOneShotAnimationManager_UnloadAnimation_UpstreamDecl
#define MapPropOneShotAnimationManager_IsAnimationLoopFinished MapPropOneShotAnimationManager_IsAnimationLoopFinished_UpstreamDecl
#define MapPropManager_RemoveMapPropByIndex                    MapPropManager_RemoveMapPropByIndex_UpstreamDecl
#define MapPropManager_GetMapPropByIndex_Checked_RequireActive MapPropManager_GetMapPropByIndex_Checked_RequireActive_UpstreamDecl
#define MapProp_GetTranslation                                 MapProp_GetTranslation_UpstreamDecl
#define MapProp_GetRenderSurface                               MapProp_GetRenderSurface_UpstreamDecl
#define MapProp_GetResModel                                    MapProp_GetResModel_UpstreamDecl

// map_prop.h also drags in safari_zone.h, which this TU deliberately did not include --
// it was matched against opaque `void *` safari handles and an int-returning sub_0202F620.
// Same treatment: shadow the typed upstream decls, keep ours.
#define sub_0202F620                    sub_0202F620_UpstreamDecl
#define Save_SafariZone_Get             Save_SafariZone_Get_UpstreamDecl
#define SafariZone_GetAreaSet           SafariZone_GetAreaSet_UpstreamDecl
#define SafariZone_GetObjectUnlockLevel SafariZone_GetObjectUnlockLevel_UpstreamDecl

#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"
#include "nnsys/g3d/binres/res_struct_accessor_inline.h"

#include "bg_window.h"
#include "field_roamer.h"
#include "field_system.h"
#include "field_warp_tasks.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "roamer.h"
#include "save_local_field_data.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "task.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "map_object_movement_commands.h"

// Headers are in; drop the shadowing so the externs below bind the real symbols with
// this TU's matching-time signatures.
#undef MapPropOneShotAnimationManager_LoadPropAnimations
#undef MapPropOneShotAnimationManager_SetAnimationRenderObj
#undef MapPropOneShotAnimationManager_PlayAnimation
#undef MapPropOneShotAnimationManager_UnloadAnimation
#undef MapPropOneShotAnimationManager_IsAnimationLoopFinished
#undef MapPropManager_RemoveMapPropByIndex
#undef MapPropManager_GetMapPropByIndex_Checked_RequireActive
#undef MapProp_GetTranslation
#undef MapProp_GetRenderSurface
#undef MapProp_GetResModel
#undef sub_0202F620
#undef Save_SafariZone_Get
#undef SafariZone_GetAreaSet
#undef SafariZone_GetObjectUnlockLevel

// WIP_LOCAL marks a function that is file-local in the original (-> `static` in
// the final) but is left global during WIP so it survives dead-code elimination
// for objdiff. See VISIBILITY NOTE above. Finalize to `static` at flip-to-src.
#define WIP_LOCAL /* static */

// 8-byte pair used for the FieldEffect_GetUserData struct-copy getters (name TBD).
typedef struct UnkPair8 {
    s32 unk0;
    s32 unk4;
} UnkPair8;

// 16-byte blob copied via two ldm/stm word-pairs (FieldEffect_GetUserData -> obj+0x58).
typedef struct UnkBlob16 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} UnkBlob16;

// 24-byte blob copied via three ldm/stm word-pairs (FieldEffect_GetUserData -> work+0xc).
typedef struct UnkBlob24 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} UnkBlob24;

// AnimManager per-resource entry (arrays at mgr->0x144 / 0x148, stride 8).
typedef struct AnimResEntry {
    s16 id;
    s16 unk2;
    SpriteResource *res;
} AnimResEntry;

// follow-mon work byte at +0x86c: low nibble is an 8-byte-stride array index.
typedef struct ov02_FollowMonStep {
    u8 idx : 4;
    u8 hi : 4;
} ov02_FollowMonStep;

struct ov02_PokeathlonStarBits {
    u16 s0 : 3;
    u16 s1 : 3;
    u16 s2 : 3;
    u16 s3 : 3;
    u16 s4 : 3;
};

// FieldMoveCutIn_CreateScrollSprites per-entry rodata (ov02_022535E4, stride 0x14).
typedef struct ov02_A9D8Entry {
    fx32 unk0;
    fx32 unk4;
    fx32 unk8;
    void *unkC;
    u32 unk10;
} ov02_A9D8Entry;

// follow-mon task-data accessor (defined in unk_020689C8.c); local extern matches
// the convention used by other overlays that don't pull in the full header.
extern void *FieldEffect_GetState(void *work);
extern void FieldEffect_Destroy(int a0);                                                // unk_020689C8.h
extern void MapPropOneShotAnimationManager_PlayAnimation(void *a, int b, int c); // no header yet
extern BOOL FollowMon_StartFlash(FieldSystem *fieldSystem, u8 a1, u8 a2);
extern BOOL FollowMon_StartRecall(FieldSystem *fieldSystem); // no header included here
extern void FieldEffectPool_Delete(void *a0);                  // unk_020689C8.h

// Nested state-machine dispatch: outer table indexed by sm[0] -> inner func table
// indexed by sm[1]; each func(sm) returns 1 to keep running. Tables are file-local
// rodata defined later (extern here so the runners' relocations resolve by name).
typedef BOOL (*ov02_StateMachineFunc)(void *sm);

// Field-move task state tables: func(taskManager, fieldSystem, env) -> 1=loop, 2=free.
typedef int (*ov02_FieldTaskFunc)(TaskManager *taskManager, FieldSystem *fieldSystem, void *env);
extern BOOL Task_WaitFollowMonMovement(TaskManager *taskMan);                                                             // field/field_follow_mon.h, not included
extern void FieldEffectPool_DrawAll(void *a0);                                                                          // unk_020689C8.h
extern void FieldCameraZoom_End(SysTask *task);                                                                    // no header included here
extern BOOL FieldCameraZoom_IsDone(SysTask *task);                                                                    // no header included here
extern void FollowMonBillboard_SetHeight(LocalMapObject *object, int a1);                                                   // no header included here
extern void FollowMonBillboard_SetShadowAnchored(LocalMapObject *object, int a1);                                                   // no header included here
extern void *Save_VarsFlags_Get(SaveData *saveData);                                                         // save_vars_flags.h, not included
extern BOOL Save_VarsFlags_CheckSafariSysFlag(void *varsFlags);                                              // save_vars_flags.h, not included
extern BOOL sub_0202F620(void *safariZone);                                                                  // no header included here
extern fx32 Terrain_GetHeight_Bdhc(FieldSystem *fieldSystem, fx32 refHeight, fx32 xFx32, fx32 zFx32, u8 *outSelector); // unk_02054648.h, not included
WIP_LOCAL BOOL FieldSystem_CanPlaceSafariObject(FieldSystem *fieldSystem);
// --- Task_FollowMonInteract (0x02250110) deps ---
extern void ClearFrameAndWindow2(Window *window, BOOL dont_copy_to_vram);                                                   // render_window.h, not included
extern void RemoveWindow(Window *window);                                                                                   // render_window.h, not included
extern void *Save_FashionData_Get(SaveData *saveData);                                                                      // fashion_case.h, not included
extern void *Save_FashionData_GetFashionCase(void *fashionData);                                                            // fashion_case.h, not included
extern int FashionCase_CanGiveAccessory(void *fashionCase, int a1, int a2);                                                                 // fashion_case.h, not included
extern void FashionCase_GiveFashionItem(void *fashionCase, int id, int quantity);                                           // fashion_case.h, not included
extern void SetFlag99C(void *state);                                                                                        // sys_flags.h, not included
extern BOOL DialogBox_IsPrintFinished(u8 printerId);                                                                        // text_0205B4EC.h, not included
extern void FieldSystem_SwitchBottomScreenApp(FieldSystem *fieldSystem, int a1, void *a2);                                                      // overlay_01.h, not included
extern int FieldSystem_GetBottomScreenAppId(FieldSystem *fieldSystem);                                                                         // overlay_01.h, not included
extern BOOL FieldSystem_GetBottomScreenAppState(FieldSystem *fieldSystem);                                                                        // overlay_01.h, not included
extern void FieldSystem_SetTouchscreenChoiceMenuState(FieldSystem *fieldSystem, int a1, int a2, void *a3);                                              // overlay_01.h, not included
extern int FieldSystem_GetTouchscreenChoiceMenuState(FieldSystem *fieldSystem);                                                                         // overlay_01.h, not included
WIP_LOCAL int FollowMon_SelectInteraction(FieldSystem *fieldSystem);                                                                      // still in asm
WIP_LOCAL int FollowMonInteraction_Run(FieldSystem *fieldSystem, void *a1);                                                            // still in asm
extern void sub_02066BE8(void *state, u32 a1, u16 value);                                                                   // sys_vars.h, not included
extern void Field3dModel_LoadFromData(Field3dModel *model, void *narcData);                                                             // no header included here
extern void Field3dModel_UnloadKeepData(Field3dModel *model);                                                                             // no header included here
extern void Field3dModelAnimation_LoadFromData(Field3DModelAnimation *anim, Field3dModel *model, void *anmResource, NNSFndAllocator *allocator); // no header included here
extern void *FieldCameraZoom_Start(FieldSystem *fieldSystem, int a1);                                                               // no header included here
extern void FieldCameraZoom_Set(void *a0, int a1, fx32 a2, int a3);                                                               // no header included here
extern void *Save_SafariZone_Get(SaveData *saveData);                                                                       // safari_zone.h, not included
extern void *SafariZone_GetAreaSet(void *safari_zone, int area_set_no);                                                     // safari_zone.h, not included
extern int SafariZone_FindSpotFor2x2Object(void *a0, int a1, int a2, int a3, fx32 a4, u16 *a5, u16 *a6, int a7);                              // still in asm
// --- NONMATCHING-finalization deps (asm-only references) ---
extern void _s32_div_f(void);                 // int-division runtime
extern void _fadd(void);                      // soft-float runtime (asm bl)
extern void _fsub(void);                      // soft-float runtime
extern void _fflt(void);                      // soft-float runtime
extern void _ffix(void);                      // soft-float runtime
extern void Field3dObject_SetXRotation(void); // not in any included header (asm-only ref)
extern void FieldSystem_CheckTileCollision(void);               // not in any included header (asm-only ref)
WIP_LOCAL void WaterfallEffect_Update(void *a0, void *a1, void *a2);
extern u16 GF_DegreeToSinCosIdxNoWrap(u16 deg);                           // math_util.h, not included
extern BOOL Save_VarsFlags_CheckFlagInArray(void *varsFlags, u16 flagId); // save_vars_flags.h, not included
WIP_LOCAL void CutInScrollSprite_Update(void *a0, void *a1);
WIP_LOCAL void WhirlpoolEffect_Init(void *a0, void *a1, void *a2);
WIP_LOCAL int HeadbuttEffect_Update(void *a0, void *a1, void *a2);
WIP_LOCAL int FollowMonRule_Matches(void *a0, void *a1, void *a2);
WIP_LOCAL int SafariZone_FindSpotFor1x2Object(void *a0, int a1, int a2, int a3, fx32 a4, u16 *a5, u16 *a6, int a7);
WIP_LOCAL int SafariZone_FindSpotFor2x1Object(void *a0, int a1, int a2, int a3, fx32 a4, u16 *a5, u16 *a6, int a7);
typedef struct ov02_BF58Cfg {
    int unk0;
    int unk4;
} ov02_BF58Cfg;
extern void *ov01_021F771C(void *a0);            // no header included here
extern void BillboardObject_SetScale(void *a0, VecFx32 *a1); // no header included here
WIP_LOCAL BOOL ov02_022508D8(TaskManager *taskManager);

typedef struct ov02_FieldList5 {
    u32 v[5];
} ov02_FieldList5;
extern int FieldSystem_FindMapPropOfModels(void *a0, void *a1, int a2, void **a3, void **a4);                                                                                 // no header included here
extern void *MapProp_GetRenderSurface(void *a0);                                                                                                           // no header included here
extern void *MapProp_GetResModel(void *a0);                                                                                                                // no header included here
extern void MapPropOneShotAnimationManager_LoadPropAnimations(void *a0, void *a1, int a2, void *a3, void *a4, void *a5, void *a6, int a7, int a8, int a9); // no header included here
extern void *AreaDataManager_GetMapPropModel(int a0, void *a1);                                                                                                              // no header included here
extern BOOL MapPropOneShotAnimationManager_IsAnimationLoopFinished(void *a0, int a1);                                                                      // no header included here
extern void MapPropOneShotAnimationManager_UnloadAnimation(void *a0, void *a1, int a2);                                                                    // no header included here
extern NNSG3dResMdlSet *NNS_G3dGetMdlSet(const NNSG3dResFileHeader *header);                                                                               // res_struct_accessor.h, not included (IPA)
#undef MapPropManager_LoadOne
extern int MapPropManager_LoadOne(void *a0, int a1, const VecFx32 *a2, const VecFx32 *a3, void *a4);  // overlay_01.h not included (real return non-void)
extern void *MapPropManager_GetMapPropByIndex_Checked_RequireActive(void *a0, int a1);                // no header included here
extern void MapPropOneShotAnimationManager_SetAnimationRenderObj(void *a0, int a1, int a2, void *a3); // no header included here
extern void MapPropManager_RemoveMapPropByIndex(int a0, void *a1);                                    // no header included here
typedef struct ov02_SafariObjCfg {
    u8 buildModel;
    u8 isAnimated : 1;
    u8 width : 3;
    u8 height : 3;
    u8 hasGenderedLayout : 1;
    u8 objectType;
} ov02_SafariObjCfg;
extern void GetSafariObjectConfig(ov02_SafariObjCfg *a0, int a1, int a2);                                            // unk_02097268.h, not included
extern int GetDeltaXByFacingDirection(int direction);                                                                // unk_0205FD20.h, not included
extern int GetDeltaYByFacingDirection(int direction);                                                                // unk_0205FD20.h, not included
extern PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);                                                // player_data.h, not included
extern u32 PlayerProfile_GetTrainerGender(PlayerProfile *profile);                                                   // player_data.h, not included
extern BOOL FollowMon_IsNextToPlayer(FieldSystem *fieldSystem);                                                                 // overlay_01.h, not included
extern int FieldSystem_GetLeadMonIndex(FieldSystem *fieldSystem);                                                                  // overlay_01.h, not included
extern void PlayCryEx(int, int, int, int, int, int);                                                                 // sound_02004A44.h, not included
extern void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable);                                                      // gf_gfx_planes.h, not included
extern void *GfGfxLoader_LoadFromNarc(NarcId narcId, s32 fileId, BOOL isCompressed, enum HeapID heapID, BOOL atEnd); // gf_gfx_loader.h, not included
extern void FieldMessage_LoadTextPalettes(int a0, int a1);                                                           // text_0205B4EC.h, not included
extern void *FieldEffectPool_New(int a0, int a1);                                                                           // unk_020689C8.h, not included
extern u16 GF_DegreeToSinCosIdx(u16 deg);                                                                            // math_util.h, not included
extern u16 LCRandom(void);                                                                                           // math_util.h, not included
extern fx32 GF_SinDeg(u16 deg);                                                                                      // math_util.h, not included
extern fx32 GF_CosDeg(u16 deg);                                                                                      // math_util.h, not included
extern void FollowMonBillboard_AddHeight(LocalMapObject *mapObject, int a1);                                                        // no header included here
extern BOOL FollowMonObject_IsLarge(LocalMapObject *mapObject);                                                                // no header included here
extern void TallGrass_SpawnAt(LocalMapObject *mapObject, int a1, u32 x, u32 z, int a4);                                  // no header included here
extern void VeryTallGrass_SpawnAt(LocalMapObject *mapObject, int a1, u32 x, u32 z, int a4);                                  // no header included here
extern BOOL MetatileBehavior_IsVeryTallGrass(u8 tile);                                                               // no header included here
WIP_LOCAL void FieldMoveCutIn_CreateScrollSprites(void *work, int a1);                                                                    // still in asm; forward decl for callers

// NewMsgDataFromNarc / MessageFormat_* / Buffer* / MapHeader_GetMapSec are reachable
// transitively (msgdata.h / message_format.h / map_header.h) — no local externs.
extern PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);                             // player_data.h, not included
extern String *String_New(u32 maxsize, enum HeapID heapID);                                       // pm_string.h, not included
extern void DialogBox_AddWindowToLayer3(BgConfig *bgConfig, Window *window, int a2);              // text_0205B4EC.h, not included
extern void DialogBox_LoadFrame(Window *window, Options *options);                                // text_0205B4EC.h, not included
extern u8 DialogBox_PrintMessage(Window *window, String *string, Options *options, BOOL speedup); // text_0205B4EC.h, not included
extern u32 PlayerProfile_GetTrainerID(PlayerProfile *profile);                                    // player_data.h, not included
extern void *Save_SafariZone_Get(SaveData *saveData);                                             // safari_zone.h, not included (opaque)
extern u8 SafariZone_GetObjectUnlockLevel(void *safariZone);                                      // safari_zone.h, not included
extern void *Field_GetBgEvents(FieldSystem *fieldSystem);                                         // map_events.h, not included (BgEvent opaque)
extern u32 Field_GetNumBgEvents(FieldSystem *fieldSystem);                                        // map_events.h, not included

// Task_ScriptFieldMoveEffect dispatch tables (rodata, still in asm; defined later). Indexed by
// data[0xc]: A34 update funcs return int (1 => advance state); A04 delete funcs
// (result ignored). Declared int(*)(void*) to match the blx call sites.
typedef int (*ov02_AnimDispatchFunc)(void *data);
// ScriptFieldMoveEffect_Start create-dispatch table (rodata, defined later): each entry builds a
// Field3dObjectTask from the fieldSystem. Indexed by the anim type.
typedef Field3dObjectTask *(*ov02_CreateDispatchFunc)(FieldSystem *fieldSystem);
extern BOOL FieldSystem_FindMapProp(FieldSystem *fieldSystem, int targetType, int *outObj, void **outHandle); // unk_02054648.h, not included
extern void Terrain_GetChunkOrigin(int idx, int width, VecFx32 *out);                                        // unk_02054648.h, not included
extern void MapProp_GetTranslation(VecFx32 *out, void *src);                                       // unk_02054648.h, not included
extern u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);                             // unk_02054648.h, not included
extern void FieldSystem_ShowEmoteBubble(FieldSystem *fieldSystem, LocalMapObject *partnerPokeObj, int a2);       // overlay_01.h, not included
extern u16 PlayerProfile_GetTrainerID_VisibleHalf(PlayerProfile *profile);                         // player_data.h, not included
typedef struct WallpaperPasswordBank WallpaperPasswordBank;                                        // opaque; easy_chat.h not included
extern WallpaperPasswordBank *WallpaperPasswordBank_Create(enum HeapID heapID);
extern void WallpaperPasswordBank_Delete(WallpaperPasswordBank *bank);
extern u32 WallpaperPasswordBank_GetCount(WallpaperPasswordBank *bank);
extern s16 WallpaperPasswordBank_GetIndexOfWord(WallpaperPasswordBank *bank, s32 word);
extern u32 FieldEffect_GetParam(void *a0);                                           // unk_020689C8.c, no C proto
extern void FieldEffect_SetPosition(void *a0, VecFx32 *src);                            // unk_020689C8.c, no C proto
extern void Field3dObject_SetPos(Field3dObject *object, const VecFx32 *pos); // .public, no C proto yet
extern void GetFlyWarpData(u16 spawnId, Location *dest);                     // unk_0203BA5C.h, not included
extern void GetSpecialSpawnWarpData(u16 spawnId, Location *dest);            // unk_0203BA5C.h, not included

// unk_020689C8.c task launcher: FieldEffectPool_AddBasicEffect(mgr, src, pos, a3, a4, priority).
// The UnkTemplateBase/UnkTaskWork types are local to that TU; mirror minimally.
// src points to a 0x14-byte template (file-local rodata, still in asm for now).
typedef struct ov02_LaunchTemplate {
    u32 unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    void *unk10;
} ov02_LaunchTemplate;
extern void *FieldEffectPool_AddBasicEffect(void *mgr, const ov02_LaunchTemplate *src, VecFx32 *pos, u32 a3, void *a4, u32 priority);

// STRUCT-CLEANUP TODO: the getters/setters/deleters below use byte-offset casts
// because their owning struct isn't named yet. They byte-match; replace the casts
// with real struct field accesses once those structs are reversed.
WIP_LOCAL u8 FlyLandingEffect_GetStatus(void *work);
WIP_LOCAL void FlyLandingEffect_Destroy(void *a0, void *work);
WIP_LOCAL BOOL ov02_0224953C(void *work);
WIP_LOCAL int ov02_0224997C(void *work);
WIP_LOCAL int FlyLandingEffect_ShowPlayer(void *work);
WIP_LOCAL int ov02_02249774(void *work);
WIP_LOCAL void FollowMonMovement_PlaySound(void *arg0, u16 arg1, int arg2, int arg3);
WIP_LOCAL int FieldEscapeRope_SpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldDig_SinkStart(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void Task_ScriptFieldMoveEffect(SysTask *task, void *data);
WIP_LOCAL BOOL ov02_0224E640(SaveData *saveData);
WIP_LOCAL void RockDebris_SpawnPiece(Field3dObject *obj, VecFx32 *arg1, fx32 arg2, fx32 arg3);
WIP_LOCAL void FollowMonSituation_SetHiddenItems(FieldSystem *fieldSystem, void *out);
WIP_LOCAL Sprite *FlyLanding_CreateFlyerSprite(void *mgr);
WIP_LOCAL void ov02_0224A66C(void *work);
WIP_LOCAL void ov02_0224A674(void *work);
WIP_LOCAL void ov02_0224A690(void *work);
WIP_LOCAL void CutInScrollSprite_Destroy(void *a0, void *work);
WIP_LOCAL void CutInFlyer_Destroy(void *a0, void *work);
WIP_LOCAL void TrainerJumpEffect_Stop(void *work);
WIP_LOCAL BOOL CutInFlyer_PickUpTrainer(void *work);
WIP_LOCAL void RockDebris_InitPiece(Field3dObject *obj, Field3dModel *model, NNSFndAllocator *allocator, void **anmResources);
WIP_LOCAL BOOL FlyLanding_IsDone(SysTask *task);
WIP_LOCAL BOOL MapId_IsSafariZone(int a0);
WIP_LOCAL BOOL FollowMonInteraction_StepHasMovement(void *a0, u16 *a1);
WIP_LOCAL Sprite *SpriteGfxManager_CreateSprite(void *mgr, VecFx32 *pos, int charId, int plttId, int cellId, int cellAnmId, int priority, int drawPriority);
WIP_LOCAL Sprite *FlyLanding_CreateTrainerSprite(void *mgr, int a1);
WIP_LOCAL int FieldMoveArrival_Finish(void *a0, void *a1, void *work);
WIP_LOCAL Sprite *ov02_0224A33C(void *mgr, VecFx32 *pos, int charId, int plttId, int cellId, int mode, int priority, int drawPriority);
WIP_LOCAL Sprite *FieldMoveCutIn_CreateMonSprite(void *mgr, VecFx32 *pos);
WIP_LOCAL Sprite *ov02_0224A3F0(void *mgr, VecFx32 *pos, int drawPriority, int seq);
WIP_LOCAL void ov02_0224A570(NARC *narc, u32 fileId, NNSG2dPaletteData **a2);
WIP_LOCAL void ov02_0224A5D0(BgConfig *bgConfig, NARC *narc, u32 fileId, NNSG2dScreenData **a3);
WIP_LOCAL void FieldMoveCutIn_LoadModel(void *work);
WIP_LOCAL void FieldMoveCutIn_UnloadModel(void *work);
WIP_LOCAL int FieldMoveCutIn_StepShowOtherObject(void *work);
WIP_LOCAL void CutInModelEffect_Update(void *a0, void *work);
WIP_LOCAL void FieldMoveCutIn_CreateModelEffect(void *work); // still in asm
WIP_LOCAL int FieldMoveCutIn_StepWaitModelEffect(void *work);
WIP_LOCAL int FieldTeleport_SpinStart(void *a0, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ov02_RepelActiveRoamersFromMapNo(RoamerSaveData *roamerSave, u32 mapNo);
WIP_LOCAL void RockDebris_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224A598(BgConfig *bgConfig, NARC *narc, u32 fileId, NNSG2dCharacterData **a3);
WIP_LOCAL void TrainerJumpEffect_StartHop(void *mgr);
WIP_LOCAL void FollowMon_PlaceholdersSet(void *work, void *messageFormat);
WIP_LOCAL void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);
WIP_LOCAL int FieldEscapeRope_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void RockClimbDebris_SpawnPiece(Field3dObject *obj, PlayerAvatar *playerAvatar, fx32 arg2, fx32 arg3);
WIP_LOCAL int FieldTeleport_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL BOOL FollowMon_TryPrintInteractionMessage(void *work, void *window, void *arg2);
WIP_LOCAL void *ov02_02249458(FieldSystem *fieldSystem, int a1, Pokemon *a2, int a3);
WIP_LOCAL int ov02_02249858(void *work);
WIP_LOCAL int FieldDig_DigHole(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldMoveArrival_FadeIn(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_02249C74(void *work);
WIP_LOCAL int ov02_02249A5C(void *work);
WIP_LOCAL void FollowMonMovement_ApplyOffset(void *arg0, LocalMapObject *mapObject);
WIP_LOCAL void ov02_022494C4(FieldSystem *fieldSystem, void *a1, void *a2, void *a3);
WIP_LOCAL int ov02_022499EC(void *work);
WIP_LOCAL int ov02_022495E8(void *work);
WIP_LOCAL int ov02_022497C0(void *work);
WIP_LOCAL int SwapPositions_GetFaceMovement(int a0);
WIP_LOCAL int SwapPositions_GetTurnAroundMovement(int a0);
WIP_LOCAL int SwapPositions_GetOppositeStep(int a0);
WIP_LOCAL void FollowMon_GetCoordsInDirection(LocalMapObject *mapObject, int dir, u32 *outX, u32 *outZ);
WIP_LOCAL int FollowMonMovement_Step(void *a0, LocalMapObject *a1);
WIP_LOCAL void FollowMonSituation_SetFacing(FieldSystem *fieldSystem, void *arg1);
WIP_LOCAL int FollowMonSituation_ConvertItemPocket(int a0);
WIP_LOCAL void FollowMonSituation_SetTimeOfDay(FieldSystem *fieldSystem, void *arg1);
WIP_LOCAL void ov02_0224A288(void *work);
WIP_LOCAL int FlyLanding_FreeGraphics(void *work);
WIP_LOCAL int ov02_02249658(void *work);
WIP_LOCAL int FieldTeleport_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldDig_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldDig_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldEscapeRope_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ScriptFieldMoveEffect_Start(FieldSystem *fieldSystem, u16 *p_ret, int type, enum HeapID heapID);
WIP_LOCAL BOOL FollowMonRule_CheckSpeciesAttr(u32 a0, u32 a1);
WIP_LOCAL BOOL FollowMonRule_CheckShinyLeaf(u32 a0, u32 a1);
WIP_LOCAL void FollowMon_ApplyInteractionEffects(void *work);
WIP_LOCAL int ov02_022498BC(void *work);
WIP_LOCAL void ov02_0224A080(void *work, NARC *narc);
WIP_LOCAL void ov02_02249F6C(void *work);
WIP_LOCAL void ov02_0224A028(void *work);
WIP_LOCAL void ov02_02249FD4(void *work);
WIP_LOCAL BOOL FollowMonMovement_HasNextStep(void *p);
WIP_LOCAL BOOL FlyLandingEffect_StartClimb(void *mgr);
WIP_LOCAL BOOL FlyLandingEffect_Init(void *a0, void *obj);
WIP_LOCAL void FieldMoveCutIn_TransferMonChar(void *mgr, void *src);
WIP_LOCAL Sprite *ov02_0224A418(void *mgr, VecFx32 *pos);
WIP_LOCAL void CutEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);
WIP_LOCAL void RockSmashEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);
WIP_LOCAL BOOL CutInModelEffect_Init(void *a0, void **out);
WIP_LOCAL Roamer *Roamers_GetActiveRoamerBySpecies(RoamerSaveData *roamerSave, int a1);
WIP_LOCAL void FieldMoveCutIn_CreateTrainerClipEffect(void *work);
WIP_LOCAL void FieldMoveCutIn_CreateFlyer(void *work);
WIP_LOCAL void *FieldMoveCutIn_CreateTrainerJumpEffect(void *mgr, void *arg1);
WIP_LOCAL void FlyLandingEffect_Start(void *arg0);
WIP_LOCAL void *FlyLandingEffect_Create(void *arg0, void *arg1, void *arg2, void *arg3);
WIP_LOCAL void FieldMoveCutIn_CreateScrollSprite(void *arg0, VecFx32 *pos, VecFx32 *vec, void *arg3, u32 arg4, void *arg5);
WIP_LOCAL void RockDebris_UpdatePiece(void *obj);
WIP_LOCAL void RockClimbDebris_Update(void *a0, void *a1, void *data);
WIP_LOCAL void RockDebris_Update(void *a0, void *a1, void *data);
WIP_LOCAL int ov02_02249690(void *work);
WIP_LOCAL int ov02_022496D0(void *work);
WIP_LOCAL int FlyLandingEffect_StartFlyAway(void *work);
WIP_LOCAL int FlyLandingEffect_FlyAway(void *work);
WIP_LOCAL int FlyLandingEffect_SwoopIn(void *work);
WIP_LOCAL int FlyLandingEffect_Climb(void *work);
WIP_LOCAL int CutInFlyer_FlyAway(void *work);
WIP_LOCAL int CutInFlyer_SwoopDown(void *work);
WIP_LOCAL int CutInFlyer_TakeOff(void *work);
WIP_LOCAL int ov02_02249BD8(void *work);
WIP_LOCAL int FollowMonRule_CheckFriendship(int a0, int a1);
WIP_LOCAL int FollowMonRule_CheckMood(int a0, int a1);
WIP_LOCAL int FollowMonInteraction_RunMovement(FieldSystem *fieldSystem, void *a1, int a2);
WIP_LOCAL void RockSmashEffect_InitAtPlayer(void *a0, void *a1, void *data);
WIP_LOCAL int FieldTeleport_Warp(TaskManager *taskManager, void *a1, void *a2);
WIP_LOCAL void RockClimbDebris_Spawn(void *data);
WIP_LOCAL void RockClimbDebris_UpdateOffsets(void *data);
WIP_LOCAL void RockClimbDebris_InitPiece(Field3dObject *obj, Field3dModel *model, NNSFndAllocator *allocator, void **anmResources);
WIP_LOCAL void RockClimbDebris_Init(void *a0, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void RockDebris_Spawn(void *data);
WIP_LOCAL BOOL ElevatorAnimRun(TaskManager *taskManager);
WIP_LOCAL BOOL Task_HallOfFameBallAnim(TaskManager *taskManager);
WIP_LOCAL BOOL PokecenterAnimRun(TaskManager *taskManager);
WIP_LOCAL int PrimoPassword_DecodeWallpaper(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d);
WIP_LOCAL int PrimoPassword_DecodeCheck2(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d);
WIP_LOCAL void PrimoPassword_RotateBitsRight(u8 *arr, int n, u8 val);
WIP_LOCAL int FieldTeleport_FollowMonWaitSpin(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldMoveArrival_WaitFadeIn(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldTeleport_FollowMonSpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void *FieldMoveCutIn_LoadMonChar(Pokemon *mon, void *arg1, enum HeapID heapID);
WIP_LOCAL BOOL CutInScrollSprite_Init(void *a0, void *work);
WIP_LOCAL void TrainerJumpEffect_StartJump(void *work);
WIP_LOCAL int FlyLanding_CreateSprites(void *work);
WIP_LOCAL void CutEffect_InitAtPlayer(void *a0, void *a1, void *data);
WIP_LOCAL int SwapPositions_GetStepTowards(void *a, void *b);
WIP_LOCAL void FollowMonSituation_SetTerrain(FieldSystem *fieldSystem, void *work);
WIP_LOCAL void FollowMon_GetSituation(FieldSystem *fieldSystem, void *work);
WIP_LOCAL void FollowMonSituation_SetMonCondition(Pokemon *mon, void *work);
WIP_LOCAL void FollowMonSituation_SetNearbyObjects(FieldSystem *fieldSystem, void *work);
WIP_LOCAL void FollowMonSituation_SetBestPokeathlonStat(FieldSystem *fieldSystem, int species, int form, void *work);
WIP_LOCAL void FlyLandingEffect_Start(void *a0);
WIP_LOCAL void FieldMoveCutIn_CreateModelEffect(void *work);
WIP_LOCAL BOOL FollowMonInteraction_TryShowEmote(FieldSystem *fieldSystem, void *a1, void *arg2);
WIP_LOCAL void TrainerJumpEffect_Jump(void *a0, void *work);
WIP_LOCAL int FieldMoveArrival_Spin(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void CutEffect_InitAtFollower(void *a0, void *a1, void *data);
WIP_LOCAL void HeadbuttEffect_InitAtFollower(void *a0, void *a1, void *data);
WIP_LOCAL void HeadbuttEffect_InitAtPlayer(void *a0, void *a1, void *data);
WIP_LOCAL void TrainerJumpEffect_Hop(void *a0, void *work);
WIP_LOCAL void RockSmashEffect_InitAtFollower(void *a0, void *a1, void *data);
WIP_LOCAL void Field3dObject_SetPosInFrontOfMapObject(void *a0, int a1, void *data);
WIP_LOCAL void FieldMoveCutIn_TransferMonPltt(void *mgr, void *dst);
WIP_LOCAL void TrainerClipEffect_Update(void *a0, void *work);
WIP_LOCAL void FieldMoveEffectModel_Load(void *data, int a1, int a2, int a3, void *a4);
WIP_LOCAL void Field3dObject_SetPosInFrontOfPlayer(void *a0, void *data);
WIP_LOCAL void RockClimbDebris_UpdatePiece(void *obj);
WIP_LOCAL void RockClimbDebris_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL BOOL CutInFlyer_Init(void *a0, void *a1);
WIP_LOCAL void *ov02_0224A468(void *a, VecFx32 *b, int c, int d); // still in asm
WIP_LOCAL BOOL FieldSystem_FirstAliveMonHasType(FieldSystem *fieldSystem, u8 a1);
WIP_LOCAL void ov02_02249E90(SysTask *task, void *work);
WIP_LOCAL void FollowMonMovement_ApplyFacing(void *a0, u8 *a1, LocalMapObject *obj);
WIP_LOCAL void FollowMon_TryShakeGrass(void *a0, LocalMapObject *obj, u8 dir);
WIP_LOCAL BOOL FieldMoveEffectModel_AnimateOnce(void *obj);
WIP_LOCAL void ov02_022507B4(FieldSystem *fieldSystem, u8 a1);
WIP_LOCAL BOOL ov02_022507E8(TaskManager *taskManager); // still in asm
WIP_LOCAL BOOL SpriteGfxManager_TransferChar(void *mgr, int a1);
WIP_LOCAL void SpriteGfxManager_ReleaseCharData(void *mgr, int a1);
WIP_LOCAL BOOL SpriteGfxManager_TransferPltt(void *mgr, int a1);
WIP_LOCAL void SpriteGfxManager_ReleasePlttData(void *mgr, int a1);
WIP_LOCAL int FlyLanding_LoadGraphics(void *work);

// forward decls for functions referenced only from the consolidated rodata tables
extern void FieldEffect_DefaultDraw(void);
extern void FieldEffect_DefaultDestroy(void);
WIP_LOCAL void FlyLandingEffect_Update(void *a0, u8 *sm);
WIP_LOCAL int FlyLandingEffect_Idle(void *work);
WIP_LOCAL int FlyLandingEffect_HideFlyer(void *work);
WIP_LOCAL int FlyLandingEffect_Done(void);
WIP_LOCAL int ov02_022495B8(void *work);
WIP_LOCAL int ov02_022495D0(void *work);
WIP_LOCAL int ov02_02249754(void *work);
WIP_LOCAL int ov02_02249838(void *work);
WIP_LOCAL int ov02_02249940(void *work);
WIP_LOCAL int ov02_02249954(void *work);
WIP_LOCAL int ov02_02249968(void *work);
WIP_LOCAL int ov02_02249AC4(void *work);
WIP_LOCAL int ov02_02249AD8(void *work);
WIP_LOCAL BOOL ov02_02249AF0(void *work);
WIP_LOCAL int ov02_02249B10(void *work);
WIP_LOCAL int ov02_02249B38(void *work);
WIP_LOCAL BOOL ov02_02249B60(void *work);
WIP_LOCAL BOOL ov02_02249B80(void *work);
WIP_LOCAL BOOL ov02_02249BA8(void *work);
WIP_LOCAL int ov02_02249CD8(int *work);
WIP_LOCAL void CutInScrollSprite_Draw(void);
WIP_LOCAL void CutInFlyer_Update(void *a0, u8 *sm);
WIP_LOCAL void CutInFlyer_Draw(void);
WIP_LOCAL int CutInFlyer_Hide(void *work);
WIP_LOCAL int CutInFlyer_Idle(void);
WIP_LOCAL int CutInFlyer_Done(void);
WIP_LOCAL BOOL TrainerJumpEffect_Init(void *a0, void *out);
WIP_LOCAL void TrainerJumpEffect_Update(void *a0, int *a1);
WIP_LOCAL int FlyLanding_SetPlayerVisible(void *work);
WIP_LOCAL int FlyLanding_WaitLanding(void *work);
WIP_LOCAL int FlyLanding_Idle(void);
WIP_LOCAL BOOL TrainerClipEffect_Init(void *a0, void *out);
WIP_LOCAL void CutInModelEffect_Destroy(void);
WIP_LOCAL void CutInModelEffect_Draw(void *a0, void *a1);
WIP_LOCAL int FieldEscapeRope_Warp(TaskManager *taskManager, void *a1, void *a2);
WIP_LOCAL int FieldMove_ShowCutIn(void *a0, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int FieldMove_WaitCutIn(void *a0, void *a1, void *work);
WIP_LOCAL int FieldTeleport_FollowMonFlash(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL int FieldDig_Warp(TaskManager *taskManager, void *a1, void *a2);
WIP_LOCAL void CutEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void CutEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL Field3dObjectTask *CutEffect_CreateAtPlayer(FieldSystem *fieldSystem);
WIP_LOCAL Field3dObjectTask *CutEffect_CreateAtFollower(FieldSystem *fieldSystem);
WIP_LOCAL void CutEffect_Delete(Field3dObjectTask *task);
WIP_LOCAL BOOL CutEffect_IsFinished(Field3dObjectTask *task);
WIP_LOCAL void RockSmashEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void RockSmashEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL Field3dObjectTask *RockSmashEffect_CreateAtPlayer(FieldSystem *fieldSystem);
WIP_LOCAL Field3dObjectTask *RockSmashEffect_CreateAtFollower(FieldSystem *fieldSystem);
WIP_LOCAL void RockSmashEffect_Delete(Field3dObjectTask *task);
WIP_LOCAL BOOL RockSmashEffect_IsFinished(Field3dObjectTask *task);
WIP_LOCAL void WaterfallEffect_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void WaterfallEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void WaterfallEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void WhirlpoolEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void WhirlpoolEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void WhirlpoolEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void RockClimbDebris_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void HeadbuttEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void HeadbuttEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL Field3dObjectTask *HeadbuttEffect_CreateAtPlayer(FieldSystem *fieldSystem);
WIP_LOCAL Field3dObjectTask *HeadbuttEffect_CreateAtFollower(FieldSystem *fieldSystem);
WIP_LOCAL void HeadbuttEffect_Delete(Field3dObjectTask *task);
WIP_LOCAL BOOL HeadbuttEffect_IsFinished(Field3dObjectTask *task);
WIP_LOCAL void RockDebris_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void RockDebris_InitWithFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void RockDebris_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

WIP_LOCAL BOOL ElevatorAnimRun(TaskManager *taskManager);
WIP_LOCAL int PrimoPassword_DecodeWallpaper(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d);
WIP_LOCAL int PrimoPassword_DecodeCheck2(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d);
void HallOfFame_StartBallAnim(FieldSystem *fieldSystem, u8 a1);
WIP_LOCAL BOOL Task_HallOfFameBallAnim(TaskManager *taskManager);
WIP_LOCAL void Field3dObject_SetPosInFrontOfMapObject(void *a0, int a1, void *data);
WIP_LOCAL void Field3dObject_SetPosInFrontOfPlayer(void *a0, void *data);
WIP_LOCAL void FieldMoveEffectModel_Load(void *data, int a1, int a2, int a3, void *a4);
WIP_LOCAL void FieldMoveEffectModel_Unload(void *obj, void *alloc);
WIP_LOCAL BOOL FieldMoveEffectModel_AnimateOnce(void *obj);
WIP_LOCAL void FieldMoveEffectModel_AnimateLoop(void *data);
WIP_LOCAL void CutEffect_InitAtPlayer(void *a0, void *a1, void *data);
WIP_LOCAL void CutEffect_InitAtFollower(void *a0, void *a1, void *data);
WIP_LOCAL void CutEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);
WIP_LOCAL void RockSmashEffect_InitAtPlayer(void *a0, void *a1, void *data);
WIP_LOCAL void RockSmashEffect_InitAtFollower(void *a0, void *a1, void *data);
WIP_LOCAL void RockSmashEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);
WIP_LOCAL void WaterfallEffect_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void WaterfallEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void WaterfallEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void RockClimbDebris_Init(void *a0, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void RockClimbDebris_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void RockClimbDebris_Update(void *a0, void *a1, void *data);
extern void FieldMoveEffectModel_Draw(Field3dObject *);
extern void RockClimbDebris_FreePiece(void *, NNSFndAllocator *);
extern void RockClimbDebris_UpdatePieces(void *);
extern VecFx32 sHallOfFameBallOffsets[6];
#endif
