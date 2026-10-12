#ifndef POKEHEARTGOLD_OVERLAY02_4_LANDING_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY02_4_LANDING_PRIVATE_H

// WIP / DEFERRED — matching decomp of asm/overlay_02_02248728.s (364 functions:
// follow-mon sprite animation + Pokecenter/field-move task module, a 2D graphics
// renderer built on G2dRenderer / Create2DGfxResObjMan).
//
// main.lsf is kept on asm/overlay_02_02248728.o until ALL 364 functions match.
// This .c is compiled (rwildcard picks up every src/*.c) but NOT linked, so the
// ROM stays matching while WIP C accumulates. To iterate:
//   chiri pkg -- build --target main --no-compare
//   python3 tools/decomp_harness/objdiff.py \
//     build/heartgold.us/asm/overlay_02_02248728.o \
//     build/heartgold.us/src/overlay_02_02248728.o --summary
//
// Function order matches the asm. Functions still in asm are simply absent from
// this file (objdiff matches by symbol name). See the handoff block at EOF.
//
// VISIBILITY NOTE: ~318/364 functions are file-local (not in overlay_02.inc's
// .public list) and must be `static` in the final. During WIP they are declared
// WITHOUT `static` (see WIP_LOCAL below) so MWCC's dead-code elimination doesn't
// drop the unreferenced ones before objdiff can compare them. static-vs-global
// only changes the .o symbol-table binding, which is stripped from the linked
// overlay — it does NOT affect ROM bytes / the SHA1. Finalize WIP_LOCAL -> static
// when flipping main.lsf to src.
#define OV02_02248728_OWN_DECLS

// Current-tree header declarations differ from the candidate's matching-time
// opaque signatures. Shadow them while including public headers, then restore
// the real linker names below.
#define FieldSystem_CheckSafariObjectPlacement ov02_0224E698_CurrentHeaderDecl
#define FieldSystem_GetFacingSafariObject ov02_0224E754_CurrentHeaderDecl
#define AreaDataManager_GetMapPropTexture ov01_021FB9E0_CurrentHeaderDecl
#define PlayCry       PlayCry_CurrentHeaderDecl

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
#undef FieldSystem_CheckSafariObjectPlacement
#undef FieldSystem_GetFacingSafariObject
#undef AreaDataManager_GetMapPropTexture
#undef PlayCry

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

// ov02_0224A9D8 per-entry rodata (ov02_022535E4, stride 0x14).
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
extern void ov01_021F8F68(LocalMapObject *object, int a1);                                                   // no header included here
extern void ov01_021F8F08(LocalMapObject *object, int a1);                                                   // no header included here
extern void *Save_VarsFlags_Get(SaveData *saveData);                                                         // save_vars_flags.h, not included
extern BOOL Save_VarsFlags_CheckSafariSysFlag(void *varsFlags);                                              // save_vars_flags.h, not included
extern BOOL sub_0202F620(void *safariZone);                                                                  // no header included here
extern fx32 sub_02054774(FieldSystem *fieldSystem, fx32 refHeight, fx32 xFx32, fx32 zFx32, u8 *outSelector); // unk_02054648.h, not included
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
extern void ov01_021F6A9C(FieldSystem *fieldSystem, int a1, void *a2);                                                      // overlay_01.h, not included
extern int ov01_021F6B00(FieldSystem *fieldSystem);                                                                         // overlay_01.h, not included
extern BOOL ov01_021F6B10(FieldSystem *fieldSystem);                                                                        // overlay_01.h, not included
extern void ov01_021F6ABC(FieldSystem *fieldSystem, int a1, int a2, void *a3);                                              // overlay_01.h, not included
extern int ov01_021F6AEC(FieldSystem *fieldSystem);                                                                         // overlay_01.h, not included
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
WIP_LOCAL int FieldSystem_CheckSafariObjectPlacement(void *work);
// --- NONMATCHING-finalization deps (asm-only references) ---
extern void _s32_div_f(void);                 // int-division runtime
extern void _fadd(void);                      // soft-float runtime (asm bl)
extern void _fsub(void);                      // soft-float runtime
extern void _fflt(void);                      // soft-float runtime
extern void _ffix(void);                      // soft-float runtime
extern void Field3dObject_SetXRotation(void); // not in any included header (asm-only ref)
extern void sub_020548C0(void);               // not in any included header (asm-only ref)
WIP_LOCAL void ov02_0224D488(void *a0, void *a1, void *a2);
extern u16 GF_DegreeToSinCosIdxNoWrap(u16 deg);                           // math_util.h, not included
extern BOOL Save_VarsFlags_CheckFlagInArray(void *varsFlags, u16 flagId); // save_vars_flags.h, not included
WIP_LOCAL void ov02_0224AAD4(void *a0, void *a1);
WIP_LOCAL void ov02_0224D5B4(void *a0, void *a1, void *a2);
WIP_LOCAL int ov02_0224DB9C(void *a0, void *a1, void *a2);
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
extern int sub_02054C90(void *a0, void *a1, int a2, void **a3, void **a4);                                                                                 // no header included here
extern void *AreaDataManager_GetMapPropTexture(void *a0);                                                                                                                      // no header included here
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
extern void GetSafariObjectConfig(ov02_SafariObjCfg *a0, int a1, int a2); // unk_02097268.h, not included
extern int GetDeltaXByFacingDirection(int direction);                     // unk_0205FD20.h, not included
extern int GetDeltaYByFacingDirection(int direction);                     // unk_0205FD20.h, not included
extern PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);     // player_data.h, not included
extern u32 PlayerProfile_GetTrainerGender(PlayerProfile *profile);        // player_data.h, not included
WIP_LOCAL int FieldSystem_GetFacingSafariObject(void *work, u16 *out);
extern BOOL FollowMon_IsNextToPlayer(FieldSystem *fieldSystem);                                                                 // overlay_01.h, not included
extern int FieldSystem_GetLeadMonIndex(FieldSystem *fieldSystem);                                                                  // overlay_01.h, not included
extern void PlayCryEx(int, int, int, int, int, int);                                                                 // sound_02004A44.h, not included
extern void PlayCry(u16 species, u8 form);                                                                           // sound_chatot.h, not included
extern void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable);                                                      // gf_gfx_planes.h, not included
extern void *GfGfxLoader_LoadFromNarc(NarcId narcId, s32 fileId, BOOL isCompressed, enum HeapID heapID, BOOL atEnd); // gf_gfx_loader.h, not included
extern void FieldMessage_LoadTextPalettes(int a0, int a1);                                                           // text_0205B4EC.h, not included
extern void *FieldEffectPool_New(int a0, int a1);                                                                           // unk_020689C8.h, not included
extern u16 GF_DegreeToSinCosIdx(u16 deg);                                                                            // math_util.h, not included
extern u16 LCRandom(void);                                                                                           // math_util.h, not included
extern fx32 GF_SinDeg(u16 deg);                                                                                      // math_util.h, not included
extern fx32 GF_CosDeg(u16 deg);                                                                                      // math_util.h, not included
extern void ov01_021F8F74(LocalMapObject *mapObject, int a1);                                                        // no header included here
extern BOOL FollowMonObject_IsLarge(LocalMapObject *mapObject);                                                                // no header included here
extern void TallGrass_SpawnAt(LocalMapObject *mapObject, int a1, u32 x, u32 z, int a4);                                  // no header included here
extern void VeryTallGrass_SpawnAt(LocalMapObject *mapObject, int a1, u32 x, u32 z, int a4);                                  // no header included here
extern BOOL MetatileBehavior_IsVeryTallGrass(u8 tile);                                                               // no header included here
WIP_LOCAL void ov02_0224A9D8(void *work, int a1);                                                                    // still in asm; forward decl for callers

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

// ov02_0224E020 dispatch tables (rodata, still in asm; defined later). Indexed by
// data[0xc]: A34 update funcs return int (1 => advance state); A04 delete funcs
// (result ignored). Declared int(*)(void*) to match the blx call sites.
typedef int (*ov02_AnimDispatchFunc)(void *data);
// ov02_0224E074 create-dispatch table (rodata, defined later): each entry builds a
// Field3dObjectTask from the fieldSystem. Indexed by the anim type.
typedef Field3dObjectTask *(*ov02_CreateDispatchFunc)(FieldSystem *fieldSystem);
extern BOOL sub_02054C20(FieldSystem *fieldSystem, int targetType, int *outObj, void **outHandle); // unk_02054648.h, not included
extern void sub_02054DC8(int idx, int width, VecFx32 *out);                                        // unk_02054648.h, not included
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
WIP_LOCAL u8 ov02_02248D8C(void *work);
WIP_LOCAL void ov02_02248DE4(void *a0, void *work);
WIP_LOCAL BOOL ov02_0224953C(void *work);
WIP_LOCAL int ov02_0224997C(void *work);
WIP_LOCAL int ov02_0224939C(void *work);
WIP_LOCAL int ov02_02249774(void *work);
WIP_LOCAL void FollowMonMovement_PlaySound(void *arg0, u16 arg1, int arg2, int arg3);
WIP_LOCAL int ov02_0224C05C(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C87C(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ov02_0224E020(SysTask *task, void *data);
WIP_LOCAL BOOL ov02_0224E640(SaveData *saveData);
WIP_LOCAL void ov02_0224DE10(Field3dObject *obj, VecFx32 *arg1, fx32 arg2, fx32 arg3);
WIP_LOCAL void FollowMonSituation_SetHiddenItems(FieldSystem *fieldSystem, void *out);
WIP_LOCAL Sprite *ov02_02248CAC(void *mgr);
WIP_LOCAL void ov02_0224A66C(void *work);
WIP_LOCAL void ov02_0224A674(void *work);
WIP_LOCAL void ov02_0224A690(void *work);
WIP_LOCAL void ov02_0224AAC8(void *a0, void *work);
WIP_LOCAL void ov02_0224ABF8(void *a0, void *work);
WIP_LOCAL void ov02_0224B2C0(void *work);
WIP_LOCAL BOOL ov02_0224B0E0(void *work);
WIP_LOCAL void ov02_0224DEA8(Field3dObject *obj, Field3dModel *model, NNSFndAllocator *allocator, void **anmResources);
WIP_LOCAL BOOL ov02_0224B43C(SysTask *task);
WIP_LOCAL BOOL MapId_IsSafariZone(int a0);
WIP_LOCAL BOOL FollowMonInteraction_StepHasMovement(void *a0, u16 *a1);
WIP_LOCAL Sprite *ov02_02248C10(void *mgr, VecFx32 *pos, int charId, int plttId, int cellId, int cellAnmId, int priority, int drawPriority);
WIP_LOCAL Sprite *ov02_02248D18(void *mgr, int a1);
WIP_LOCAL int ov02_0224C338(void *a0, void *a1, void *work);
WIP_LOCAL Sprite *ov02_0224A33C(void *mgr, VecFx32 *pos, int charId, int plttId, int cellId, int mode, int priority, int drawPriority);
WIP_LOCAL Sprite *ov02_0224A9B8(void *mgr, VecFx32 *pos);
WIP_LOCAL Sprite *ov02_0224A3F0(void *mgr, VecFx32 *pos, int drawPriority, int seq);
WIP_LOCAL void ov02_0224A570(NARC *narc, u32 fileId, NNSG2dPaletteData **a2);
WIP_LOCAL void ov02_0224A5D0(BgConfig *bgConfig, NARC *narc, u32 fileId, NNSG2dScreenData **a3);
WIP_LOCAL void ov02_0224B88C(void *work);
WIP_LOCAL void ov02_0224B90C(void *work);
WIP_LOCAL int ov02_0224B938(void *work);
WIP_LOCAL void ov02_0224B808(void *a0, void *work);
WIP_LOCAL void ov02_0224B784(void *work); // still in asm
WIP_LOCAL int ov02_0224B964(void *work);
WIP_LOCAL int ov02_0224C71C(void *a0, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ov02_RepelActiveRoamersFromMapNo(RoamerSaveData *roamerSave, u32 mapNo);
WIP_LOCAL void ov02_0224DD4C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224A598(BgConfig *bgConfig, NARC *narc, u32 fileId, NNSG2dCharacterData **a3);
WIP_LOCAL void ov02_0224B314(void *mgr);
WIP_LOCAL void FollowMon_PlaceholdersSet(void *work, void *messageFormat);
WIP_LOCAL void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);
WIP_LOCAL int ov02_0224C14C(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ov02_0224D698(Field3dObject *obj, PlayerAvatar *playerAvatar, fx32 arg2, fx32 arg3);
WIP_LOCAL int ov02_0224C7D4(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL BOOL FollowMon_TryPrintInteractionMessage(void *work, void *window, void *arg2);
WIP_LOCAL void *ov02_02249458(FieldSystem *fieldSystem, int a1, Pokemon *a2, int a3);
WIP_LOCAL int ov02_02249858(void *work);
WIP_LOCAL int ov02_0224C8D0(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C234(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
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
WIP_LOCAL int ov02_0224B664(void *work);
WIP_LOCAL int ov02_02249658(void *work);
WIP_LOCAL int ov02_0224C75C(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C93C(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C9B8(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C0B0(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ov02_0224E074(FieldSystem *fieldSystem, u16 *p_ret, int type, enum HeapID heapID);
WIP_LOCAL BOOL FollowMonRule_CheckSpeciesAttr(u32 a0, u32 a1);
WIP_LOCAL BOOL FollowMonRule_CheckShinyLeaf(u32 a0, u32 a1);
WIP_LOCAL void FollowMon_ApplyInteractionEffects(void *work);
WIP_LOCAL int ov02_022498BC(void *work);
WIP_LOCAL void ov02_0224A080(void *work, NARC *narc);
WIP_LOCAL void ov02_02249F6C(void *work);
WIP_LOCAL void ov02_0224A028(void *work);
WIP_LOCAL void ov02_02249FD4(void *work);
WIP_LOCAL BOOL FollowMonMovement_HasNextStep(void *p);
WIP_LOCAL BOOL ov02_02249088(void *mgr);
WIP_LOCAL BOOL ov02_02248D98(void *a0, void *obj);
WIP_LOCAL void ov02_0224A834(void *mgr, void *src);
WIP_LOCAL Sprite *ov02_0224A418(void *mgr, VecFx32 *pos);
WIP_LOCAL void ov02_0224D288(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);
WIP_LOCAL void ov02_0224D3B4(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);
WIP_LOCAL BOOL ov02_0224B7CC(void *a0, void **out);
WIP_LOCAL Roamer *ov02_0224BAA8(RoamerSaveData *roamerSave, int a1);
WIP_LOCAL void ov02_0224B72C(void *work);
WIP_LOCAL void ov02_0224AB58(void *work);
WIP_LOCAL void *ov02_0224B298(void *mgr, void *arg1);
WIP_LOCAL void ov02_02248E20(void *arg0);
WIP_LOCAL void *ov02_02248D58(void *arg0, void *arg1, void *arg2, void *arg3);
WIP_LOCAL void ov02_0224AA44(void *arg0, VecFx32 *pos, VecFx32 *vec, void *arg3, u32 arg4, void *arg5);
WIP_LOCAL void ov02_0224DE6C(void *obj);
WIP_LOCAL void ov02_0224D950(void *a0, void *a1, void *data);
WIP_LOCAL void ov02_0224DD8C(void *a0, void *a1, void *data);
WIP_LOCAL int ov02_02249690(void *work);
WIP_LOCAL int ov02_022496D0(void *work);
WIP_LOCAL int ov02_022491CC(void *work);
WIP_LOCAL int ov02_02249290(void *work);
WIP_LOCAL int ov02_02248F88(void *work);
WIP_LOCAL int ov02_022490BC(void *work);
WIP_LOCAL int ov02_0224B158(void *work);
WIP_LOCAL int ov02_0224AF70(void *work);
WIP_LOCAL int ov02_0224ACE0(void *work);
WIP_LOCAL int ov02_02249BD8(void *work);
WIP_LOCAL int FollowMonRule_CheckFriendship(int a0, int a1);
WIP_LOCAL int FollowMonRule_CheckMood(int a0, int a1);
WIP_LOCAL int FollowMonInteraction_RunMovement(FieldSystem *fieldSystem, void *a1, int a2);
WIP_LOCAL void ov02_0224D310(void *a0, void *a1, void *data);
WIP_LOCAL int ov02_0224C840(TaskManager *taskManager, void *a1, void *a2);
WIP_LOCAL void ov02_0224D820(void *data);
WIP_LOCAL void ov02_0224D7B0(void *data);
WIP_LOCAL void ov02_0224D73C(Field3dObject *obj, Field3dModel *model, NNSFndAllocator *allocator, void **anmResources);
WIP_LOCAL void ov02_0224D880(void *a0, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ov02_0224DF1C(void *data);
WIP_LOCAL BOOL ov02_0224BE24(TaskManager *taskManager);
WIP_LOCAL BOOL ov02_0224CE28(TaskManager *taskManager);
WIP_LOCAL BOOL PokecenterAnimRun(TaskManager *taskManager);
WIP_LOCAL int ov02_0224CAB8(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d);
WIP_LOCAL int ov02_0224CBF8(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d);
WIP_LOCAL void ov02_0224CA58(u8 *arr, int n, u8 val);
WIP_LOCAL int ov02_0224C6DC(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C2A8(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C698(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);
WIP_LOCAL BOOL ov02_0224AA80(void *a0, void *work);
WIP_LOCAL void ov02_0224B2CC(void *work);
WIP_LOCAL int ov02_0224B5F0(void *work);
WIP_LOCAL void ov02_0224D1E4(void *a0, void *a1, void *data);
WIP_LOCAL int SwapPositions_GetStepTowards(void *a, void *b);
WIP_LOCAL void FollowMonSituation_SetTerrain(FieldSystem *fieldSystem, void *work);
WIP_LOCAL void FollowMon_GetSituation(FieldSystem *fieldSystem, void *work);
WIP_LOCAL void FollowMonSituation_SetMonCondition(Pokemon *mon, void *work);
WIP_LOCAL void FollowMonSituation_SetNearbyObjects(FieldSystem *fieldSystem, void *work);
WIP_LOCAL void FollowMonSituation_SetBestPokeathlonStat(FieldSystem *fieldSystem, int species, int form, void *work);
WIP_LOCAL void ov02_02248E20(void *a0);
WIP_LOCAL void ov02_0224B784(void *work);
WIP_LOCAL BOOL FollowMonInteraction_TryShowEmote(FieldSystem *fieldSystem, void *a1, void *arg2);
WIP_LOCAL void ov02_0224B364(void *a0, void *work);
WIP_LOCAL int ov02_0224C2EC(TaskManager *taskManager, FieldSystem *fieldSystem, void *work);
WIP_LOCAL void ov02_0224D22C(void *a0, void *a1, void *data);
WIP_LOCAL void ov02_0224DAA4(void *a0, void *a1, void *data);
WIP_LOCAL void ov02_0224D9C0(void *a0, void *a1, void *data);
WIP_LOCAL void ov02_0224B3B0(void *a0, void *work);
WIP_LOCAL void ov02_0224D358(void *a0, void *a1, void *data);
WIP_LOCAL void ov02_0224CFD8(void *a0, int a1, void *data);
WIP_LOCAL void ov02_0224A88C(void *mgr, void *dst);
WIP_LOCAL void ov02_0224B6E4(void *a0, void *work);
WIP_LOCAL void ov02_0224D0C8(void *data, int a1, int a2, int a3, void *a4);
WIP_LOCAL void ov02_0224D044(void *a0, void *data);
WIP_LOCAL void ov02_0224D700(void *obj);
WIP_LOCAL void ov02_0224D914(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL BOOL ov02_0224ABCC(void *a0, void *a1);
WIP_LOCAL void *ov02_0224A468(void *a, VecFx32 *b, int c, int d); // still in asm
WIP_LOCAL BOOL FieldSystem_FirstAliveMonHasType(FieldSystem *fieldSystem, u8 a1);
WIP_LOCAL void ov02_02249E90(SysTask *task, void *work);
WIP_LOCAL void FollowMonMovement_ApplyFacing(void *a0, u8 *a1, LocalMapObject *obj);
WIP_LOCAL void FollowMon_TryShakeGrass(void *a0, LocalMapObject *obj, u8 dir);
WIP_LOCAL BOOL ov02_0224D178(void *obj);
WIP_LOCAL void ov02_022507B4(FieldSystem *fieldSystem, u8 a1);
WIP_LOCAL BOOL ov02_022507E8(TaskManager *taskManager); // still in asm
WIP_LOCAL BOOL ov02_022489F0(void *mgr, int a1);
WIP_LOCAL void ov02_02248A24(void *mgr, int a1);
WIP_LOCAL BOOL ov02_02248AC8(void *mgr, int a1);
WIP_LOCAL void ov02_02248AFC(void *mgr, int a1);
WIP_LOCAL int ov02_0224B4AC(void *work);

// forward decls for functions referenced only from the consolidated rodata tables
extern void FieldEffect_DefaultDraw(void);
extern void FieldEffect_DefaultDestroy(void);
WIP_LOCAL void ov02_02248DF0(void *a0, u8 *sm);
WIP_LOCAL int ov02_02248E10(void *work);
WIP_LOCAL int ov02_022491A8(void *work);
WIP_LOCAL int ov02_022493EC(void);
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
WIP_LOCAL void ov02_0224AB54(void);
WIP_LOCAL void ov02_0224AC04(void *a0, u8 *sm);
WIP_LOCAL void ov02_0224AC24(void);
WIP_LOCAL int ov02_0224AC28(void *work);
WIP_LOCAL int ov02_0224ADEC(void);
WIP_LOCAL int ov02_0224B294(void);
WIP_LOCAL BOOL ov02_0224B350(void *a0, void *out);
WIP_LOCAL void ov02_0224B3FC(void *a0, int *a1);
WIP_LOCAL int ov02_0224B494(void *work);
WIP_LOCAL int ov02_0224B638(void *work);
WIP_LOCAL int ov02_0224B68C(void);
WIP_LOCAL BOOL ov02_0224B6D0(void *a0, void *out);
WIP_LOCAL void ov02_0224B804(void);
WIP_LOCAL void ov02_0224B87C(void *a0, void *a1);
WIP_LOCAL int ov02_0224C1B8(TaskManager *taskManager, void *a1, void *a2);
WIP_LOCAL int ov02_0224C4B4(void *a0, FieldSystem *fieldSystem, void *work);
WIP_LOCAL int ov02_0224C4D8(void *a0, void *a1, void *work);
WIP_LOCAL int ov02_0224C680(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL int ov02_0224CA38(TaskManager *taskManager, void *a1, void *a2);
WIP_LOCAL void ov02_0224D278(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D2BC(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL Field3dObjectTask *ov02_0224D2C8(FieldSystem *fieldSystem);
WIP_LOCAL Field3dObjectTask *ov02_0224D2DC(FieldSystem *fieldSystem);
WIP_LOCAL void ov02_0224D2F0(Field3dObjectTask *task);
WIP_LOCAL BOOL ov02_0224D2F8(Field3dObjectTask *task);
WIP_LOCAL void ov02_0224D3A4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D3E8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL Field3dObjectTask *ov02_0224D3F4(FieldSystem *fieldSystem);
WIP_LOCAL Field3dObjectTask *ov02_0224D408(FieldSystem *fieldSystem);
WIP_LOCAL void ov02_0224D41C(Field3dObjectTask *task);
WIP_LOCAL BOOL ov02_0224D424(Field3dObjectTask *task);
WIP_LOCAL void ov02_0224D43C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D468(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D580(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D648(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D658(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D670(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224D98C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224DB8C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224DC58(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL Field3dObjectTask *ov02_0224DC64(FieldSystem *fieldSystem);
WIP_LOCAL Field3dObjectTask *ov02_0224DC78(FieldSystem *fieldSystem);
WIP_LOCAL void ov02_0224DC8C(Field3dObjectTask *task);
WIP_LOCAL BOOL ov02_0224DC94(Field3dObjectTask *task);
WIP_LOCAL void ov02_0224DCB0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224DD38(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
WIP_LOCAL void ov02_0224DDC8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DEF4(void *obj, NNSFndAllocator *allocator);
void ov02_0224E008(void *data);
void FollowMonSituation_SetSpeciesAttr(int index, void *out);
int FollowMonSituation_ConvertType(int index);
void FollowMonInteraction_LoadScript(void *work, int member);
void FollowMonInteraction_PrintGiftMessage(void *work, void *window, int flags);
void FollowMon_ExpandGiftMessage(void *work, String *dest, enum HeapID heapID, int flags, u8 value);

extern const VecFx32 ov02_02253B24;
extern const VecFx32 ov02_02253B30;

#endif
