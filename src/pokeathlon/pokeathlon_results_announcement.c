#include "pokeathlon/pokeathlon_results_announcement.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg/msg_0309.h"

#include "assert.h"
#include "bg_window.h"
#include "brightness.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "obj_pltt_transfer.h"
#include "player_data.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "sprite_resource.h"
#include "wireless_status_icon.h"

// Declared by no public header.
void WirelessStatusIcon_Refresh(void);
void WirelessStatusIcon_SetBackPriority(void);
void NNS_G2dSetImageLocation(NNSG2dImageProxy *proxy, NNS_G2D_VRAM_TYPE type, u32 addr);
void NNS_G2dSetImagePaletteLocation(NNSG2dImagePaletteProxy *proxy, NNS_G2D_VRAM_TYPE type, u32 addr);
void TextOBJ_UpdatePosition(TextOBJ *textObj);
void TextOBJ_SetPalIndexRespectVramOffset(TextOBJ *textObj, int index);

// Called once without its argument (the plate is still in r0 there), so it has no prototype.
u16 PokeathlonResultsAnnouncement_GetPlateRank();

// Course helpers, the announcer's text box and the scene (OVY_96, not yet tidied).
PokeathlonResultsAnnouncementScore *PokeathlonCourseScoreList_GetRanked(void *scoreList, u8 rank);
int PokeathlonCourseScoreList_GetCount(void *scoreList);
int ov96_021E9528(void *scoreList);
int PokeathlonCourseScoreList_GetEventPoints(void *scoreList, u32 a1, int participant);
u16 ov96_021E8424(u8 team);
void *PokeathlonCourseSpritePool_New(enum HeapID heapId, const void *config);
void PokeathlonCourseSpritePool_Free(void *scene);
void PokeathlonCourseSpritePool_SetResourceId(void *scene, int a1, int a2);
void PokeathlonCourseSpritePool_LoadChar(void *scene, int narcId, int a2, int a3, int a4);
void PokeathlonCourseSpritePool_LoadPltt(void *scene, int narcId, int a2, int a3, int a4, int a5);
void PokeathlonCourseSpritePool_LoadCell(void *scene, int narcId, int a2, int a3);
void PokeathlonCourseSpritePool_LoadAnim(void *scene, int narcId, int a2, int a3);
void PokeathlonCourseSpritePool_TransferResources(void *scene);
Sprite *PokeathlonCourseSpritePool_CreateSprite(void *scene, int priority, int screen, u16 resId, u8 anim);
void *PokeathlonCourseSpritePool_Take(void *scene, u8 resId, u8 anim);
void PokeathlonCourseSpritePoolSprite_Show(void *sprite, int a1, int a2);
Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(void *sprite);
void PokeathlonCourseSpritePool_Draw(void *scene);
void PokeathlonCourseSpritePool_SetSurfaceCoords(void *scene, int a1, int a2, int a3, int a4);
SpriteList *PokeathlonCourseSpritePool_GetSpriteList(void *scene);
void *PokeathlonEventBanner_New(u32 a0, enum HeapID heapId);
void PokeathlonEventBanner_Free(void *a0);
void PokeathlonEventBanner_AddWindows(void *a0, BgConfig *bgConfig);
void PokeathlonEventBanner_LoadGraphics(void *a0);
void PokeathlonEventBanner_PrintTexts(void *a0);
void *PokeathlonCourseMessageBox_New(enum HeapID heapId);
void PokeathlonCourseMessageBox_Init(void *messageBox, BgConfig *bgConfig, int a2, int mode, SaveData *saveData);
void PokeathlonCourseMessageBox_Free(void *messageBox);
void PokeathlonCourseMessageBox_Update(void *messageBox);
void PokeathlonCourseMessageBox_Print(void *messageBox, int msgId);
void PokeathlonCourseMessageBox_Close(void *messageBox);
void *PokeathlonCourseMessageBox_GetMsgFormat(void *messageBox);
u32 PokeathlonCourseMessageBox_IsBusy(void *messageBox);
void PokeathlonCourse_LoadTrainerPic(ManagedSprite *sprite, int a1, int a2, enum HeapID heapId);
void PokeathlonCourse_LoadTrainerPicIntoSprite(Sprite *sprite, u32 a1, int a2, enum HeapID heapId);
void PokeathlonCourse_LoadPlayerTrainerPic(Sprite *sprite, PokeathlonCourseData *course, u32 participant, int a3);
int PokeathlonCourse_GetTrainerPicFileId(int a0);
int PokeathlonCourse_GetNpcTrainerPicFileId(int a0);

extern const OamCharTransferParam ov96_0221C2C4;
extern const SpriteResourceCountsListUnion ov96_0221C2D8;
extern const OamManagerParam ov96_0221C2F0;
extern const PokeathlonResultsAnnouncementSceneConfig ov96_0221C2A8;
extern const GraphicsModes ov96_0221C2B4;
extern const PokeathlonResultsAnnouncementBgTemplates ov96_0221C338;
extern const u8 ov96_0221C290[6]; // BG layers
extern const u8 ov96_0221C298[6]; // BG types
extern const GraphicsBanks ov96_0221C310;
extern const VecFx32 ov96_0221C3E0;

#pragma force_active on

BOOL PokeathlonResultsAnnouncement_Init(PokeathlonCourseData *course) {
    PokeathlonResultsAnnouncement *announcement = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonResultsAnnouncementSceneConfig sceneConfig;
    PokeathlonResultsAnnouncementAffine affine;
    void *scoreList;
    int value;

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, (enum HeapID)137, 0x40000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
        PokeathlonResultsAnnouncement_SetGfxBanks();
        announcement = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonResultsAnnouncement));
        MI_CpuFill8(announcement, 0, sizeof(PokeathlonResultsAnnouncement));
        announcement->heapId = (enum HeapID)137;
        FontID_Alloc(4, (enum HeapID)137);
        announcement->bgConfig = BgConfig_Alloc((enum HeapID)137);
        announcement->messageBox = PokeathlonCourseMessageBox_New(announcement->heapId);
        PokeathlonResultsAnnouncement_CreatePlates(&announcement->ranking.plates, (enum HeapID)137, announcement->bgConfig, course);
        announcement->unk80 = PokeathlonEventBanner_New(PokeathlonCourse_GetField3D8_ForCurrentParticipant(course), announcement->heapId);
        PokeathlonResultsAnnouncement_InitSprites(announcement);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        sceneConfig = ov96_0221C2A8;
        announcement->scene = PokeathlonCourseSpritePool_New(announcement->heapId, &sceneConfig);
        PokeathlonCourseSpritePool_SetSurfaceCoords(announcement->scene, 0, 0, 0, FX32_CONST(300));
        scoreList = PokeathlonCourse_GetGraphicsSystem(course);
        value = PokeathlonCourseScoreList_GetCount(scoreList);
        PokeathlonResultsAnnouncement_StartRanking(&announcement->ranking, course, (u8)value, ov96_021E9528(scoreList));
        announcement->scoreboard = PokeathlonResultsAnnouncement_NewScoreboard(announcement->heapId, announcement->bgConfig, announcement->scene);
        PokeathlonResultsAnnouncement_LoadScoreboardResources(announcement->scoreboard, 2);
        PokeathlonCourseSpritePool_TransferResources(announcement->scene);
        PokeathlonResultsAnnouncement_InitScoreboard(announcement->scoreboard, course, 300);
        PokeathlonResultsAnnouncement_LoadSpriteResources(announcement);
        PokeathlonResultsAnnouncement_CreateSprite(announcement);
        PokeathlonCourse_LoadTrainerPic(announcement->sprites[0], PokeathlonCourse_GetTrainerPicFileId(2), 11, announcement->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)announcement->bgConfig); // takes the BgConfig
        PokeathlonResultsAnnouncement_InitBgs(announcement->bgConfig, announcement->heapId);
        PokeathlonEventBanner_AddWindows(announcement->unk80, announcement->bgConfig);
        PokeathlonResultsAnnouncement_LoadBgGfx(announcement);
        PokeathlonEventBanner_LoadGraphics(announcement->unk80);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
        value = PokeathlonCourse_GetMode(course);
        PokeathlonCourseMessageBox_Init(announcement->messageBox, announcement->bgConfig, 5, value, PokeathlonCourse_GetSaveData(course));
        PokeathlonEventBanner_PrintTexts(announcement->unk80);
        affine.a = FX32_ONE;
        affine.b = 0;
        affine.c = 0;
        affine.d = FX32_ONE;
        OS_WaitVBlankIntr();
        Bg_SetTextDimAndAffineParams(announcement->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_ADD_X, 0, (MtxFx22 *)&affine, 128, 96);
        Bg_SetTextDimAndAffineParams(announcement->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SUB_Y, 16, (MtxFx22 *)&affine, 128, 96);
        Bg_SetTextDimAndAffineParams(announcement->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_ADD_X, 0, (MtxFx22 *)&affine, 128, 96);
        Bg_SetTextDimAndAffineParams(announcement->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SUB_Y, 16, (MtxFx22 *)&affine, 128, 96);
        PokeathlonResultsAnnouncement_InitWindow(&announcement->window, announcement->bgConfig, announcement->heapId);
        PokeathlonResultsAnnouncement_PrintTitle(&announcement->window, announcement->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 3:
        if (ov96_021E5F24(course) == 0) {
            // the host picks the announcer's closing comment (one of 5) for everyone
            u8 *comment = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
            *comment = MTRandom() % 5;
        }
        if (PokeathlonCourse_GetMode(course) == 1) {
            WirelessStatusIcon_CreateOnScreenIfConnected(1);
            WirelessStatusIcon_SetBackPriority();
            WirelessStatusIcon_Refresh();
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        PokeathlonCourse_SetField1F4(course, 1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 4:
        PlayBGM(SEQ_GS_PT_RESULT);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, announcement->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonResultsAnnouncement_Draw(PokeathlonCourseData *course) {
    PokeathlonResultsAnnouncement *announcement = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonCourseSpritePool_Draw(announcement->scene);
    SpriteSystem_DrawSprites(announcement->spriteManager);
    PokeathlonResultsAnnouncement_DrawPlates(announcement->ranking.plates);
    return TRUE;
}

BOOL PokeathlonResultsAnnouncement_Exit(PokeathlonCourseData *course) {
    PokeathlonResultsAnnouncement *announcement = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    FontID_Release(4);
    G2_SetBlendBrightness(0x3F, 0);
    PokeathlonEventBanner_Free(announcement->unk80);
    RemoveWindow(&announcement->window);
    PokeathlonCourseMessageBox_Free(announcement->messageBox);
    for (i = 0; i < 6; i++) {
        FreeBgTilemapBuffer(announcement->bgConfig, ov96_0221C290[i]);
    }
    PokeathlonResultsAnnouncement_FreeScoreboard(announcement->scoreboard);
    PokeathlonResultsAnnouncement_FreePlates(announcement->ranking.plates);
    PokeathlonCourseSpritePool_Free(announcement->scene);
    PokeathlonResultsAnnouncement_FreeSprites(announcement);
    Heap_Free(announcement->bgConfig);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    Heap_Destroy((enum HeapID)137);
    WirelessStatusIcon_Destroy();
    return TRUE;
}

void PokeathlonResultsAnnouncement_SetGfxBanks(void) {
    GraphicsBanks banks = ov96_0221C310;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on

void PokeathlonResultsAnnouncement_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    GraphicsModes modes = ov96_0221C2B4;
    PokeathlonResultsAnnouncementBgTemplates templates;
    volatile int i;
    const u8 *type;
    const u8 *layer;
    BgTemplate *template;
    int next;

    SetBothScreensModesAndDisable(&modes);
    templates = ov96_0221C338;
    type = ov96_0221C298;
    layer = ov96_0221C290;
    i = 0;
    template = templates.templates;
    do {
        InitBgFromTemplate(bgConfig, *layer, template, *type);
        BgClearTilemapBufferAndCommit(bgConfig, *layer);
        BG_ClearCharDataRange(*layer, 64, 0, heapId);
        next = i + 1;
        type++;
        template ++;
        layer++;
        i = next;
    } while (next < 6);
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_LoadBgGfx(PokeathlonResultsAnnouncement *announcement) {
    GfGfxLoader_LoadCharData(NARC_a_1_6_0, 3, announcement->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, announcement->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_0, 5, announcement->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, announcement->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_0, 3, announcement->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, announcement->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_0, 4, announcement->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, announcement->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_0, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, announcement->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_0, 2, announcement->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, announcement->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_0, 1, announcement->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, announcement->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_0, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, announcement->heapId);
}

// Byte 2 of the host's block: every player has seen the whole announcement.
BOOL PokeathlonResultsAnnouncement_AllPlayersFinished(PokeathlonCourseData *course) {
    return ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0)[2];
}

#pragma force_active on

// State 0 of the screen: the announcer's lines, the ranking, the winner and the scoreboard.
BOOL PokeathlonResultsAnnouncement_Run(PokeathlonCourseData *course, u8 *state) {
    int waiting = FALSE;
    int done;
    PokeathlonResultsAnnouncement *announcement = PokeathlonCourse_GetHeapAllocPtr4(course);
    MessageFormat *messageFormat;
    int msgId;
    PokeathlonResultsAnnouncementScore *ranked[4];
    int i;

    done = FALSE;
    IsFanfarePlaying();
    if (PokeathlonCourse_GetMode(course) == 1) {
        waiting = PokeathlonResultsAnnouncement_WaitForPlayers(course);
    }
    PokeathlonCourseMessageBox_Update(announcement->messageBox);
    if (!(PokeathlonCourseMessageBox_IsBusy(announcement->messageBox) || waiting)) {
        messageFormat = PokeathlonCourseMessageBox_GetMsgFormat(announcement->messageBox);
        msgId = -1;
        switch (*state) {
        case 0:
            msgId = PokeathlonResultsAnnouncement_GetIntroMessage(course);
            (*state)++;
            break;
        case 1:
            if (!PokeathlonResultsAnnouncement_UpdateRanking(&announcement->ranking, course)) {
                break;
            }
            for (i = 0; i < 4; i++) {
                ranked[i] = PokeathlonCourseScoreList_GetRanked(PokeathlonCourse_GetGraphicsSystem(course), (u8)i);
            }
            if (ranked[0]->rank == ranked[1]->rank) {
                msgId = msg_0309_00254; // "This... was a very close competition!"
            } else {
                PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(course, ranked[0]->participant);
                BufferPlayersName(messageFormat, 0, profile);
                msgId = msg_0309_00253; // "Team {STRVAR_1}! Congratulations on First Place!"
            }
            (*state)++;
            break;
        case 2:
            StartBrightnessTransition(16, -8, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), SCREEN_MASK_MAIN);
            UpdateMainScreenBrightnessSurface((GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD));
            reg_G2_BLDALPHA = 16;
            (*state)++;
            break;
        case 3:
            if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
                msgId = msg_0309_00255; // "This event's results will be determined by points!"
                (*state)++;
            }
            break;
        case 4:
            for (i = 0; i < 4; i++) {
                u32 field3D8 = PokeathlonCourse_GetField3D8_ForCurrentParticipant(course);
                int points = PokeathlonCourseScoreList_GetEventPoints(PokeathlonCourse_GetGraphicsSystem(course), field3D8, i);
                if (points > 999) {
                    points = 999;
                }
                PokeathlonResultsAnnouncement_SetScore(announcement->scoreboard, points, (u8)i);
            }
            StartBrightnessTransition(16, 0, 16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), SCREEN_MASK_SUB);
            (*state)++;
            break;
        case 5:
            if (IsBrightnessTransitionActive(SCREEN_MASK_SUB)) {
                (*state)++;
            }
            break;
        case 6:
            msgId = PokeathlonResultsAnnouncement_GetCommentMessage(course);
            announcement->finished = TRUE;
            (*state)++;
            break;
        case 7:
            if (PokeathlonCourse_GetMode(course) == 0 || PokeathlonResultsAnnouncement_AllPlayersFinished(course)) {
                BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 12, 1, announcement->heapId);
                GF_SndStartFadeOutBGM(0, 12);
                (*state)++;
            }
            break;
        case 8:
            if (IsPaletteFadeFinished()) {
                PokeathlonCourseMessageBox_Close(announcement->messageBox);
                done = TRUE;
            }
            break;
        }
        if (msgId != -1) {
            PokeathlonCourseMessageBox_Print(announcement->messageBox, msgId);
            announcement->messageCount++;
        }
    }
    return done;
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_CreatePlates(PokeathlonResultsAnnouncementPlates **plates, enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    *plates = PokeathlonResultsAnnouncement_NewPlates(heapId, bgConfig, course);
}

static inline void PokeathlonResultsAnnouncement_SetPlateTotal(PokeathlonResultsAnnouncementRanking *ranking, u32 total) {
    ranking->total = total;
}

#pragma force_active on

// The plates come in from the last place up, the winner's a little later.
void PokeathlonResultsAnnouncement_StartRanking(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonCourseData *course, u8 total, int unused) {
    int delay;
    int i, x, y;

    PokeathlonResultsAnnouncement_SetPlateTotal(ranking, total);
    PokeathlonResultsAnnouncement_InitPlates(ranking->plates, unused);
    x = 0;
    i = x;
    y = x;
    for (; i < 4; i++) {
        delay = x + 10;
        PokeathlonResultsAnnouncementPlate *plate = PokeathlonResultsAnnouncement_AddPlate(ranking, course, 256, 156 - y);
        if (!PokeathlonResultsAnnouncement_GetPlateRank()) {
            delay += 10;
        }
        PokeathlonResultsAnnouncement_QueueSlide(ranking, plate, delay);
        x += 10;
        y += 40;
    }
    if (total > 4) {
        PokeathlonResultsAnnouncementPlate *plate = PokeathlonResultsAnnouncement_AddPlate(ranking, course, 256, 40);
        PokeathlonResultsAnnouncement_SetIncomingSlide(ranking, plate, 0);
    }
    ranking->state = 1;
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_MoveSlide(PokeathlonResultsAnnouncementSlide *slide, int dx, int dy) {
    if (slide->active) {
        if (slide->delay == 0) {
            PokeathlonResultsAnnouncement_MovePlate(slide->plate, dx, dy);
            if (slide->started == FALSE) {
                PokeathlonResultsAnnouncement_GetPlateRank(slide->plate);
                PlaySE(SEQ_SE_GS_KEKKA);
                slide->started = TRUE;
            }
        } else {
            slide->delay--;
        }
    }
}

#pragma force_active on

void PokeathlonResultsAnnouncement_MoveSlides(PokeathlonResultsAnnouncementRanking *ranking, int dx, int dy) {
    int i;
    PokeathlonResultsAnnouncementSlide *slide;

    i = 0;
    slide = ranking->slides;
    for (; i < 4; i++, slide++) {
        PokeathlonResultsAnnouncement_MoveSlide(slide, dx, dy);
    }
}

#pragma force_active reset

// direction 0: in from the right (stops at x 0); 1: out downwards.
BOOL PokeathlonResultsAnnouncement_IsSlideDone(PokeathlonResultsAnnouncementSlide *slide, int direction) {
    int x, y;
    BOOL ret = FALSE;
    PokeathlonResultsAnnouncement_GetPlatePos(slide->plate, &x, &y);
    if (direction == 0) {
        if (x <= 0) {
            PokeathlonResultsAnnouncement_SetPlatePos(slide->plate, 0, y);
            ret = TRUE;
        }
    } else if (direction == 1) {
        if (y >= 192) {
            ret = TRUE;
        }
    }
    return ret;
}

void PokeathlonResultsAnnouncement_Wait(PokeathlonResultsAnnouncementRanking *ranking, int frames) {
    ranking->state = 3;
    ranking->timer = frames;
}

#pragma force_active on

BOOL PokeathlonResultsAnnouncement_UpdateRanking(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonCourseData *course) {
    u8 *state = &ranking->state;
    switch (*state) {
    case 0:
        PokeathlonResultsAnnouncement_MoveSlide(&ranking->incoming, -80, 0);
        if (PokeathlonResultsAnnouncement_IsSlideDone(&ranking->incoming, 0)) {
            PokeathlonResultsAnnouncement_QueueSlide(ranking, ranking->incoming.plate, 0);
            PokeathlonResultsAnnouncement_ClearSlide(&ranking->incoming);
            if (PokeathlonResultsAnnouncement_HasMorePlates(ranking)) {
                PokeathlonResultsAnnouncement_SetIncomingSlide(ranking, PokeathlonResultsAnnouncement_AddPlate(ranking, course, 256, 40), 0);
            }
            PokeathlonResultsAnnouncement_Wait(ranking, 10);
        }
        break;
    case 1: {
        int i;
        PokeathlonResultsAnnouncementSlide *slide;
        PokeathlonResultsAnnouncement_MoveSlides(ranking, -80, 0);
        for (i = 0, slide = ranking->slides; i < 4; i++, slide++) {
            if (!PokeathlonResultsAnnouncement_IsSlideDone(slide, 0)) {
                break;
            }
            if (ranking->fanfarePlayed == FALSE && PokeathlonResultsAnnouncement_GetPlateRank(slide->plate) == 0) {
                PlayFanfare(SEQ_ME_PT_RESULTG);
                ranking->fanfarePlayed = TRUE;
            }
        }
        if (i == 4) {
            PokeathlonResultsAnnouncement_Wait(ranking, 10);
        }
        break;
    }
    case 2: {
        PokeathlonResultsAnnouncementSlide *slide;
        PokeathlonResultsAnnouncement_MoveSlides(ranking, 0, 8);
        slide = PokeathlonResultsAnnouncement_GetLeavingSlide(ranking);
        if (PokeathlonResultsAnnouncement_IsSlideDone(slide, 1)) {
            PokeathlonResultsAnnouncement_RemoveSlide(ranking, slide);
            *state = 0;
        }
        break;
    }
    case 3:
        if (ranking->timer-- <= 0) {
            if (PokeathlonResultsAnnouncement_HasMorePlates(ranking)) {
                *state = 2;
            } else {
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_QueueSlide(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonResultsAnnouncementPlate *plate, int delay) {
    PokeathlonResultsAnnouncementSlide *slide = &ranking->slides[ranking->slide];
    if (slide->active) {
        GF_AssertFail();
    }
    slide->active = TRUE;
    slide->plate = plate;
    slide->delay = delay;
    ranking->slide = (ranking->slide + 1) % 4;
    ranking->count++;
}

void PokeathlonResultsAnnouncement_SetIncomingSlide(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonResultsAnnouncementPlate *plate, int delay) {
    PokeathlonResultsAnnouncementSlide *slide = &ranking->incoming;
    if (slide->active) {
        GF_AssertFail();
    }
    slide->active = TRUE;
    slide->plate = plate;
    slide->delay = delay;
}

void PokeathlonResultsAnnouncement_ClearSlide(PokeathlonResultsAnnouncementSlide *slide) {
    slide->active = FALSE;
    slide->plate = NULL;
    slide->delay = 0;
}

#pragma force_active on

BOOL PokeathlonResultsAnnouncement_HasMorePlates(PokeathlonResultsAnnouncementRanking *ranking) {
    BOOL result = FALSE;
    if ((u32)ranking->total > (u32)ranking->count) {
        result = TRUE;
    }
    return result;
}

// The plates are created from the last place up.
PokeathlonResultsAnnouncementPlate *PokeathlonResultsAnnouncement_AddPlate(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonCourseData *course, int x, int y) {
    void *scoreList = PokeathlonCourse_GetGraphicsSystem(course);
    int last = PokeathlonCourseScoreList_GetCount(scoreList) - 1;
    PokeathlonResultsAnnouncementScore *score = PokeathlonCourseScoreList_GetRanked(scoreList, last - ranking->next);
    ranking->next++;
    return PokeathlonResultsAnnouncement_ShowPlate(ranking->plates, x, y, score);
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_RemoveSlide(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonResultsAnnouncementSlide *slide) {
    PokeathlonResultsAnnouncement_HidePlate(slide->plate);
    PokeathlonResultsAnnouncement_ClearSlide(slide);
}

PokeathlonResultsAnnouncementSlide *PokeathlonResultsAnnouncement_GetLeavingSlide(PokeathlonResultsAnnouncementRanking *ranking) {
    return &ranking->slides[ranking->slide];
}

// "Results of the first event!?" etc.
u32 PokeathlonResultsAnnouncement_GetIntroMessage(PokeathlonCourseData *course) {
    u32 msgId = PokeathlonCourse_GetCurrentParticipantIndex(course); // the round
    if (PokeathlonCourse_GetMode(course) == 0) {
        msgId += msg_0309_00246;
    } else {
        msgId += msg_0309_00249;
    }
    return msgId;
}

// One of 5 comments per round, picked by the host.
u32 PokeathlonResultsAnnouncement_GetCommentMessage(PokeathlonCourseData *course) {
    u32 round = PokeathlonCourse_GetCurrentParticipantIndex(course);
    u8 *comment = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    u32 msgId;
    if (*comment >= 5) {
        GF_AssertFail();
    }
    if (PokeathlonCourse_GetMode(course) == 0) {
        msgId = msg_0309_00256 + round * 5 + *comment;
    } else {
        msgId = msg_0309_00271 + round * 5 + *comment;
    }
    return msgId;
}

void PokeathlonResultsAnnouncement_InitSprites(PokeathlonResultsAnnouncement *announcement) {
    SpriteResourceCountsListUnion counts = ov96_0221C2D8;
    OamManagerParam oamManagerParam = ov96_0221C2F0;
    OamCharTransferParam oamTransferParam = ov96_0221C2C4;
    oamTransferParam.maxTasks = 26;
    announcement->spriteSystem = SpriteSystem_Alloc(announcement->heapId);
    announcement->spriteManager = SpriteManager_New(announcement->spriteSystem);
    SpriteSystem_Init(announcement->spriteSystem, &oamManagerParam, &oamTransferParam, 32);
    SpriteSystem_InitSprites(announcement->spriteSystem, announcement->spriteManager, 2);
    SpriteSystem_InitManagerWithCapacities(announcement->spriteSystem, announcement->spriteManager, &counts);
}

void PokeathlonResultsAnnouncement_FreeSprites(PokeathlonResultsAnnouncement *announcement) {
    u8 i = 0;
    ManagedSprite *null = (ManagedSprite *)i;
    do {
        ManagedSprite *sprite = announcement->sprites[i];
        if (sprite != NULL) {
            Sprite_DeleteAndFreeResources(sprite);
            announcement->sprites[i] = null;
        }
        i++;
    } while (i < 2);
    SpriteSystem_FreeResourcesAndManager(announcement->spriteSystem, announcement->spriteManager);
    SpriteSystem_Free(announcement->spriteSystem);
}

void PokeathlonResultsAnnouncement_LoadSpriteResources(PokeathlonResultsAnnouncement *announcement) {
    SpriteSystem *spriteSystem = announcement->spriteSystem;
    SpriteManager *spriteManager = announcement->spriteManager;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 9, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 1000);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 1000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 10, TRUE, 1000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 11, TRUE, 1000);
}

void PokeathlonResultsAnnouncement_CreateSprite(PokeathlonResultsAnnouncement *announcement) {
    ManagedSpriteTemplate template = { 0 };
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 1000;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.bgPriority = 1;
    template.x = 224;
    template.y = 112;
    announcement->sprites[0] = SpriteSystem_NewSprite(announcement->spriteSystem, announcement->spriteManager, &template);
}

// Link play: every player sends how far it got; the host collects them. Returns TRUE while this player has to
// wait for the others.
BOOL PokeathlonResultsAnnouncement_WaitForPlayers(PokeathlonCourseData *course) {
    PokeathlonResultsAnnouncement *announcement = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *data = PokeathlonCourse_GetDataCopyArea(course);
    u8 *out = ov96_021E8A20(data + 0x28); // the host's block for the others
    u8 *host = ov96_021E8A20(data + 0xF0);
    u8 self = (u8)ov96_021E5F24(course);
    u8 progress = (u8)announcement->messageCount;
    BOOL behind = FALSE;
    if (self == 0) {
        BOOL allFinished = TRUE;
        u8 *dst;
        u8 *src;
        u32 n;
        int i;
        int count;
        out[1] = progress;
        dst = ov96_021E8A20(data + 0x50);
        src = ov96_021E8A20(data);
        n = 0x24;
        while (n != 0) {
            *dst++ = *src++;
            n--;
        }
        i = 0;
        count = PokeathlonCourse_GetParticipantCount(course);
        if (count > 0) {
            u8 *block = data + 0x50;
            do {
                u8 *player = ov96_021E8A20(block);
                if (player[0] < progress || player[1] == TRUE) {
                    behind = TRUE;
                }
                if (player[2] == FALSE) {
                    allFinished = FALSE;
                }
                block += 0x28;
                i++;
            } while (i < PokeathlonCourse_GetParticipantCount(course));
        }
        if (allFinished) {
            out[2] = TRUE;
        }
        out[3] = behind;
    }
    out = ov96_021E8A20(data); // this player's block
    out[0] = progress;
    out[1] = (u8)PokeathlonCourseMessageBox_IsBusy(announcement->messageBox);
    out[2] = (u8)announcement->finished;
    if (host[3] != 0 && progress >= host[1]) {
        return TRUE;
    }
    return FALSE;
}

PokeathlonResultsAnnouncementPlates *PokeathlonResultsAnnouncement_NewPlates(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonResultsAnnouncementPlates *plates = Heap_Alloc(heapId, sizeof(PokeathlonResultsAnnouncementPlates));
    int i;
    MI_CpuFill8(plates, 0, sizeof(PokeathlonResultsAnnouncementPlates));
    plates->heapId = heapId;
    plates->fontSystem = FontSystem_NewInit(10, heapId);
    plates->bgConfig = bgConfig;
    plates->course = course;
    i = 0;
    do {
        plates->plates[i].index = i;
        i++;
    } while (i < 5);
    return plates;
}

void PokeathlonResultsAnnouncement_FreePlates(PokeathlonResultsAnnouncementPlates *plates) {
    PokeathlonResultsAnnouncementText *name;
    PokeathlonResultsAnnouncementText *text;
    PokeathlonResultsAnnouncementTextWindow *window;
    int i;
    i = 0;
    name = &plates->plates[0].name;
    text = &plates->plates[0].text;
    do {
        PokeathlonResultsAnnouncement_DestroyText(name);
        PokeathlonResultsAnnouncement_DestroyText(text);
        Sprite_Delete(plates->plates[i].sprites[0]);
        Sprite_Delete(plates->plates[i].sprites[1]);
        Sprite_Delete(plates->plates[i].sprites[2]);
        Sprite_Delete(plates->plates[i].sprites[3]);
        name = (PokeathlonResultsAnnouncementText *)((u8 *)name + sizeof(PokeathlonResultsAnnouncementPlate));
        text = (PokeathlonResultsAnnouncementText *)((u8 *)text + sizeof(PokeathlonResultsAnnouncementPlate));
        i++;
    } while (i < 5);
    i = 0;
    window = &plates->nameWindow;
    do {
        TextOBJLayout_Delete(window->layout);
        RemoveWindow(&window->window);
        i++;
        window++;
    } while (i < 2);
    FontSystem_Delete(plates->fontSystem);
    SpriteTransfer_DeleteCharTransferTask(plates->resources[0]);
    SpriteTransfer_DeletePlttTransferTask(plates->resources[1]);
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(plates->resourceManagers[i]);
    }
    SpriteList_Delete(plates->spriteList);
    Heap_Free(plates);
}

void PokeathlonResultsAnnouncement_DrawPlates(PokeathlonResultsAnnouncementPlates *plates) {
    SpriteList_RenderAndAnimateSprites(plates->spriteList);
}

void PokeathlonResultsAnnouncement_InitPlates(PokeathlonResultsAnnouncementPlates *plates, int unused) {
    int i;
    PokeathlonResultsAnnouncementPlate *plate;

    PokeathlonResultsAnnouncement_LoadPlateResources(plates);
    PokeathlonResultsAnnouncement_InitPlateResourcesHeader(plates);
    i = 0;
    plate = plates->plates;
    for (; i < 5; i++, plate++) {
        PokeathlonResultsAnnouncement_CreatePlateSprites(plates, (u8)i, plate->sprites);
    }
    PokeathlonResultsAnnouncement_InitTextWindows(plates);
    G2dRenderer_SetSubSurfaceCoords(&plates->renderer, 0, FX32_CONST(300));
    G2dRenderer_SetMainSurfaceCoords(&plates->renderer, 0, 0);
}

// Shows the next free plate at (x, y) for a ranking entry.
PokeathlonResultsAnnouncementPlate *PokeathlonResultsAnnouncement_ShowPlate(PokeathlonResultsAnnouncementPlates *plates, int x, int y, PokeathlonResultsAnnouncementScore *score) {
    PokeathlonResultsAnnouncementPlate *plate;
    PokeathlonParticipantData *participant;
    BOOL isNpc;
    int index;

    index = plates->nextPlate;
    plate = &plates->plates[index];
    if (plate->active) {
        GF_AssertFail();
    }
    participant = PokeathlonCourse_GetParticipantData(plates->course, score->participant);
    isNpc = *(s32 *)participant > 0; // first word: the NPC team
    plate->x = x;
    plate->y = y;
    plate->participant = score->participant;
    plate->rank = score->rank;
    PokeathlonResultsAnnouncement_PrintPlateText(plates, score->text, plate);
    PokeathlonResultsAnnouncement_PrintPlateName(plates, score->participant, plate);
    plates->firstPlace = plate->rank == 0;
    if ((s32)plate->participant >= 4) {
        GF_AssertFail();
    }
    if (isNpc) {
        u32 picture = PokeathlonCourse_GetNpcTrainerPicFileId(ov96_021E8424((u8) * (s32 *)participant));
        PokeathlonCourse_LoadTrainerPicIntoSprite(plate->sprites[3], picture, 11, plates->heapId);
    } else {
        PokeathlonCourse_LoadPlayerTrainerPic(plate->sprites[3], plates->course, (u8)plate->participant, 27);
    }
    PokeathlonResultsAnnouncement_RefreshPlate(plate);
    PokeathlonResultsAnnouncement_SetPlateVisible(plates->firstPlace, TRUE, plate);
    plate->active = TRUE;
    plates->nextPlate = (plates->nextPlate + 1) % 5;
    return plate;
}

void PokeathlonResultsAnnouncement_HidePlate(PokeathlonResultsAnnouncementPlate *plate) {
    if (plate->active == FALSE) {
        GF_AssertFail();
    }
    plate->active = FALSE;
    PokeathlonResultsAnnouncement_DestroyText(&plate->name);
    PokeathlonResultsAnnouncement_DestroyText(&plate->text);
}

void PokeathlonResultsAnnouncement_GetPlatePos(PokeathlonResultsAnnouncementPlate *plate, int *x, int *y) {
    *x = plate->x;
    *y = plate->y;
}

void PokeathlonResultsAnnouncement_SetPlatePos(PokeathlonResultsAnnouncementPlate *plate, int x, int y) {
    plate->x = x;
    plate->y = y;
    PokeathlonResultsAnnouncement_SetPlateSpritePositions(x, y, plate);
}

#pragma force_active on

void PokeathlonResultsAnnouncement_MovePlate(PokeathlonResultsAnnouncementPlate *plate, int dx, int dy) {
    plate->x += dx;
    plate->y += dy;
    PokeathlonResultsAnnouncement_MovePlateSprites(dx, dy, plate);
}

#pragma force_active reset

u16 PokeathlonResultsAnnouncement_GetPlateRank(PokeathlonResultsAnnouncementPlate *plate) {
    return plate->rank;
}

#pragma force_active on

void PokeathlonResultsAnnouncement_LoadPlateResources(PokeathlonResultsAnnouncementPlates *plates) {
    int i;
    plates->spriteList = G2dRenderer_Init(55, &plates->renderer, plates->heapId);
    for (i = 0; i < 6; i++) {
        plates->resourceManagers[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, plates->heapId);
    }
    plates->resources[0] = AddCharResObjFromNarc(plates->resourceManagers[GF_GFX_RES_TYPE_CHAR], NARC_a_1_6_0, 9, FALSE, 103, NNS_G2D_VRAM_TYPE_2DMAIN, plates->heapId);
    plates->resources[1] = AddPlttResObjFromNarc(plates->resourceManagers[GF_GFX_RES_TYPE_PLTT], NARC_a_1_6_0, 6, FALSE, 103, NNS_G2D_VRAM_TYPE_2DMAIN, 6, plates->heapId);
    plates->resources[2] = AddCellOrAnimResObjFromNarc(plates->resourceManagers[GF_GFX_RES_TYPE_CELL], NARC_a_1_6_0, 8, FALSE, 103, GF_GFX_RES_TYPE_CELL, plates->heapId);
    plates->resources[3] = AddCellOrAnimResObjFromNarc(plates->resourceManagers[GF_GFX_RES_TYPE_ANIM], NARC_a_1_6_0, 7, FALSE, 103, GF_GFX_RES_TYPE_ANIM, plates->heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(plates->resources[0]);
    SpriteTransfer_CreateExtPlttTransferTask(plates->resources[1]);
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_InitPlateResourcesHeader(PokeathlonResultsAnnouncementPlates *plates) {
    CreateSpriteResourcesHeader(&plates->resourcesHeader, 103, 103, 103, 103, -1, -1, FALSE, 0, plates->resourceManagers[GF_GFX_RES_TYPE_CHAR], plates->resourceManagers[GF_GFX_RES_TYPE_PLTT], plates->resourceManagers[GF_GFX_RES_TYPE_CELL], plates->resourceManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

void PokeathlonResultsAnnouncement_InitPlateSpriteTemplate(SpriteTemplate *template, PokeathlonResultsAnnouncementPlates *plates, SpriteResourcesHeader *unused, NNS_G2D_VRAM_TYPE whichScreen) {
    template->spriteList = plates->spriteList;
    template->header = &plates->resourcesHeader;
    template->position.x = 0;
    template->position.y = 0;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 2;
    template->whichScreen = whichScreen;
    template->heapID = plates->heapId;
}

Sprite *PokeathlonResultsAnnouncement_CreatePlateSprite(SpriteTemplate *template, int anim, int priority) {
    Sprite *sprite = Sprite_CreateAffine(template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, anim);
    Sprite_SetDrawFlag(sprite, FALSE);
    Sprite_SetDrawPriority(sprite, priority);
    return sprite;
}

#pragma force_active on

// The team picture of each plate gets its own character and palette slot.
void PokeathlonResultsAnnouncement_CreatePlateSprites(PokeathlonResultsAnnouncementPlates *plates, int index, Sprite **sprites) {
    SpriteTemplate template;
    Sprite *sprite;
    NNSG2dImageProxy *imageProxy;
    NNSG2dImagePaletteProxy *paletteProxy;
    NNS_G2D_VRAM_TYPE vramType;

    struct {
        volatile u32 location;
        volatile u32 offset;
        volatile u32 paletteLocation;
        volatile u32 paletteOffset;
    } slots;

    PokeathlonResultsAnnouncement_InitPlateSpriteTemplate(&template, plates, &plates->resourcesHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    sprites[0] = PokeathlonResultsAnnouncement_CreatePlateSprite(&template, 17, 4);
    sprites[1] = PokeathlonResultsAnnouncement_CreatePlateSprite(&template, 5, 0);
    sprites[2] = PokeathlonResultsAnnouncement_CreatePlateSprite(&template, 18, 1);
    sprites[3] = PokeathlonResultsAnnouncement_CreatePlateSprite(&template, 0, 2);
    sprite = sprites[3];
    vramType = Sprite_GetVramType(sprite);
    imageProxy = Sprite_GetImageProxy(sprite);
    paletteProxy = Sprite_GetPaletteProxy(sprite);
    slots.location = NNS_G2dGetImageLocation(imageProxy, vramType);
    slots.offset = index << 9;
    NNS_G2dInitImageProxy(imageProxy);
    NNS_G2dSetImageLocation(imageProxy, vramType, slots.location + slots.offset);
    slots.paletteLocation = NNS_G2dGetImagePaletteLocation(paletteProxy, vramType);
    slots.paletteOffset = (index + 6) << 5;
    NNS_G2dInitImagePaletteProxy(paletteProxy);
    NNS_G2dSetImagePaletteLocation(paletteProxy, vramType, slots.paletteLocation + slots.paletteOffset);
    Sprite_SetImageProxy(sprite, imageProxy);
    Sprite_SetPaletteProxy(sprite, paletteProxy);
    Sprite_SetPalOffset(sprite, index);
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_PrintPlateName(PokeathlonResultsAnnouncementPlates *plates, int participant, PokeathlonResultsAnnouncementPlate *plate) {
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(plates->course, participant);
    String *name = PlayerProfile_GetPlayerName_NewString(profile, plates->heapId);
    PokeathlonResultsAnnouncement_CreateText(plates, &plates->nameWindow, name, 0, -56, -16, plate->sprites[0], &plate->name);
    String_Delete(name);
}

void PokeathlonResultsAnnouncement_PrintPlateText(PokeathlonResultsAnnouncementPlates *plates, String *string, PokeathlonResultsAnnouncementPlate *plate) {
    PokeathlonResultsAnnouncement_CreateText(plates, &plates->textWindow, string, 0, -56, 0, plate->sprites[0], &plate->text);
}

// Plate color by participant, rank sprite.
void PokeathlonResultsAnnouncement_RefreshPlate(PokeathlonResultsAnnouncementPlate *plate) {
    PokeathlonResultsAnnouncement_SetPlateSpritePositions(plate->x, plate->y, plate);
    Sprite_SetPaletteOverride(plate->sprites[0], plate->participant + 2);
    Sprite_SetAnimCtrlSeq(plate->sprites[1], plate->rank + 5);
}

void PokeathlonResultsAnnouncement_SetPlateVisible(BOOL firstPlace, BOOL draw, PokeathlonResultsAnnouncementPlate *plate) {
    Sprite_SetDrawFlag(plate->sprites[0], draw);
    Sprite_SetDrawFlag(plate->sprites[1], draw);
    if (firstPlace) {
        Sprite_SetDrawFlag(plate->sprites[2], draw);
    }
    TextOBJ_SetSpritesDrawFlag(plate->name.textObj, draw);
    TextOBJ_SetSpritesDrawFlag(plate->text.textObj, draw);
}

#pragma force_active on

void PokeathlonResultsAnnouncement_SetPlateSpritePositions(int x, int y, PokeathlonResultsAnnouncementPlate *plate) {
    VecFx32 pos = ov96_0221C3E0;
    pos.y += y << FX32_SHIFT;
    pos.x = (x + 128) << FX32_SHIFT;
    Sprite_SetMatrix(plate->sprites[0], &pos);
    pos.x = (x + 16) << FX32_SHIFT;
    Sprite_SetMatrix(plate->sprites[1], &pos);
    pos.x = (x + 48) << FX32_SHIFT;
    Sprite_SetMatrix(plate->sprites[2], &pos);
    pos.y -= FX32_CONST(4);
    Sprite_SetMatrix(plate->sprites[3], &pos);
    TextOBJ_UpdatePosition(plate->name.textObj);
    TextOBJ_UpdatePosition(plate->text.textObj);
}

void PokeathlonResultsAnnouncement_MovePlateSprites(int dx, int dy, PokeathlonResultsAnnouncementPlate *plate) {
    VecFx32 pos = *Sprite_GetMatrixPtr(plate->sprites[0]);
    pos.x += dx << FX32_SHIFT;
    pos.y += dy << FX32_SHIFT;
    Sprite_SetMatrix(plate->sprites[0], &pos);
    pos = *Sprite_GetMatrixPtr(plate->sprites[1]);
    pos.x += dx << FX32_SHIFT;
    pos.y += dy << FX32_SHIFT;
    Sprite_SetMatrix(plate->sprites[1], &pos);
    pos = *Sprite_GetMatrixPtr(plate->sprites[3]);
    pos.x += dx << FX32_SHIFT;
    pos.y += dy << FX32_SHIFT;
    Sprite_SetMatrix(plate->sprites[3], &pos);
    Sprite_SetMatrix(plate->sprites[2], &pos);
    TextOBJ_UpdatePosition(plate->name.textObj);
    TextOBJ_UpdatePosition(plate->text.textObj);
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_InitTextWindow(PokeathlonResultsAnnouncementPlates *plates, PokeathlonResultsAnnouncementTextWindow *window, int width) {
    TextOBJLayout *layout;
    InitWindow(&window->window);
    AddTextWindowTopLeftCorner(plates->bgConfig, &window->window, width, 2, 0, 0);
    layout = TextOBJLayout_New(&window->window, plates->heapId);
    window->layout = layout;
    window->charSize = TextOBJLayout_GetCharSize(layout, NNS_G2D_VRAM_TYPE_2DMAIN);
}

void PokeathlonResultsAnnouncement_InitTextWindows(PokeathlonResultsAnnouncementPlates *plates) {
    PokeathlonResultsAnnouncement_InitTextWindow(plates, &plates->nameWindow, 8);
    PokeathlonResultsAnnouncement_InitTextWindow(plates, &plates->textWindow, 24);
}

#pragma force_active on

void PokeathlonResultsAnnouncement_CreateText(PokeathlonResultsAnnouncementPlates *plates, PokeathlonResultsAnnouncementTextWindow *window, String *string, int x, int dx, int dy, Sprite *parent, PokeathlonResultsAnnouncementText *out) {
    TextOBJTemplate template;
    ObjCharSpaceReservation charSlot;
    TextOBJ *textObj;
    FillWindowPixelBufferText_AssumeTileSize32(&window->window, 0);
    AddTextPrinterParameterizedWithColorAndSpacing(&window->window, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0, 0, NULL);
    ObjCharTransfer_ReserveCharSpace(window->charSize, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &charSlot);
    template.fontSystem = plates->fontSystem;
    template.window = &window->window;
    template.spriteList = plates->spriteList;
    template.plttResourceProxy = SpriteTransfer_GetPaletteProxy(plates->resources[1], NULL);
    template.sprite = parent;
    template.offset = charSlot.offset;
    template.x = dx;
    template.y = dy;
    template.unk_20 = 0;
    template.unk_24 = 3;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = plates->heapId;
    textObj = FontOAM_Create(&template);
    TextOBJ_SetPaletteNum(textObj, 0);
    out->textObj = textObj;
    out->charSlot = charSlot;
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_DestroyText(PokeathlonResultsAnnouncementText *text) {
    if (text->textObj != NULL) {
        TextOBJ_Destroy(text->textObj);
        ObjCharTransfer_FreeCharSpace(&text->charSlot);
        text->textObj = NULL;
    }
}

PokeathlonResultsAnnouncementScoreboard *PokeathlonResultsAnnouncement_NewScoreboard(enum HeapID heapId, BgConfig *bgConfig, void *scene) {
    PokeathlonResultsAnnouncementScoreboard *scoreboard = Heap_Alloc(heapId, sizeof(PokeathlonResultsAnnouncementScoreboard));
    PokeathlonResultsAnnouncementTextWindow *window;
    TextOBJLayout *layout;
    MI_CpuFill8(scoreboard, 0, sizeof(PokeathlonResultsAnnouncementScoreboard));
    scoreboard->heapId = heapId;
    scoreboard->fontSystem = FontSystem_NewInit(4, heapId);
    scoreboard->scene = scene;
    window = &scoreboard->window;
    InitWindow(&window->window);
    AddTextWindowTopLeftCorner(bgConfig, &window->window, 8, 2, 0, 0);
    layout = TextOBJLayout_New(&window->window, scoreboard->heapId);
    window->layout = layout;
    window->charSize = TextOBJLayout_GetCharSize(layout, NNS_G2D_VRAM_TYPE_2DSUB);
    return scoreboard;
}

void PokeathlonResultsAnnouncement_FreeScoreboard(PokeathlonResultsAnnouncementScoreboard *scoreboard) {
    s32 i;
    PokeathlonResultsAnnouncementText *name;
    PokeathlonResultsAnnouncementTextWindow *window;
    i = 0;
    name = &scoreboard->teams[0].name;
    for (; i < 4; i++, name = (PokeathlonResultsAnnouncementText *)((u8 *)name + sizeof(PokeathlonResultsAnnouncementScoreboardTeam))) {
        PokeathlonResultsAnnouncement_DestroyScoreboardText(name);
    }
    window = &scoreboard->window;
    TextOBJLayout_Delete(window->layout);
    RemoveWindow(&window->window);
    FontSystem_Delete(scoreboard->fontSystem);
    Heap_Free(scoreboard);
}

void PokeathlonResultsAnnouncement_LoadScoreboardResources(PokeathlonResultsAnnouncementScoreboard *scoreboard, s32 a1) {
    PokeathlonCourseSpritePool_SetResourceId(scoreboard->scene, a1, 104);
    PokeathlonCourseSpritePool_LoadChar(scoreboard->scene, NARC_poketool_pokezukan, 3, 104, 2);
    PokeathlonCourseSpritePool_LoadPltt(scoreboard->scene, NARC_poketool_pokezukan, 0, 104, 2, 5);
    PokeathlonCourseSpritePool_LoadCell(scoreboard->scene, NARC_poketool_pokezukan, 2, 104);
    PokeathlonCourseSpritePool_LoadAnim(scoreboard->scene, NARC_poketool_pokezukan, 1, 104);
}

#pragma force_active on

void PokeathlonResultsAnnouncement_InitScoreboard(PokeathlonResultsAnnouncementScoreboard *scoreboard, PokeathlonCourseData *course, int y) {
    int i;
    int j;
    Sprite *sprite;
    VecFx32 pos;
    String *name;
    PokeathlonResultsAnnouncementScoreboardTeam *team;
    for (i = 0; i < 4; i++) {
        sprite = PokeathlonCourseSpritePool_CreateSprite(scoreboard->scene, 0, 2, 104, 3);
        pos.z = 0;
        pos.x = (i * 64) << FX32_SHIFT;
        pos.y = (y + 16) << FX32_SHIFT;
        Sprite_SetMatrix(sprite, &pos);
        Sprite_SetAnimActiveFlag(sprite, TRUE);
        Sprite_SetAnimCtrlSeq(sprite, 0);
        Sprite_SetPalIndexRespectVramOffset(sprite, i);
        Sprite_SetDrawPriority(sprite, 2);
        for (j = 0; j < 3; j++) {
            sprite = PokeathlonCourseSpritePool_CreateSprite(scoreboard->scene, 0, 2, 104, 4);
            pos.z = 0;
            pos.x = (i * 64 + 8 + j * 16) << FX32_SHIFT;
            pos.y = (y + 24) << FX32_SHIFT;
            Sprite_SetMatrix(sprite, &pos);
            Sprite_SetAnimActiveFlag(sprite, TRUE);
            Sprite_SetAnimCtrlSeq(sprite, 1);
            Sprite_SetDrawPriority(sprite, 1);
        }
    }
    for (i = 0; i < 4; i++) {
        team = &scoreboard->teams[i];
        team->plate = PokeathlonCourseSpritePool_Take(scoreboard->scene, 104, 3);
        PokeathlonCourseSpritePoolSprite_Show(team->plate, 1, 1);
        for (j = 0; j < 3; j++) {
            team->digits[j] = PokeathlonCourseSpritePool_Take(scoreboard->scene, 104, 4);
            PokeathlonCourseSpritePoolSprite_Show(team->digits[j], 1, 1);
        }
        name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, i), scoreboard->heapId);
        sprite = PokeathlonCourseSpritePoolSprite_GetSprite(scoreboard->teams[i].plate);
        PokeathlonResultsAnnouncement_CreateScoreboardText(scoreboard, &scoreboard->window, name, 2, 0, -16, sprite, &scoreboard->teams[i].name);
        String_Delete(name);
        TextOBJ_SetSpritesDrawFlag(scoreboard->teams[i].name.textObj, TRUE);
        scoreboard->teams[i].y = y + 16;
    }
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_SetScore(PokeathlonResultsAnnouncementScoreboard *scoreboard, u32 points, u32 team) {
    if (team >= 4) {
        GF_AssertFail();
    }
    scoreboard->teams[team].points = points;
    PokeathlonResultsAnnouncement_SetScoreDigits(&scoreboard->teams[team], (s32)points);
}

#pragma force_active on

void PokeathlonResultsAnnouncement_CreateScoreboardText(PokeathlonResultsAnnouncementScoreboard *scoreboard, PokeathlonResultsAnnouncementTextWindow *window, String *string, int x, int dx, int dy, Sprite *parent, PokeathlonResultsAnnouncementText *out) {
    TextOBJTemplate template;
    ObjCharSpaceReservation charSlot;
    TextOBJ *textObj;
    FillWindowPixelBufferText_AssumeTileSize32(&window->window, 0);
    AddTextPrinterParameterizedWithColorAndSpacing(&window->window, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0, 0, NULL);
    ObjCharTransfer_ReserveCharSpace(window->charSize, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, &charSlot);
    template.fontSystem = scoreboard->fontSystem;
    template.window = &window->window;
    template.spriteList = PokeathlonCourseSpritePool_GetSpriteList(scoreboard->scene);
    template.plttResourceProxy = ObjPlttTransfer_GetPlttProxyByID(104);
    template.sprite = parent;
    template.offset = charSlot.offset;
    template.x = dx;
    template.y = dy;
    template.unk_20 = 0;
    template.unk_24 = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = scoreboard->heapId;
    textObj = FontOAM_Create(&template);
    TextOBJ_SetPalIndexRespectVramOffset(textObj, 4);
    out->textObj = textObj;
    out->charSlot = charSlot;
}

#pragma force_active reset

void PokeathlonResultsAnnouncement_DestroyScoreboardText(PokeathlonResultsAnnouncementText *text) {
    if (text->textObj != NULL) {
        TextOBJ_Destroy(text->textObj);
        ObjCharTransfer_FreeCharSpace(&text->charSlot);
        text->textObj = NULL;
    }
}

void PokeathlonResultsAnnouncement_SetScoreDigits(PokeathlonResultsAnnouncementScoreboardTeam *team, s32 points) {
    s32 hundreds = points / 100;
    s32 rest = points - hundreds * 100;
    s32 tens = rest / 10;
    Sprite *sprite = PokeathlonCourseSpritePoolSprite_GetSprite(team->digits[0]);
    Sprite_SetAnimCtrlSeq(sprite, hundreds + 1);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(team->digits[1]);
    Sprite_SetAnimCtrlSeq(sprite, tens + 1);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(team->digits[2]);
    Sprite_SetAnimCtrlSeq(sprite, rest - tens * 10 + 1);
}
