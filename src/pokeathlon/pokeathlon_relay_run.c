#include "pokeathlon/pokeathlon_relay_run.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_course_internal.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

typedef char PokeathlonRelayRunSizeCheck[(sizeof(PokeathlonRelayRun) == 0x648 && sizeof(PokeathlonRelayRunTeam) == 0xD4 && sizeof(PokeathlonRelayRunMember) == 0x1C && sizeof(PokeathlonRelayRunPacket) == 0x28 && sizeof(PokeathlonRelayRunTouch) == 0x24 && sizeof(PokeathlonRelayRunLink) == 0x118) ? 1 : -1];
typedef char PokeathlonRelayRunLayoutCheck[(offsetof(PokeathlonRelayRun, teams) == 0x30 && offsetof(PokeathlonRelayRun, finished) == 0x3C0 && offsetof(PokeathlonRelayRun, cpu) == 0x3DC && offsetof(PokeathlonRelayRun, obstacles) == 0x4D4 && offsetof(PokeathlonRelayRun, timer) == 0x640 && offsetof(PokeathlonRelayRunTeam, position) == 0x7C && offsetof(PokeathlonRelayRunTeam, laps) == 0x9C && offsetof(PokeathlonRelayRunTeam, owner) == 0xD0) ? 1 : -1];

// The compiler's int to float helper: the flick strength is converted and dropped.
f32 _fflt(int value);

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
void PokeathlonCourseSpritePool_CreateSprite(void *pool, int priority, int whichScreen, int resId, int kind);
void *PokeathlonCourseSpritePool_Take(void *pool, int resId, int kind);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *entry, int seq);
void PokeathlonCourseSpritePoolSprite_TryChangeAnim(void *entry, int seq);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *entry, VecFx32 *position);
void PokeathlonCourseObjects_PauseAnims(void *objects, BOOL paused);
void PokeathlonCourseObject_SetCollision(void *object, s32 offsetY, s32 radius);
void PokeathlonCourseObject_SetGroundPosition(void *object, s32 x, s32 y);
void PokeathlonCourseObject_SetTouchRadius(void *object, s32 radius);
void PokeathlonCourseObject_GroundToCenter(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
BOOL PokeathlonCourseObject_IsInTouchRange(void *object, fx32 x, fx32 y, s32 targetX, s32 targetY);
s32 PokeathlonCourseObject_GetCollisionRadius(void *object);

// Course points of an event from its score list (course points code).
void PokeathlonEvent_CalcPoints(void *scoreList, int event);

extern const GraphicsBanks ov96_0221C6F0;
extern const GraphicsModes ov96_0221C61C;
extern const BgTemplate ov96_0221C62C;
extern const BgTemplate ov96_0221C648;
extern const BgTemplate ov96_0221C664;
extern const BgTemplate ov96_0221C680;
extern const BgTemplate ov96_0221C69C;
extern const BgTemplate ov96_0221C6B8;
extern const BgTemplate ov96_0221C6D4;
extern const VecFx32 ov96_0221C5E4; // sprite pool template

#pragma push
#pragma opt_common_subs off
#pragma force_active on

BOOL PokeathlonRelayRun_Init(PokeathlonCourseData *course) {
    PokeathlonRelayRun *relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0: {
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_144, 0x68000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        PokeathlonRelayRun_SetGfxBanks();
        PokeathlonRelayRun *data = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonRelayRun));
        MI_CpuFill8(data, 0, sizeof(PokeathlonRelayRun));
        data->bgConfig = BgConfig_Alloc(HEAP_ID_144);
        PokeathlonCourse_SetField3A4(course, (u32)data->layoutToSend, (u32)data->layout, sizeof(data->layout));
        PokeathlonCourse_SetHudPlanes(course, 8);
        ObjCharTransferTemplate charTransfer;
        charTransfer.maxTasks = 151;
        charTransfer.sizeMain = 0x40000;
        charTransfer.sizeSub = 0x4000;
        charTransfer.heapID = HEAP_ID_144;
        PokeathlonCourse_InitObjTransfer(&charTransfer, 22, HEAP_ID_144, GX_OBJVRAMMODE_CHAR_1D_256K, GX_OBJVRAMMODE_CHAR_1D_32K);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_144);
        data->heapId = HEAP_ID_144;
        FontID_Alloc(4, HEAP_ID_144);
        PokeathlonRelayRun_InitBgs(data->bgConfig);
        PokeathlonRelayRun_AddResultWindow(data);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 1: {
        relayRun->hud = PokeathlonRelayRunHud_New(relayRun->heapId, relayRun->bgConfig, course);
        int count = PokeathlonCourse_GetParticipantCount(course);
        int mode = PokeathlonCourse_GetMode(course);
        relayRun->cpu = PokeathlonRelayRunCpu_New(relayRun->heapId, RELAY_RUN_TEAMS - count, mode);
        relayRun->monResources = PokeathlonCourseSprites_CreateResources(relayRun->heapId, 0x2BF, 1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 2: {
        VecFx32 template = ov96_0221C5E4;
        relayRun->spritePool = PokeathlonCourseSpritePool_New(relayRun->heapId, &template);
        PokeathlonCourseSpritePool_SetSurfaceCoords(relayRun->spritePool, 0, 0, 0, FX32_CONST(512));
        relayRun->monObjects = PokeathlonCourseMonObjects_Create(relayRun->heapId, 12, 4, relayRun->monResources, PokeathlonCourseSpritePool_GetSpriteList(relayRun->spritePool));
        PokeathlonCourseSpritePool_SetResourceId(relayRun->spritePool, 0, 101);
        PokeathlonCourseSpritePool_SetResourceId(relayRun->spritePool, 1, 102);
        PokeathlonCourseSpritePool_SetResourceId(relayRun->spritePool, 2, 103);
        PokeathlonCourseSpritePool_SetResourceId(relayRun->spritePool, 3, 104);
        PokeathlonRelayRunHud_LoadSpriteGraphics(relayRun->hud, relayRun->spritePool);
        PokeathlonRelayRun_LoadSpriteGraphics(relayRun->spritePool);
        PokeathlonCourseSpritePool_TransferResources(relayRun->spritePool);
        Sprite_SetDrawPriority(*PokeathlonCourse_CreateHudSpritesInPool(course, 0, relayRun->monResources, relayRun->spritePool), 1);
        PokeathlonRelayRun_CreateSprites(relayRun, relayRun->spritePool);
        PokeathlonRelayRunHud_CreateSprites(relayRun->hud, relayRun->spritePool, relayRun->monResources);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 3: {
        PokeathlonCourseMonInfo infos[12];
        PokeathlonCoursePoint mons[12];
        PokeathlonCourseMonGfxTemplate template;
        int i;
        u8 own;
        for (i = 0; i < 12; i++) {
            int member = i % RELAY_RUN_MEMBERS;
            int team = i / RELAY_RUN_MEMBERS;
            PokeathlonCourse_GetMonInfo(course, team, member, &infos[i]);
            template.sizeTypes[i] = PokeathlonCourse_GetSpriteSizeType((PokeathlonCourseUnkTypes *)PokeathlonCourse_GetPerformance(course, team, member));
            mons[i].x = infos[i].species;
            mons[i].y = infos[i].form;
        }
        PokeathlonRelayRunHud_LoadMonIcons(relayRun->hud, mons);
        own = PokeathlonCourse_GetNetId(course);
        PokeathlonRelayRunHud_LoadMonPics(relayRun->hud, &infos[own * RELAY_RUN_MEMBERS]);
        PokeathlonRelayRunHud_ShowMonPic(relayRun->hud, 0, 1);
        PokeathlonRelayRunHud_ShowMonPic(relayRun->hud, 1, 2);
        template.loadAtOnce = FALSE;
        template.priority = 2;
        template.vramTransfer = FALSE;
        template.vram = 1;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        PokeathlonCourseMonObjects_Load(relayRun->monObjects, 12, infos, &template, 0, 0);
        PokeathlonRelayRunHud_SetDrawPriorities(relayRun->hud, PokeathlonCourse_GetNetId(course));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 4: {
        int own;
        PokeathlonRelayRunCollision collisions[3];
        PokeathlonCoursePoint ownPosition;
        int centerX, centerY;
        u8 setup[160];
        PokeathlonRelayRunTeam *team;
        int n;
        int i, j;
        if (!PokeathlonCourseMonObjects_IsLoaded(relayRun->monObjects)) {
            break;
        }
        own = PokeathlonCourse_GetNetId(course);
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)relayRun->bgConfig); // takes the BgConfig
        PokeathlonCourse_SetField1F4(course, 1);
        ReadWholeNarcMemberByIdPair(collisions, NARC_a_1_7_0, 12);
        for (i = 0; i < 12; i++) {
            PokeathlonCourseMonObject *object = PokeathlonCourseMonObjects_Get(relayRun->monObjects, (u8)i);
            int member = i % RELAY_RUN_MEMBERS;
            int teamId;
            int y;
            if (member == 0) {
                PokeathlonCourseMonObject_SetDrawFlag(object, TRUE);
            }
            {
                PokeathlonRelayRunCollision *collision = &collisions[PokeathlonCourse_GetCollisionSizeClass((PokeathlonCourseUnkTypes *)PokeathlonCourse_GetPerformance(course, i / RELAY_RUN_MEMBERS, i % RELAY_RUN_MEMBERS))];
                PokeathlonCourseObject_SetCollision(object, collision[-1].offsetY, collision[-1].radius);
            }
            object = PokeathlonCourseMonObjects_Get(relayRun->monObjects, (u8)i);
            teamId = i / RELAY_RUN_MEMBERS;
            team = &relayRun->teams[teamId];
            team->monObjects[member] = object;
            y = (teamId + 1) * 27 + 40;
            team->animState = 4;
            PokeathlonCourseMonObject_SetAnimState(object, 4);
            PokeathlonCourseObject_SetGroundPosition(object, RELAY_RUN_RUNNER_X, y);
            PokeathlonCourseObject_SetTouchRadius(object, PokeathlonCourse_GetTouchRadius());
            team->position.x = 0;
            team->position.y = y << FX32_SHIFT;
            if (teamId == own && member == 0) {
                PokeathlonCourseObject_GroundToCenter(object, RELAY_RUN_RUNNER_X, y, &centerX, &centerY);
                ownPosition.x = centerX;
                ownPosition.y = centerY;
            }
        }
        for (i = 0; i < RELAY_RUN_TEAMS; i++) {
            PokeathlonRelayRunTeam *team = &relayRun->teams[i];
            PokeathlonRelayRunTeam_SetDrawPriority(team, team->position.y / FX32_ONE, i == PokeathlonCourse_GetNetId(course));
        }
        ReadWholeNarcMemberByIdPair(setup, NARC_a_1_7_0, 2);
        relayRun->timer = RELAY_RUN_DURATION;
        PokeathlonRelayRunHud_SetTimer(relayRun->hud, relayRun->timer);
        for (n = 0; n < RELAY_RUN_TEAMS; n++) {
            relayRun->dragging[n] = FALSE;
            for (j = 0; j < RELAY_RUN_MEMBERS; j++) {
                PokeathlonRelayRun_InitMember(course, setup, n, j, &relayRun->teams[n]);
            }
            if (PokeathlonCourse_GetNetId(course) == 0) {
                PokeathlonRelayRunCpu_SetTeam(relayRun->cpu, n, &relayRun->teams[n]);
            }
            relayRun->teams[n].owner = n;
        }
        if (PokeathlonCourse_GetNetId(course) == 0) {
            PokeathlonRelayRun_PackPacket(relayRun, (PokeathlonRelayRunPacket *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + offsetof(PokeathlonRelayRunLink, packetToSend)));
        }
        PokeathlonCourse_CreateStartSpritesInPool(course, 0, relayRun->monResources, relayRun->spritePool, NNS_G2D_VRAM_TYPE_2DMAIN, 1, &ownPosition);
        PokeathlonRelayRun_LoadBgGraphics(relayRun);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 5:
        if (PokeathlonCourse_GetNetId(course) == 0) {
            s32 *layout = relayRun->layoutToSend;
            u8 index = LCRandom() % 10;
            ReadWholeNarcMemberByIdPair(layout, NARC_a_2_2_7, index);
            PokeathlonRelayRunCpu_SetObstacles(relayRun->cpu, layout);
        }
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 6:
        GF_ASSERT(GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON));
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset
#pragma pop

BOOL PokeathlonRelayRun_Draw(PokeathlonCourseData *course) {
    PokeathlonRelayRun *relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonCourseSpritePool_Draw(relayRun->spritePool);
    return TRUE;
}

// Results: the laps each team ran, with tenths of a lap.
#pragma force_active on

BOOL PokeathlonRelayRun_ShowResults(PokeathlonCourseData *course) {
    void *gfx = PokeathlonCourse_GetGraphicsSystem(course);
    int i;
    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        // read through an int-returning type: the original truncates the result again
        int distance = (u16)((int (*)(PokeathlonCourseData *, int))PokeathlonCourse_GetField5F0_AtIndex)(course, (u8)i);
        PokeathlonCourse_AddScoreEntry2(course, gfx, i, distance, (u32)distance >> 10, (distance % RELAY_RUN_TRACK_LENGTH) * 10 / RELAY_RUN_TRACK_LENGTH, 0, 1, 2, 1, msg_0309_00297);
    }
    PokeathlonCourseScoreList_Rank(gfx, 1);
    PokeathlonEvent_CalcPoints(gfx, 6);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 6);
    return TRUE;
}

#pragma force_active reset

BOOL PokeathlonRelayRun_Exit(PokeathlonCourseData *course) {
    PokeathlonRelayRun *relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    WirelessStatusIcon_Destroy();
    PokeathlonCourse_ResetField3A4(course);
    FreeBgTilemapBuffer(relayRun->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(relayRun->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(relayRun->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(relayRun->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(relayRun->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(relayRun->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(relayRun->bgConfig, GF_BG_LYR_SUB_2);
    RemoveWindow(&relayRun->window);
    Heap_Free(relayRun->bgConfig);
    PokeathlonCourseSpritePool_Free(relayRun->spritePool);
    PokeathlonCourseMonObjects_Free(relayRun->monObjects);
    PokeathlonCourseSprites_FreeResources(relayRun->monResources);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    PokeathlonRelayRunCpu_Delete(relayRun->cpu);
    PokeathlonRelayRunHud_Free(relayRun->hud);
    FontID_Release(4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_144);
    return TRUE;
}

void PokeathlonRelayRun_SetGfxBanks(void) {
    GraphicsBanks banks = ov96_0221C6F0;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on

void PokeathlonRelayRun_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main0, main1, main2, main3, sub0, sub1, sub2;
    modes = ov96_0221C61C;
    SetBothScreensModesAndDisable(&modes);
    main0 = ov96_0221C648;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main1 = ov96_0221C664;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main2 = ov96_0221C680;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    main3 = ov96_0221C69C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = ov96_0221C6B8;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = ov96_0221C6D4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    sub2 = ov96_0221C62C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active reset

void PokeathlonRelayRun_LoadBgGraphics(PokeathlonRelayRun *relayRun) {
    GfGfxLoader_LoadCharData(NARC_a_1_7_8, 1, relayRun->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, relayRun->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 2, relayRun->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, relayRun->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 3, relayRun->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, relayRun->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 4, relayRun->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, relayRun->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_7_8, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40, relayRun->heapId);
    PokeathlonRelayRunHud_LoadBgGraphics(relayRun->hud);
}

void PokeathlonRelayRun_LoadSpriteGraphics(void *spritePool) {
    PokeathlonCourseSpritePool_LoadChar(spritePool, NARC_a_1_7_8, 17, 104, 1);
    PokeathlonCourseSpritePool_LoadPltt(spritePool, NARC_a_1_7_8, 14, 104, 1, 2);
    PokeathlonCourseSpritePool_LoadCell(spritePool, NARC_a_1_7_8, 16, 104);
    PokeathlonCourseSpritePool_LoadAnim(spritePool, NARC_a_1_7_8, 15, 104);
}

#pragma force_active on

void PokeathlonRelayRun_CreateSprites(PokeathlonRelayRun *relayRun, void *spritePool) {
    int i;
    VecFx32 position;
    for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
        PokeathlonCourseSpritePool_CreateSprite(spritePool, 2, 1, 104, 6);
    }
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        PokeathlonCourseSpritePool_CreateSprite(spritePool, 2, 1, 104, 7);
        PokeathlonCourseSpritePool_CreateSprite(spritePool, 2, 1, 104, 9);
        PokeathlonCourseSpritePool_CreateSprite(spritePool, 2, 1, 104, 11);
        PokeathlonCourseSpritePool_CreateSprite(spritePool, 2, 1, 104, 13);
    }
    for (i = 0; i < 4; i++) {
        PokeathlonCourseSpritePool_CreateSprite(spritePool, 2, 1, 104, 14);
    }
    PokeathlonCourseSpritePool_CreateSprite(spritePool, 1, 1, 104, 12);
    PokeathlonCourseSpritePool_CreateSprite(spritePool, 1, 1, 104, 15);
    for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
        relayRun->obstacles[i].sprite = PokeathlonCourseSpritePool_Take(spritePool, 104, 6);
    }
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        relayRun->teams[i].swapSprite = PokeathlonCourseSpritePool_Take(spritePool, 104, 7);
        relayRun->teams[i].conditionSprite = PokeathlonCourseSpritePool_Take(spritePool, 104, 9);
        relayRun->teams[i].speedSprite = PokeathlonCourseSpritePool_Take(spritePool, 104, 11);
        relayRun->teams[i].shadowSprite = PokeathlonCourseSpritePool_Take(spritePool, 104, 13);
        PokeathlonCourseSpritePoolSprite_SetAnim(relayRun->teams[i].swapSprite, 3);
        PokeathlonCourseSpritePoolSprite_SetAnim(relayRun->teams[i].shadowSprite, 8);
    }
    for (i = 0; i < 4; i++) {
        relayRun->bumpMarks[i] = PokeathlonCourseSpritePool_Take(spritePool, 104, 14);
    }
    relayRun->signSprite = PokeathlonCourseSpritePool_Take(spritePool, 104, 12);
    PokeathlonCourseSpritePoolSprite_SetAnim(relayRun->signSprite, 11);
    PokeathlonCourseSpritePoolSprite_Show(relayRun->signSprite, TRUE, TRUE);
    position.x = FX32_CONST(32);
    position.y = FX32_CONST(16);
    position.z = 0;
    PokeathlonCourseSpritePoolSprite_SetPosition(relayRun->signSprite, &position);
    relayRun->lapCounter.plate = PokeathlonCourseSpritePool_Take(spritePool, 104, 15);
    PokeathlonCourseSpritePoolSprite_SetAnim(relayRun->lapCounter.plate, 10);
    relayRun->lapCounter.digits[0] = PokeathlonCourseSprites_CreateLapCounterDigit(relayRun->monResources, PokeathlonCourseSpritePool_GetSpriteList(relayRun->spritePool), 0, relayRun->heapId);
    relayRun->lapCounter.digits[1] = PokeathlonCourseSprites_CreateLapCounterDigit(relayRun->monResources, PokeathlonCourseSpritePool_GetSpriteList(relayRun->spritePool), 0, relayRun->heapId);
}

#pragma force_active reset

// State 0: place the obstacles of the layout the host sent, then fade in.
#pragma force_active on

BOOL PokeathlonRelayRun_Start(PokeathlonCourseData *course, u8 *state) {
    int i;
    PokeathlonRelayRun *relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    VecFx32 position;
    switch (*state) {
    case 0:
        for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
            u8 kind = relayRun->layout[i];
            u8 y = relayRun->layout[i] >> 8;
            u16 x = relayRun->layout[i] >> 16;
            if (kind != 0) {
                relayRun->obstacles[i].active = TRUE;
                relayRun->obstacles[i].kind = kind;
                relayRun->obstacles[i].x = x;
                relayRun->obstacles[i].y = y;
                PokeathlonCourseSpritePoolSprite_SetAnim(relayRun->obstacles[i].sprite, kind - 1);
                PokeathlonCourseSpritePoolSprite_Show(relayRun->obstacles[i].sprite, TRUE, TRUE);
                position.z = 0;
                position.x = (x + RELAY_RUN_RUNNER_X) << FX32_SHIFT;
                position.y = (y + 32) << FX32_SHIFT;
                PokeathlonCourseSpritePoolSprite_SetPosition(relayRun->obstacles[i].sprite, &position);
                PokeathlonRelayRunObstacle_SetDrawPriority(&relayRun->obstacles[i]);
            }
        }
        PokeathlonRelayRun_ScrollObstacles(relayRun, 0);
        (*state)++;
        PokeathlonCourse_SetStateTransitionType(course, 18);
        break;
    case 1:
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, relayRun->heapId);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

// State 1: countdown, the race, then the finish.
BOOL PokeathlonRelayRun_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonRelayRun *relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            (*state)++;
        }
        break;
    case 1:
        PokeathlonRelayRun_UpdateHost(course);
        if (PokeathlonRelayRun_Update(course)) {
            (*state)++;
        }
        break;
    case 2:
        PokeathlonRelayRun_UpdateHost(course);
        if (PokeathlonCourse_RunFinish(course)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, relayRun->heapId);
            PokeathlonCourse_SetStateField07(course, 2);
        }
        break;
    }
    return FALSE;
}

// State 2: after the fade, the host stores each team's distance (laps << 10 | pixels into the lap) as its score.
BOOL PokeathlonRelayRun_SaveScores(PokeathlonCourseData *course, u8 *state) {
    int i;
    PokeathlonCourseData *savedCourse = course;
    PokeathlonRelayRunTeam *team = PokeathlonCourse_GetHeapAllocPtr4(course);

    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    if (PokeathlonCourse_GetNetId(savedCourse)) {
        return TRUE;
    }
    for (i = 0, team = (PokeathlonRelayRunTeam *)((u8 *)team + offsetof(PokeathlonRelayRun, teams)); i < RELAY_RUN_TEAMS; i++, team++) {
        PokeathlonCourse_SetField5E0_AtIndex(savedCourse, (u8)i, (u16)(team->position.x / FX32_ONE + (team->laps << 10)));
    }
    return TRUE;
}

// teamCursor and dragCursor are relayRun moved on by i teams / i flags, so that ->teams[0] / ->dragging[0] is
// team i's (the loop below walks them that way, as the original does).
static inline void PokeathlonRelayRun_ClearDrag(PokeathlonRelayRun *teamCursor, PokeathlonRelayRun *dragCursor) {
    dragCursor->dragging[0] = FALSE;
    teamCursor->teams[0].heldFrames = 0;
    teamCursor->teams[0].dragStart.x = 0;
    teamCursor->teams[0].dragStart.y = 0;
    teamCursor->teams[0].dragStart.z = 0;
    teamCursor->teams[0].dragEnd = teamCursor->teams[0].dragStart;
}

// Host, every frame: reads everyone's stylus and moves the teams. Touching the swap button (or dragging backwards)
// hands over to the next member; a short drag released flicks the runner forward, faster the longer the drag
// and the stronger the Pokemon, up to its top speed.
#pragma force_active on

void PokeathlonRelayRun_UpdateHost(PokeathlonCourseData *course) {
    int i;
    PokeathlonRelayRun *relayRun;
    u8 *link;
    int clear;
    f32 acceleration, maxSpeed;
    PokeathlonRelayRun *pressCursor;
    PokeathlonRelayRun *teamCursor;
    PokeathlonRelayRun *dragCursor;
    u8 *cursor;
    PokeathlonRelayRunTeam *runner;
    VecFx32 *dragStart, *dragEnd, *velocity, *velocity2;

    struct {
        VecFx32 limitBase, base, flick, combined;
    } work;

    link = PokeathlonCourse_GetDataCopyArea(course);
    relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (PokeathlonCourse_GetNetId(course)) {
        return;
    }
    if (relayRun->finished) {
        link += offsetof(PokeathlonRelayRunLink, packetToSend);
        PokeathlonRelayRun_PackPacket(relayRun, (PokeathlonRelayRunPacket *)PokeathlonCourse_ToBytes(link));
        return;
    }
    {
        PokeathlonRelayRunTouch *hostTouch = (PokeathlonRelayRunTouch *)PokeathlonCourse_ToBytes(link + offsetof(PokeathlonRelayRunLink, touches)); // the host's own stylus is block 0
        *hostTouch = *(PokeathlonRelayRunTouch *)PokeathlonCourse_ToBytes(link);
    }
    if (relayRun->timer) {
        relayRun->timer--;
    }
    PokeathlonCourse_ToBytes(link + offsetof(PokeathlonRelayRunLink, packetToSend));
    i = 0;
    pressCursor = relayRun;
    teamCursor = relayRun;
    dragCursor = relayRun;
    cursor = link + offsetof(PokeathlonRelayRunLink, touches);
    runner = relayRun->teams;
    dragStart = &relayRun->teams[0].dragStart;
    dragEnd = &relayRun->teams[0].dragEnd;
    velocity = &relayRun->teams[0].velocity;
    velocity2 = &relayRun->teams[0].velocity;
    for (; i < RELAY_RUN_TEAMS; pressCursor = (PokeathlonRelayRun *)((u8 *)pressCursor + sizeof(PokeathlonRelayRunPress)), cursor += sizeof(PokeathlonRelayRunTouchBlock), teamCursor = (PokeathlonRelayRun *)((u8 *)teamCursor + sizeof(PokeathlonRelayRunTeam)), runner++, dragCursor = (PokeathlonRelayRun *)((u8 *)dragCursor + sizeof(BOOL)), dragStart = (VecFx32 *)((u8 *)dragStart + sizeof(PokeathlonRelayRunTeam)), dragEnd = (VecFx32 *)((u8 *)dragEnd + sizeof(PokeathlonRelayRunTeam)), velocity = (VecFx32 *)((u8 *)velocity + sizeof(PokeathlonRelayRunTeam)), velocity2 = (VecFx32 *)((u8 *)velocity2 + sizeof(PokeathlonRelayRunTeam)), i++) {
        PokeathlonRelayRunTouch *touch = (PokeathlonRelayRunTouch *)PokeathlonCourse_ToBytes(cursor);
        if (touch->held) {
            if (pressCursor->presses[0].pressed && pressCursor->presses[0].held) {
                pressCursor->presses[0].pressed = FALSE;
            } else if (!pressCursor->presses[0].pressed && !pressCursor->presses[0].held) {
                pressCursor->presses[0].pressed = TRUE;
                pressCursor->presses[0].held = TRUE;
            }
        } else {
            pressCursor->presses[0].pressed = FALSE;
            pressCursor->presses[0].held = FALSE;
        }
        PokeathlonRelayRun_UpdateSwapSign(relayRun, (u8)i, PokeathlonRelayRun_IsTouchingSwapButton(relayRun, touch->x, touch->y));
        if (teamCursor->teams[0].swapping) {
            if (teamCursor->teams[0].swapPhase == 1 && touch->displayPhase == 1) {
                teamCursor->teams[0].current = (teamCursor->teams[0].current + 1) % RELAY_RUN_MEMBERS;
                teamCursor->teams[0].swapPhase = 2;
            } else if (teamCursor->teams[0].swapPhase == 2 && touch->displayPhase == 2) {
                teamCursor->teams[0].swapPhase = 0;
                teamCursor->teams[0].swapping = FALSE;
                teamCursor->teams[0].tired = FALSE;
            }
            PokeathlonRelayRun_ClearDrag(teamCursor, dragCursor);
            continue;
        }
        if (teamCursor->teams[0].members[teamCursor->teams[0].current].condition == RELAY_RUN_EXHAUSTED) {
            PokeathlonRelayRun_ClearDrag(teamCursor, dragCursor);
            continue;
        }
        if (teamCursor->teams[0].stunTimer) {
            PokeathlonRelayRun_ClearDrag(teamCursor, dragCursor);
            continue;
        }
        clear = FALSE;
        if (pressCursor->presses[0].pressed) {
            if (PokeathlonRelayRun_IsTouchingSwapButton(relayRun, touch->x, touch->y)) {
                teamCursor->teams[0].swapping = TRUE;
                teamCursor->teams[0].swapPhase = 1;
                PokeathlonCourse_AddMonCounter(course, teamCursor->teams[0].owner, teamCursor->teams[0].current, 7, 1);
            } else if (PokeathlonRelayRun_IsTouchingRunner(relayRun, (u8)i, touch->x, touch->y)) {
                dragCursor->dragging[0] = TRUE;
                teamCursor->teams[0].dragStart.x = touch->x << FX32_SHIFT;
                teamCursor->teams[0].dragStart.y = touch->y << FX32_SHIFT;
                teamCursor->teams[0].dragStart.z = 0;
                teamCursor->teams[0].dragEnd = teamCursor->teams[0].dragStart;
            }
        } else if (pressCursor->presses[0].held) {
            if (teamCursor->teams[0].heldFrames < 255) {
                teamCursor->teams[0].heldFrames++;
            } else {
                relayRun->heldTooLong = TRUE;
            }
            if (!dragCursor->dragging[0]) {
                continue;
            }
            teamCursor->teams[0].dragEnd.x = touch->x << FX32_SHIFT;
            teamCursor->teams[0].dragEnd.y = touch->y << FX32_SHIFT;
        } else {
            if (dragCursor->dragging[0]) {
                if (teamCursor->teams[0].heldFrames <= 30) {
                    teamCursor->teams[0].flicked = TRUE;
                }
                dragCursor->dragging[0] = FALSE;
                relayRun->heldTooLong = FALSE;
            }
            clear = TRUE;
        }
        if (teamCursor->teams[0].flicked) {
            f32 weight, speed;
            fx32 scale;
            {
                fx32 *zero = (fx32 *)&work.base;
                *zero++ = 0;
                *zero++ = 0;
                *zero = 0;
            }
            acceleration = teamCursor->teams[0].members[teamCursor->teams[0].current].acceleration;
            weight = teamCursor->teams[0].members[teamCursor->teams[0].current].weight;
            maxSpeed = PokeathlonRelayRunTeam_GetMaxSpeed(runner, teamCursor->teams[0].current);
            scale = (fx32)(4096.0 * ((double)(acceleration * weight) / 8.0));
            VEC_Subtract(dragEnd, dragStart, &work.flick);
            if (work.flick.x < 0) {
                if (work.flick.x < FX32_CONST(-16)) {
                    teamCursor->teams[0].swapping = TRUE;
                    teamCursor->teams[0].swapPhase = 1;
                    PokeathlonCourse_AddMonCounter(course, teamCursor->teams[0].owner, teamCursor->teams[0].current, 7, 1);
                }
                work.flick.x = 0;
                work.flick.y = 0;
                work.flick.z = 0;
            }
            VEC_MultAdd(scale, &work.flick, &work.base, &work.flick);
            _fflt(VEC_Mag(&work.flick) / FX32_ONE);
            work.combined = teamCursor->teams[0].velocity;
            PokeathlonRelayRun_AddFlick(&work.combined, &work.flick, &work.combined);
            speed = VEC_Mag(&work.combined);
            if (speed > 4096.0f * maxSpeed) {
                {
                    fx32 *zero = (fx32 *)&work.limitBase;
                    *zero++ = 0;
                    *zero++ = 0;
                    *zero = 0;
                }
                VEC_Normalize(&work.combined, &work.combined);
                VEC_MultAdd((fx32)(4096.0f * maxSpeed), &work.combined, &work.limitBase, velocity);
            } else {
                PokeathlonRelayRun_AddFlick(velocity2, &work.flick, velocity);
            }
            {
                fx32 magnitude = VEC_Mag(&runner->velocity);
                if (magnitude >= ((int)runner->members[runner->current].speedLimit - 3) * FX32_ONE) {
                    PokeathlonCourse_AddMonCounter(course, runner->owner, runner->current, 6, 1);
                }
            }
            teamCursor->teams[0].flicked = FALSE;
        }
        if (clear) {
            teamCursor->teams[0].heldFrames = 0;
            teamCursor->teams[0].dragStart.x = 0;
            teamCursor->teams[0].dragStart.y = 0;
            teamCursor->teams[0].dragStart.z = 0;
            teamCursor->teams[0].dragEnd = teamCursor->teams[0].dragStart;
        }
    }
    PokeathlonRelayRunCpu_Update(course, relayRun->cpu, relayRun->timer);
    PokeathlonRelayRun_MoveTeams(course);
    if (relayRun->timer == 0) {
        relayRun->finished = TRUE;
    } else {
        relayRun->finished = FALSE;
    }
    link += offsetof(PokeathlonRelayRunLink, packetToSend);
    PokeathlonRelayRun_PackPacket(relayRun, (PokeathlonRelayRunPacket *)PokeathlonCourse_ToBytes(link));
}

void PokeathlonRelayRun_PackPacket(PokeathlonRelayRun *relayRun, PokeathlonRelayRunPacket *packet) {
    int i;
    int j;
    u32 flags;
    u8 swapPhases;
    u8 behindStart;
    swapPhases = 0;
    flags = 0;
    behindStart = 0;
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        PokeathlonRelayRunTeam *team = &relayRun->teams[i];
        u8 runner = 0;
        int laps;
        runner += team->current;
        runner += team->speedGrade * 4;
        swapPhases += team->swapPhase << (i * 2);
        laps = team->swapping * 128;
        laps += team->laps;
        packet->y[i] = team->position.y / FX32_ONE;
        packet->x[i] = team->position.x / FX32_ONE;
        packet->runner[i] = runner;
        packet->laps[i] = laps;
        for (j = 0; j < RELAY_RUN_MEMBERS; j++) {
            flags += team->members[j].condition << ((j + i * RELAY_RUN_MEMBERS) * 2);
        }
        packet->swapSigns[i] = relayRun->swapSigns[i];
        behindStart += team->behindStart << i;
    }
    packet->swapPhases = swapPhases;
    packet->flags = flags;
    packet->flags += relayRun->finished << 24;
    packet->flags += relayRun->bumped << 25;
    packet->flags += behindStart << 26;
    packet->bumpX = relayRun->bumpX;
    packet->bumpY = relayRun->bumpY;
    packet->timer = relayRun->timer;
}

#pragma force_active reset

// Every player, every frame: sends the stylus to the host and draws the packet it received. TRUE once the
// host says the time is up (then stops the animations and, on the host, counts each member's distance).
BOOL PokeathlonRelayRun_Update(PokeathlonCourseData *course) {
    PokeathlonRelayRun *relayRun;
    PokeathlonRelayRunPacket *packet;
    PokeathlonRelayRunTouch *touch;
    u8 j;
    u8 i;

    relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    packet = (PokeathlonRelayRunPacket *)PokeathlonCourse_GetDataCopyArea(course);
    touch = (PokeathlonRelayRunTouch *)PokeathlonCourse_ToBytes(packet);
    touch->held = FALSE;
    packet = (PokeathlonRelayRunPacket *)PokeathlonCourse_ToBytes((u8 *)packet + offsetof(PokeathlonRelayRunLink, packet));
    {
        u8 finished = (packet->flags >> 24) & 1;
        if (finished) {
            PokeathlonCourseSpritePoolSprite_Show(relayRun->lapCounter.plate, TRUE, FALSE);
            Sprite_SetDrawFlag(relayRun->lapCounter.digits[0], FALSE);
            Sprite_SetDrawFlag(relayRun->lapCounter.digits[1], FALSE);
            PokeathlonCourseSpritePool_PauseAnims(relayRun->spritePool, TRUE);
            PokeathlonCourseObjects_PauseAnims(relayRun->monObjects, TRUE);
            PokeathlonCourse_SetFinishCallback(course, PokeathlonRelayRun_SaveResult);
            if (!PokeathlonCourse_GetNetId(course)) {
                for (i = 0; i < RELAY_RUN_TEAMS; i++) {
                    // team i through relayRun moved by i teams (shifted->teams[0]), the way the original addresses it
                    PokeathlonRelayRun *shifted = (PokeathlonRelayRun *)((u8 *)relayRun + i * sizeof(PokeathlonRelayRunTeam));
                    for (j = 0; j < RELAY_RUN_MEMBERS; j++) {
                        s32 distance = shifted->teams[0].distance[j] / RELAY_RUN_TRACK_LENGTH;
                        PokeathlonCourse_AddMonCounter(course, i, j, 3, (u8)(distance / FX32_ONE));
                    }
                }
            }
            return TRUE;
        }
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        touch->x = gSystem.touchX;
        touch->y = gSystem.touchY;
        touch->held = TRUE;
    }
    PokeathlonRelayRun_DrawPacket(course);
    return FALSE;
}

// Draws the packet from the host: the teams and the track scrolled to the player's own runner, the lap counter,
// the timer and the sign at the top of the screen.
#pragma force_active on

void PokeathlonRelayRun_DrawPacket(PokeathlonCourseData *course) {
    PokeathlonRelayRun *relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *link = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonRelayRunPacket *packet = (PokeathlonRelayRunPacket *)PokeathlonCourse_ToBytes(link + offsetof(PokeathlonRelayRunLink, packet));
    u8 own = PokeathlonCourse_GetNetId(course);
    int i;
    u8 laps;
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        PokeathlonRelayRunHud_SetMapPosition(relayRun->hud, i, packet->x[i]);
    }
    ScheduleSetBgPosText(relayRun->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, packet->x[own]);
    ScheduleSetBgPosText(relayRun->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, packet->x[own] / 2);
    ScheduleSetBgPosText(relayRun->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, packet->x[own] * 2);
    PokeathlonRelayRun_DrawTeams(course, own, relayRun->teams, (PokeathlonRelayRunLink *)link);
    PokeathlonRelayRun_ScrollObstacles(relayRun, packet->x[own]);
    laps = packet->laps[own] & 0x7F;
    PokeathlonRelayRunHud_PrintDistance(relayRun->hud, laps, (u8)(((u8)((packet->flags >> 26) & 0xF) >> own) & 1) ? 0 : packet->x[own]);
    if (!(u8)((packet->flags >> 24) & 1)) {
        PokeathlonRelayRun_UpdateLapCounter(&relayRun->lapCounter, laps);
    }
    PokeathlonRelayRunHud_SetTimer(relayRun->hud, packet->timer);
    switch (packet->swapSigns[own]) {
    case 0:
        relayRun->signSoundPlayed = FALSE;
        PokeathlonCourseSpritePoolSprite_TryChangeAnim(relayRun->signSprite, 11);
        break;
    case 3:
        relayRun->signSoundPlayed = FALSE;
        PokeathlonCourseSpritePoolSprite_TryChangeAnim(relayRun->signSprite, 13);
        break;
    case 2:
        if (relayRun->signSoundPlayed == FALSE) {
            relayRun->signSoundPlayed = TRUE;
            PlaySE(SEQ_SE_GS_CHANGE1);
        }
        PokeathlonCourseSpritePoolSprite_TryChangeAnim(relayRun->signSprite, 12);
        break;
    case 4:
        if (relayRun->signSoundPlayed == FALSE) {
            relayRun->signSoundPlayed = TRUE;
            PlaySE(SEQ_SE_GS_CHANGE2);
        }
        PokeathlonCourseSpritePoolSprite_TryChangeAnim(relayRun->signSprite, 14);
        break;
    case 1:
        relayRun->signSoundPlayed = FALSE;
        PokeathlonCourseSpritePoolSprite_TryChangeAnim(relayRun->signSprite, 15);
        break;
    }
    PokeathlonRelayRun_UpdateStartLine(relayRun->bgConfig, packet->x[own]);
    PokeathlonCourse_UpdateTimeUpCountdown(course, packet->timer);
}

#pragma force_active reset

BOOL PokeathlonRelayRun_IsTouchingRunner(PokeathlonRelayRun *relayRun, int teamId, int x, int y) {
    s32 centerX;
    s32 centerY;
    PokeathlonRelayRunTeam *team = &relayRun->teams[teamId];
    void *object;
    GF_ASSERT(team->current < RELAY_RUN_MEMBERS);
    object = team->monObjects[team->current];
    PokeathlonCourseObject_GroundToCenter(object, RELAY_RUN_RUNNER_X, team->position.y / FX32_ONE, &centerX, &centerY);
    if (PokeathlonCourseObject_IsInTouchRange(object, centerX << FX32_SHIFT, centerY << FX32_SHIFT, x, y)) {
        return TRUE;
    }
    return FALSE;
}

// The swap button: the top-left 64x32 corner of the touch screen.
BOOL PokeathlonRelayRun_IsTouchingSwapButton(PokeathlonRelayRun *relayRun, u32 x, u32 y) {
    if (x < 64 && y < 32) {
        return TRUE;
    }
    return FALSE;
}

// Host: moves every team by its velocity, bounces it off the walls and the obstacles, counts the laps, applies
// the friction, grades the speed and spends / restores the members' energy.
#pragma force_active on

void PokeathlonRelayRun_MoveTeams(PokeathlonCourseData *course) {
    VecFx32 bump;
    VecFx32 from;
    VecFx32 direction;
    u8 skip[RELAY_RUN_TEAMS];
    PokeathlonRelayRun *relayRun;
    int i;
    int j;
    PokeathlonRelayRunTeam *team;
    int result;
    int hit;
    int x;
    fx32 speed;
    fx32 limit;
    fx32 friction;
    f32 coefficient;
    f32 energy;
    u8 speedLimit;
    relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    relayRun->bumped = FALSE;
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        skip[i] = FALSE;
        if (relayRun->teams[i].stunTimer) {
            PokeathlonRelayRunTeam_UpdateStun(course, relayRun, &relayRun->teams[i]);
            skip[i] = TRUE;
        }
    }
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        if (skip[i]) {
            continue;
        }
        team = &relayRun->teams[i];
        from = team->position;
        VEC_Add(&team->position, &team->velocity, &team->position);
        team->distance[team->current] += team->velocity.x;
        if (team->position.y < FX32_CONST(32)) {
            team->position.y = FX32_CONST(32);
        } else if (team->position.y > FX32_CONST(168)) {
            team->position.y = FX32_CONST(168);
        }
        result = PokeathlonRelayRun_CollideWithObstacles(relayRun, team->wallState, PokeathlonCourseObject_GetCollisionRadius(team->monObjects[team->current]), &from, &team->position, &team->position);
        hit = (u8)(result >> 8);
        result = (u8)result;
        if (hit == 1) {
            team->onPatch = TRUE;
        } else {
            team->onPatch = FALSE;
        }
        team->wallState = result;
        switch (result) {
        case 2:
            if (team->velocity.y > 0) {
                team->velocity.y *= -1;
            }
            break;
        case 3:
            if (team->velocity.x < 0) {
                team->velocity.x *= -1;
            }
            break;
        case 4:
            if (team->velocity.y < 0) {
                team->velocity.y *= -1;
            }
            break;
        case 5:
            if (team->velocity.x > 0) {
                team->velocity.x *= -1;
            }
            break;
        case 6:
            if (team->velocity.x > 0) {
                team->velocity.x *= -1;
            }
            if (team->velocity.y > 0) {
                team->velocity.y *= -1;
            }
            break;
        case 7:
            if (team->velocity.x < 0) {
                team->velocity.x *= -1;
            }
            if (team->velocity.y > 0) {
                team->velocity.y *= -1;
            }
            break;
        case 8:
            if (team->velocity.x < 0) {
                team->velocity.x *= -1;
            }
            if (team->velocity.y < 0) {
                team->velocity.y *= -1;
            }
            break;
        case 9:
            if (team->velocity.x > 0) {
                team->velocity.x *= -1;
            }
            if (team->velocity.y < 0) {
                team->velocity.y *= -1;
            }
            break;
        }
        result = PokeathlonRelayRun_CollideRunners(course, relayRun->teams, &bump);
        if (relayRun->bumped == FALSE && result) {
            relayRun->bumped = TRUE;
            relayRun->bumpX = bump.x / FX32_ONE;
            relayRun->bumpY = bump.y / FX32_ONE;
        }
        x = team->position.x;
        // 4194304.0 = FX32_CONST(RELAY_RUN_TRACK_LENGTH): one lap
        if (x >= 4194304.0) {
            team->position.x = x - 4194304.0;
            if (!team->behindStart && team->laps < RELAY_RUN_MAX_LAPS) {
                team->laps++;
            }
            if (team->bestLaps < team->laps) {
                team->bestLaps = team->laps;
            }
            team->behindStart = FALSE;
        } else if (x < 0) {
            team->position.x = 4194304.0 + x;
            if (team->laps != 0) {
                team->laps--;
            } else if (team->laps == 0) {
                team->behindStart = TRUE;
            }
        }
        direction = team->velocity;
        speed = VEC_Mag(&direction);
        if (!team->swapping && team->onPatch) {
            limit = team->members[team->current].patchSpeed << FX32_SHIFT;
            if (speed > limit) {
                VecFx32 zero = { 0, 0, 0 };
                VecFx32 slowed;
                slowed = team->velocity;
                VEC_Normalize(&slowed, &slowed);
                VEC_MultAdd(limit, &slowed, &zero, &slowed);
                team->velocity = slowed;
                direction = team->velocity;
                speed = VEC_Mag(&direction);
            }
        }
        if (speed > 0) {
            coefficient = 1.0f;
            friction = 4096.0 * (0.4 * coefficient);
            if (relayRun->heldTooLong) {
                friction += FX32_CONST(3);
            }
            VEC_Normalize(&direction, &direction);
            direction.x = FX_Mul(direction.x, friction);
            direction.y = FX_Mul(direction.y, friction);
            limit = VEC_Mag(&direction);
            if (VEC_Mag(&team->velocity) >= limit) {
                VEC_Subtract(&team->velocity, &direction, &team->velocity);
            } else {
                team->velocity.x = 0;
                team->velocity.y = 0;
            }
        }
        speed = VEC_Mag(&team->velocity);
        speedLimit = team->members[team->current].speedLimit;
        if (speed >= (speedLimit - 1) << FX32_SHIFT) {
            team->speedGrade = 3;
        } else if (speed >= (speedLimit - 3) << FX32_SHIFT) {
            team->speedGrade = 2;
        } else if (speed == 0) {
            team->speedGrade = 0;
        } else {
            team->speedGrade = 1;
        }
        if (team->swapping) {
            continue;
        }
        for (j = 0; j < RELAY_RUN_MEMBERS; j++) {
            energy = team->members[j].energy;
            if (j == team->current) {
                if (team->members[j].condition != RELAY_RUN_EXHAUSTED) {
                    switch (team->speedGrade) {
                    case 0:
                        energy += 0.5;
                        break;
                    case 1:
                    case 2:
                    case 3:
                        energy = energy - 0.5;
                        break;
                    }
                    if (team->tired) {
                        if (energy > 40.0) {
                            energy = 40.0f;
                        }
                    } else if (energy > team->members[j].maxEnergy) {
                        energy = team->members[j].maxEnergy;
                    }
                } else if (team->members[j].condition == RELAY_RUN_EXHAUSTED) {
                    team->recoveryTimer--;
                    if (team->recoveryTimer == 0) {
                        team->members[j].condition = RELAY_RUN_TIRED;
                        energy = 40.0f;
                    }
                }
            } else {
                energy += 0.5;
                if (energy > team->members[j].maxEnergy) {
                    energy = team->members[j].maxEnergy;
                }
            }
            if (energy <= 0.0f) {
                GF_ASSERT(j == team->current);
                team->members[j].energy = 0.0f;
                if (team->members[j].condition != RELAY_RUN_EXHAUSTED) {
                    team->members[j].condition = RELAY_RUN_EXHAUSTED;
                    team->tired = TRUE;
                    team->recoveryTimer = team->members[j].recoveryTime;
                    PokeathlonCourse_AddMonCounter(course, team->owner, team->current, 1, 1);
                }
            } else if (energy <= 40.0) {
                team->members[j].energy = energy;
                team->members[j].condition = RELAY_RUN_TIRED;
            } else {
                team->members[j].energy = energy;
                team->members[j].condition = RELAY_RUN_FINE;
            }
        }
    }
}

#pragma force_active reset

void PokeathlonRelayRunTeam_UpdateStun(PokeathlonCourseData *course, PokeathlonRelayRun *relayRun, PokeathlonRelayRunTeam *team) {
    if (team->stunTimer != 0) {
        team->stunTimer--;
    }
}
