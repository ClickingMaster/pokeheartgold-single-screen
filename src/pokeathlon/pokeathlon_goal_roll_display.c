#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_goal_roll.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "player_data.h"
#include "text.h"

// NitroSDK MATH_QSort (no header declares it)
void MATH_QSort(void *base, u32 count, u32 width, int (*compare)(const void *, const void *), void *context);

// Course helpers and the sprite pool (OVY_96, not yet tidied).
u8 *PokeathlonCourse_ToBytes(void *ptr);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u32 score);
Sprite *PokeathlonCourseSprites_CreateTimerDigit(PokeathlonGoalRollResources *resources, SpriteList *spriteList, int a2, enum HeapID heapId);
void PokeathlonCourseMonObject_SetDrawPriority(void *object, u32 priority);
PokeathlonGoalRollSprite *PokeathlonCourseSpritePool_CreateAndTake(PokeathlonGoalRollSpritePool *pool, int a1, int a2, int a3, u8 a4);
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonGoalRollSprite *sprite, BOOL a1, BOOL show);
void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonGoalRollSprite *sprite, int anim);
void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonGoalRollSprite *sprite, const VecFx32 *position);
SpriteList *PokeathlonCourseSpritePool_GetSpriteList(PokeathlonGoalRollSpritePool *pool);

// declared writable here: the plate task was built so (the loads stay in order with the stores)
extern PokeathlonGoalRollPaletteData ov96_0221D1F4;
extern const u8 ov96_0221D648[2];
extern const u8 ov96_0221D64C[2];
extern const WindowTemplate ov96_0221D650;
extern const WindowTemplate ov96_0221D658;
extern const VecFx32 ov96_0221D660;
extern const VecFx32 ov96_0221D66C;

// BgTilemapRectChangePalette as the plate task was built: with int arguments.
typedef void (*PokeathlonGoalRollChangePaletteFunc)(BgConfig *bgConfig, int bgId, int x, int y, int width, int height, int palette);
#define ChangePlatePalette ((PokeathlonGoalRollChangePaletteFunc)BgTilemapRectChangePalette)

#pragma force_active on

void PokeathlonGoalRoll_StartPlateFlash(PokeathlonGoalRoll *goalRoll, int participant, int gained, int bigBall) {
    goalRoll->plates[participant].state = 1;
    goalRoll->plates[participant].gained = gained;
    goalRoll->plates[participant].bigBall = bigBall;
}

// Shows "+1", "+2" or "-1" on a participant's plate and flashes it for 30 frames.
void PokeathlonGoalRoll_PlateTask(SysTask *task, PokeathlonGoalRollPlate *plate) {
    switch (plate->state) {
    case 0:
        break;
    case 1:
        if (plate->gained) {
            if (plate->bigBall) {
                PokeathlonGoalRoll_PrintPlate(plate->owner, plate->id, 1);
            } else {
                PokeathlonGoalRoll_PrintPlate(plate->owner, plate->id, 2);
            }
        } else {
            PokeathlonGoalRoll_PrintPlate(plate->owner, plate->id, 3);
        }
        plate->timer = 0;
        plate->toggle = 0;
        plate->delay = 0;
        plate->state = 2;
        break;
    case 2:
        plate->timer++;
        if (plate->timer > 30) {
            ChangePlatePalette(plate->owner->bgConfig, GF_BG_LYR_MAIN_1, (plate->id & 31) * 8, 0, 8, 2, 1);
            ScheduleBgTilemapBufferTransfer(plate->owner->bgConfig, GF_BG_LYR_MAIN_1);
            PokeathlonGoalRoll_PrintPlate(plate->owner, plate->id, 0);
            plate->unk03 = -1;
            plate->state = 0;
            break;
        }
        if (plate->delay == 0) {
            struct {
                u8 lost[2];
                u8 gained[2];
            } palettes;

            u8 *gainedPalettes = palettes.gained;
            palettes.gained[0] = ov96_0221D1F4.plateFlash[2];
            palettes.gained[1] = ov96_0221D1F4.plateFlash[3];
            u8 first = ov96_0221D1F4.plateFlash[0];
            u8 *lostPalettes = palettes.lost;
            palettes.lost[0] = first;
            palettes.lost[1] = ov96_0221D1F4.plateFlash[1];
            int palette;
            plate->toggle = (plate->toggle + 1) % 2;
            plate->delay = 5;
            if (plate->gained) {
                palette = gainedPalettes[plate->toggle];
            } else {
                palette = lostPalettes[plate->toggle];
            }
            ChangePlatePalette(plate->owner->bgConfig, GF_BG_LYR_MAIN_1, (plate->id & 31) * 8, 0, 8, 2, palette);
            ScheduleBgTilemapBufferTransfer(plate->owner->bgConfig, GF_BG_LYR_MAIN_1);
        } else {
            plate->delay--;
        }
        break;
    }
}

#pragma force_active reset

// The y code of a ball in `goal` last touched by `team` (see PokeathlonGoalRoll_EncodeBallY).
u32 PokeathlonGoalRoll_GetGoalCode(u32 goal, u8 team) {
    u32 code;
    switch (goal) {
    case 0:
        code = 0xD8;
        break;
    case 1:
        code = 0xDC;
        break;
    case 2:
        code = 0xE0;
        break;
    case 3:
        code = 0xE4;
        break;
    default:
        GF_ASSERT(FALSE);
        return 0;
    }
    if (team >= 4) {
        GF_ASSERT(FALSE);
        return 0;
    }
    return (u8)(code + team);
}

#pragma force_active on

u32 PokeathlonGoalRoll_GetBallQuadrant(PokeathlonGoalRollBall *ball) {
    return PokeathlonGoalRoll_GetQuadrant(ball->position.x / 4096, ball->position.y / 4096);
}

#pragma force_active reset

// 0 top left, 1 top right, 2 bottom left, 3 bottom right
u32 PokeathlonGoalRoll_GetQuadrant(s32 x, s32 y) {
    if (x < 128) {
        if (y < 112) {
            return 0;
        }
        return 2;
    }
    if (y < 112) {
        return 1;
    }
    return 3;
}

#pragma force_active on

// Until the last 300 frames, the names of the participants with the highest score are highlighted.
void PokeathlonGoalRoll_UpdateLeaders(PokeathlonGoalRoll *goalRoll) {
    u8 best = 0, i;
    if (goalRoll->timer <= 300) {
        for (i = 0; i < 4; i++) {
            goalRoll->leading[i] = FALSE;
        }
        return;
    }
    for (i = 0; i < 4; i++) {
        if (best < goalRoll->shownScores[i]) {
            best = goalRoll->shownScores[i];
        }
    }
    for (i = 0; i < 4; i++) {
        if (best != 0 && best == goalRoll->shownScores[i]) {
            goalRoll->leading[i] = TRUE;
        } else {
            goalRoll->leading[i] = FALSE;
        }
    }
}

#pragma force_active reset

void PokeathlonGoalRoll_RefreshPlates(PokeathlonGoalRoll *goalRoll) {
    u8 i = 0;
    do {
        if (goalRoll->plates[i].state == 0) {
            PokeathlonGoalRoll_PrintPlate(goalRoll, i, 0);
        }
        i++;
    } while (i < 4);
}

typedef struct PokeathlonGoalRollDrawOrder {
    u8 *y;
    PokeathlonGoalRollMon *mon;
} PokeathlonGoalRollDrawOrder;

#pragma force_active on

// Draws the own team's Pokemon lower on the screen in front.
void PokeathlonGoalRoll_SortOwnTeamOrder(PokeathlonGoalRollMon *team, u8 *monY) {
    PokeathlonGoalRollDrawOrder mons[3];
    PokeathlonGoalRollDrawOrder *order[3];
    PokeathlonGoalRollDrawOrder *high, *low;
    u8 i;
    for (i = 0; i < 3; i++) {
        mons[i].y = monY + i;
        mons[i].mon = team + i;
    }
    low = mons;
    if (*mons[0].y >= *mons[1].y) {
        high = low;
        low = &mons[1];
    } else {
        high = &mons[1];
    }
    if (*high->y >= *mons[2].y) {
        if (*low->y >= *mons[2].y) {
            order[0] = high;
            order[1] = low;
            order[2] = &mons[2];
        } else {
            order[0] = high;
            order[1] = &mons[2];
            order[2] = low;
        }
    } else {
        order[0] = &mons[2];
        order[1] = high;
        order[2] = low;
    }
    for (i = 0; i < 3; i++) {
        PokeathlonCourseMonObject_SetDrawPriority(order[i]->mon->object, i + 8);
    }
}

#pragma force_active reset

typedef struct PokeathlonGoalRollMonY {
    u16 index;
    u16 y;
} PokeathlonGoalRollMonY;

// MATH_QSort order: lower on the screen first, then by mon index.
int PokeathlonGoalRoll_CompareMonY(const void *a, const void *b) {
    const PokeathlonGoalRollMonY *monA = a, *monB = b;
    if (monA->y > monB->y) {
        return -1;
    }
    if (monA->y < monB->y) {
        return 1;
    }
    if (monA->index < monB->index) {
        return -1;
    }
    if (monA->index > monB->index) {
        return 1;
    }
    return 0;
}

#pragma force_active on

// Draws the other participants' Pokemon lower on the screen in front.
void PokeathlonGoalRoll_SortOtherMonsOrder(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollFramePacket *frame, u8 own) {
    PokeathlonGoalRollMonY mons[9];
    u8 i, count = 0;
    i = count;
    for (; i < 12; i++) {
        if (i / 3 != own) {
            mons[count].index = i;
            mons[count].y = frame->monY[i];
            count++;
        }
    }
    MATH_QSort(mons, 9, sizeof(PokeathlonGoalRollMonY), PokeathlonGoalRoll_CompareMonY, goalRoll->sortWork);
    {
        u8 j;
        for (j = 0; j < 9; j++) {
            u8 index = mons[j].index;
            int participant = index / 3;
            int member = index % 3;
            PokeathlonCourseMonObject_SetDrawPriority(goalRoll->mons[participant][member].object, j + 16);
        }
    }
}

#pragma force_active reset

// Course callback at the end of the match: the own team's total.
void PokeathlonGoalRoll_OnTimeUp(PokeathlonCourseData *course) {
    void *area = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourse_ToBytes((u8 *)area + 0xF0);
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    u32 score = PokeathlonGoalRollDisplay_GetScore(goalRoll->display);
    PokeathlonCourse_SetRecordScore(course, score);
    PokeathlonGoalRoll_PrintTotalPoints(goalRoll, score);
}

#pragma force_active on

PokeathlonGoalRollDisplay *PokeathlonGoalRollDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonGoalRollDisplay *display = Heap_Alloc(heapId, sizeof(PokeathlonGoalRollDisplay));
    MI_CpuFill8(display, 0, sizeof(PokeathlonGoalRollDisplay));
    display->heapId = heapId;
    display->bgConfig = bgConfig;
    display->course = course;
    PokeathlonGoalRollDisplay_InitWindows(display);
    display->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, heapId);
    display->msgFormat = MessageFormat_New(heapId);
    return display;
}

#pragma force_active reset

void PokeathlonGoalRollDisplay_Free(PokeathlonGoalRollDisplay *display) {
    Heap_Free(display->digitScreenData);
    MessageFormat_Delete(display->msgFormat);
    DestroyMsgData(display->msgData);
    RemoveWindow(&display->nameWindow);
    RemoveWindow(&display->messageWindow);
    Heap_Free(display);
}

void PokeathlonGoalRollDisplay_LoadBgGfx(PokeathlonGoalRollDisplay *display) {
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 8, display->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 9, display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, display->heapId);
    display->digitScreenData = GfGfxLoader_GetScrnData(NARC_a_2_3_4, 10, FALSE, &display->digitScreen, display->heapId);
    PokeathlonGoalRollDisplay_DrawScore(display, 0);
}

void PokeathlonGoalRollDisplay_CreateSprites(PokeathlonGoalRollSpritePool *pool, PokeathlonGoalRollDisplay *display, PokeathlonGoalRollResources *resources) {
    VecFx32 numberPosition = ov96_0221D660;
    VecFx32 labelPosition = ov96_0221D66C;
    PokeathlonGoalRollSprite *sprite;
    u8 i;

    sprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 0, 2, 102, 9);
    PokeathlonCourseSpritePoolSprite_Show(sprite, TRUE, TRUE);
    PokeathlonCourseSpritePoolSprite_SetPosition(sprite, &numberPosition);
    PokeathlonCourseSpritePoolSprite_SetAnim(sprite, PokeathlonCourse_GetNetId(display->course) + 1);

    sprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 0, 2, 102, 10);
    PokeathlonCourseSpritePoolSprite_Show(sprite, TRUE, TRUE);
    PokeathlonCourseSpritePoolSprite_SetPosition(sprite, &labelPosition);
    PokeathlonCourseSpritePoolSprite_SetAnim(sprite, 5);
    display->timerLabelSprite = sprite;

    for (i = 0; i < 2; i++) {
        VecFx32 position;
        display->timerDigits[i] = PokeathlonCourseSprites_CreateTimerDigit(resources, PokeathlonCourseSpritePool_GetSpriteList(pool), 0, display->heapId);
        Sprite_SetDrawFlag(display->timerDigits[i], TRUE);
        position.x = ov96_0221D648[i] << 12;
        position.y = 10 << 18;
        position.z = 0;
        Sprite_SetMatrix(display->timerDigits[i], &position);
        Sprite_SetAnimCtrlSeq(display->timerDigits[i], ov96_0221D64C[i]);
    }
}

void PokeathlonGoalRollDisplay_PrintName(PokeathlonGoalRollDisplay *display) {
    String *name;
    u8 own;
    FillWindowPixelBuffer(&display->nameWindow, 0);
    own = PokeathlonCourse_GetNetId(display->course);
    name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(display->course, own), display->heapId);
    AddTextPrinterParameterizedWithColor(&display->nameWindow, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(name);
    CopyWindowToVram(&display->nameWindow);
}

void PokeathlonGoalRollDisplay_PrintSecondWind(PokeathlonGoalRollDisplay *display) {
    String *string;
    FillWindowPixelBuffer(&display->messageWindow, 0);
    string = ReadMsgData_ExpandPlaceholders(display->msgFormat, display->msgData, msg_0309_00311, display->heapId);
    AddTextPrinterParameterizedWithColor(&display->messageWindow, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(&display->messageWindow);
}

void PokeathlonGoalRollDisplay_InitWindows(PokeathlonGoalRollDisplay *display) {
    AddWindow(display->bgConfig, &display->nameWindow, &ov96_0221D650);
    AddWindow(display->bgConfig, &display->messageWindow, &ov96_0221D658);
    BG_FillCharDataRange(display->bgConfig, GF_BG_LYR_SUB_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, display->heapId);
}

void PokeathlonGoalRollDisplay_SetScore(PokeathlonGoalRollDisplay *display, u32 score, u32 participant) {
    if (participant == PokeathlonCourse_GetNetId(display->course) && display->ownScore != score) {
        display->ownScore = score;
    }
}

u32 PokeathlonGoalRollDisplay_GetScore(PokeathlonGoalRollDisplay *display) {
    return (u8)display->ownScore;
}
