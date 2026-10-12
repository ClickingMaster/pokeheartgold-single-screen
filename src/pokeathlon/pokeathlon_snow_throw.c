#include "pokeathlon/pokeathlon_snow_throw.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_event_points.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

typedef char PokeathlonSnowThrowSizeCheck[(sizeof(PokeathlonSnowThrow) == 0x5F4 && sizeof(PokeathlonSnowThrowBall) == 0x48 && sizeof(PokeathlonSnowThrowMon) == 0x20 && sizeof(PokeathlonSnowThrowPacket) == 0x28 && sizeof(PokeathlonSnowThrowLink) == 0x118 && offsetof(PokeathlonSnowThrow, balls) == 0xB8 && offsetof(PokeathlonSnowThrow, mons) == 0x418 && offsetof(PokeathlonSnowThrow, cpu) == 0x5DC && offsetof(PokeathlonSnowThrow, timer) == 0x5E8) ? 1 : -1];

// fx_mtx43.c (not declared in the fx headers here)
void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *mtx, VecFx32 *result);

// Sprite pool helpers (pokeathlon_course_objects.c), declared with untyped entries as the course objects use them.
void PokeathlonCourseSpritePool_Draw(void *pool);
void PokeathlonCourseSpritePool_Free(void *pool);
void *PokeathlonCourseSpritePool_New(enum HeapID heapId, VecFx32 *template);
void PokeathlonCourseSpritePool_SetSurfaceCoords(void *pool, fx32 mainX, fx32 mainY, fx32 subX, fx32 subY);
void PokeathlonCourseSpritePool_SetResourceId(void *pool, int index, u32 id);
void PokeathlonCourseSpritePool_TransferResources(void *pool);
void PokeathlonCourseSpritePool_PauseAnims(void *pool, BOOL paused);
void PokeathlonCourseSpritePool_LoadChar(void *pool, int narcId, int fileId, int resId, int vram);
void PokeathlonCourseSpritePool_LoadPltt(void *pool, int narcId, int fileId, int resId, int vram, int count);
void PokeathlonCourseSpritePool_LoadCell(void *pool, int narcId, int fileId, int resId);
void PokeathlonCourseSpritePool_LoadAnim(void *pool, int narcId, int fileId, int resId);
void *PokeathlonCourseSpritePool_CreateAndTake(void *pool, int priority, int whichScreen, int resId, int kind);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *entry, int seq);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(void *entry, int priority);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *entry, VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(void *entry);
void PokeathlonCourseSpritePoolSprite_SetScale(void *entry, VecFx32 *scale, int affineType);
void PokeathlonCourseObjects_PauseAnims(void *objects, BOOL paused);
void PokeathlonCourseObject_SetGroundPosition(void *object, s32 x, s32 y);

extern const VecFx32 ov96_0221C7B8; // sprite pool template

// The animation state (facing) of each other team's Pokemon, by its place in the view.
typedef struct PokeathlonSnowThrowViewAnims {
    u32 animStates[SNOW_THROW_TEAMS - 1];
} PokeathlonSnowThrowViewAnims;

// Where the marks of the other teams' colours are on the field screen.
typedef struct PokeathlonSnowThrowTeamMarkPositions {
    VecFx32 positions[SNOW_THROW_TEAMS - 1];
} PokeathlonSnowThrowTeamMarkPositions;

extern const PokeathlonSnowThrowViewAnims ov96_0221C7D0;
extern const VecFx32 ov96_0221C7C4;
extern const s32 ov96_0221C8C8[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
extern const s32 ov96_0221C8EC[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
extern const u8 ov96_0221CA1C[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
extern const GraphicsBanks ov96_0221C934;
extern const GraphicsModes ov96_0221C7DC;
extern const BgTemplate ov96_0221C83C;
extern const BgTemplate ov96_0221C858;
extern const BgTemplate ov96_0221C890;
extern const BgTemplate ov96_0221C8AC;
extern const BgTemplate ov96_0221C804;
extern const BgTemplate ov96_0221C820;
extern const BgTemplate ov96_0221C874;
extern const PokeathlonSnowThrowTeamMarkPositions ov96_0221C910;
// Where each Pokemon stands on the field (fx32 field units, unturned).
extern const VecFx32 ov96_0221C98C[SNOW_THROW_MONS];
extern const WindowTemplate ov96_0221C7B0;
extern const WindowTemplate ov96_0221C7EC[SNOW_THROW_TEAMS - 1];

// The Pokemon the k-th of the other teams' nine is (they are listed from the team after the own one on).
static inline u8 PokeathlonSnowThrow_OtherMon(int own, int k) {
    return ((u8)(((own + 1) * SNOW_THROW_MEMBERS) % SNOW_THROW_MONS) + k) % SNOW_THROW_MONS;
}

// Set-up, a step per frame (PokeathlonCourse_GetField1ED): heaps and screens, the top screen and the CPU,
// sprites, the Pokemon graphics, then the field once they are loaded.
#pragma force_active on

BOOL PokeathlonSnowThrow_Init(PokeathlonCourseData *course) {
    PokeathlonCourseMonInfo infos[SNOW_THROW_MONS];
    PokeathlonCourseMonGfxTemplate template;
    s32 stats[4][5];
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrow *data;
    int participant;
    int i;
    int k;
    void *object;
    u8 mon;
    int x;
    int team;
    PokeathlonSnowThrowMon *teamMon;
    int first;
    int count;
    int own;
    int member;
    int y, j;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0: {
        ObjCharTransferTemplate charTransfer;

        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_146, 0x40000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        PokeathlonSnowThrow_SetGfxBanks();
        data = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonSnowThrow));
        MI_CpuFill8(data, 0, sizeof(PokeathlonSnowThrow));
        data->sortBuffer = Heap_Alloc(HEAP_ID_146, 40);
        MI_CpuFill8(data->sortBuffer, 0, 40);
        data->bgConfig = BgConfig_Alloc(HEAP_ID_146);
        PokeathlonCourse_SetHudPlanes(course, 8);
        charTransfer.maxTasks = 115;
        charTransfer.sizeMain = 0x40000;
        charTransfer.sizeSub = 0x4000;
        charTransfer.heapID = PokeathlonCourse_GetHeapID(course);
        PokeathlonCourse_InitObjTransfer(&charTransfer, 22, HEAP_ID_146, GX_OBJVRAMMODE_CHAR_1D_256K, GX_OBJVRAMMODE_CHAR_1D_256K);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_146);
        data->heapId = HEAP_ID_146;
        FontID_Alloc(4, HEAP_ID_146);
        PokeathlonSnowThrow_InitBgs(data->bgConfig);
        PokeathlonSnowThrow_InitWindows(data);
        PokeathlonSnowThrow_PrintNames(data, course);
        gSystem.screensFlipped = FALSE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 1:
        work->display = PokeathlonSnowThrowDisplay_New(work->heapId, work->bgConfig, course);
        count = PokeathlonCourse_GetParticipantCount(course);
        work->cpu = PokeathlonSnowThrowCpu_New(work->heapId, SNOW_THROW_TEAMS - count, PokeathlonCourse_GetMode(course));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2: {
        VecFx32 poolTemplate = ov96_0221C7B8;

        work->spritePool = PokeathlonCourseSpritePool_New(work->heapId, &poolTemplate);
        PokeathlonCourseSpritePool_SetSurfaceCoords(work->spritePool, 0, 0, 0, FX32_CONST(288));
        PokeathlonCourseSpritePool_SetResourceId(work->spritePool, 0, 106);
        PokeathlonCourseSpritePool_SetResourceId(work->spritePool, 1, 101);
        PokeathlonCourseSpritePool_SetResourceId(work->spritePool, 2, 105);
        PokeathlonSnowThrow_LoadSpriteGraphics(work->spritePool);
        PokeathlonSnowThrowDisplay_LoadSpriteGraphics(work->display, work->spritePool);
        PokeathlonCourseSpritePool_TransferResources(work->spritePool);
        work->monResources = PokeathlonCourseSprites_CreateResources(work->heapId, 1255, 1);
        work->monObjectList = PokeathlonCourseMonObjects_Create(work->heapId, 9, 32, work->monResources, PokeathlonCourseSpritePool_GetSpriteList(work->spritePool));
        Sprite_SetDrawPriority(*ov96_021E6290(course, 0, work->monResources, work->spritePool), 1);
        own = ov96_021E5F24(course);
        PokeathlonSnowThrow_CreateSprites(work, work->spritePool, own);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 3:
        // the other teams' Pokemon first, the own three last
        own = (u8)ov96_021E5F24(course);
        for (i = 0; i < SNOW_THROW_MONS; i++) {
            count = PokeathlonSnowThrow_OtherMon(own, i);
            member = count % SNOW_THROW_MEMBERS;
            participant = count / SNOW_THROW_MEMBERS;
            PokeathlonCourse_GetMonInfo(course, participant, member, &infos[i]);
            template.sizeTypes[i] = ov96_021E6108((PokeathlonCourseUnkTypes *)PokeathlonCourse_GetPerformance(course, participant, member));
        }
        PokeathlonSnowThrowDisplay_CreateSprites(work->display, work->spritePool, &infos[9]);
        for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
            VecFx32 position = { 0, 0, 0 };
            PokeathlonCourseMonInfo *info = &infos[9 + i];
            u8 height = GetMonPicHeightBySpeciesGenderForm(info->species, info->gender, 0, info->form, info->personality);
            position.x = (16 + i * 50) << FX32_SHIFT;
            position.y = (height + 376) << FX32_SHIFT;
            position.y -= FX32_ONE * 16;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->mons[own * SNOW_THROW_MEMBERS + i].stunSprite, &position);
        }
        template.loadAtOnce = FALSE;
        template.priority = 3;
        template.vramTransfer = FALSE;
        template.vram = 1;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        PokeathlonCourseMonObjects_Load(work->monObjectList, 9, infos, &template, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 4:
        if (!PokeathlonCourseMonObjects_IsLoaded(work->monObjectList)) {
            break;
        } else {
            PokeathlonSnowThrowViewAnims anims = ov96_0221C7D0;

            PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)work->bgConfig); // takes the BgConfig
            PokeathlonCourse_SetField1F4(course, 1);
            own = ov96_021E5F24(course);
            for (k = 0; k < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; k++) {
                work->monObjects[k] = PokeathlonCourseMonObjects_Get(work->monObjectList, (u8)k);
                object = work->monObjects[k];
                PokeathlonCourseMonObject_SetDrawFlag(object, TRUE);
                mon = ((u8)(((own + 1) * SNOW_THROW_MEMBERS) % SNOW_THROW_MONS) + k) % SNOW_THROW_MONS; // PokeathlonSnowThrow_OtherMon
                x = ov96_0221C8C8[k];
                y = ov96_0221C8EC[k];
                PokeathlonCourseMonObject_SetAnimState(object, anims.animStates[k / SNOW_THROW_MEMBERS]);
                PokeathlonCourseObject_SetGroundPosition(object, x, y);
                {
                    VecFx32 markPosition = { 0, 0, 0 };
                    if (PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(object))->isLarge) {
                        markPosition.x = x << FX32_SHIFT;
                        markPosition.y = (y - 40) << FX32_SHIFT;
                    } else {
                        markPosition.x = x << FX32_SHIFT;
                        markPosition.y = (y - 25) << FX32_SHIFT;
                    }
                    PokeathlonCourseSpritePoolSprite_SetPosition(work->mons[mon].stunSprite, &markPosition);
                    PokeathlonCourseSpritePoolSprite_SetPosition(work->tiredMarks[k], &markPosition);
                }
                PokeathlonCourseMonObject_SetDrawPriority(object, ov96_0221CA1C[k] + 32);
            }
            ReadWholeNarcMemberByIdPair(stats, NARC_a_1_7_0, 3);
            work->timer = SNOW_THROW_DURATION;
            for (team = 0, first = 0; team < SNOW_THROW_TEAMS; first += SNOW_THROW_MEMBERS, team++) {
                work->heldBall[team] = SNOW_THROW_NONE;
                for (j = 0; j < SNOW_THROW_MEMBERS; j++) {
                    teamMon = &work->mons[j + first];
                    PokeathlonSnowThrow_InitMon(course, stats, team, j, teamMon);
                    if (ov96_021E5F24(course) == 0) {
                        PokeathlonSnowThrowCpu_SetMon(work->cpu, j + first, teamMon, &work->balls[j + first]);
                    }
                }
            }
            if (ov96_021E5F24(course) == 0) {
                PokeathlonSnowThrow_PackPacket(work, (PokeathlonSnowThrowPacket *)ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course) + offsetof(PokeathlonSnowThrowLink, packetToSend)));
            }
            PokeathlonSnowThrowDisplay_SetMonShadows(work->display, work->spritePool, work->monResources);
            PokeathlonSnowThrow_LoadBgGraphics(work, ov96_021E5F24(course));
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            WirelessStatusIcon_CreateOnScreenIfConnected(1);
            BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, work->heapId);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            GF_ASSERT(GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonSnowThrow_Draw(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonSnowThrowDisplay_UpdateShake(work->display);
    PokeathlonCourseSpritePool_Draw(work->spritePool);
    return TRUE;
}

// Results: the hits each team scored.
BOOL PokeathlonSnowThrow_ShowResults(PokeathlonCourseData *course) {
    void *gfx = PokeathlonCourse_GetGraphicsSystem(course);
    int i;
    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        PokeathlonCourse_AddScoreEntry(course, gfx, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, msg_0309_00299);
    }
    PokeathlonCourseScoreList_Rank(gfx, 1);
    PokeathlonEvent_CalcPoints(gfx, 8);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 8);
    return TRUE;
}

#pragma force_active on

BOOL PokeathlonSnowThrow_Exit(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;
    Window *window;

    WirelessStatusIcon_Destroy();
    PokeathlonCourse_ResetField3A4(course);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_3);
    RemoveWindow(&work->scoreWindow);
    for (i = 0, window = work->nameWindows; i < SNOW_THROW_TEAMS - 1; i++, window++) {
        RemoveWindow(window);
    }
    Heap_Free(work->bgConfig);
    PokeathlonCourseSpritePool_Free(work->spritePool);
    PokeathlonCourseMonObjects_Free(work->monObjectList);
    PokeathlonCourseSprites_FreeResources(work->monResources);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    PokeathlonSnowThrowCpu_Delete(work->cpu);
    PokeathlonSnowThrowDisplay_Delete(work->display);
    FontID_Release(4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    Heap_Free(work->sortBuffer);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_146);
    return TRUE;
}

#pragma force_active reset

void PokeathlonSnowThrow_SetGfxBanks(void) {
    GraphicsBanks banks = ov96_0221C934;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on

void PokeathlonSnowThrow_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main0, main1, main3, sub0, sub1, sub2, sub3;

    modes = ov96_0221C7DC;
    SetBothScreensModesAndDisable(&modes);
    main0 = ov96_0221C83C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main1 = ov96_0221C858;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main3 = ov96_0221C890;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = ov96_0221C8AC;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = ov96_0221C804;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    sub2 = ov96_0221C820;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    sub3 = ov96_0221C874;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active reset

// The field (turned for the own team) and the top screen.
void PokeathlonSnowThrow_LoadBgGraphics(PokeathlonSnowThrow *work, u8 own) {
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 1, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x4000, FALSE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 2, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_1_7, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, work->heapId);
    PokeathlonSnowThrowDisplay_LoadBgGraphics(work->display, own);
}

void PokeathlonSnowThrow_LoadSpriteGraphics(void *pool) {
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_1_7, 21, 106, 3);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_1_7, 18, 106, 3, 2);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_1_7, 20, 106);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_1_7, 19, 106);
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_1_7, 17, 105, 1);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_1_7, 14, 105, 1, 3);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_1_7, 16, 105);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_1_7, 15, 105);
}

#pragma push
#pragma opt_common_subs off

#pragma force_active on

void PokeathlonSnowThrow_CreateSprites(PokeathlonSnowThrow *work, void *pool, u8 own) {
    int i;
    u8 anim;
    VecFx32 position;
    VecFx32 scale;
    VecFx32 framePosition;
    PokeathlonSnowThrowTeamMarkPositions teamMarkPositions;

    for (i = 0; i < 2; i++) {
        work->timerDigits[i] = ov96_021EA374(work->monResources, PokeathlonCourseSpritePool_GetSpriteList(pool), 3, work->heapId);
        Sprite_SetDrawFlag(work->timerDigits[i], TRUE);
        position.x = (i * 16) * FX32_ONE + 24 * FX32_ONE;
        position.y = 8 * FX32_ONE;
        position.z = 0;
        Sprite_SetMatrix(work->timerDigits[i], &position);
        Sprite_SetDrawPriority(work->timerDigits[i], 2);
    }
    PokeathlonSnowThrow_SetTimerDigits(work, SNOW_THROW_DURATION / 30);
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        work->balls[i].index = i;
        work->balls[i].sprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 3);
        work->balls[i].shadowSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 4);
        work->balls[i].splashSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 6);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->balls[i].shadowSprite, 1);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[i].sprite, 20);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[i].shadowSprite, 1000);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[i].splashSprite, 5);
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        work->mons[i].stunSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 9);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->mons[i].stunSprite, 2);
        scale.x = FX32_ONE;
        scale.y = FX32_ONE;
        scale.z = 0;
        PokeathlonCourseSpritePoolSprite_SetPosition(work->mons[i].stunSprite, &scale);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->mons[i].stunSprite, 4);
    }
    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        work->hitMarks[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, 1, 105, 11);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->hitMarks[i], 6);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->hitMarks[i], 2);
    }
    work->frameSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 1, 105, 7);
    framePosition.x = 8 * FX32_ONE;
    framePosition.y = 8 * FX32_ONE;
    framePosition.z = 0;
    PokeathlonCourseSpritePoolSprite_SetPosition(work->frameSprite, &framePosition);
    PokeathlonCourseSpritePoolSprite_SetAnim(work->frameSprite, 5);
    PokeathlonCourseSpritePoolSprite_Show(work->frameSprite, TRUE, TRUE);
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->frameSprite, 2);
    teamMarkPositions = ov96_0221C910;
    anim = (own + 1) % SNOW_THROW_TEAMS;
    for (i = 0; i < SNOW_THROW_TEAMS - 1; i++) {
        work->teamMarks[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 1, 105, 8);
        PokeathlonCourseSpritePoolSprite_SetPosition(work->teamMarks[i], &teamMarkPositions.positions[i]);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->teamMarks[i], anim + 1);
        PokeathlonCourseSpritePoolSprite_Show(work->teamMarks[i], TRUE, TRUE);
        anim = (anim + 1) % SNOW_THROW_TEAMS;
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->teamMarks[i], 3);
    }
    for (i = 0; i < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; i++) {
        work->tiredMarks[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 1, 105, 10);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->tiredMarks[i], 0);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->tiredMarks[i], 4);
    }
}

#pragma force_active reset

#pragma pop

BOOL PokeathlonSnowThrow_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            (*state)++;
        }
        break;
    case 1:
        PokeathlonSnowThrow_UpdateHost(course);
        if (PokeathlonSnowThrow_Update(course)) {
            (*state)++;
        }
        break;
    case 2:
        PokeathlonSnowThrow_UpdateHost(course);
        if (PokeathlonCourse_RunFinish(course)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    }
    return FALSE;
}

// The host stores everyone's score (the hits of the team) for the results.
BOOL PokeathlonSnowThrow_SaveScores(PokeathlonCourseData *course, u8 *state) {
    PokeathlonSnowThrowLink *link = (PokeathlonSnowThrowLink *)PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonSnowThrowPacket *packet;
    int i;
    PokeathlonCourse_GetHeapAllocPtr4(course);
    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    if (ov96_021E5F24(course)) {
        return TRUE;
    }
    packet = (PokeathlonSnowThrowPacket *)ov96_021E8A20(&link->packetToSend);
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        PokeathlonCourse_SetField5E0_AtIndex(course, (u8)i, packet->teams[i] & 0x3FF);
    }
    return TRUE;
}

// Host: takes everyone's stylus (a touch on an own Pokemon starts a snowball, more taps on it make it bigger, a
// flick away from it throws it), steps the snowballs, the Pokemon and the CPU, and sends the packet.
#pragma force_active on

void PokeathlonSnowThrow_UpdateHost(PokeathlonCourseData *course) {
    VecFx32 touched;
    VecFx32 touchStart;
    VecFx32 released;
    int i;
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrowTouchBlock *block;
    PokeathlonSnowThrowTouch *touch;
    int ball;
    f32 energyFactor;
    f32 speed;
    f32 hold;
    f32 strength;

    block = (PokeathlonSnowThrowTouchBlock *)PokeathlonCourse_GetDataCopyArea(course);
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (ov96_021E5F24(course) != 0) {
        return;
    }
    if (work->finished) {
        block++;
        PokeathlonSnowThrow_PackPacket(work, (PokeathlonSnowThrowPacket *)ov96_021E8A20(block));
        return;
    }
    // the host's own stylus goes into its slot of everyone's
    *(PokeathlonSnowThrowTouch *)ov96_021E8A20(&block[2]) = *(PokeathlonSnowThrowTouch *)ov96_021E8A20(block);
    if (work->timer != 0) {
        work->timer--;
    }
    ov96_021E8A20(&block[1]);
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        touch = (PokeathlonSnowThrowTouch *)ov96_021E8A20(&(block + 2)[i]);
        if (touch->held) {
            if (work->presses[i].pressed && work->presses[i].held) {
                work->presses[i].pressed = FALSE;
            } else if (work->presses[i].pressed == FALSE && work->presses[i].held == FALSE) {
                work->presses[i].pressed = TRUE;
                work->presses[i].held = TRUE;
            }
        } else {
            work->presses[i].pressed = FALSE;
            work->presses[i].held = FALSE;
        }
        if (work->presses[i].pressed) {
            ball = PokeathlonSnowThrow_FindOwnMonAt(work, i, touch->x, touch->y, &touched);
            if (ball != SNOW_THROW_NONE) {
                if (work->mons[ball].stunTimer != 0) {
                    continue;
                }
                if (work->balls[ball].state == SNOW_THROW_BALL_NONE || work->balls[ball].state == SNOW_THROW_BALL_HELD) {
                    work->heldBall[i] = ball;
                    work->balls[ball].dragStart = touched;
                    touchStart.x = touch->x << FX32_SHIFT;
                    touchStart.z = touch->y << FX32_SHIFT;
                    touchStart.y = 0;
                    work->balls[ball].touchStart = touchStart;
                    if (work->balls[ball].state == SNOW_THROW_BALL_NONE) {
                        work->balls[ball].position = ov96_0221C98C[ball];
                        work->balls[ball].state = SNOW_THROW_BALL_HELD;
                        work->balls[ball].taps = 0;
                        work->balls[ball].size = 0;
                    }
                }
            }
            work->holdFrames = 0;
        } else if (work->presses[i].held) {
            if (work->heldBall[i] != SNOW_THROW_NONE && work->holdFrames < 30.0) {
                work->holdFrames++;
            }
        } else {
            if (work->heldBall[i] != SNOW_THROW_NONE) {
                ball = PokeathlonSnowThrow_FindOwnMonAt(work, i, touch->x, touch->y, &released);
                if (ball != SNOW_THROW_NONE && ball == work->heldBall[i]) {
                    // released on the Pokemon: one more tap
                    if (work->balls[ball].taps < 4) {
                        work->balls[ball].taps++;
                        if (work->balls[ball].taps <= 1) {
                            work->balls[ball].size = 0;
                        } else if (work->balls[ball].taps <= 2) {
                            work->balls[ball].size = 1;
                        } else if (work->balls[ball].taps <= 3) {
                            work->balls[ball].size = 2;
                        } else {
                            work->balls[ball].size = 3;
                        }
                    }
                } else {
                    // released away from it: a throw, if the stylus went up the screen
                    VecFx32 zero = { 0, 0, 0 };
                    VecFx32 drag;
                    VecFx32 touchDrag;
                    VecFx32 touchEnd;
                    int held = work->heldBall[i];

                    VEC_Subtract(&released, &work->balls[held].dragStart, &drag);
                    touchEnd.x = touch->x << FX32_SHIFT;
                    touchEnd.z = touch->y << FX32_SHIFT;
                    touchEnd.y = 0;
                    VEC_Subtract(&touchEnd, &work->balls[held].touchStart, &touchDrag);
                    if (touchDrag.z <= 0) {
                        work->balls[held].velocity = drag;
                        if (work->mons[held].energy <= 60) {
                            energyFactor = 0.5f;
                        } else if (work->mons[held].energy <= 70) {
                            energyFactor = 0.6f;
                        } else if (work->mons[held].energy <= 80) {
                            energyFactor = 0.7f;
                        } else if (work->mons[held].energy <= 90) {
                            energyFactor = 0.8f;
                        } else {
                            energyFactor = 1.0f;
                        }
                        speed = VEC_Mag(&work->balls[held].velocity) / FX32_ONE;
                        if (speed > 2.0) {
                            speed = 2.0f;
                        } else if (speed < 1.0) {
                            speed = 1.0f;
                        }
                        hold = 1.0 + (f32)work->holdFrames / 30.0;
                        strength = energyFactor * (speed * (2.0 - work->mons[held].throwWeight)) / hold;
                        if (strength > 3.0) {
                            strength = 3.0f;
                        } else if (strength < 1.0) {
                            strength = 1.0f;
                        }
                        VEC_Normalize(&work->balls[held].velocity, &work->balls[held].velocity);
                        VEC_MultAdd(4096.0f * strength, &work->balls[held].velocity, &zero, &work->balls[held].velocity);
                        PokeathlonSnowThrow_AddAimSpread((u8)work->mons[held].aimSpread, &work->balls[held].velocity);
                        work->balls[held].state = SNOW_THROW_BALL_FLYING;
                    }
                }
                work->heldBall[i] = SNOW_THROW_NONE;
            }
            work->holdFrames = 0;
        }
    }
    PokeathlonSnowThrowCpu_Update(work->cpu, SNOW_THROW_DURATION - work->timer);
    PokeathlonSnowThrow_UpdateMons(course);
    PokeathlonSnowThrow_MoveBalls(course);
    PokeathlonSnowThrow_CollideBalls(work);
    PokeathlonSnowThrow_HitMons(course, work);
    if (work->timer == 0) {
        work->finished = TRUE;
    } else {
        work->finished = FALSE;
    }
    block++;
    PokeathlonSnowThrow_PackPacket(work, (PokeathlonSnowThrowPacket *)ov96_021E8A20(block));
}

#pragma force_active reset

// Host: packs what everyone draws (see PokeathlonSnowThrowPacket).
#pragma force_active on

void PokeathlonSnowThrow_PackPacket(PokeathlonSnowThrow *work, PokeathlonSnowThrowPacket *packet) {
    u8 sizes[SNOW_THROW_TEAMS] = { 0, 0, 0, 0 };
    u16 hits[SNOW_THROW_TEAMS] = { 0, 0, 0, 0 };
    int i;
    u32 states = 0;
    u8 hit, wasHit;
    int shift;
    int x, z, team;
    u8 *ballBytes, *monBytes, *packetBytes;

    // (steps byte pointers through the snowballs, the Pokemon and the packet, as built)
#define BALL ((PokeathlonSnowThrowBall *)(ballBytes + offsetof(PokeathlonSnowThrow, balls)))
#define MON  ((PokeathlonSnowThrowMon *)(monBytes + offsetof(PokeathlonSnowThrow, mons)))
    for (i = 0, shift = 0, ballBytes = (u8 *)work, monBytes = (u8 *)work, packetBytes = (u8 *)packet; i < SNOW_THROW_MONS; shift += 2, ballBytes += sizeof(PokeathlonSnowThrowBall), packetBytes += sizeof(u16), monBytes += sizeof(PokeathlonSnowThrowMon), i++) {
        states |= (u32)BALL->state << shift;
        wasHit = MON->wasHit;
        hit = BALL->hit != 0 ? 1 : 0;
        BALL->hit = FALSE;
        x = BALL->position.x / FX32_ONE;
        z = BALL->position.z / FX32_ONE;
        if (x < 1) {
            x = 1;
        } else if (x > 64) {
            x = 64;
        }
        if (z < 1) {
            z = 1;
        } else if (z > 64) {
            z = 64;
        }
        team = i / SNOW_THROW_MEMBERS;
        sizes[team] += BALL->size << ((i % SNOW_THROW_MEMBERS) * 2);
        {
            int value = (x - 1) + ((z - 1) << 6) + (wasHit << 12) + (hit << 13);
            value += MON->condition << 14;
            *(u16 *)(packetBytes + offsetof(PokeathlonSnowThrowPacket, mons)) = value;
        }
        hits[team] += MON->hits;
        if (hits[team] > 999) {
            hits[team] = 999;
        }
    }
#undef BALL
#undef MON
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        packet->teams[i] = (sizes[i] << 10) + hits[i];
    }
    packet->flags = states;
    packet->flags = states + ((u32)work->finished << 24);
    packet->flags += ((u32)((work->timer + 30) / 30)) << 25;
}

#pragma force_active reset

// Sends the own stylus to the host. TRUE when the host says the event is over.
#pragma force_active on

BOOL PokeathlonSnowThrow_Update(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrowLink *link;
    PokeathlonSnowThrowTouch *touch;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    link = (PokeathlonSnowThrowLink *)PokeathlonCourse_GetDataCopyArea(course);
    touch = (PokeathlonSnowThrowTouch *)ov96_021E8A20(link);
    touch->held = FALSE;
    {
        PokeathlonSnowThrowPacket *packet = (PokeathlonSnowThrowPacket *)ov96_021E8A20(&link->packet);
        u8 finished = (packet->flags >> 24) & 1;
        if (finished) {
            PokeathlonCourseSpritePool_PauseAnims(work->spritePool, TRUE);
            PokeathlonCourseObjects_PauseAnims(work->monObjectList, TRUE);
            PokeathlonCourse_SetFinishCallback(course, PokeathlonSnowThrow_ShowOwnScore);
            return TRUE;
        }
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        touch->x = gSystem.touchX;
        touch->y = gSystem.touchY;
        touch->held = TRUE;
    }
    PokeathlonSnowThrow_DrawPacket(course);
    return FALSE;
}

#pragma force_active reset

// Every player: draws the received packet, the field turned so that the own team is at the bottom.
#pragma force_active on

void PokeathlonSnowThrow_DrawPacket(PokeathlonCourseData *course) {
    VecFx32 hitPosition;
    MtxFx43 hitRotation;
    VecFx32 hitFrom;
    VecFx32 hitTurned;
    VecFx32 ballPosition;
    MtxFx43 ballRotation;
    VecFx32 ballFrom;
    VecFx32 ballTurned;
    VecFx32 scale;
    int hitX;
    int hitY;
    int screenX;
    int screenY;
    int cellX;
    int cellZ;
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrowPacket *packet;
    u8 seconds;
    u8 condition;
    u8 wasHit;
    u8 ballState;
    int ballX;
    int ballZ;
    u8 own;
    int team;
    int turns;
    const fx16 *sinCos;
    int y;
    int member;
    int k;
    u8 mon;
    u8 state;
    u8 size;
    int x;
    int z;
    int i;
    int done;
    f32 factor;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    packet = (PokeathlonSnowThrowPacket *)ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course) + offsetof(PokeathlonSnowThrowLink, packet));
    own = ov96_021E5F24(course);
    seconds = (packet->flags >> 25) & 0x7F;
    PokeathlonSnowThrow_SetTimerDigits(work, seconds == 0 ? 0 : (u8)(seconds - 1));
    PokeathlonSnowThrowDisplay_SetScore(work->display, packet->teams[own] & 0x3FF);
    // where the own snowballs hit someone
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        if ((u8)((packet->mons[i] >> 13) & 1) && own == i / SNOW_THROW_MEMBERS) {
            PokeathlonCourseSpritePoolSprite_Show(work->hitMarks[(u8)(i % SNOW_THROW_MEMBERS)], TRUE, TRUE);
            PokeathlonCourseSpritePoolSprite_SetAnim(work->hitMarks[(u8)(i % SNOW_THROW_MEMBERS)], 6);
            x = packet->mons[i] & 0x3F;
            z = (packet->mons[i] >> 6) & 0x3F;
            x++;
            z++;
            if (own != 0) {
                turns = SNOW_THROW_TEAMS - own;
                hitFrom.x = x << FX32_SHIFT;
                hitFrom.z = z << FX32_SHIFT;
                hitFrom.y = 0;
                hitFrom.x -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                hitFrom.z -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                sinCos = &FX_SinCosTable_[(((u8)turns * 0x4000) >> 4) << 1];
                MTX_RotY43_(&hitRotation, sinCos[0], sinCos[1]);
                MTX_MultVec43(&hitFrom, &hitRotation, &hitTurned);
                hitTurned.x += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                hitTurned.z += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                x = hitTurned.x / FX32_ONE;
                z = hitTurned.z / FX32_ONE;
            }
            PokeathlonSnowThrow_FieldToScreen(x, z, &hitX, &hitY);
            hitPosition.x = hitX << FX32_SHIFT;
            hitPosition.y = hitY << FX32_SHIFT;
            hitPosition.z = 0;
            hitPosition.y = (hitY - 8 - hitY / 20) << FX32_SHIFT;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->hitMarks[(u8)(i % SNOW_THROW_MEMBERS)], &hitPosition);
            PlaySE(SEQ_SE_GS_POINT3);
        }
    }
    // the own team, on the top screen
    for (k = 0; k < SNOW_THROW_MEMBERS; k++) {
        mon = own * SNOW_THROW_MEMBERS + k;
        condition = (packet->mons[mon] >> 14) & 3;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, TRUE);
            PokeathlonSnowThrowDisplay_ShowTired(work->display, k, FALSE);
        } else if (condition == SNOW_THROW_TIRED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonSnowThrowDisplay_ShowTired(work->display, k, TRUE);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonSnowThrowDisplay_ShowTired(work->display, k, FALSE);
        }
        wasHit = (packet->mons[mon] >> 12) & 1;
        ballState = (packet->flags >> (mon * 2)) & 3;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 10);
            work->mons[mon].animTimer = 10;
            if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                PlaySE(SEQ_SE_GS_W146);
            }
            if (wasHit) {
                PlaySE(SEQ_SE_GS_TAMA);
            }
        } else if (wasHit) {
            PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 7);
            work->mons[mon].animTimer = 7;
            PlaySE(SEQ_SE_GS_TAMA);
            PokeathlonSnowThrowDisplay_Shake(work->display);
        } else if (ballState == SNOW_THROW_BALL_FLYING && work->mons[mon].throwShown == 0) {
            if (condition == SNOW_THROW_STUNNED) {
                GF_AssertFail();
            }
            PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 4);
            work->mons[mon].animTimer = 4;
            work->mons[mon].throwShown = 1;
            if (work->unk5EF[k] == 0) {
                PlaySE(SEQ_SE_GS_FW104);
            }
        } else {
            done = FALSE;
            if (work->mons[mon].animTimer == 10) {
                done = TRUE;
            } else if (!PokeathlonSnowThrowDisplay_IsPicAnimated(work->display, k)) {
                done = TRUE;
            }
            if (done) {
                PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 1);
                work->mons[mon].animTimer = 1;
            }
        }
        if (ballState != SNOW_THROW_BALL_FLYING) {
            work->mons[mon].throwShown = 0;
        }
    }
    // the other teams' Pokemon on the field
    for (k = 0; k < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; k++) {
        mon = PokeathlonSnowThrow_OtherMon(own, k);
        condition = (packet->mons[mon] >> 14) & 3;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, TRUE);
            PokeathlonCourseSpritePoolSprite_Show(work->tiredMarks[k], TRUE, FALSE);
        } else if (condition == SNOW_THROW_TIRED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_Show(work->tiredMarks[k], TRUE, TRUE);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_Show(work->tiredMarks[k], TRUE, FALSE);
        }
        wasHit = (packet->mons[mon] >> 12) & 1;
        ballState = (packet->flags >> (mon * 2)) & 3;
        cellX = packet->mons[mon] & 0x3F;
        cellZ = (packet->mons[mon] >> 6) & 0x3F;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 20);
            work->mons[mon].animTimer = 20;
            if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                PlaySE(SEQ_SE_GS_W146);
            }
        } else if (wasHit) {
            PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 22);
            work->mons[mon].animTimer = 22;
        } else if (ballState == SNOW_THROW_BALL_FLYING && cellX + 1 == ov96_0221C98C[mon].x / FX32_ONE && cellZ + 1 == ov96_0221C98C[mon].z / FX32_ONE) {
            // just thrown
            if (condition == SNOW_THROW_STUNNED) {
                GF_AssertFail();
            }
            PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 16);
            work->mons[mon].animTimer = 16;
        } else {
            done = FALSE;
            if (work->mons[mon].animTimer == 20) {
                done = TRUE;
            } else if (!PokeathlonCourseMonObject_IsAnimated(work->monObjects[k])) {
                done = TRUE;
            }
            if (done) {
                PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 0);
                work->mons[mon].animTimer = 0;
            }
        }
    }
    // the snowballs
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        state = (packet->flags >> (i * 2)) & 3;
        if (state == SNOW_THROW_BALL_FLYING || state == SNOW_THROW_BALL_HELD) {
            work->balls[i].splashShown = FALSE;
            if (state != SNOW_THROW_BALL_HELD) {
                PokeathlonCourseSpritePoolSprite_Show(work->balls[i].shadowSprite, TRUE, TRUE);
            } else {
                PokeathlonCourseSpritePoolSprite_Show(work->balls[i].shadowSprite, TRUE, FALSE);
            }
            PokeathlonCourseSpritePoolSprite_Show(work->balls[i].sprite, TRUE, TRUE);
            team = i / SNOW_THROW_MEMBERS;
            member = i % SNOW_THROW_MEMBERS;
            size = ((u8)((packet->teams[i / SNOW_THROW_MEMBERS] >> 10) & 0x3F) >> (member * 2)) & 3;
            ballX = (packet->mons[i] & 0x3F) + 1;
            ballZ = ((packet->mons[i] >> 6) & 0x3F) + 1;
            if (own != 0) {
                turns = SNOW_THROW_TEAMS - own;
                ballFrom.x = ballX << FX32_SHIFT;
                ballFrom.z = ballZ << FX32_SHIFT;
                ballFrom.y = 0;
                ballFrom.x -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                ballFrom.z -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                sinCos = &FX_SinCosTable_[(((u8)turns * 0x4000) >> 4) << 1];
                MTX_RotY43_(&ballRotation, sinCos[0], sinCos[1]);
                MTX_MultVec43(&ballFrom, &ballRotation, &ballTurned);
                ballTurned.x += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                ballTurned.z += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                ballX = ballTurned.x / FX32_ONE;
                ballZ = ballTurned.z / FX32_ONE;
            }
            PokeathlonSnowThrow_FieldToScreen(ballX, ballZ, &screenX, &screenY);
            ballPosition.x = screenX << FX32_SHIFT;
            ballPosition.y = screenY << FX32_SHIFT;
            ballPosition.z = 0;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->balls[i].shadowSprite, &ballPosition);
            y = screenY;
            ballPosition.y = (y - 8 - y / 20) << FX32_SHIFT;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->balls[i].sprite, &ballPosition);
            // farther away is smaller; bigger snowballs are bigger
            scale = ov96_0221C7C4;
            factor = ((f32)screenY + 312.0) / 752.0 * (1.0 + (f32)size / 3.0);
            scale.x = 4096.0f * factor;
            scale.y = 4096.0f * factor;
            PokeathlonCourseSpritePoolSprite_SetScale(work->balls[i].shadowSprite, &scale, 2);
            PokeathlonCourseSpritePoolSprite_SetScale(work->balls[i].sprite, &scale, 2);
            PokeathlonCourseSpritePoolSprite_SetScale(work->balls[i].splashSprite, &scale, 2);
            if (team == own && size != work->shownSizes[member]) {
                PlaySE(SEQ_SE_GS_166);
                work->shownSizes[member] = size;
            }
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->balls[i].shadowSprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_Show(work->balls[i].sprite, TRUE, FALSE);
            if (state == SNOW_THROW_BALL_SPLASHED && !work->balls[i].splashShown) {
                work->balls[i].splashShown = TRUE;
                PokeathlonCourseSpritePoolSprite_SetPosition(work->balls[i].splashSprite, PokeathlonCourseSpritePoolSprite_GetPosition(work->balls[i].sprite));
                PokeathlonCourseSpritePoolSprite_SetAnim(work->balls[i].splashSprite, 3);
                PokeathlonCourseSpritePoolSprite_Show(work->balls[i].splashSprite, TRUE, TRUE);
                if (i / SNOW_THROW_MEMBERS == own) {
                    PlaySE(SEQ_SE_GS_TAMA3);
                    work->unk5EF[i % SNOW_THROW_MEMBERS] = 0;
                    work->shownSizes[i % SNOW_THROW_MEMBERS] = 0;
                }
            }
        }
    }
    PokeathlonSnowThrow_SortSprites(work, own);
    PokeathlonCourse_UpdateTimeUpCountdown(course, seconds * 30);
}

#pragma force_active reset

// The own Pokemon (of `team`) at touch screen x, y: returns its index (SNOW_THROW_NONE if none) and the field point.
u8 PokeathlonSnowThrow_FindOwnMonAt(PokeathlonSnowThrow *work, u8 team, u32 x, u32 y, VecFx32 *out) {
    f32 fieldX;
    f32 fieldZ;
    VecFx32 position;
    MtxFx43 rotation;
    VecFx32 relative;
    VecFx32 turned;
    VecFx32 diff;
    int i;

#pragma unused(work)
    PokeathlonSnowThrow_ScreenToField(x, y + 288, &fieldX, &fieldZ);
    position.x = (fx32)(fieldX * FX32_ONE);
    position.z = (fx32)(fieldZ * FX32_ONE);
    position.y = 0;
    if (team != 0) {
        // turn the view back
        relative = position;
        relative.x -= SNOW_THROW_FIELD_CENTER * FX32_ONE;
        relative.z -= SNOW_THROW_FIELD_CENTER * FX32_ONE;
        MTX_RotY43(&rotation, FX_SinIdx(team << 14), FX_CosIdx(team << 14));
        MTX_MultVec43(&relative, &rotation, &turned);
        turned.x += SNOW_THROW_FIELD_CENTER * FX32_ONE;
        turned.z += SNOW_THROW_FIELD_CENTER * FX32_ONE;
        position = turned;
    }
    *out = position;
    for (i = team * SNOW_THROW_MEMBERS; i < (team + 1) * SNOW_THROW_MEMBERS; i++) {
        VEC_Subtract(&ov96_0221C98C[i], &position, &diff);
        if (VEC_Mag(&diff) <= 4 * FX32_ONE) {
            return (u8)i;
        }
    }
    return SNOW_THROW_NONE;
}

// The throw parameters of a Pokemon from its Pokeathlon stats (tables in tenths, NARC a/1/7/0 member 3).
void PokeathlonSnowThrow_InitMon(PokeathlonCourseData *course, s32 stats[][5], u8 team, u8 member, PokeathlonSnowThrowMon *mon) {
    u8 *levels = PokeathlonCourse_GetMonStats(course, team, member);
    mon->throwWeight = (double)(float)stats[0][levels[4]] / 10.0;
    mon->throwPower = (double)(float)stats[1][levels[0]] / 10.0;
    mon->aimSpread = stats[2][levels[4]];
    mon->hitDamage = (float)stats[3][levels[3]];
    mon->hits = 0;
    mon->stunTimer = 0;
    mon->condition = SNOW_THROW_FINE;
    mon->energy = SNOW_THROW_MAX_ENERGY;
}

void PokeathlonSnowThrow_InitWindows(PokeathlonSnowThrow *work) {
    int i;

    AddWindow(work->bgConfig, &work->scoreWindow, &ov96_0221C7B0);
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    FillWindowPixelBuffer(&work->scoreWindow, 0);
    for (i = 0; i < SNOW_THROW_TEAMS - 1; i++) {
        AddWindow(work->bgConfig, &work->nameWindows[i], &ov96_0221C7EC[i]);
        FillWindowPixelBuffer(&work->nameWindows[i], 0);
    }
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
}

void PokeathlonSnowThrow_PrintScore(PokeathlonSnowThrow *work, u16 score) {
    MsgData *msgData;
    MessageFormat *format;
    String *string;

    FillWindowPixelBuffer(&work->scoreWindow, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, work->heapId);
    format = MessageFormat_New(work->heapId);
    BufferIntegerAsString(format, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(format, msgData, msg_0309_00164, work->heapId);
    AddTextPrinterParameterizedWithColor(&work->scoreWindow, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    MessageFormat_Delete(format);
    DestroyMsgData(msgData);
    CopyWindowToVram(&work->scoreWindow);
}

// Host: stun and energy recovery of every Pokemon.
void PokeathlonSnowThrow_UpdateMons(PokeathlonCourseData *course) {
    volatile PokeathlonSnowThrow *work = PokeathlonCourse_GetHeapAllocPtr4(course); // (re-read after each write, as built)
    int i;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        if (work->mons[i].stunTimer != 0) {
            work->mons[i].stunTimer = work->mons[i].stunTimer - 1;
        }
        if (work->mons[i].energy < SNOW_THROW_MAX_ENERGY) {
            work->mons[i].energy = work->mons[i].energy + 1;
        }
        if (work->mons[i].stunTimer != 0) {
            work->mons[i].condition = SNOW_THROW_STUNNED;
        } else if (work->mons[i].energy <= 100) {
            work->mons[i].condition = SNOW_THROW_TIRED;
        } else {
            work->mons[i].condition = SNOW_THROW_FINE;
        }
    }
}

// Host: moves the flying snowballs; a snowball that leaves the field is gone, a splash lasts 8 frames.
#pragma force_active on

void PokeathlonSnowThrow_MoveBalls(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;
    BOOL gone;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        gone = FALSE;
        if (work->balls[i].state == SNOW_THROW_BALL_FLYING) {
            VEC_MultAdd(FX32_ONE, &work->balls[i].velocity, &work->balls[i].position, &work->balls[i].position);
        } else if (work->balls[i].state == SNOW_THROW_BALL_SPLASHED) {
            work->balls[i].splashTimer++;
            if (work->balls[i].splashTimer >= 8) {
                work->balls[i].splashTimer = 0;
                work->balls[i].state = SNOW_THROW_BALL_NONE;
            }
        }
        if (work->balls[i].position.x < FX32_ONE || work->balls[i].position.x > 64 * FX32_ONE || work->balls[i].position.z < FX32_ONE || work->balls[i].position.z > 64 * FX32_ONE) {
            gone = TRUE;
        }
        if (gone) {
            work->balls[i].state = SNOW_THROW_BALL_NONE;
            work->balls[i].velocity.x = 0;
            work->balls[i].velocity.z = 0;
            work->balls[i].velocity.y = 0;
            work->balls[i].dragStart.x = 0;
            work->balls[i].dragStart.z = 0;
            work->balls[i].dragStart.y = 0;
        }
    }
}

#pragma force_active reset

// PokeathlonCourse_AddMonCounter as HitMons calls it: the participant and member are passed as bytes.
typedef void (*PokeathlonSnowThrowAddMonCounterFunc)(PokeathlonCourseData *course, u8 participant, u8 member, int counter, int amount);

// Host: a flying snowball within 3 units of another team's Pokemon hits it: a hit for the thrower, energy off the
// Pokemon hit (by speed, size, the thrower's power and the target's damage factor), and a stun if that was a lot.
#pragma force_active on

void PokeathlonSnowThrow_HitMons(PokeathlonCourseData *course, PokeathlonSnowThrow *work) {
    int j, i;
    int team;
    f32 sizeFactor;
    int damage;
    int speed;
    u8 lost;
    VecFx32 diff;

    for (j = 0; j < SNOW_THROW_MONS; j++) {
        work->mons[j].wasHit = FALSE;
        for (i = 0; i < SNOW_THROW_MONS; i++) {
            if (work->balls[i].state != SNOW_THROW_BALL_FLYING) {
                continue;
            }
            team = i / SNOW_THROW_MEMBERS;
            if (team == j / SNOW_THROW_MEMBERS) {
                continue;
            }
            VEC_Subtract(&ov96_0221C98C[j], &work->balls[i].position, &diff);
            if (VEC_Mag(&diff) >= 3 * FX32_ONE) {
                continue;
            }
            work->balls[i].state = SNOW_THROW_BALL_SPLASHED;
            ((PokeathlonSnowThrowAddMonCounterFunc)PokeathlonCourse_AddMonCounter)(course, team, i % SNOW_THROW_MEMBERS, 3, 1);
            work->mons[i].hits++;
            if (work->mons[i].hits > 999) {
                work->mons[i].hits = 999;
            }
            work->balls[i].hit = TRUE;
            work->mons[j].wasHit = TRUE;
            switch (work->balls[i].size) {
            case 0:
                sizeFactor = 1.0f;
                break;
            case 1:
                sizeFactor = 20.0f;
                break;
            case 2:
                sizeFactor = 50.0f;
                break;
            case 3:
                sizeFactor = 100.0f;
                break;
            default:
                GF_ASSERT(FALSE);
                sizeFactor = 1.0f;
                break;
            }
            work->balls[i].size = 0;
            work->balls[i].taps = 0;
            speed = VEC_Mag(&work->balls[i].velocity) / FX32_ONE;
            damage = (f32)speed * sizeFactor * work->mons[i].throwPower * work->mons[j].hitDamage;
            GF_ASSERT(damage >= 0);
            work->mons[j].energy = work->mons[j].energy - damage - 30;
            if (work->mons[j].energy < 0) {
                work->mons[j].energy = 0;
            }
            lost = SNOW_THROW_MAX_ENERGY - work->mons[j].energy;
            if (work->mons[j].stunTimer == 0) {
                if (lost > 40) {
                    work->mons[j].stunTimer = lost;
                    ((PokeathlonSnowThrowAddMonCounterFunc)PokeathlonCourse_AddMonCounter)(course, j / SNOW_THROW_MEMBERS, j % SNOW_THROW_MEMBERS, 1, 1);
                }
            } else {
                work->mons[j].stunTimer += lost;
                ((PokeathlonSnowThrowAddMonCounterFunc)PokeathlonCourse_AddMonCounter)(course, j / SNOW_THROW_MEMBERS, j % SNOW_THROW_MEMBERS, 1, 1);
            }
        }
    }
}

#pragma force_active reset
