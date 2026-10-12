#include "pokeathlon/pokeathlon_lamp_jump.h"

#include "global.h"

#include "constants/sndseq.h"

#include "assert.h"
#include "bg_window.h"
#include "camera.h"
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
#include "sprite.h"
#include "system.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"

// sound_playback.c (no header declares them)
BOOL PlaySEOnPlayer(u16 seqNo, int playerNo);
u32 GF_SndCountPlayingSeqOnPlayer(u32 playerNo);

// Course helpers, sprite scene and the rest of the event (OVY_96, not yet tidied).
void *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int member);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int member);
int ov96_021E6104(); // called with and without an argument
int ov96_021E6108(void *mon);
int ov96_021E6138(void *mon);
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, PokeathlonLampJumpMonView *view);
void **ov96_021E6290(PokeathlonCourseData *course, int a1, int a2, void *scene);
void ov96_021E634C(PokeathlonCourseData *course, int a1, int a2, void *scene, u32 a4, u8 a5, u32 a6);
u32 PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, int timer);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, int a1);
u32 PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int a3, int a4);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u16 score);
BOOL PokeathlonCourse_IsInRange(int x, int y, int touchX, int touchY, int radius, int *distance);
void PokeathlonCourse_InitObjTransfer(PokeathlonLampJumpSpriteConfig *config, int a1, int heapId, int a3, int a4);
void PokeathlonCourseScoreList_Rank(void *gfx, u32 a1);
void PokeathlonCourseScoreList_Init(void *gfx);
void PokeathlonCourseScoreList_AddPointsToTotals(void *gfx);
void PokeathlonCourseScoreList_SaveEventPoints(void *gfx, u32 a1);
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, void *gfx, u32 participant, u32 score, u8 a4, u8 a5, u32 a6);
void *PokeathlonCourseSprites_CreateResources(int heapId, int a1, int a2);
void PokeathlonCourseSprites_FreeResources(void *a0);
void *PokeathlonCourseMonObjects_Create(enum HeapID heapId, void *a1, void *a2, void *a3, void *a4);
void PokeathlonCourseMonObjects_Free(void *manager);
void PokeathlonCourseMonObjects_Load(void *manager, int count, PokeathlonLampJumpMonView *views, PokeathlonLampJumpMonSpriteInfo *info, int a4, int a5);
int PokeathlonCourseMonObjects_IsLoaded(void *manager);
void *PokeathlonCourseMonObjects_Get(void *manager, u8 index);
void PokeathlonCourseMonObject_SetDrawFlag(void *sprite, int a1);
void PokeathlonCourseMonObject_SetAnimState(void *sprite, int anim);
void ov96_021EAD08(void *sprite, int a1);
void PokeathlonCourseObject_SetTouchRadius(void *sprite, int a1);
void PokeathlonCourseObject_SetCollision(void *sprite, int x, int y);
void PokeathlonCourseObject_SetGroundPosition(void *sprite, int x, int y);
void PokeathlonCourseObject_GroundToCenter(void *sprite, int x, int y, int *outX, int *outY);
void PokeathlonCourseObjects_PauseAnims(void *manager, int a1);
void *PokeathlonCourseSpritePool_New(int heapId, PokeathlonLampJumpSceneConfig *config);
void PokeathlonCourseSpritePool_Free(void *scene);
void PokeathlonCourseSpritePool_SetResourceId(void *scene, int a1, int a2);
void PokeathlonCourseSpritePool_TransferResources(void *scene);
void PokeathlonCourseSpritePoolSprite_Show(void *sprite, int a1, int a2);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *sprite, int anim);
BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(void *sprite);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *sprite, VecFx32 *pos);
void PokeathlonCourseSpritePool_Draw(void *scene);
void PokeathlonCourseSpritePool_SetSurfaceCoords(void *scene, int a1, int a2, int a3, int a4);
void *PokeathlonCourseSpritePool_GetSpriteList(void *scene);
void PokeathlonCourseSpritePool_PauseAnims(void *scene, int a1);
void PokeathlonEvent_CalcPoints(void *gfx, u32 a1);
void PokeathlonLampJump_InitCamera(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_Draw3D(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_CalcTrampolineSurface(s32 (*a0)[128]);
void PokeathlonLampJump_UpdateJumps(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
void PokeathlonLampJump_SetRestingHeights(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_LoadSpriteResources(void *scene);
void PokeathlonLampJump_CreateSprites(PokeathlonLampJump *lampJump, void *scene);
void PokeathlonLampJump_PlaceSprites(PokeathlonLampJump *lampJump, u8 participant);
void PokeathlonLampJump_UpdateScoreDisplay(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_UpdateTimerDisplay(PokeathlonLampJump *lampJump, u16 timer);
void PokeathlonLampJump_PrintPlayerNames(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
void PokeathlonLampJump_InitWindows(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_PrintTotal(PokeathlonLampJump *lampJump, u16 score);
void PokeathlonLampJump_SetMonParams(PokeathlonCourseData *course, PokeathlonLampJumpMonParams *params, u8 mon, PokeathlonLampJumpMon *lampJumpMon);
void PokeathlonLampJump_CollideMons(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
void *PokeathlonLampJump_NewLamps(int heapId);
void PokeathlonLampJump_FreeLamps(void *a0);
void PokeathlonLampJump_UpdateLamps(void *a0);
void *PokeathlonLampJump_NewCpuPlayers(u32 heapId, int count, u32 mode, u32 a3, PokeathlonCourseData *course);
void PokeathlonLampJump_FreeCpuPlayers(void *a0);
void PokeathlonLampJump_UpdateCpuPlayers(PokeathlonCourseData *course, void *a1);

extern const PokeathlonLampJumpSceneConfig ov96_0221C0B8;
extern const GraphicsModes ov96_0221C100;
extern const BgTemplate ov96_0221C124;
extern const BgTemplate ov96_0221C140;
extern const BgTemplate ov96_0221C15C;
extern const BgTemplate ov96_0221C178;
extern const BgTemplate ov96_0221C194;
extern const BgTemplate ov96_0221C1B0;
extern const GraphicsBanks ov96_0221C1F4;
extern const u8 ov96_0221DC18[]; // sound player of each Pokemon

#pragma force_active on

BOOL PokeathlonLampJump_Init(PokeathlonCourseData *course) {
    PokeathlonLampJumpMonSpriteInfo info;
    PokeathlonLampJumpSpriteConfig spriteConfig;
    PokeathlonLampJumpSceneConfig sceneConfig;
    PokeathlonLampJumpMonView views[3];
    PokeathlonLampJumpSpriteOffset offsets[3];
    PokeathlonLampJumpScreenPos screenPos[3];
    PokeathlonLampJumpMonParams params;
    int screenX, screenY;
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i, mode, count, monIndex, x, jumpIndex, jumpX, column;
    u8 participant;
    void *sprite;

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, (enum HeapID)143, 0x50000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
        PokeathlonLampJump_SetGfxBanks();
        lampJump = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonLampJump));
        MI_CpuFill8(lampJump, 0, sizeof(PokeathlonLampJump));
        lampJump->bgConfig = BgConfig_Alloc((enum HeapID)143);
        PokeathlonCourse_SetHudPlanes(course, 8);
        spriteConfig.narcId = NARC_a_2_1_1;
        spriteConfig.mainSize = 0x40000;
        spriteConfig.subSize = 0x4000;
        spriteConfig.heapId = PokeathlonCourse_GetHeapID(course);
        PokeathlonCourse_InitObjTransfer(&spriteConfig, 15, 143, 0x300010, 0x300010);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, (enum HeapID)143);
        lampJump->heapId = (enum HeapID)143;
        FontID_Alloc(4, lampJump->heapId);
        PokeathlonLampJump_InitBgs(lampJump->bgConfig);
        PokeathlonLampJump_Init3D(lampJump);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        GF_CreateVramTransferManager(12, lampJump->heapId);
        lampJump->cellTransferState = CreateCellTransferStateManager(12, lampJump->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        lampJump->unk8C = PokeathlonLampJump_NewLamps(lampJump->heapId);
        count = PokeathlonCourse_GetParticipantCount(course);
        mode = PokeathlonCourse_GetMode(course);
        lampJump->unk138 = PokeathlonLampJump_NewCpuPlayers(lampJump->heapId, 4 - count, mode, (u32)PokeathlonCourse_GetDataCopyArea(course), course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        sceneConfig = ov96_0221C0B8;
        lampJump->scene = PokeathlonCourseSpritePool_New(lampJump->heapId, &sceneConfig);
        PokeathlonCourseSpritePool_SetSurfaceCoords(lampJump->scene, 0, FX32_CONST(192), 0, 0);
        lampJump->monSpriteManager = PokeathlonCourseMonObjects_Create(lampJump->heapId, (void *)3, (void *)10, NULL, PokeathlonCourseSpritePool_GetSpriteList(lampJump->scene));
        PokeathlonCourseSpritePool_SetResourceId(lampJump->scene, 0, 101);
        PokeathlonLampJump_LoadSpriteResources(lampJump->scene);
        PokeathlonCourseSpritePool_TransferResources(lampJump->scene);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 3:
        participant = ov96_021E5F24(course);
        for (i = 0; i < 3; i++) {
            PokeathlonCourse_GetMonInfo(course, participant, i, &views[i]);
            info.unk14[i] = ov96_021E6108(PokeathlonCourse_GetPerformance(course, participant, i));
        }
        info.unk00[0] = 0;
        info.unk00[1] = 1;
        info.unk00[2] = 1;
        info.unk00[3] = 3;
        info.unk00[4] = 3;
        PokeathlonCourseMonObjects_Load(lampJump->monSpriteManager, 3, views, &info, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 4:
        if (!PokeathlonCourseMonObjects_IsLoaded(lampJump->monSpriteManager)) {
            break;
        }
        lampJump->unk188 = PokeathlonCourseSprites_CreateResources(lampJump->heapId, 929, 1);
        {
            Sprite **playerSprite = (Sprite **)ov96_021E6290(course, 192, (int)lampJump->unk188, lampJump->scene);
            Sprite_SetDrawPriority(*playerSprite, 1);
        }
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)lampJump->bgConfig); // takes the BgConfig
        PokeathlonCourse_SetField1F4(course, 1);
        ReadWholeNarcMemberByIdPair(offsets, NARC_a_1_7_0, 16);
        for (monIndex = 0; monIndex < 3; monIndex++) {
            PokeathlonLampJumpMon *mon = &lampJump->mons[monIndex];

            x = 64 + monIndex * 64;
            sprite = PokeathlonCourseMonObjects_Get(lampJump->monSpriteManager, monIndex);
            PokeathlonCourseMonObject_SetDrawFlag(sprite, 1);
            mon->sprite = sprite;
            count = ov96_021E6138(PokeathlonCourse_GetPerformance(course, 0, monIndex));
            PokeathlonCourseObject_SetCollision(sprite, offsets[count - 1].x, offsets[count - 1].y);
            PokeathlonCourseMonObject_SetAnimState(sprite, 2);
            PokeathlonCourseObject_SetGroundPosition(sprite, x, 288);
            PokeathlonCourseObject_SetTouchRadius(sprite, ov96_021E6104());
            mon->drawX = (u32)x << FX32_SHIFT;
            mon->drawY = 288 << FX32_SHIFT;
            mon->x = (u32)x << FX32_SHIFT;
            PokeathlonCourseObject_GroundToCenter(sprite, x, 288, &screenX, &screenY);
            screenPos[monIndex].x = screenX;
            screenPos[monIndex].y = screenY;
        }
        ov96_021E634C(course, 0, (int)lampJump->unk188, lampJump->scene, 1, 3, (u32)screenPos);
        lampJump->selectedMon = 3;
        PokeathlonLampJump_LoadBgGfx(lampJump);
        PokeathlonLampJump_CreateSprites(lampJump, lampJump->scene);
        PokeathlonLampJump_InitWindows(lampJump);
        PokeathlonLampJump_PlaceSprites(lampJump, ov96_021E5F24(course));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 5:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        ReadWholeNarcMemberByIdPair(&params, NARC_a_1_7_0, 6);
        for (i = 0; i < 3; i++) {
            PokeathlonLampJump_SetMonParams(course, &params, i, &lampJump->mons[i]);
        }
        ov96_021E5F24(course);
        lampJump->timer = 1800;
        PokeathlonLampJump_UpdateTimerDisplay(lampJump, lampJump->timer);
        PokeathlonLampJump_PrintPlayerNames(course, lampJump);
        for (column = 0; column < 128; column++) {
            lampJump->unk1A0[0][column] = 0;
            lampJump->unk1A0[1][column] = 0;
            lampJump->unk1A0[2][column] = 0;
            lampJump->unk1A0[3][column] = 0;
            lampJump->unk1A0[4][column] = 0;
            lampJump->unk1A0[5][column] = 0;
            lampJump->unk1A0[6][column] = 0;
        }
        for (jumpIndex = 0; jumpIndex < 3; jumpIndex++) {
            lampJump->jumps[jumpIndex].height = 0;
            jumpX = jumpIndex * 64 + 64;
            lampJump->monColumns[jumpIndex] = jumpX / 2;
            lampJump->jumps[jumpIndex].unk04 = 4096.0 * ((f32)jumpX / 64.0 - 2.0);
            lampJump->mons[jumpIndex].unk28 = TRUE;
        }
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 6:
        PokeathlonLampJump_UpdateScoreDisplay(lampJump);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonLampJump_Draw(PokeathlonCourseData *course) {
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);

    PokeathlonCourseSpritePool_Draw(lampJump->scene);
    thunk_UpdateCellTransferStateManager();
    return TRUE;
}

BOOL ov96_021F54D4(PokeathlonCourseData *course) {
    void *gfx;
    int i;

    PokeathlonCourse_GetHeapAllocPtr4(course);
    gfx = PokeathlonCourse_GetGraphicsSystem(course);
    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < 4; i++) {
        PokeathlonCourse_AddScoreEntry(course, gfx, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, 0x128);
    }
    PokeathlonCourseScoreList_Rank(gfx, 1);
    PokeathlonEvent_CalcPoints(gfx, 5);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 5);
    return TRUE;
}

#pragma force_active on

BOOL PokeathlonLampJump_Exit(PokeathlonCourseData *course) {
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;
    Window *window;

    WirelessStatusIcon_Destroy();
    Camera_Delete(lampJump->camera);
    PokeathlonLampJump_FreeLamps(lampJump->unk8C);
    Heap_Free(lampJump->bg2ScreenFile);
    FreeBgTilemapBuffer(lampJump->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(lampJump->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(lampJump->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(lampJump->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(lampJump->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(lampJump->bgConfig, GF_BG_LYR_SUB_2);
    for (i = 0, window = lampJump->windows; i < 5; i++, window++) {
        RemoveWindow(window);
    }
    Heap_Free(lampJump->bgConfig);
    PokeathlonCourse_ResetField3A4(course);
    PokeathlonCourseSpritePool_Free(lampJump->scene);
    PokeathlonCourseMonObjects_Free(lampJump->monSpriteManager);
    PokeathlonCourseSprites_FreeResources(lampJump->unk188);
    PokeathlonLampJump_FreeCpuPlayers(lampJump->unk138);
    FontID_Release(4);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    DestroyCellTransferStateManager(lampJump->cellTransferState);
    GF_DestroyVramTransferManager();
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    reg_G2_BLDCNT = 0;
    Heap_Destroy((enum HeapID)143);
    return TRUE;
}

#pragma force_active reset

void PokeathlonLampJump_SetGfxBanks(void) {
    GraphicsBanks banks = ov96_0221C1F4;
    GfGfx_SetBanks(&banks);
}

BOOL PokeathlonLampJump_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (*state) {
    case 0:
        PokeathlonLampJump_UpdateDisplay(course);
        PokeathlonCourse_SetStateTransitionType(course, 20);
        (*state)++;
        break;
    case 1:
        PokeathlonLampJump_UpdateDisplay(course);
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0, 6, 1, lampJump->heapId);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        PokeathlonLampJump_UpdateDisplay(course);
        break;
    case 3:
        PokeathlonLampJump_UpdateDisplay(course);
        if (PokeathlonCourse_RunStartCountdown(course)) {
            (*state)++;
        }
        break;
    case 4:
        PokeathlonLampJump_HandleTouch(course);
        if (PokeathlonLampJump_Update(course)) {
            (*state)++;
        }
        break;
    case 5:
        if (PokeathlonCourse_RunFinish(course)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, lampJump->heapId);
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

BOOL PokeathlonLampJump_CalcScores(PokeathlonCourseData *course, u8 *state) {
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);
    int self, i, score, participant, member, total;
    u8 participantId, random;
    u8 *stats;

    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    self = ov96_021E5F24(course);
    score = 0;
    for (i = 0; i < 3; i++) {
        score += lampJump->mons[i].score;
    }
    if (score > 999) {
        score = 999;
    }
    PokeathlonCourse_SetField5E0_AtIndex(course, (u8)self, (u16)score);
    if (self == 0) {
        // the host simulates the participants without a player
        for (participant = PokeathlonCourse_GetParticipantCount(course); participant < 4; participant++) {
            member = 0;
            total = member;
            participantId = (u8)participant;
            for (; member < 3; member++) {
                stats = PokeathlonCourse_GetMonStats(course, participant, member);
                if (stats[2]) {
                    int bonus = LCRandom() % 21;
                    int base = stats[3] * 2 + (stats[2] * 6 + 80 + stats[4] * 2);
                    total += base + bonus;
                    LCRandom();
                }
                PokeathlonCourse_AddMonCounter(course, participantId, (u8)member, 2, 0);
                random = (u8)(LCRandom() % 21);
                PokeathlonCourse_AddMonCounter(course, participantId, (u8)member, 4, random);
                PokeathlonCourse_AddMonCounter(course, participantId, (u8)member, 1, random);
            }
            PokeathlonCourse_SetField5E0_AtIndex(course, (u8)participant, (u16)total);
        }
    }
    return TRUE;
}

#pragma force_active reset

void PokeathlonLampJump_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main2;
    BgTemplate main1;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub1;
    BgTemplate sub2;

    modes = ov96_0221C100;
    SetBothScreensModesAndDisable(&modes);
    main2 = ov96_0221C124;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    main1 = ov96_0221C140;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main3 = ov96_0221C15C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = ov96_0221C178;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = ov96_0221C194;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    sub2 = ov96_0221C1B0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonLampJump_LoadBgGfx(PokeathlonLampJump *lampJump) {
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 7, lampJump->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 5, lampJump->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 10, lampJump->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_8, 12, lampJump->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 8, lampJump->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 6, lampJump->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 11, lampJump->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_8, 13, lampJump->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, lampJump->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_8, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, lampJump->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_8, 9, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, lampJump->heapId);
    lampJump->bg2ScreenFile = GfGfxLoader_GetScrnData(NARC_a_1_6_8, 14, FALSE, &lampJump->bg2Screen, lampJump->heapId);
}

#pragma force_active on

BOOL PokeathlonLampJump_Update(PokeathlonCourseData *course) {
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);
    u32 *sent = (u32 *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course));

    lampJump->unk142 = 0;
    if (lampJump->timer) {
        lampJump->timer--;
    }
    if ((u8)(lampJump->timer <= 0)) {
        int score = 0, i;

        PokeathlonCourseSpritePool_PauseAnims(lampJump->scene, 1);
        PokeathlonCourseObjects_PauseAnims(lampJump->monSpriteManager, 1);
        for (i = 0; i < 3; i++) {
            score += lampJump->mons[i].score;
        }
        if (score > 999) {
            score = 999;
        }
        PokeathlonCourse_SetRecordScore(course, (u16)score);
        PokeathlonLampJump_PrintTotal(lampJump, (u16)score);
        PokeathlonLampJump_UpdateDisplay(course);
        return TRUE;
    }
    PokeathlonLampJump_UpdateJumps(course, lampJump);
    PokeathlonLampJump_CalcTrampolineSurface(lampJump->unk1A0);
    PokeathlonLampJump_SetRestingHeights(lampJump);
    PokeathlonLampJump_CollideMons(course, lampJump);
    PokeathlonLampJump_UpdateLamps(lampJump->unk8C);
    PokeathlonLampJump_UpdateDisplay(course);
    *sent = lampJump->unk142;
    return FALSE;
}

#pragma force_active reset

void PokeathlonLampJump_UpdateDisplay(PokeathlonCourseData *course) {
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);

    PokeathlonLampJump_UpdateMonSprites(course);
    PokeathlonLampJump_UpdateTimerDisplay(lampJump, (u16)lampJump->timer);
    PokeathlonLampJump_UpdateScoreDisplay(lampJump);
    PokeathlonLampJump_UpdatePlayerSprites(course);
    PokeathlonCourse_UpdateTimeUpCountdown(course, lampJump->timer);
    PokeathlonLampJump_Draw3D(lampJump);
}

#pragma force_active on

void PokeathlonLampJump_HandleTouch(PokeathlonCourseData *course) {
    u8 *data = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);
    u32 selected;

    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)ov96_021E5F24(course), 3, 0, 1);
        selected = PokeathlonLampJump_FindTouchedMon(lampJump, gSystem.touchX, gSystem.touchY);
        if (selected < 3) {
            lampJump->selectedMon = selected;
            lampJump->touchStart.x = gSystem.touchX << FX32_SHIFT;
            lampJump->touchStart.y = (gSystem.touchY << FX32_SHIFT) + FX32_CONST(192);
            lampJump->touchStart.z = 0;
        }
    } else if (System_GetTouchHeld()) {
        if (lampJump->selectedMon < 3) {
            if (lampJump->holdFrames < 7) {
                lampJump->holdFrames++;
            } else {
                VecFx32 flick = { 0, 0, 0 };
                PokeathlonLampJump_GetFlick((u8)gSystem.touchX, (u8)gSystem.touchY, &lampJump->touchStart, &flick);
                PokeathlonLampJump_LaunchMon(lampJump, lampJump->selectedMon, &flick);
                lampJump->selectedMon = 3;
                lampJump->holdFrames = 0;
            }
        }
    } else {
        if (lampJump->selectedMon < 3 && lampJump->holdFrames < 7) {
            VecFx32 flick = { 0, 0, 0 };
            PokeathlonLampJump_GetFlick((u8)gSystem.touchX, (u8)gSystem.touchY, &lampJump->touchStart, &flick);
            PokeathlonLampJump_LaunchMon(lampJump, lampJump->selectedMon, &flick);
        }
        lampJump->selectedMon = 3;
        lampJump->holdFrames = 0;
    }
    if (ov96_021E5F24(course) == 0) {
        // the host gathers the first word of every player's link block (0x28 bytes each)
        PokeathlonLampJumpSyncBlock *gathered = (PokeathlonLampJumpSyncBlock *)ov96_021E8A20(data + 0x28);
        PokeathlonLampJumpSyncBlock *hostBlock = (PokeathlonLampJumpSyncBlock *)ov96_021E8A20(data + 0x50);
        *hostBlock = *(PokeathlonLampJumpSyncBlock *)ov96_021E8A20(data);
        PokeathlonLampJump_UpdateCpuPlayers(course, lampJump->unk138);
        {
            u8 values[4];
            int i;

            i = 0;
            data += 0x50;
            for (; i < 4; i++) {
                values[i] = ((PokeathlonLampJumpSyncBlock *)ov96_021E8A20(data))->words[0];
                data += 0x28;
            }
            for (i = 0; i < 4; i++) {
                gathered->bytes[i] = values[i];
            }
        }
    }
}

void PokeathlonLampJump_UpdateMonSprites(PokeathlonCourseData *course) {
    int i, x, y;
    PokeathlonLampJump *lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonLampJumpMon *mon;

    for (i = 0, mon = lampJump->mons; i < 3; mon++, i++) {
        mon->drawY = FX32_CONST(288) - lampJump->jumps[i].height * 64;
        mon->drawX = mon->x;
        if (mon->drawX < FX32_CONST(32)) {
            mon->drawX = FX32_CONST(32);
        } else if (mon->drawX > FX32_CONST(223)) {
            mon->drawX = FX32_CONST(223);
        }
        lampJump->monColumns[i] = (u8)((mon->drawX / FX32_ONE) / 2);
        x = mon->drawX / FX32_ONE;
        y = mon->drawY / FX32_ONE;
        PokeathlonCourseObject_SetGroundPosition(mon->sprite, x, y);
        {
            VecFx32 pos = { 0, 0, 0 };
            pos.x = x << FX32_SHIFT;
            pos.y = y << FX32_SHIFT;
            if (((u8 *)PokeathlonCourse_GetPerformance(course, ov96_021E5F24(course), i))[5]) {
                pos.y -= FX32_CONST(24);
            } else {
                pos.y -= FX32_CONST(16);
            }
            PokeathlonCourseSpritePoolSprite_SetPosition(mon->effectSprite, &pos);
        }
        if (mon->unk26) {
            PokeathlonCourseSpritePoolSprite_Show(mon->effectSprite, 1, 1);
            ov96_021EAD08(mon->sprite, 20);
            if (!GF_SndCountPlayingSeqOnPlayer(ov96_0221DC18[i])) {
                PlaySEOnPlayer(SEQ_SE_GS_W146, ov96_0221DC18[i]);
            }
        } else {
            PokeathlonCourseSpritePoolSprite_Show(mon->effectSprite, 1, 0);
            ov96_021EAD08(mon->sprite, 0);
            if (lampJump->jumps[i].jumping) {
                PokeathlonCourseMonObject_SetAnimState(mon->sprite, 1);
            } else {
                PokeathlonCourseMonObject_SetAnimState(mon->sprite, 2);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (lampJump->effectPending[i]) {
            lampJump->effectPending[i] = FALSE;
            PokeathlonCourseSpritePoolSprite_Show(lampJump->effectSprites[i], 1, 1);
            PokeathlonCourseSpritePoolSprite_SetPosition(lampJump->effectSprites[i], &lampJump->effectPositions[i]);
            PokeathlonCourseSpritePoolSprite_SetAnim(lampJump->effectSprites[i], 9);
        }
    }
}

#pragma force_active reset

void PokeathlonLampJump_UpdatePlayerSprites(PokeathlonCourseData *course) {
    u8 *received;
    PokeathlonLampJump *lampJump;
    int i;

    received = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    lampJump = PokeathlonCourse_GetHeapAllocPtr4(course);
    for (i = 0; i < 4; i++) {
        if (i == ov96_021E5F24(course)) {
            if (lampJump->unk142 != 0 && PokeathlonCourseSpritePoolSprite_IsAnimated(lampJump->playerSprites[i]) == FALSE) {
                PokeathlonCourseSpritePoolSprite_SetAnim(lampJump->playerSprites[i], i + 13);
            }
        } else if (received[i] != 0 && PokeathlonCourseSpritePoolSprite_IsAnimated(lampJump->playerSprites[i]) == FALSE) {
            PokeathlonCourseSpritePoolSprite_SetAnim(lampJump->playerSprites[i], i + 13);
        }
    }
}

void PokeathlonLampJump_GetFlick(int touchX, int touchY, const VecFx32 *start, VecFx32 *flick) {
    VecFx32 touch;

    touch.x = touchX << FX32_SHIFT;
    touch.y = (touchY << FX32_SHIFT) + FX32_CONST(192);
    touch.z = 0;
    VEC_Subtract(&touch, start, flick);
    if (flick->y > FX32_CONST(112)) {
        flick->y = FX32_CONST(112);
    }
}

#pragma force_active on

u8 PokeathlonLampJump_FindTouchedMon(PokeathlonLampJump *lampJump, u16 touchX, u16 touchY) {
    fx32 minDistance = ov96_021E6104(lampJump) << FX32_SHIFT;
    u8 i;
    u8 count = 0;
    u8 nearest;
    s32 distances[3];
    u8 inRange[3];
    int distance;
    s32 x;
    s32 y;

    for (i = 0; i < 3; i++) {
        PokeathlonLampJumpMon *mon = &lampJump->mons[i];

        if (mon->unk26 != 0) {
            inRange[i] = FALSE;
            distances[i] = 0;
        } else {
            PokeathlonCourseObject_GroundToCenter(mon->sprite, mon->x / FX32_ONE, (FX32_CONST(96) - (lampJump->jumps[i].height << 6)) / FX32_ONE, &x, &y);
            if (PokeathlonCourse_IsInRange(x, y, touchX, touchY, ov96_021E6104(), &distance)) {
                distances[i] = distance;
                inRange[i] = TRUE;
                count++;
            } else {
                distances[i] = 0;
                inRange[i] = FALSE;
            }
        }
    }
    if (count == 0) {
        return 3;
    }
    nearest = 3;
    for (i = 0; i < 3; i++) {
        if (inRange[i] && distances[i] < minDistance) {
            nearest = i;
            minDistance = distances[i];
        }
    }
    if (nearest == 3) {
        GF_ASSERT(FALSE);
        return 3;
    }
    return nearest;
}

#pragma force_active reset

BOOL PokeathlonLampJump_CheckCollision(const VecFx32 *a, fx32 radiusA, const VecFx32 *b, fx32 radiusB) {
    VecFx32 diff;

    VEC_Subtract(a, b, &diff);
    if (VEC_Mag(&diff) > radiusA + radiusB) {
        return FALSE;
    }
    return TRUE;
}

void PokeathlonLampJump_LaunchMon(PokeathlonLampJump *lampJump, s32 mon, VecFx32 *flick) {
    PokeathlonLampJumpJump *jump;
    s32 velocity;
    s32 velocityX;
    s32 *monVelocityX;

    if (flick->y > 0) {
        jump = &lampJump->jumps[mon];
        if (jump->jumping == FALSE) {
            velocity = (s32)((double)(flick->y / 64) / 2.5);
            velocityX = -flick->x / 4;
            if (velocityX >= 0) {
                if (velocityX > 0xA000) {
                    velocityX = 0xA000;
                }
            } else if (velocityX < -0xA000) {
                velocityX = -0xA000;
            }
            monVelocityX = &lampJump->mons[mon].velocityX;
            *monVelocityX = velocityX;
            *monVelocityX = *monVelocityX * lampJump->mons[mon].flickScale / 12;
            jump->velocity = -velocity;
            jump->launchVelocity = -velocity;
            jump->jumping = TRUE;
            PlaySEOnPlayer(SEQ_SE_GS_GUGU, ov96_0221DC18[mon]);
        }
    }
}

void PokeathlonLampJump_Init3D(PokeathlonLampJump *lampJump) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    reg_G3_SWAP_BUFFERS = 2;
    reg_G2_BG0CNT = (u16)((reg_G2_BG0CNT & ~3) | 2);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(FALSE);
    G3X_EdgeMarking(FALSE);
    G3X_SetClearColor(0, 0, 0x7FFF, 63, FALSE);
    reg_G3_VIEWPORT = 0xBFFF0000;
    lampJump->camera = Camera_New(lampJump->heapId);
    PokeathlonLampJump_InitCamera(lampJump);
}
