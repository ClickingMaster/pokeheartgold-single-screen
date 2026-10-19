#include "pokeathlon/pokeathlon_pennant_capture_play.h"

#include "global.h"

#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade.h"
#include "system.h"
#include "touchscreen.h"

enum {
    PENNANT_CAPTURE_STATE_SETUP,
    PENNANT_CAPTURE_STATE_PLAY,
    PENNANT_CAPTURE_STATE_END,
};

enum PokeathlonPennantCaptureSetupState {
    PENNANT_CAPTURE_SETUP_LAYOUT,
    PENNANT_CAPTURE_SETUP_FADE_IN,
    PENNANT_CAPTURE_SETUP_WAIT_FADE_IN,
};

extern const GraphicsModes gPennantCaptureGraphicsModes;
extern const BgTemplate gPennantCaptureBgTemplate_Sub0;
extern const BgTemplate gPennantCaptureBgTemplate_Sub1;
extern const BgTemplate gPennantCaptureBgTemplate_Sub2;
extern const BgTemplate gPennantCaptureBgTemplate_Main0;
extern const BgTemplate gPennantCaptureBgTemplate_Main3;

// One link entry: [0] own input, [1] own frame (host), [2-5] the participants' inputs, [6] the host's frame.
typedef struct PokeathlonPennantCaptureLinkEntry {
    u8 data[0x28];
} PokeathlonPennantCaptureLinkEntry;

// The field layout (NARC 227): pixel positions / 8; 0, 0 = unused.
typedef struct PokeathlonPennantCaptureLayout {
    u8 fieldItems[10][2];
    u8 obstacles[20][2];
    u8 pennantSpots[20][2];
} PokeathlonPennantCaptureLayout;

static void PokeathlonPennantCapture_HostStep(PokeathlonCourseData *course);
static BOOL PokeathlonPennantCapture_SendInput(PokeathlonCourseData *course);
static void PokeathlonPennantCapture_ShowFrame(PokeathlonCourseData *course);
static BOOL PokeathlonPennantCapture_IsTouchOnOwnMon(PokeathlonPennantCapture *work, int participant, int x, int y);

void PokeathlonPennantCapture_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes = gPennantCaptureGraphicsModes;
    BgTemplate bg0, bg3, bg4, bg5, bg6;

    SetBothScreensModesAndDisable(&modes);
    bg0 = gPennantCaptureBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, 0, &bg0, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 0);
    bg3 = gPennantCaptureBgTemplate_Main3;
    InitBgFromTemplate(bgConfig, 3, &bg3, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 3);
    bg4 = gPennantCaptureBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, 4, &bg4, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 4);
    bg5 = gPennantCaptureBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, 5, &bg5, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 5);
    bg6 = gPennantCaptureBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, 6, &bg6, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 6);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonPennantCapture_LoadBgGfx(PokeathlonPennantCapture *work) {
    GfGfxLoader_LoadCharData(NARC_a_2_2_4, 1, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, work->heapId);
    work->bgCharsFile = GfGfxLoader_LoadFromNarc(NARC_a_2_2_4, 3, FALSE, work->heapId, FALSE);
    NNS_G2dGetUnpackedBGCharacterData(work->bgCharsFile, &work->bgChars);
    GfGfxLoader_LoadScrnData(NARC_a_2_2_4, 2, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_2_4, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 64, work->heapId);
    PokeathlonPennantCaptureDisplay_LoadBgGfx(work->display);
}

void PokeathlonPennantCapture_LoadSpriteResources(PokeathlonCourseSpritePool *pool) {
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_2_4, 17, 0x68, 1);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_2_4, 14, 0x68, 1, 2);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_2_4, 16, 0x68);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_2_4, 15, 0x68);
}

void PokeathlonPennantCapture_CreateSprites(PokeathlonPennantCapture *work, PokeathlonCourseSpritePool *pool) {
    int i;
    u8 j;

    for (i = 0; i < 20; i++) {
        work->obstacles[i].sprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 6);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->obstacles[i].sprite, 1);
    }
    for (i = 0; i < 10; i++) {
        work->fieldItems[i].sprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 17);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->fieldItems[i].sprite, 6);
        work->fieldItems[i].shadowSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 18);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->fieldItems[i].shadowSprite, 5);
    }
    for (i = 0; i < 4; i++) {
        work->players[i].carrySprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 9);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->players[i].carrySprite, 2);
        for (j = 0; j < 2; j++) {
            work->players[i].pointSprites[j] = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 23);
            PokeathlonCourseSpritePoolSprite_SetAnim(work->players[i].pointSprites[j], 3);
            PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->players[i].pointSprites[j], 2);
        }
        work->players[i].pennantSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 27);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->players[i].pennantSprite, 8);
    }
    for (i = 0; i < 4; i++) {
        work->pickupEffects[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 14);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->pickupEffects[i], 7);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->pickupEffects[i], 4);
    }
    for (i = 0; i < 4; i++) {
        work->dropEffects[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 25);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->dropEffects[i], 4);
    }
    for (i = 0; i < 5; i++) {
        work->pennants.pennants[i].poleSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 26);
        work->pennants.pennants[i].sprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 16);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->pennants.pennants[i].sprite, 0);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->pennants.pennants[i].poleSprite, 4);
    }
}

// Places the obstacles, field items and pennant spots of the chosen layout, then fades in.
BOOL PokeathlonPennantCapture_State_Setup(PokeathlonCourseData *course, u8 *state) {
    PokeathlonPennantCaptureLayout layout;
    int i;
    u16 x, y;
    PokeathlonPennantCapture *work = PokeathlonCourse_GetWork(course);
    u8 *fieldItem, *obstacle, *spot;

    switch (*state) {
    case PENNANT_CAPTURE_SETUP_LAYOUT:
        PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
        ReadWholeNarcMemberByIdPair(&layout, (NarcId)0xE3, work->layoutFileId);
        spot = layout.pennantSpots[0];
        fieldItem = layout.fieldItems[0];
        for (i = 0, obstacle = layout.obstacles[0]; i < 20; obstacle += 2, i++) {
            y = obstacle[1] * 8;
            x = obstacle[0] * 8;
            work->obstaclePositions[i].x = x;
            work->obstaclePositions[i].y = obstacle[1] * 8;
            if (x != 0 && y != 0) {
                work->obstacles[i].placed = TRUE;
                work->obstacles[i].x = x;
                work->obstacles[i].y = y;
                PokeathlonCourseSpritePoolSprite_Show(work->obstacles[i].sprite, 1, 1);
            }
        }
        for (i = 0; i < 10; fieldItem += 2, i++) {
            y = fieldItem[1] * 8;
            x = fieldItem[0] * 8;
            work->fieldItemPositions[i].x = x;
            work->fieldItemPositions[i].y = fieldItem[1] * 8;
            if (x != 0 && y != 0) {
                work->fieldItems[i].placed = TRUE;
                work->fieldItems[i].x = x;
                work->fieldItems[i].y = y;
                PokeathlonCourseSpritePoolSprite_Show(work->fieldItems[i].sprite, 1, 1);
                PokeathlonCourseSpritePoolSprite_Show(work->fieldItems[i].shadowSprite, 1, 1);
            }
        }
        for (i = 0; i < 20; i++, spot += 2) {
            work->pennants.spots[i].x = spot[0] * 8;
            work->pennants.spots[i].y = spot[1] * 8;
        }
        if (!PokeathlonCourse_GetNetId(course)) {
            PokeathlonPennantCapture_RefillPennants(work);
            PokeathlonPennantCaptureCpu_SetObstacles(work->cpu, layout.obstacles[0]);
            PokeathlonPennantCaptureCpu_SetObstacles(work->cpu, layout.fieldItems[0]);
            PokeathlonPennantCaptureCpu_SetPennants(work->cpu, work->pennants.pennants);
        }
        PokeathlonPennantCapture_UpdateFieldDrawPriorities(work);
        GF_ASSERT(GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON));
        (*state)++;
        PokeathlonCourse_SetStateTransitionType(course, 0x13);
        break;
    case PENNANT_CAPTURE_SETUP_FADE_IN:
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, work->heapId);
        (*state)++;
        break;
    case PENNANT_CAPTURE_SETUP_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourse_SetNextState(course, PENNANT_CAPTURE_STATE_PLAY);
        }
        break;
    }
    PokeathlonPennantCapture_ShowFrame(course);
    return FALSE;
}

// Start countdown, play until the time is up, then the finish and the fade out.
BOOL PokeathlonPennantCapture_State_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonPennantCapture *work = PokeathlonCourse_GetWork(course);

    switch (*state) {
    case EVENT_PLAY_STATE_COUNTDOWN:
        PokeathlonPennantCapture_ShowFrame(course);
        if (PokeathlonCourse_RunStartCountdown(course)) {
            (*state)++;
        }
        break;
    case EVENT_PLAY_STATE_RUN:
        PokeathlonPennantCapture_HostStep(course);
        if (PokeathlonPennantCapture_SendInput(course)) {
            (*state)++;
        }
        break;
    case EVENT_PLAY_STATE_FINISH:
        PokeathlonPennantCapture_HostStep(course);
        if (PokeathlonCourse_RunFinish(course)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            PokeathlonCourse_SetNextState(course, PENNANT_CAPTURE_STATE_END);
        }
        break;
    }
    return FALSE;
}

// After the fade out: the host stores every participant's score (points + pennants still carried, at most 999).
BOOL PokeathlonPennantCapture_State_End(PokeathlonCourseData *course, u8 *state) {
    u8 *link;
    u32 *frame;
    int i;

    link = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourse_GetWork(course);
    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    if (PokeathlonCourse_GetNetId(course)) {
        return TRUE;
    }
    link += 0x28;
    frame = (u32 *)PokeathlonCourse_ToBytes(link);
    for (i = 0; i < 4; i++) {
        u16 score = (frame[i] >> 18) & 0x3FF;
        u16 pennants = (((u16 *)frame)[i + 12] >> 11) & 31;
        u16 total = score + pennants;
        if (total > 999) {
            total = 999;
        }
        PokeathlonCourse_SetEventScore(course, (u8)i, total);
    }
    return TRUE;
}

// AddPoint2D on 3D vectors: adds x and y only.
#define AddVecXY ((void (*)(VecFx32 *, VecFx32 *, VecFx32 *))PokeathlonPennantCapture_AddPoint2D)

static inline void ResetDrag(PokeathlonPennantCapturePlayer *player) {
    player->holdTime = 0;
    player->cpuFrom.x = 0;
    player->cpuFrom.y = 0;
    player->cpuFrom.z = 0;
    player->cpuTarget = player->cpuFrom;
}

// Host only: reads the participants' inputs, turns touch drags on the own Pokemon into flicks, moves everything
// and packs the frame every participant shows.
static void PokeathlonPennantCapture_HostStep(PokeathlonCourseData *course) {
    VecFx32 velocity;
    VecFx32 delta;
    int i;
    PokeathlonPennantCapture *work;
    PokeathlonPennantCapture *self;
    PokeathlonPennantCaptureLinkEntry *link;
    PokeathlonPennantCaptureInput *input;
    BOOL released;
    f32 power, scaleFactor;
    f32 maxSpeed;
    fx32 scale;
    f32 speed;
    f32 length;

    link = (PokeathlonPennantCaptureLinkEntry *)PokeathlonCourse_GetDataCopyArea(course);
    work = PokeathlonCourse_GetWork(course);
    self = work;
    if (PokeathlonCourse_GetNetId(course) != 0) {
        return;
    }
    if (work->finished) {
        link++;
        PokeathlonPennantCapture_PackFrame(work, (PokeathlonPennantCaptureFrame *)PokeathlonCourse_ToBytes(link));
        return;
    }
    *(PokeathlonPennantCaptureInput *)PokeathlonCourse_ToBytes(&link[2]) = *(PokeathlonPennantCaptureInput *)PokeathlonCourse_ToBytes(link);
    if (work->timeLeft != 0) {
        work->timeLeft--;
    }
    PokeathlonPennantCaptureCpu_Update(work->cpu);
    work->flicked = 0;
    for (i = 0; i < 4; i++) {
        input = (PokeathlonPennantCaptureInput *)PokeathlonCourse_ToBytes(&(link + 2)[i]);
        if (input->touching) {
            if (work->touches[i].pressed && work->touches[i].held) {
                work->touches[i].pressed = FALSE;
            } else if (work->touches[i].pressed == FALSE && work->touches[i].held == FALSE) {
                work->touches[i].pressed = TRUE;
                work->touches[i].held = TRUE;
            }
        } else {
            work->touches[i].pressed = FALSE;
            work->touches[i].held = FALSE;
        }
        if (work->players[i].swapPending) {
            if (work->players[i].swapPhase == 1 && input->swapPhase == 1) {
                work->players[i].member = (work->players[i].member + 1) % 3;
                work->players[i].swapPhase = 2;
                work->players[i].direction = 1;
            } else if (work->players[i].swapPhase == 2 && input->swapPhase == 2) {
                work->players[i].swapPhase = 0;
                work->players[i].swapPending = 0;
                work->players[i].depositing = 0;
                work->players[i].direction = 1;
            }
            work->dragging[i] = FALSE;
            ResetDrag(&work->players[i]);
        } else if (work->players[i].knockedDown == 1) {
            work->dragging[i] = FALSE;
            ResetDrag(&work->players[i]);
        } else if (work->players[i].freezeTimer) {
            work->dragging[i] = FALSE;
            ResetDrag(&work->players[i]);
        } else if (work->players[i].depositing) {
            work->dragging[i] = FALSE;
            ResetDrag(&work->players[i]);
        } else {
            released = FALSE;
            if (work->touches[i].pressed) {
                if (PokeathlonPennantCapture_IsTouchOnOwnMon(work, (u8)i, input->x, input->y)) {
                    work->dragging[i] = TRUE;
                    work->players[i].cpuFrom.x = input->x << FX32_SHIFT;
                    work->players[i].cpuFrom.y = input->y << FX32_SHIFT;
                    work->players[i].cpuFrom.z = 0;
                    work->players[i].cpuTarget = work->players[i].cpuFrom;
                }
            } else if (work->touches[i].held) {
                if (work->players[i].holdTime < 0xFF) {
                    work->players[i].holdTime++;
                } else {
                    work->heldTooLong = 1;
                }
                if (work->dragging[i] == FALSE) {
                    continue;
                }
                work->players[i].cpuTarget.x = input->x << FX32_SHIFT;
                work->players[i].cpuTarget.y = input->y << FX32_SHIFT;
            } else {
                if (work->dragging[i]) {
                    if (work->players[i].holdTime <= 30) {
                        work->players[i].cpuHasTarget = 1;
                    }
                    work->dragging[i] = FALSE;
                    work->heldTooLong = 0;
                }
                released = TRUE;
            }
            if (work->players[i].cpuHasTarget) {
                VecFx32 zero = { 0, 0, 0 };
                VecFx32 result;

                power = work->players[i].moveParams[work->players[i].member].unk00;
                scaleFactor = work->players[i].moveParams[work->players[i].member].unk04;
                maxSpeed = PokeathlonPennantCapture_GetMaxSpeed(&work->players[i], work->players[i].member);
                scale = power * scaleFactor / 8.0 * 4096.0;
                VEC_Subtract(&work->players[i].cpuTarget, &work->players[i].cpuFrom, &delta);
                VEC_MultAdd(scale, &delta, &zero, &delta);
                length = VEC_Mag(&delta) / FX32_ONE;
                velocity = work->players[i].velocity;
                AddVecXY(&velocity, &delta, &velocity);
                speed = VEC_Mag(&velocity);
                if (speed > 4096.0f * maxSpeed) {
                    VecFx32 zero2 = { 0, 0, 0 };

                    VEC_Normalize(&velocity, &velocity);
                    VEC_MultAdd(4096.0f * maxSpeed, &velocity, &zero2, &work->players[i].velocity);
                    VEC_MultAdd(4096.0f * maxSpeed, &velocity, &zero2, &result);
                } else {
                    AddVecXY(&self->players[i].velocity, &delta, &work->players[i].velocity);
                    result = delta;
                }
                work->players[i].direction = PokeathlonPennantCapture_GetDirection(&self->players[i].velocity);
                work->flicked = 1;
                work->flickX = work->players[i].position.x / FX32_ONE;
                work->flickY = work->players[i].position.y / FX32_ONE;
                work->players[i].cpuHasTarget = 0;
            }
            if (released) {
                ResetDrag(&work->players[i]);
            }
        }
    }
    PokeathlonPennantCapture_UpdatePlayers(course);
    PokeathlonPennantCapture_RefillPennants(work);
    if (work->timeLeft == 0) {
        work->finished = 1;
    } else {
        work->finished = 0;
    }
    link++;
    PokeathlonPennantCapture_PackFrame(work, (PokeathlonPennantCaptureFrame *)PokeathlonCourse_ToBytes(link));
}

// Frame layout: words[0-3] one per participant: x (9 bits), y (9), score (10), pennants (4);
// words[4]: flick y, x, flicked, depositing (4 bits), knocked-down contacts (8), finished (bit 31);
// words[5]: last hit y, x, hit reported, pennants full (4 bits); spots[i]: pennant spot | member << 5 |
// (direction - 1) << 7 | knocked down << 9 | points to add << 11.
void PokeathlonPennantCapture_PackFrame(PokeathlonPennantCapture *work, PokeathlonPennantCaptureFrame *frame) {
    u8 contacts = 0, depositing = 0, full = 0;
    int i, j;
    PokeathlonPennantCapturePlayer *player;
    u8 *word;
    u16 *spot;

    for (i = 0; i < 5; i++) {
        frame->spots[i] = work->pennants.pennants[i].spot;
    }
    player = work->players;
    word = (u8 *)frame;
    spot = (u16 *)frame;
    for (i = 0, j = 0; i < 4; i++, j += 2, player++) {
        *(u32 *)word = (player->position.x / FX32_ONE) | ((player->position.y / FX32_ONE) << 9) | (player->score << 18) | (player->pennants << 28);
        spot[12] |= player->member << 5;
        spot[12] |= (player->direction - 1) << 7;
        spot[12] |= player->knockedDown << 9;
        spot[12] |= player->pointsToAdd << 11;
        word += 4;
        spot++;
        depositing |= player->depositing << i;
        contacts |= player->swapPhase << j;
        full |= work->pennantsFull[i] << i;
    }
    frame->words[5] = 0;
    frame->words[5] |= work->hitX;
    frame->words[5] |= work->hitY << 9;
    frame->words[5] |= work->hitReported << 18;
    frame->words[5] |= full << 19;
    frame->words[4] = 0;
    frame->words[4] |= work->flickX;
    frame->words[4] |= work->flickY << 9;
    frame->words[4] |= work->flicked << 18;
    frame->words[4] |= depositing << 19;
    frame->words[4] |= contacts << 23;
    frame->words[4] |= work->finished << 31;
    frame->timeLeft = work->timeLeft;
}

// Sends the own touch input; TRUE once the host's frame says the event is finished.
static BOOL PokeathlonPennantCapture_SendInput(PokeathlonCourseData *course) {
    PokeathlonPennantCapture *work;
    u8 *link;
    PokeathlonPennantCaptureInput *input;
    PokeathlonPennantCaptureFrame *frame;

    work = PokeathlonCourse_GetWork(course);
    link = PokeathlonCourse_GetDataCopyArea(course);
    input = (PokeathlonPennantCaptureInput *)PokeathlonCourse_ToBytes(link);
    input->touching = FALSE;
    frame = (PokeathlonPennantCaptureFrame *)PokeathlonCourse_ToBytes(link + 0xF0);
    {
        u8 finished = 1;
        finished &= frame->words[4] >> 31;
        if (finished) {
            PokeathlonCourseSpritePool_PauseAnims(work->spritePool, TRUE);
            PokeathlonCourseObjects_PauseAnims(work->monObjects, 1);
            PokeathlonCourse_SetFinishCallback(course, PokeathlonPennantCapture_UpdateOwnScore);
            return TRUE;
        }
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, PokeathlonCourse_GetNetId(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        input->x = gSystem.touchX;
        input->y = gSystem.touchY;
        input->touching = TRUE;
    }
    PokeathlonPennantCapture_ShowFrame(course);
    return FALSE;
}

// Every participant: scrolls the field to the own Pokemon and shows the host's frame.
static void PokeathlonPennantCapture_ShowFrame(PokeathlonCourseData *course) {
    PokeathlonPennantCapture *work = PokeathlonCourse_GetWork(course);
    u8 *link = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonPennantCaptureFrame *frame = (PokeathlonPennantCaptureFrame *)PokeathlonCourse_ToBytes(link + 0xF0);
    u8 viewer = PokeathlonCourse_GetNetId(course);
    u32 packed = frame->words[viewer];
    u32 x = packed & 0x1FF;
    u32 y = (packed >> 9) & 0x1FF;

    ScheduleSetBgPosText(work->bgConfig, 0, BG_POS_OP_SET_X, x - 128);
    ScheduleSetBgPosText(work->bgConfig, 0, BG_POS_OP_SET_Y, y - 96);
    PokeathlonPennantCapture_ApplyFrame(course, viewer, work->players, (PokeathlonPennantCaptureLinkData *)link);
    PokeathlonPennantCapture_UpdateObstacleSprites(work, x, y);
    PokeathlonPennantCapture_UpdateFieldItemSprites(work, x, y);
    PokeathlonPennantCapture_UpdatePennantSprites(work, frame, x, y);
    PokeathlonPennantCapture_AnimateBg(work);
    PokeathlonPennantCaptureDisplay_SetTimer(work->display, frame->timeLeft);
    PokeathlonCourse_UpdateTimeUpCountdown(course, frame->timeLeft);
}

static BOOL PokeathlonPennantCapture_IsTouchOnOwnMon(PokeathlonPennantCapture *work, int participant, int x, int y) {
    PokeathlonPennantCapturePlayer *player = &work->players[participant];
    int monX;
    int monY;
    PokeathlonCourseMonObject *object;

    if (player->member >= 3) {
        GF_AssertFail();
    }
    object = player->monObjects[player->member];
    PokeathlonCourseObject_GroundToCenter(object, 128, 96, &monX, &monY);
    if (PokeathlonCourseObject_IsInTouchRange(object, monX << FX32_SHIFT, monY << FX32_SHIFT, x, y)) {
        return TRUE;
    }
    return FALSE;
}
