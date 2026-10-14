#include "pokeathlon/pokeathlon_course_ceremony.h"

#include "global.h"

#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_ui.h"
#include "pokeathlon/pokeathlon_util.h"

#include "assert.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "palette.h"
#include "poke_overlay.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "system.h"
#include "wireless_status_icon.h"

FS_EXTERN_OVERLAY(OVY_98);

// Course code (OVY_96, not yet tidied).
PokeathlonParticipantData *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int member);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int member);
u32 PokeathlonCourse_GetSpriteSizeType(PokeathlonParticipantData *performance);
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, PokeathlonCourseObjectInfo *info);
u32 PokeathlonCourse_GetPerformanceFileId(u16 species, u16 form);
u32 PokeathlonCourse_GetNpcTeamSpriteId(u8 npcTeam);
u32 PokeathlonCourse_GetNpcTeamMsgId(PokeathlonCourseData *course, u8 npcTeam);
void *PokeathlonCourseMonGfx_GetInfo(void *gfx);
u32 PokeathlonCourseScoreList_GetEventPoints(void *scoreList, int event, int team);
void *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, int flags, int charVram);
void PokeathlonCourseSprites_FreeResources(void *resources);
void *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int count, int baseDrawPriority, void *resources, SpriteList *spriteList);
void PokeathlonCourseMonObjects_Free(void *objects);
void PokeathlonCourseMonObjects_Load(void *objects, int count, PokeathlonCourseObjectInfo *infos, PokeathlonCourseObjectsTemplate *template, BOOL priority, BOOL baseIndex);
BOOL PokeathlonCourseMonObjects_IsLoaded(void *objects);
void *PokeathlonCourseMonObject_GetGfx(void *object);
void PokeathlonCourseMonObject_SetDrawFlag(void *object, int draw);
void PokeathlonCourseMonObject_SetAnimState(void *object, int state);
void PokeathlonCourseMonObject_SetAnim(void *object, int anim);
BOOL PokeathlonCourseMonObject_IsAnimated(void *object);
void PokeathlonCourseMonObjects_SetPalette(void *objects, u8 index, int vram, void *palette);

extern const GraphicsBanks gPokeathlonCourseCeremonyGraphicsBanks;
extern const s32 gPokeathlonCourseCeremonyCheerPitches[];
extern const GraphicsModes gPokeathlonCourseCeremonyGraphicsModes;
extern const BgTemplate gPokeathlonCourseCeremonyBgTemplate0;
extern const BgTemplate gPokeathlonCourseCeremonyBgTemplate1;
extern const BgTemplate gPokeathlonCourseCeremonyBgTemplate4;
extern const BgTemplate gPokeathlonCourseCeremonyBgTemplate5;
extern const SpriteResourceCountsListUnion gPokeathlonCourseCeremonySpriteResourceCounts;
extern const OamManagerParam gPokeathlonCourseCeremonyOamManagerParam;
extern const OamCharTransferParam gPokeathlonCourseCeremonyOamCharTransferParam;
extern const ManagedSpriteTemplate gPokeathlonCourseConfettiSpriteTemplate;
extern const ManagedSpriteTemplate gPokeathlonCourseConfettiTopSpriteTemplate;
extern const s32 gPokeathlonCourseCeremonyCheerThresholds[12];
extern const u32 gPokeathlonCourseCeremonyIntroStatWeights[29];
extern const u32 gPokeathlonCourseCeremonyIntroStatThresholds[4];

// Not declared by a header yet.
void SetBothScreensBrightnessByColor(u16 color);

// NitroSDK math (not declared by a header yet).
u32 MATH_CountPopulation(u32 value);
void MATH_QSort(void *head, u32 num, u32 width, int (*compare)(const void *, const void *), void *stackBuffer);

BOOL PokeathlonCourseOpening_Init(PokeathlonCourseData *course) {
    return PokeathlonCourseCeremony_Init(course);
}

BOOL PokeathlonCourseOpening_Main(PokeathlonCourseData *course) {
    return PokeathlonCourseCeremony_Main(course);
}

BOOL PokeathlonCourseOpening_Exit(PokeathlonCourseData *course) {
    return PokeathlonCourseCeremony_Exit(course);
}

BOOL PokeathlonCourseClosing_Init(PokeathlonCourseData *course) {
    return PokeathlonCourseCeremony_Init(course);
}

BOOL PokeathlonCourseClosing_Main(PokeathlonCourseData *course) {
    return PokeathlonCourseCeremony_Main(course);
}

BOOL PokeathlonCourseClosing_Exit(PokeathlonCourseData *course) {
    return PokeathlonCourseCeremony_Exit(course);
}

void PokeathlonCourseCeremony_SetBanks(void) {
    GraphicsBanks banks = gPokeathlonCourseCeremonyGraphicsBanks;

    GfGfx_SetBanks(&banks);
}

BOOL PokeathlonCourseOpening_FadeIn(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (*state) {
    case 0:
        if (!PokeathlonCourse_GetNetId(course)) {
            PokeathlonCourseCeremony_ChooseIntros(course);
        }
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0, 6, 1, ceremony->heapId);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

// The commentator welcomes everyone to the course, then introduces every team in turn: its
// line, its name and Pokemon, while the team's Pokemon jump and the crowd cheers.
#pragma force_active on

BOOL PokeathlonCourseOpening_Introduce(PokeathlonCourseData *course) {
    int i;
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    BOOL waiting = FALSE;
    if (PokeathlonCourse_GetMode(course) == 1) {
        waiting = PokeathlonCourseCeremony_SyncOpening(course);
    }
    PokeathlonCourseMessageBox_Update(ceremony->messageBox);
    if (!PokeathlonCourseMessageBox_IsBusy(ceremony->messageBox) && !waiting) {
        MessageFormat *msgFormat = PokeathlonCourseMessageBox_GetMsgFormat(ceremony->messageBox);
        int msgId = -1;
        switch (ceremony->state) {
        case 0:
            msgId = 0;
            BufferPokeathlonCourseName(msgFormat, 0, ceremony->course);
            ceremony->state = 1;
            break;
        case 1:
            SetBothScreensBrightnessByColor(0x7FFF);
            PlaySE(SEQ_SE_GS_VOICE01);
            ceremony->state = 2;
            break;
        case 2:
            if (ceremony->counter < 4) {
                ceremony->counter++;
            } else {
                ceremony->counter = 0;
                ceremony->state = 3;
            }
            break;
        case 3:
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 24, 1, ceremony->heapId);
            ceremony->state = 4;
            break;
        case 4:
            if (IsPaletteFadeFinished()) {
                ceremony->state = 5;
            }
            break;
        case 5:
            if (*(int *)PokeathlonCourse_GetParticipantData(course, ceremony->team) > 0) {
                PokeathlonCourseMessageBox_PrintNpcLine(ceremony->messageBox, PokeathlonCourseCeremony_GetIntroMsgId(course, ceremony->team));
            } else {
                msgId = PokeathlonCourseCeremony_GetIntroMsgId(course, ceremony->team);
                PokeathlonCourseCeremony_BufferNumber(msgFormat, PokeathlonCourseCeremony_GetIntroValue(course, ceremony->team), 7, 1);
            }
            ceremony->state = 6;
            break;
        case 6: {
            BufferPlayersName(msgFormat, 0, PokeathlonCourse_GetPlayerProfileFromData(course, ceremony->team));
            int i;
            for (i = 0; i < 3; i++) {
                PokeathlonCourseCeremony_BufferMonName(course, ceremony->team, (u8)i, i + 1);
            }
            PokeathlonCourseCeremony_SetTrainerPose(ceremony->trainerSprites, course, ceremony->team, 2, ceremony->heapId);
            ManagedSprite_SetAnim(ceremony->trainerSprites[ceremony->team], 1);
            u8 cheer = PokeathlonCourseCeremony_GetIntroCheer(course, ceremony->team);
            int seqNo;
            if (cheer < 5) {
                seqNo = SEQ_SE_GS_VOICE01;
            } else {
                seqNo = SEQ_SE_GS_VOICE02;
            }
            PlaySE_SetPitch((u16)seqNo, (s16)gPokeathlonCourseCeremonyCheerPitches[cheer]);
            ceremony->cheering = TRUE;
            msgId = 1;
            ceremony->state = 7;
            break;
        }
        case 7:
            if (ceremony->cheering == FALSE) {
                if (ceremony->team >= 4) {
                    ceremony->finished = TRUE;
                    msgId = 2;
                    ceremony->state = 8;
                } else {
                    ceremony->state = 5;
                }
            }
            break;
        case 8:
            if (PokeathlonCourse_GetMode(course) == 0 || PokeathlonCourseCeremony_IsOpeningFinished(course)) {
                if (PokeathlonCourseCeremony_IsCryActive(ceremony)) {
                    Heap_Free(ceremony->cryData);
                    SysTask_Destroy(ceremony->cryTask);
                }
                PokeathlonCourseMessageBox_Close(ceremony->messageBox);
                PokeathlonCourse_SetStateField07(course, 2);
                return FALSE;
            }
            break;
        default:
            GF_AssertFail();
            break;
        }
        if (msgId != -1) {
            PokeathlonCourseMessageBox_Print(ceremony->messageBox, msgId);
            ceremony->messageCount++;
        }
    }
    if (ceremony->cheering) {
        int index = (u8)(ceremony->team * 3);
        if (ceremony->timer++ == 0) {
            for (i = 0; i < 3; i++, index++) {
                void *object = PokeathlonCourseMonObjects_Get(ceremony->objects, (u8)index);
                PokeathlonCourseMonObject_SetAnimState(object, 2);
                PokeathlonCourseMonObject_SetAnim(object, 16);
            }
        } else if (ceremony->timer++ >= 60) {
            int i;
            for (i = 0; i < 3; index++, i++) {
                void *object = PokeathlonCourseMonObjects_Get(ceremony->objects, (u8)index);
                PokeathlonCourseMonObject_SetAnimState(object, 1);
                PokeathlonCourseMonObject_SetAnim(object, 0);
                Sprite_GetCellAnim(PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonObject_GetGfx(object)))->animCtrl.currentTime = i * 3 << FX32_SHIFT;
            }
            PokeathlonCourseCeremony_SetTrainerPose(ceremony->trainerSprites, course, ceremony->team, 1, ceremony->heapId);
            ceremony->cheering = FALSE;
            ceremony->team++;
            ceremony->timer = 0;
        }
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonCourseOpening_FadeOut(PokeathlonCourseData *course, u8 *state) {
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFF, 90, 1, PokeathlonCourse_GetHeapID(course));
        GF_SndStartFadeOutBGM(0, 90);
        PlaySE(SEQ_SE_GS_VOICE02);
        (*state)++;
        break;
    case 1:
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

BOOL PokeathlonCourseClosing_FadeIn(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (*state) {
    case 0:
        if (!PokeathlonCourse_GetNetId(course)) {
            PokeathlonCourseCeremony_ChooseAwards(course);
        }
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0, 6, 1, ceremony->heapId);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

// The awards: Challenge Bonus of every team, No-Miss Bonus, points leader and prize for effort
// (each Pokemon's cry), then the event points of every round counted up, the winner (by drawing
// lots when tied) with its medals and course record.
#pragma force_active on

BOOL PokeathlonCourseClosing_Awards(PokeathlonCourseData *course) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    BOOL waiting = FALSE;
    int msgId;
    int i;
    MessageFormat *msgFormat;
    u8 drawingLots[4];
    int rounds;
    PokeathlonCourseCeremony_UpdateScores(ceremony->scores);
    PokeathlonCourseConfetti_Update(ceremony->confetti);
    if (PokeathlonCourse_GetMode(course) == 1) {
        waiting = PokeathlonCourseCeremony_SyncClosing(course);
    }
    PokeathlonCourseMessageBox_Update(ceremony->messageBox);
    if (PokeathlonCourseMessageBox_IsBusy(ceremony->messageBox) || waiting) {
        goto end;
    }
    msgFormat = PokeathlonCourseMessageBox_GetMsgFormat(ceremony->messageBox);
    msgId = -1;
    switch (ceremony->state) {
    case 0:
        msgId = 3;
        ceremony->state = 1;
        break;
    case 1:
        msgId = 4;
        ceremony->state = 2;
        break;
    case 2:
        for (i = 0; i < 4; i++) {
            int points = PokeathlonCourseCeremony_GetChallengeBonus(course, (u8)i);
            PokeathlonCourseCeremony_AddPoints(ceremony->scores, (u8)i, points);
        }
        ceremony->state = 3;
        // fallthrough
    case 3:
        if (ceremony->team >= 4) {
            ceremony->team = 0;
            msgId = 6;
            ceremony->state = 4;
        } else {
            int points;
            msgId = 5;
            points = PokeathlonCourseCeremony_GetChallengeBonus(course, ceremony->team);
            PokeathlonCourseCeremony_BufferTeamPoints(course, ceremony->team, points);
            ceremony->team++;
        }
        break;
    case 4:
        if (PokeathlonCourseCeremony_HasNoMissBonus(course)) {
            msgId = 7;
            ceremony->state = 5;
        } else {
            msgId = 9;
            ceremony->state = 6;
        }
        break;
    case 5:
        if (!PokeathlonCourseCeremony_IsCryActive(ceremony)) {
            if (ceremony->counter >= (u32)PokeathlonCourseCeremony_CountNoMissMons(course)) {
                ceremony->team = 0;
                ceremony->member = 0;
                ceremony->counter = 0;
                msgId = 10;
                ceremony->state = 6;
            } else if (PokeathlonCourseCeremony_AwardNextMon(course, PokeathlonCourseCeremony_IsNoMissMon, PokeathlonCourseCeremony_GetNoMissPoints(ceremony))) {
                msgId = 8;
            }
        }
        break;
    case 6:
        msgId = 11;
        ceremony->state = 7;
        break;
    case 7:
        if (!PokeathlonCourseCeremony_IsCryActive(ceremony)) {
            if (ceremony->counter >= (u32)PokeathlonCourseCeremony_CountPointsLeaders(course)) {
                ceremony->team = 0;
                ceremony->member = 0;
                ceremony->counter = 0;
                msgId = 10;
                ceremony->state = 8;
            } else if (PokeathlonCourseCeremony_AwardNextMon(course, PokeathlonCourseCeremony_IsPointsLeader, PokeathlonCourseCeremony_GetPointsLeaderPoints(ceremony))) {
                msgId = 12;
            }
        }
        break;
    case 8:
        if (PokeathlonCourseCeremony_GetEffortPrizeKind(course) != 4) {
            msgId = 14;
            ceremony->state = 9;
        } else {
            ceremony->state = 11;
        }
        break;
    case 9:
        msgId = PokeathlonCourseCeremony_GetEffortPrizeKind(course) + 15;
        ceremony->state = 10;
        break;
    case 10:
        if (!PokeathlonCourseCeremony_IsCryActive(ceremony)) {
            if (ceremony->counter >= (u32)PokeathlonCourseCeremony_CountEffortPrizes(course)) {
                ceremony->team = 0;
                ceremony->member = 0;
                ceremony->counter = 0;
                msgId = 21;
                ceremony->state = 11;
            } else if (PokeathlonCourseCeremony_AwardNextMon(course, PokeathlonCourseCeremony_IsEffortPrizeMon, PokeathlonCourseCeremony_GetEffortPrizePoints(ceremony))) {
                msgId = 20;
            }
        }
        break;
    case 11:
        msgId = 22;
        ceremony->state = 12;
        StopBGM(GF_GetCurrentPlayingBGM(), 16);
        PlaySE(SEQ_SE_DP_CON_033);
        break;
    case 12:
        if (PokeathlonCourseCeremony_AreScoresCounted(ceremony->scores)) {
            if ((s32)ceremony->timer++ > 30) {
                rounds = PokeathlonCourse_GetMode(course);
                if (rounds == 0) {
                    rounds = 3;
                } else {
                    rounds = 4;
                }
                ceremony->timer = 0;
                if (ceremony->counter >= (u8)rounds) {
                    ceremony->counter = 0;
                    ceremony->state = 13;
                } else {
                    msgId = 23;
                    PokeathlonCourseCeremony_BufferNumber(msgFormat, ceremony->counter + 1, 1, 0);
                    for (i = 0; i < 4; i++) {
                        int points = PokeathlonCourseCeremony_GetEventPoints(course, (u8)i, ceremony->counter);
                        PokeathlonCourseCeremony_AddPoints(ceremony->scores, (u8)i, (u16)points);
                    }
                    ceremony->counter++;
                }
            }
        }
        break;
    case 13:
        PokeathlonCourseCeremony_GetDrawingLots(course, drawingLots);
        PokeathlonCourseCeremony_RankTeams(ceremony->scores, ceremony->ranks, drawingLots);
        if (ceremony->ranks[0].place == ceremony->ranks[1].place) {
            ceremony->state = 14;
        } else {
            ceremony->state = 15;
        }
        break;
    case 14:
        msgId = 24;
        PokeathlonCourseCeremony_BreakTie(ceremony->ranks);
        ceremony->state = 15;
        break;
    case 15:
        msgId = 25;
        PokeathlonCourseCeremony_BufferNumber(msgFormat, PokeathlonCourseCeremony_GetPoints(ceremony->scores, (u8)ceremony->ranks[0].team), 3, 0);
        PokeathlonCourseCeremony_SaveResult(course, ceremony->scores);
        ceremony->state = 16;
        break;
    case 16:
        PokeathlonCourseCeremony_LoadWinnerBg(ceremony->bgConfig, ceremony->heapId);
        ceremony->state = 17;
        // fallthrough
    case 17: {
        int winner = ceremony->ranks[0].team;
        msgId = 26;
        BufferPlayersName(msgFormat, 0, PokeathlonCourse_GetPlayerProfileFromData(course, winner));
        PokeathlonCourseCeremony_ShowWinnerTeam(course, (u8)winner);
        PokeathlonCourseConfetti_Start(ceremony->confetti, TRUE);
        PokeathlonCourseCeremony_SetTrainerPose(ceremony->trainerSprites, course, winner, 2, ceremony->heapId);
        ManagedSprite_SetAnim(ceremony->trainerSprites[winner], 1);
        StopSE(SEQ_SE_DP_CON_033, 0);
        PlaySE(SEQ_SE_GS_VOICE10);
        PlaySE(SEQ_SE_GS_VOICE07);
        PlayBGM(SEQ_GS_PT_VICTORY);
        if (ceremony->course == 10) {
            ceremony->state = 22;
        } else {
            ceremony->state = 18;
        }
        break;
    }
    case 22:
        msgId = 30;
        BufferPokeathlonCourseName(msgFormat, 0, ceremony->course);
        ceremony->state = 20;
        break;
    case 18:
        msgId = 27;
        BufferPlayersName(msgFormat, 0, PokeathlonCourse_GetPlayerProfileFromData(course, ceremony->ranks[0].team));
        ManagedSprite_SetDrawFlag(ceremony->medalSprites[0], TRUE);
        BufferPokeathlonMedalName(msgFormat, 1, PokeathlonCourseCeremony_GetCourseMedal(ceremony->course, 0));
        if (ceremony->course >= 5) {
            msgId = 28;
            ManagedSprite_SetDrawFlag(ceremony->medalSprites[1], TRUE);
            BufferPokeathlonMedalName(msgFormat, 2, PokeathlonCourseCeremony_GetCourseMedal(ceremony->course, 1));
        }
        ceremony->state = 19;
        break;
    case 19: {
        u32 courseId = ceremony->course;
        u32 winner;
        int own;
        int points;
        PokeathlonSave *pokeathlonSave;
        if (PokeathlonCourse_GetMode(course) != 0) {
            goto noRecord;
        }
        winner = ceremony->ranks[0].team;
        own = PokeathlonCourse_GetNetId(course);
        if (winner != (u32)own) {
            goto noRecord;
        }
        GF_ASSERT(winner == 0);
        pokeathlonSave = PokeathlonSave_dummy2(Save_Pokeathlon_Get(PokeathlonCourse_GetSaveData(course)));
        points = (s32)PokeathlonCourseCeremony_GetPoints(ceremony->scores, (u8)ceremony->ranks[0].team);
        // The best total of the course.
        if (pokeathlonSave->prefix.nativeHalfwords[courseId * 22 + 3] < (u32)points) {
            msgId = 29;
            BufferPlayersName(msgFormat, 0, PokeathlonCourse_GetPlayerProfileFromData(course, ceremony->ranks[0].team));
            BufferPokeathlonCourseName(msgFormat, 1, courseId);
        }
        goto next;
noRecord:
        msgId = 30;
        BufferPokeathlonCourseName(msgFormat, 0, courseId);
next:
        ceremony->state = 20;
        break;
    }
    case 20:
        msgId = 31;
        StopSE(SEQ_SE_GS_VOICE10, 120);
        ceremony->finished = TRUE;
        ceremony->state = 21;
        break;
    case 21:
        if (PokeathlonCourse_GetMode(course) == 0 || PokeathlonCourseCeremony_IsClosingFinished(course)) {
            if (PokeathlonCourseCeremony_IsCryActive(ceremony)) {
                Heap_Free(ceremony->cryData);
                SysTask_Destroy(ceremony->cryTask);
            }
            PokeathlonCourseMessageBox_Close(ceremony->messageBox);
            PokeathlonCourse_SetStateField07(course, 2);
            return FALSE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (msgId != -1) {
        PokeathlonCourseMessageBox_Print(ceremony->messageBox, msgId);
        ceremony->messageCount++;
    }
end:
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonCourseClosing_FadeOut(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);

    PokeathlonCourseConfetti_Update(ceremony->confetti);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFF, 30, 1, PokeathlonCourse_GetHeapID(course));
        GF_SndStartFadeOutBGM(0, 30);
        (*state)++;
        break;
    case 1:
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

// An awarded Pokemon jumps while its cry plays.
void PokeathlonCourseCeremony_CryTask(SysTask *task, void *data) {
    PokeathlonCourseCeremonyCryTask *cry = data;
    void *object = cry->object;

    switch (cry->state) {
    case 0:
        PokeathlonCourseMonObject_SetAnimState(object, 2);
        PokeathlonCourseMonObject_SetAnim(object, 16);
        cry->state++;
        break;
    case 1:
        if (cry->timer++ > 60) {
            cry->state++;
        }
        break;
    case 2:
        if (!PokeathlonCourseMonObject_IsAnimated(object) && !IsCryFinished()) {
            PokeathlonCourseMonObject_SetAnimState(object, 1);
            PokeathlonCourseMonObject_SetAnim(object, 0);
            *cry->active = FALSE;
            Heap_Free(cry);
            SysTask_Destroy(task);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void PokeathlonCourseCeremony_StartCry(PokeathlonCourseCeremony *ceremony, u8 index) {
    PokeathlonCourseCeremonyCryTask *cry = Heap_AllocAtEnd(ceremony->heapId, sizeof(PokeathlonCourseCeremonyCryTask));

    MI_CpuFill8(cry, 0, sizeof(PokeathlonCourseCeremonyCryTask));
    ceremony->cryData = cry;
    cry->object = PokeathlonCourseMonObjects_Get(ceremony->objects, index);
    ceremony->cryActive = TRUE;
    cry->active = &ceremony->cryActive;
    ceremony->cryTask = SysTask_CreateOnMainQueue(PokeathlonCourseCeremony_CryTask, cry, 1);
}

BOOL PokeathlonCourseCeremony_IsCryActive(PokeathlonCourseCeremony *ceremony) {
    return ceremony->cryActive;
}

#pragma force_active on

void PokeathlonCourseCeremony_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes = gPokeathlonCourseCeremonyGraphicsModes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg4;
    BgTemplate bg5;
    SetBothScreensModesAndDisable(&modes);
    bg0 = gPokeathlonCourseCeremonyBgTemplate0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BG_FillCharDataRange(bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 0);
    bg1 = gPokeathlonCourseCeremonyBgTemplate1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BG_FillCharDataRange(bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 0);
    bg4 = gPokeathlonCourseCeremonyBgTemplate4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    bg5 = gPokeathlonCourseCeremonyBgTemplate5;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
}

#pragma force_active reset

void PokeathlonCourseCeremony_LoadBgGfx(BgConfig *bgConfig, enum HeapID heapId) {
    GfGfxLoader_LoadCharData(NARC_a_1_5_4, 3, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_5_4, 1, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_4, 6, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_4, 4, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_4, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_4, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, heapId);
}

void PokeathlonCourseCeremony_LoadWinnerBg(BgConfig *bgConfig, enum HeapID heapId) {
    GfGfxLoader_LoadCharData(NARC_a_1_5_4, 3, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_4, 7, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapId);
}

void PokeathlonCourseCeremony_InitSpriteSystem(PokeathlonCourseCeremony *ceremony) {
    SpriteResourceCountsListUnion counts = gPokeathlonCourseCeremonySpriteResourceCounts;
    OamManagerParam oamManagerParam = gPokeathlonCourseCeremonyOamManagerParam;
    OamCharTransferParam charTransferParam = gPokeathlonCourseCeremonyOamCharTransferParam;

    charTransferParam.maxTasks = 0x80;
    ceremony->spriteSystem = SpriteSystem_Alloc(ceremony->heapId);
    ceremony->spriteManager = SpriteManager_New(ceremony->spriteSystem);
    SpriteSystem_Init(ceremony->spriteSystem, &oamManagerParam, &charTransferParam, 32);
    SpriteSystem_InitSprites(ceremony->spriteSystem, ceremony->spriteManager, 0x80);
    SpriteSystem_InitManagerWithCapacities(ceremony->spriteSystem, ceremony->spriteManager, &counts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(ceremony->spriteSystem), 0, 0x20C000);
}

void PokeathlonCourseCeremony_FreeSprites(PokeathlonCourseCeremony *ceremony) {
    u8 i;

    for (i = 0; i < 26; i++) {
        if (ceremony->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(ceremony->sprites[i]);
            ceremony->sprites[i] = NULL;
        }
    }
    SpriteSystem_FreeResourcesAndManager(ceremony->spriteSystem, ceremony->spriteManager);
    SpriteSystem_Free(ceremony->spriteSystem);
}

void PokeathlonCourseCeremony_CreateTrainerSprites(PokeathlonCourseCeremony *ceremony) {
    ManagedSpriteTemplate template = { 0 };
    int i;
    int x = 32;
    // Walks through trainerSprites by moving a view of the ceremony one pointer on.
    PokeathlonCourseCeremony *cursor = ceremony;

    for (i = 0; i < 5;) {
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = i + 100;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = i + 100;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 100;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 100;
        template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        template.bgPriority = 1;
        template.drawPriority = 1;
        template.x = x;
        template.y = 136;
        if (i == 4) {
            template.x = 128;
            template.y = 112;
        }
        cursor->trainerSprites[0] = SpriteSystem_NewSpriteWithYOffset(ceremony->spriteSystem, ceremony->spriteManager, &template, 0x20C000);
        ManagedSprite_SetAnimateFlag(cursor->trainerSprites[0], TRUE);
        i++;
        x += 64;
        cursor = (PokeathlonCourseCeremony *)((u8 *)cursor + sizeof(ManagedSprite *));
    }
}

#pragma force_active on

void PokeathlonCourseCeremony_CreateScoreSprites(PokeathlonCourseCeremony *ceremony) {
    ManagedSpriteTemplate template = { 0 };
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        int index = i * 4 + 5;
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = 105;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = 105;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 101;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 101;
        template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        template.bgPriority = 1;
        template.drawPriority = 2;
        template.x = i * 64 + 32;
        template.y = 72;
        template.animation = 0;
        template.pal = i + 1;
        ceremony->sprites[index] = SpriteSystem_NewSpriteWithYOffset(ceremony->spriteSystem, ceremony->spriteManager, &template, 0x20C000);
        ManagedSprite_SetAnimateFlag(ceremony->sprites[index], TRUE);
        template.pal = 0;
        template.bgPriority = 1;
        template.drawPriority = 1;
        template.x -= 16;
        template.animation = 1;
        for (j = 0; j < 3; j++) {
            int digit = index + 1 + j;
            ceremony->sprites[digit] = SpriteSystem_NewSpriteWithYOffset(ceremony->spriteSystem, ceremony->spriteManager, &template, 0x20C000);
            ManagedSprite_SetAnimateFlag(ceremony->sprites[digit], TRUE);
            template.x += 16;
        }
    }
}

#pragma force_active reset

void PokeathlonCourseCeremony_CreateMonPicSprites(PokeathlonCourseCeremony *ceremony) {
    ManagedSpriteTemplate template = { 0 };
    int i;
    int x = 48;
    int palette = 2;

    for (i = 0; i < 3; i++) {
        ManagedSprite *sprite;
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = 109 + i;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = 106;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 104;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 104;
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.x = x;
        template.y = 112;
        template.bgPriority = 0;
        template.drawPriority = 1;
        {
            PokeathlonCourseCeremony *slot = (PokeathlonCourseCeremony *)((u8 *)ceremony + (21 + i) * sizeof(ManagedSprite *));
            sprite = SpriteSystem_NewSpriteWithYOffset(ceremony->spriteSystem, ceremony->spriteManager, &template, 0x20C000);
            slot->sprites[0] = sprite;
            ManagedSprite_SetPaletteOverride(sprite, palette);
            ManagedSprite_SetDrawFlag(slot->sprites[0], FALSE);
        }
        palette += 3;
        x += 80;
    }
}

#pragma force_active on

void PokeathlonCourseCeremony_CreateMedalSprites(PokeathlonCourseCeremony *ceremony) {
    ManagedSpriteTemplate template = { 0 };
    int i;
    for (i = 0; i < 2; i++) {
        int index = i + 24;
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = i + 107;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = i + 107;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 103;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 103;
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.bgPriority = 0;
        template.drawPriority = 2;
        template.x = i * 64 + 96;
        template.y = 56;
        if (ceremony->course < 5) {
            template.x = 128;
        }
        ceremony->sprites[index] = SpriteSystem_NewSpriteWithYOffset(ceremony->spriteSystem, ceremony->spriteManager, &template, 0x20C000);
        ManagedSprite_SetAnimateFlag(ceremony->sprites[index], TRUE);
        ManagedSprite_SetDrawFlag(ceremony->sprites[index], FALSE);
    }
}

#pragma force_active reset

// Pictures of the winning team's Pokemon, and their palettes for the overworld sprites.
#pragma force_active on

void PokeathlonCourseCeremony_LoadWinnerMonPics(PokeathlonCourseData *course, int team) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    NNSG2dPaletteData *palette;
    PokeathlonCourseObjectInfo info;
    PokepicTemplate template;
    int i;
    void *file;
    PokeathlonCourse_GetParticipantMons(course, team);
    PokeathlonCourse_GetParticipantData(course, team);
    for (i = 0; i < 3; i++) {
        PokeathlonCourse_GetMonInfo(course, team, i, &info);
        PokeathlonCourse_LoadMonPicWithPalette(ceremony->monPicSprites[i], &info, TRUE, FALSE, ceremony->heapId, FALSE);
        GetMonSpriteCharAndPlttNarcIdsEx(&template, info.species, info.gender, 2, info.shiny, info.form, info.personality);
        file = GfGfxLoader_GetPlttData((NarcId) template.narcID, template.palDataID, &palette, ceremony->heapId);
        PokeathlonCourseMonObjects_SetPalette(ceremony->objects, i * 3 + 1, 1, palette->pRawData);
        Heap_Free(file);
    }
}

#pragma force_active reset

void PokeathlonCourseCeremony_LoadSpriteResources(PokeathlonCourseData *course) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    SpriteSystem *spriteSystem = ceremony->spriteSystem;
    SpriteManager *spriteManager = ceremony->spriteManager;
    u8 i;
    u8 courseId;

    for (i = 0; i < 5; i++) {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 9, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 100 + i);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 100 + i);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 10, TRUE, 100);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 11, TRUE, 100);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 13, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 105);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 12, FALSE, 5, NNS_G2D_VRAM_TYPE_2DSUB, 105);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 14, TRUE, 101);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 15, TRUE, 101);
    PokeathlonCourseCeremony_LoadMonPicResources(ceremony);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 16, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 106);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 12, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 106);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 17, TRUE, 102);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 18, TRUE, 102);
    courseId = ceremony->course;
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, PokeathlonCourseCeremony_GetCourseMedal(courseId, 0) + 23, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 107);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, PokeathlonCourseCeremony_GetCourseMedal(courseId, 1) + 23, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 108);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, PokeathlonCourseCeremony_GetCourseMedal(courseId, 0) + 28, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 107);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, PokeathlonCourseCeremony_GetCourseMedal(courseId, 1) + 28, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 108);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 33, TRUE, 103);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 34, TRUE, 103);
}

void PokeathlonCourseCeremony_LoadMonPicResources(PokeathlonCourseCeremony *ceremony) {
    SpriteSystem *spriteSystem = ceremony->spriteSystem;
    int i;
    SpriteManager *spriteManager = ceremony->spriteManager;
    NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DMAIN;

    for (i = 0; i < 3; i++) {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 20, TRUE, vram, 109 + i);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 21, TRUE, 104);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 22, TRUE, 104);
}

// The pictures replace the overworld sprites of the winner's Pokemon on the top screen.
#pragma force_active on

void PokeathlonCourseCeremony_ShowWinnerTeam(PokeathlonCourseData *course, int team) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    u32 i;
    void *member1;
    void *member2;
    i = 0;
    for (; (s32)i < 4; i++) {
        member1 = PokeathlonCourseMonObjects_Get(ceremony->objects, (u8)(3 * i + 1));
        member2 = PokeathlonCourseMonObjects_Get(ceremony->objects, (u8)(3 * i + 2));
        PokeathlonCourseMonObject_SetDrawFlag(member1, FALSE);
        PokeathlonCourseMonObject_SetDrawFlag(member2, FALSE);
    }
    PokeathlonCourseCeremony_LoadWinnerMonPics(course, team);
}

#pragma force_active reset

void PokeathlonCourseCeremony_LoadTrainerSprites(PokeathlonCourseData *course) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    ManagedSprite **sprites;
    int i = 0;
    PokeathlonCourseCeremony *cursor = ceremony;
    sprites = ceremony->trainerSprites;

    for (; i < 5; i++) {
        int count = PokeathlonCourse_GetParticipantCount(course);
        if (i == 4) {
            int fileId = PokeathlonCourse_GetTrainerPicFileId(2);
            PokeathlonCourse_LoadTrainerPic(cursor->trainerSprites[0], fileId, 11, ceremony->heapId);
        } else if (i < count) {
            PokeathlonCourse_LoadTeamTrainerPic(sprites, course, (u8)i, 0);
        } else {
            u8 npcTeam = *(u32 *)PokeathlonCourse_GetParticipantData(course, i);
            int fileId = PokeathlonCourse_GetNpcTrainerPicFileId(PokeathlonCourse_GetNpcTeamSpriteId(npcTeam));
            PokeathlonCourse_LoadTrainerPic(cursor->trainerSprites[0], fileId, 0, ceremony->heapId);
        }
        cursor = (PokeathlonCourseCeremony *)((u8 *)cursor + sizeof(ManagedSprite *));
    }
}

// pose 1: standing, 2: cheering (frame 27 for the NPC trainers).
void PokeathlonCourseCeremony_SetTrainerPose(ManagedSprite **sprites, PokeathlonCourseData *course, int team, int pose, enum HeapID heapId) {
    BOOL isNpc;
    int frame;

    isNpc = *(int *)PokeathlonCourse_GetParticipantData(course, team) > 0;
    switch (pose) {
    case 1:
        frame = 0;
        break;
    case 2:
        frame = isNpc ? 11 : 27;
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (isNpc) {
        u8 npcTeam = *(u32 *)PokeathlonCourse_GetParticipantData(course, team);
        int fileId = PokeathlonCourse_GetNpcTrainerPicFileId(PokeathlonCourse_GetNpcTeamSpriteId(npcTeam));
        PokeathlonCourse_LoadTrainerPic(sprites[team], fileId, (u8)frame, heapId);
    } else {
        PokeathlonCourse_LoadTeamTrainerPic(sprites, course, (u8)team, frame);
    }
}

PokeathlonCourseCeremonyScore *PokeathlonCourseCeremony_CreateScores(enum HeapID heapId, ManagedSprite **sprites) {
    PokeathlonCourseCeremonyScore *scores = Heap_Alloc(heapId, sizeof(PokeathlonCourseCeremonyScore) * 4);
    int i;
    int j;

    ManagedSprite **dest;
    int index;

    MI_CpuFill8(scores, 0, sizeof(PokeathlonCourseCeremonyScore) * 4);
    dest = scores->sprites;
    i = 0;
    index = 5;
    for (; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            dest[j] = sprites[index + j];
        }
        dest += sizeof(PokeathlonCourseCeremonyScore) / sizeof(ManagedSprite *);
        index += 4;
    }
    return scores;
}

void PokeathlonCourseCeremony_FreeScores(PokeathlonCourseCeremonyScore *scores) {
    Heap_Free(scores);
}

#pragma force_active on

void PokeathlonCourseCeremony_UpdateScores(PokeathlonCourseCeremonyScore *scores) {
    int i;
    for (i = 0; i < 4; i++) {
        PokeathlonCourseCeremonyScore_Update(&scores[i]);
    }
}

BOOL PokeathlonCourseCeremony_AreScoresCounted(PokeathlonCourseCeremonyScore *scores) {
    int i;
    BOOL result = TRUE;
    for (i = 0; i < 4; i++) {
        if (PokeathlonCourseCeremonyScore_IsCounted(&scores[i]) == FALSE) {
            result = FALSE;
        }
    }
    return result;
}

#pragma force_active reset

void PokeathlonCourseCeremony_AddPoints(PokeathlonCourseCeremonyScore *scores, int team, u32 points) {
    if (points != 0) {
        PokeathlonCourseCeremonyScore_Add(&scores[team], points);
    }
}

u32 PokeathlonCourseCeremony_GetPoints(PokeathlonCourseCeremonyScore *scores, u8 team) {
    return scores[team].points;
}

int PokeathlonCourseCeremony_CompareRanks(const void *a, const void *b) {
    u16 right = *(const u32 *)b;
    u16 left = *(const u32 *)a;

    if (left == right) {
        return 0;
    }
    if (left < right) {
        return 1;
    }
    return -1;
}

// Sorts the teams by points; equal points share the place.
void PokeathlonCourseCeremony_RankTeams(PokeathlonCourseCeremonyScore *scores, PokeathlonCourseCeremonyRank *ranks, const u8 *drawingLots) {
    int i;
    int place = 0;

    for (i = 0; i < 4; i++) {
        ranks[i].team = i;
        ranks[i].points = PokeathlonCourseCeremony_GetPoints(scores, (u8)i);
        ranks[i].drawingLot = drawingLots[i];
    }

    MATH_QSort(ranks, 4, sizeof(PokeathlonCourseCeremonyRank), PokeathlonCourseCeremony_CompareRanks, NULL);

    for (i = 0; i < 4; i++) {
        ranks[i].place = place;
        if (i != 3 && ranks[i].points != ranks[i + 1].points) {
            place = i + 1;
        }
    }
}

void PokeathlonCourseCeremonyScore_Update(PokeathlonCourseCeremonyScore *score) {
    u32 shownPoints;
    u16 hundreds;
    u16 tens;
    u16 ones;
    int i;

    if (score->counting != FALSE) {
        shownPoints = score->shownPoints + 1;
        if (shownPoints > 999) {
            shownPoints = 999;
        }
        score->shownPoints = shownPoints;
        if (!IsSEPlaying(SEQ_SE_GS_THLON02)) {
            PlaySE(SEQ_SE_GS_THLON02);
        }

        hundreds = PokeathlonUtil_GetDecimalDigit(score->shownPoints, 3);
        tens = PokeathlonUtil_GetDecimalDigit(score->shownPoints, 2);
        ones = PokeathlonUtil_GetDecimalDigit(score->shownPoints, 1);
        ManagedSprite_SetAnim(score->sprites[1], hundreds + 1);
        ManagedSprite_SetAnim(score->sprites[2], tens + 1);
        ManagedSprite_SetAnim(score->sprites[3], ones + 1);

        // the plate rises by a pixel every 20 points
        if (score->shownPoints % 20 == 0) {
            for (i = 0; i < 4; i++) {
                ManagedSprite_OffsetPositionXY(score->sprites[i], 0, -1);
            }
        }
        if (score->shownPoints >= score->points) {
            score->counting = FALSE;
        }
    }
}

BOOL PokeathlonCourseCeremonyScore_IsCounted(PokeathlonCourseCeremonyScore *score) {
    return score->counting == FALSE;
}

void PokeathlonCourseCeremonyScore_Add(PokeathlonCourseCeremonyScore *score, u32 points) {
    u32 total = score->points + points;
    if (total > 999) {
        total = 999;
    }
    score->points = total;
    score->counting = TRUE;
}

void PokeathlonCourseConfetti_Spawn(PokeathlonCourseConfettiPiece *piece, SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    ManagedSpriteTemplate template;
    u32 value;
    s32 offset;
    float floatValue;

    if (piece == NULL) {
        GF_AssertFail();
    }
    MI_CpuFill8(piece, 0, sizeof(*piece));

    value = MTRandom() % 5;
    if (value != 0) {
        value = MTRandom() % 5;
        floatValue = (float)(value << 12) + 0.5f;
    } else {
        value = MTRandom() % 5;
        floatValue = (float)(value << 12) - 0.5f;
    }
    offset = (s32)floatValue;
    piece->yVelocity = (offset >> 1) + 0x1800;

    if (MTRandom() & 1) {
        value = MTRandom() & 1;
        floatValue = (float)(value << 12) + 0.5f;
    } else {
        value = MTRandom() & 1;
        floatValue = (float)(value << 12) - 0.5f;
    }
    piece->xVelocity = ((s32)floatValue + 0x19A) >> 1;

    if ((MTRandom() & 0xFF) != 0) {
        value = MTRandom() & 0xFF;
        floatValue = (float)(value << 12) + 0.5f;
    } else {
        value = MTRandom() & 0xFF;
        floatValue = (float)(value << 12) - 0.5f;
    }
    piece->x = (s32)floatValue;

    piece->state.speed = MTRandom() % 5 + 1;
    piece->state.active = TRUE;
    template = gPokeathlonCourseConfettiSpriteTemplate;
    template.x = piece->x >> 12;
    if (!(MTRandom() & 1)) {
        template.animation = 12;
    }
    piece->sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, 0x20C000);
    ManagedSprite_SetAnimateFlag(piece->sprite, TRUE);
}

// Returns TRUE when the piece falls off the screen for the first time (it is then put back on
// top of the screen).
BOOL PokeathlonCourseConfetti_Fall(PokeathlonCourseConfettiPiece *piece) {
    BOOL result = FALSE;
    s64 velocity;
    s64 sine;
    fx32 xVelocity;

    if (piece == NULL) {
        GF_AssertFail();
    }
    xVelocity = piece->xVelocity;
    sine = GF_SinDeg(piece->state.angle * piece->state.speed);
    velocity = sine * xVelocity;
    piece->x += (velocity + 0x800) >> 12;
    piece->y += piece->yVelocity;
    ManagedSprite_SetPositionFxXYWithSubscreenOffset(piece->sprite, piece->x, piece->y, 0x20C000);
    if ((piece->y >> 12) > 224) {
        if (piece->state.landed) {
            piece->state.active = FALSE;
            Sprite_DeleteAndFreeResources(piece->sprite);
            piece->sprite = NULL;
        } else {
            piece->state.landed = TRUE;
            result = TRUE;
        }
    }
    piece->state.angle += MTRandom() & 1;
    return result;
}

// Moves the piece to the top screen.
void PokeathlonCourseConfetti_MoveToTopScreen(PokeathlonCourseConfettiPiece *piece, SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    ManagedSpriteTemplate template = gPokeathlonCourseConfettiTopSpriteTemplate;

    piece->y = 0;
    template.animation = ManagedSprite_GetActiveAnim(piece->sprite) - 11;
    template.x = piece->x >> 12;
    template.y = piece->y >> 12;
    Sprite_DeleteAndFreeResources(piece->sprite);
    piece->sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, 0x20C000);
    ManagedSprite_SetAnimateFlag(piece->sprite, TRUE);
}

PokeathlonCourseConfetti *PokeathlonCourseConfetti_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    PokeathlonCourseConfetti *confetti;

    if (spriteSystem == NULL) {
        GF_AssertFail();
    }
    if (spriteManager == NULL) {
        GF_AssertFail();
    }
    confetti = Heap_Alloc(heapId, sizeof(*confetti));
    MI_CpuFill8(confetti, 0, sizeof(*confetti));
    confetti->heapId = heapId;
    confetti->spriteSystem = spriteSystem;
    confetti->spriteManager = spriteManager;
    confetti->delay = PokeathlonCourseConfetti_GetRandomDelay();
    return confetti;
}

void PokeathlonCourseConfetti_Free(PokeathlonCourseConfetti *confetti) {
    int i;

    if (confetti == NULL) {
        GF_AssertFail();
    }
    for (i = 0; i < 15; i++) {
        if (confetti->pieces[i].sprite != NULL) {
            Sprite_DeleteAndFreeResources(confetti->pieces[i].sprite);
            confetti->pieces[i].sprite = NULL;
        }
    }
    Heap_Free(confetti);
}

void PokeathlonCourseConfetti_Update(PokeathlonCourseConfetti *confetti) {
    int i;
    PokeathlonCourseConfettiPiece *piece;
    PokeathlonCourseConfettiFlags *flags;

    if (confetti == NULL) {
        GF_AssertFail();
    }
    if (confetti->active != FALSE) {
        if (confetti->timer++ > confetti->delay) {
            PokeathlonCourseConfetti_SpawnPieces(confetti, (MTRandom() & 1) + 2);
            confetti->delay = PokeathlonCourseConfetti_GetRandomDelay();
            confetti->timer = 0;
        }
    }
    i = 0;
    flags = &confetti->pieces[0].state;
    piece = confetti->pieces;
    for (; i < 15; i++, flags = (PokeathlonCourseConfettiFlags *)((u8 *)flags + sizeof(PokeathlonCourseConfettiPiece)), piece++) {
        if (flags->active == TRUE && PokeathlonCourseConfetti_Fall(piece)) {
            PokeathlonCourseConfetti_MoveToTopScreen(piece, confetti->spriteSystem, confetti->spriteManager);
        }
    }
}

void PokeathlonCourseConfetti_Start(PokeathlonCourseConfetti *confetti, BOOL active) {
    confetti->active = active;
    GfGfxLoader_GXLoadPal(NARC_a_1_5_4, 12, GF_PAL_LOCATION_MAIN_OBJ, (enum GFPalSlotOffset)0x160, 0x20, confetti->heapId);
}

#pragma force_active on

void PokeathlonCourseConfetti_SpawnPieces(PokeathlonCourseConfetti *confetti, u8 count) {
    int spawned = 0;
    int next;
    vs32 i;
    u32 *flags;
    PokeathlonCourseConfettiPiece *piece;
    if (confetti == NULL) {
        GF_AssertFail();
    }
    i = 0;
    flags = &confetti->pieces[0].state.raw;
    piece = confetti->pieces;
    do {
        if (((*flags << 11) >> 31) == 0) {
            PokeathlonCourseConfetti_Spawn(piece, confetti->spriteSystem, confetti->spriteManager);
            if (++spawned >= count) {
                break;
            }
        }
        next = i + 1;
        flags += sizeof(PokeathlonCourseConfettiPiece) / sizeof(u32);
        piece++;
        i = next;
    } while (next < 15);
}

BOOL PokeathlonCourseCeremony_Init(PokeathlonCourseData *course) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonCourseObjectInfo infos[12];
    int i;
    void *object;
    int mode;

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_135, 0x60000);
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        PokeathlonCourseCeremony_SetBanks();
        ceremony = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonCourseCeremony));
        MI_CpuFill8(ceremony, 0, sizeof(PokeathlonCourseCeremony));
        ceremony->heapId = HEAP_ID_135;
        ceremony->course = PokeathlonCourse_FindCourseId(course);
        ceremony->bgConfig = BgConfig_Alloc(ceremony->heapId);
        ceremony->messageBox = PokeathlonCourseMessageBox_New(ceremony->heapId);
        PokeathlonCourseCeremony_InitSpriteSystem(ceremony);
        ceremony->spriteResources = PokeathlonCourseSprites_CreateResources(ceremony->heapId, 6, 1);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1: {
        PokeathlonCourseObjectsTemplate template = { 0 };
        ceremony->objects = PokeathlonCourseMonObjects_Create(ceremony->heapId, 12, 3, ceremony->spriteResources, SpriteManager_GetSpriteList(ceremony->spriteManager));
        for (i = 0; i < 12; i++) {
            int member = i % 3;
            int team = i / 3;
            PokeathlonCourse_GetMonInfo(course, team, member, &infos[i]);
            template.sizeTypes[i] = PokeathlonCourse_GetSpriteSizeType(PokeathlonCourse_GetPerformance(course, team, member));
        }
        template.loadAtOnce = FALSE;
        template.priority = 3;
        template.vramTransfer = FALSE;
        template.vram = 1;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        PokeathlonCourseMonObjects_Load(ceremony->objects, 12, infos, &template, 1, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 2:
        if (PokeathlonCourseMonObjects_IsLoaded(ceremony->objects)) {
            PokeathlonCourseCeremony_LoadSpriteResources(course);
            PokeathlonCourseCeremony_CreateTrainerSprites(ceremony);
            PokeathlonCourseCeremony_CreateScoreSprites(ceremony);
            PokeathlonCourseCeremony_CreateMonPicSprites(ceremony);
            PokeathlonCourseCeremony_CreateMedalSprites(ceremony);
            ceremony->scores = PokeathlonCourseCeremony_CreateScores(ceremony->heapId, ceremony->sprites);
            ceremony->confetti = PokeathlonCourseConfetti_New(ceremony->heapId, ceremony->spriteSystem, ceremony->spriteManager);
            PokeathlonCourseCeremony_LoadTrainerSprites(course);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 3:
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)ceremony->bgConfig);
        PokeathlonCourse_SetField1F4(course, 1);
        for (i = 0; i < 12; i++) {
            int team;
            int member;
            object = PokeathlonCourseMonObjects_Get(ceremony->objects, (u8)i);
            PokeathlonCourseMonObject_SetDrawFlag(object, TRUE);
            Sprite_GetCellAnim(PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonObject_GetGfx(object)))->animCtrl.currentTime = (i * 3) << FX32_SHIFT;
            PokeathlonCourseMonObject_SetAnimState(object, 1);
            team = i / 3;
            member = i % 3;
            PokeathlonCourseObject_SetGroundPosition(object, 32 + (team << 6), 48 * (member + 1));
        }
        PokeathlonCourseCeremony_InitBgs(ceremony->bgConfig);
        PokeathlonCourseCeremony_LoadBgGfx(ceremony->bgConfig, ceremony->heapId);
        mode = PokeathlonCourse_GetMode(course);
        PokeathlonCourseMessageBox_Init(ceremony->messageBox, ceremony->bgConfig, GF_BG_LYR_SUB_0, mode, PokeathlonCourse_GetSaveData(course));
        WirelessStatusIcon_CreateOnScreenIfConnected(2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        ceremony->state = 0;
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonCourseCeremony_Main(PokeathlonCourseData *course) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    SpriteSystem_DrawSprites(ceremony->spriteManager);
    return TRUE;
}

#pragma force_active on

BOOL PokeathlonCourseCeremony_Exit(PokeathlonCourseData *course) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourseConfetti_Free(ceremony->confetti);
    PokeathlonCourseCeremony_FreeScores(ceremony->scores);
    PokeathlonCourseMessageBox_Free(ceremony->messageBox);
    FreeBgTilemapBuffer(ceremony->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(ceremony->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(ceremony->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(ceremony->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(ceremony->bgConfig);
    PokeathlonCourseMonObjects_Free(ceremony->objects);
    PokeathlonCourseSprites_FreeResources(ceremony->spriteResources);
    WirelessStatusIcon_Destroy();
    PokeathlonCourseCeremony_FreeSprites(ceremony);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    Heap_Destroy(HEAP_ID_135);
    return TRUE;
}

#pragma force_active reset

// Puts the nickname of the team's member-th Pokemon into placeholder field.
void PokeathlonCourseCeremony_BufferMonName(PokeathlonCourseData *course, int team, int member, int field) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonCourseCeremonyMon *mons = (PokeathlonCourseCeremonyMon *)PokeathlonCourse_GetParticipantMons(course, team);
    String *string = String_New(11, ceremony->heapId);
    CopyU16ArrayToString(string, mons[member].nickname);
    BufferString(PokeathlonCourseMessageBox_GetMsgFormat(ceremony->messageBox), field, string, 2, 1, 0);
    String_Delete(string);
}

void PokeathlonCourseCeremony_BufferTeamPoints(PokeathlonCourseData *course, int team, int points) {
    MessageFormat *msgFormat = PokeathlonCourseMessageBox_GetMsgFormat(((PokeathlonCourseCeremony *)PokeathlonCourse_GetHeapAllocPtr4(course))->messageBox);
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(course, team);
    BufferPlayersName(msgFormat, 0, profile);
    PokeathlonCourseCeremony_BufferNumber(msgFormat, points, 3, 1);
}

void PokeathlonCourseCeremony_BufferTeamMon(PokeathlonCourseData *course, int team, int member) {
    MessageFormat *msgFormat = PokeathlonCourseMessageBox_GetMsgFormat(((PokeathlonCourseCeremony *)PokeathlonCourse_GetHeapAllocPtr4(course))->messageBox);
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(course, team);
    BufferPlayersName(msgFormat, 0, profile);
    PokeathlonCourseCeremony_BufferMonName(course, team, member, 1);
}

u32 PokeathlonCourseCeremony_HasNoMissBonus(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (!course) {
        GF_AssertFail();
    }
    if (!copyArea) {
        GF_AssertFail();
    }
    if (!data) {
        GF_AssertFail();
    }
    return MATH_CountPopulation(data->noMissMons);
}

u32 PokeathlonCourseCeremony_IsNoMissMon(PokeathlonCourseData *course, int team, int member) {
    int index = member + team * 3;
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (!course) {
        GF_AssertFail();
    }
    if (!copyArea) {
        GF_AssertFail();
    }
    if (!data) {
        GF_AssertFail();
    }
    if (index >= 12) {
        GF_AssertFail();
    }
    return data->noMissMons & (1 << index);
}

u32 PokeathlonCourseCeremony_IsPointsLeader(PokeathlonCourseData *course, int team, int member) {
    int index = member + team * 3;
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (!course) {
        GF_AssertFail();
    }
    if (!copyArea) {
        GF_AssertFail();
    }
    if (!data) {
        GF_AssertFail();
    }
    if (index >= 12) {
        GF_AssertFail();
    }
    return data->pointsLeaderMons & (1 << index);
}

u32 PokeathlonCourseCeremony_IsEffortPrizeMon(PokeathlonCourseData *course, int team, int member) {
    int index = member + team * 3;
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (!course) {
        GF_AssertFail();
    }
    if (!copyArea) {
        GF_AssertFail();
    }
    if (!data) {
        GF_AssertFail();
    }
    return data->effortPrizeMons & (1 << index);
}

int PokeathlonCourseConfetti_GetRandomDelay(void) {
    return (MTRandom() & 7) + 4;
}

#pragma force_active on

u8 PokeathlonCourseCeremony_GetChallengeBonus(PokeathlonCourseData *course, u8 team) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->challengeBonus[team];
}

u32 PokeathlonCourseCeremony_GetEventPoints(PokeathlonCourseData *course, u8 team, u8 round) {
    u8 event = PokeathlonCourse_GetField3D8_AtIndex(course, round);
    return PokeathlonCourseScoreList_GetEventPoints(PokeathlonCourse_GetGraphicsSystem(course), event, team);
}

#pragma force_active reset

int PokeathlonCourseCeremony_GetNoMissPoints(PokeathlonCourseCeremony *ceremony) {
    return 10;
}

int PokeathlonCourseCeremony_GetPointsLeaderPoints(PokeathlonCourseCeremony *ceremony) {
    return 20;
}

int PokeathlonCourseCeremony_GetEffortPrizePoints(PokeathlonCourseCeremony *ceremony) {
    return 10;
}

u32 PokeathlonCourseCeremony_CountNoMissMons(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return MATH_CountPopulation(data->noMissMons);
}

u32 PokeathlonCourseCeremony_CountPointsLeaders(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return MATH_CountPopulation(data->pointsLeaderMons);
}

u32 PokeathlonCourseCeremony_CountEffortPrizes(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return MATH_CountPopulation(data->effortPrizeMons);
}

void PokeathlonCourseCeremony_GetDrawingLots(PokeathlonCourseData *course, u8 *drawingLots) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    s32 i;
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    for (i = 0; i < 4; i++) {
        drawingLots[i] = data->drawingLots[i];
    }
}

u32 PokeathlonCourseCeremony_GetIntroMsgId(PokeathlonCourseData *course, u32 team) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyOpeningData *data = (PokeathlonCourseCeremonyOpeningData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->intros[team].msgId;
}

u32 PokeathlonCourseCeremony_GetIntroValue(PokeathlonCourseData *course, u32 team) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyOpeningData *data = (PokeathlonCourseCeremonyOpeningData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->intros[team].value;
}

u32 PokeathlonCourseCeremony_GetIntroCheer(PokeathlonCourseData *course, u32 team) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyOpeningData *data = (PokeathlonCourseCeremonyOpeningData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->intros[team].cheer;
}

// Host: the introduction line of every team (from the player's records, the other players'
// records or the NPC team's line).
void PokeathlonCourseCeremony_ChooseIntros(PokeathlonCourseData *course) {
    PokeathlonCourseCeremonyIntro *intro = (PokeathlonCourseCeremonyIntro *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
    s32 i;
    if (PokeathlonCourse_GetMode(course) == 0) {
        for (i = 0; i < 4; i++, intro++) {
            if (i == PokeathlonCourse_GetNetId(course)) {
                PokeathlonCourseCeremony_ChooseOwnIntro(intro, course);
            } else {
                PokeathlonCourseCeremony_ChooseNpcIntro(intro, course, (u8)i);
            }
        }
    } else {
        for (i = 0; i < 4; i++, intro++) {
            if (PokeathlonCourse_GetParticipantCount(course) > i) {
                PokeathlonCourseCeremony_ChooseLinkIntro(intro, course, (u8)i);
            } else {
                PokeathlonCourseCeremony_ChooseNpcIntro(intro, course, (u8)i);
            }
        }
    }
}

// Challenge Bonus: 75 points minus the summed performance of the team's Pokemon.
u32 PokeathlonCourseCeremony_CalcChallengeBonus(PokeathlonCourseData *course, u8 team) {
    u8 performance[20];
    NARC *narc;
    PokeathlonCourseCeremonyMon *mon;
    s32 bonus = 75;
    s32 i;
    narc = NARC_New(NARC_poketool_personal_performance, PokeathlonCourse_GetHeapID(course));
    mon = (PokeathlonCourseCeremonyMon *)PokeathlonCourse_GetParticipantMons(course, team);
    for (i = 0; i < 3; i++, mon++) {
        NARC_ReadWholeMember(narc, PokeathlonCourse_GetPerformanceFileId(mon->species, mon->form), performance);
        bonus -= performance[0] + 1;
        bonus -= performance[1] + 1;
        bonus -= performance[2] + 1;
        bonus -= performance[3] + 1;
        bonus -= performance[4] + 1;
    }
    if (bonus > 75) {
        bonus = 75;
    } else if (bonus < 0) {
        bonus = 0;
    }
    NARC_Delete(narc);
    return bonus;
}

// The statistic of the prize for effort: the first, from a random one on, that some Pokemon has
// (4: none).
u32 PokeathlonCourseCeremony_ChooseEffortPrizeKind(PokeathlonCourseData *course) {
    s32 tries;
    s32 team;
    {
        s32 found;
        s32 kind;

        found = 0;
        kind = MTRandom() & 3;
        for (tries = 0; tries < 4; tries++) {
            for (team = 0; team < 4; team++) {
                s32 member;

                for (member = 0; member < 3; member++) {
                    if (PokeathlonCourseCeremony_GetMonStat(course, kind, team, member)) {
                        found = 1;
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }
            if (found) {
                break;
            }
            kind = (kind + 1) % 4;
        }
        if (!found) {
            kind = 4;
        }
        return kind;
    }
}

// Host: the bonuses and prizes of the closing ceremony.
#pragma force_active on

void PokeathlonCourseCeremony_ChooseAwards(PokeathlonCourseData *course) {
    int team;
    int i;
    int maxEffort;
    int kind;
    int effort;
    int value;
    int bit;
    PokeathlonCourseMonEventStats *stats;
    u32 maxPoints;
    PokeathlonCourseMonEventStats *teamStats;
    PokeathlonCourseCeremonyClosingData *data;
    int member;
    int index;

    stats = (PokeathlonCourseMonEventStats *)PokeathlonCourse_GetFieldData(course);
    data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
    data->noMissMons = 0;
    data->effortPrizeMons = 0;
    data->pointsLeaderMons = 0;
    for (i = 0; i < 4; i++) {
        data->challengeBonus[i] = PokeathlonCourseCeremony_CalcChallengeBonus(course, i);
    }
    data->effortPrizeKind = PokeathlonCourseCeremony_ChooseEffortPrizeKind(course);
    maxEffort = 0;
    maxPoints = 0;
    kind = data->effortPrizeKind;
    for (team = 0, teamStats = stats; team < 4; team++) {
        for (member = 0; member < 3; member++) {
            if (kind != 4) {
                effort = PokeathlonCourseCeremony_GetMonStat(course, kind, team, member);
                if (maxEffort < effort) {
                    maxEffort = effort;
                }
            }
            if (maxPoints < teamStats[member].points) {
                maxPoints = teamStats[member].points;
            }
        }
        teamStats += 3;
    }
    for (i = 0, bit = 0; i < 4; i++) {
        for (member = 0; member < 3; member++) {
            u8 mon = member + bit;
            if (stats[member].failed == 0 && stats[member].fellDown == 0 && stats[member].selfImpeded == 0) {
                data->noMissMons += 1 << mon;
            }
            if (maxPoints == stats[member].points) {
                data->pointsLeaderMons += 1 << mon;
            }
            if (kind != 4) {
                value = PokeathlonCourseCeremony_GetMonStat(course, kind, i, member);
                if (value > maxEffort) {
                    GF_AssertFail();
                }
                if (maxEffort == value) {
                    data->effortPrizeMons += 1 << mon;
                }
            }
        }
        bit += 3;
        stats += 3;
    }
    data->maxEffort = maxEffort;
    index = MTRandom() & 3;
    for (i = 0; i < 4; i++) {
        data->drawingLots[index] = i;
        index = (index + 1) % 4;
    }
}

#pragma force_active reset

// The which-th (0, 1) medal for winning course; course 10 has the first course's medal only.
u32 PokeathlonCourseCeremony_GetCourseMedal(u32 course, u32 which) {
    switch (course) {
    case 10:
        return 0;
    case 5:
        return (u8)(which ? 4 : 0);
    case 6:
        return (u8)(which ? 2 : 4);
    case 7:
        return (u8)(which ? 1 : 2);
    case 8:
        return (u8)(which ? 3 : 1);
    case 9:
        return (u8)(which ? 0 : 3);
    default:
        return course;
    }
}

u8 PokeathlonCourseCeremony_GetEffortPrizeKind(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->effortPrizeKind;
}

// Statistic kind (0 jumped, 1 hit, 2 failed, 3 dashed) of a Pokemon.
u32 PokeathlonCourseCeremony_GetMonStat(PokeathlonCourseData *course, u8 kind, u8 team, u8 member) {
    PokeathlonCourseMonEventStats(*stats)[3] = (PokeathlonCourseMonEventStats(*)[3])PokeathlonCourse_GetFieldData(course);
    if (team >= 4) {
        GF_AssertFail();
    }
    if (member >= 3) {
        GF_AssertFail();
    }
    switch (kind) {
    case 0:
        return stats[team][member].jumped;
    case 1:
        return stats[team][member].hit;
    case 2:
        return stats[team][member].failed;
    case 3:
        return stats[team][member].dashed;
    default:
        GF_AssertFail();
        GF_AssertFail();
        return 0;
    }
}

// Next Pokemon; TRUE after the last one.
BOOL PokeathlonCourseCeremony_NextMon(PokeathlonCourseCeremony *ceremony) {
    ceremony->member++;
    if (ceremony->member >= 3) {
        ceremony->member = 0;
        ceremony->team++;
    }
    return ceremony->team == 4;
}

// Gives the next Pokemon that passes test its points: buffers its name, plays its cry.
#pragma force_active on

BOOL PokeathlonCourseCeremony_AwardNextMon(PokeathlonCourseData *course, PokeathlonCourseCeremonyAwardTest test, int points) {
    PokeathlonCourseCeremony *ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    int index;
    u16 *monForm;
    do {
        if (test(course, ceremony->team, ceremony->member)) {
            break;
        }
    } while (PokeathlonCourseCeremony_NextMon(ceremony) == FALSE);
    if (ceremony->team < 4) {
        index = ceremony->team * 3 + ceremony->member;
        ceremony->awardCounts[ceremony->team]++;
        ceremony->counter++;
        PokeathlonCourseCeremony_BufferTeamMon(course, ceremony->team, ceremony->member);
        PokeathlonCourseCeremony_AddPoints(ceremony->scores, ceremony->team, (u16)points);
        PokeathlonCourseCeremony_StartCry(ceremony, (u8)index);
        monForm = PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(PokeathlonCourseMonObjects_Get(ceremony->objects, (u8)index)));
        Chatot_SetForceDefaultCry(1);
        PlayCry(monForm[0], (u8)monForm[1]);
        PokeathlonCourseCeremony_NextMon(ceremony);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// Keeps the result of the ceremony for the course statistics.
#pragma force_active on

void PokeathlonCourseCeremony_SaveResult(PokeathlonCourseData *course, PokeathlonCourseCeremonyScore *scores) {
    int own;
    PokeathlonCourseCeremony *ceremony;
    PokeathlonCourseCeremonyResult *result;
    int i;
    int fewest;
    int place;
    own = PokeathlonCourse_GetNetId(course);
    ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    result = (PokeathlonCourseCeremonyResult *)PokeathlonCourse_GetFieldData(course);
    if (ceremony->ranks[0].team == own) {
        result->won = TRUE;
    }
    i = 0;
    fewest = 999;
    for (; i < 4; i++) {
        int points = PokeathlonCourseCeremony_GetPoints(scores, (u8)i);
        if (fewest >= points) {
            fewest = points;
        }
    }
    {
        int points = PokeathlonCourseCeremony_GetPoints(scores, (u8)own);
        if (points == fewest) {
            result->fewestPoints = TRUE;
        }
    }
    result->awards = ceremony->awardCounts[own];
    result->points = PokeathlonCourseCeremony_GetPoints(ceremony->scores, (u8)own);
    place = PokeathlonCourseCeremony_FindPlace(ceremony->ranks, (u8)own);
    if (place == -1) {
        GF_AssertFail();
    }
    result->place = (u16)place;
}

#pragma force_active reset

void PokeathlonCourseCeremony_BufferNumber(MessageFormat *msgFormat, s32 value, u32 digits, u32 field) {
    BufferIntegerAsString(msgFormat, field, value, digits, PRINTING_MODE_LEFT_ALIGN, TRUE);
}

// Cheer level 0..12 of a value (thresholds gPokeathlonCourseCeremonyCheerThresholds).
u32 PokeathlonCourseCeremony_GetCheerLevel(s32 value) {
    s32 i;
    for (i = 0; i < 12; i++) {
        if (gPokeathlonCourseCeremonyCheerThresholds[i] > value) {
            return i;
        }
    }
    return i;
}

// First of the four introduction lines (msg 309 #32 + 4 * statistic) about a statistic.
int PokeathlonCourseCeremony_GetIntroMsgBase(int statistic) {
    return statistic * 4 + 32;
}

// The 29 statistics the introduction lines are about.
void PokeathlonCourseCeremony_GetIntroStats(PokeathlonStatistics *dest, const PokeathlonStatistics *src) {
    int i;
    u32 sum = 0;

    for (i = 0; i < 10; i++) {
        sum += src->eventFirstPlaces[i];
    }
    dest->playTime = src->coursesJoined;
    dest->coursesJoined = src->courseFirstPlaces;
    dest->courseFirstPlaces = src->courseLastPlaces;
    dest->courseLastPlaces = sum;
    dest->bonusesEarned = src->eventLastPlaces;
    dest->monInstructions = src->bonusesEarned;
    dest->monFailures = src->monInstructions;
    dest->monJumps = src->monFailures;
    dest->monPointsEarned = src->monJumps;
    dest->monTackles = src->monPointsEarned;
    dest->monFalls = src->monTackles;
    dest->monDashes = src->monFalls;
    dest->monSwitches = src->monDashes;
    dest->monSelfImpedes = src->monSwitches;
    dest->linkCoursesJoined = src->linkCoursesJoined;
    dest->linkCourseFirstPlaces = src->linkCourseFirstPlaces;
    dest->linkCourseLastPlaces = src->linkCourseLastPlaces;
    for (i = 0; i < 10; i++) {
        dest->eventFirstPlaces[i] = src->eventFirstPlaces[i];
    }
    dest->eventLastPlaces = src->monSelfImpedes;
    dest->unk70 = src->unk70;
}

// Weighs the statistics (value / gPokeathlonCourseCeremonyIntroStatWeights[i], plus 0..2 at random).
void PokeathlonCourseCeremony_WeighIntroStats(u8 *weights, const u32 *stats) {
    int i;
    for (i = 0; i < 29; i++) {
        if (gPokeathlonCourseCeremonyIntroStatWeights[i] == 0) {
            GF_AssertFail();
        }
        weights[i] = MTRandom() % 3;
        if (stats[i] != 0) {
            weights[i] = weights[i] + stats[i] / gPokeathlonCourseCeremonyIntroStatWeights[i];
        }
    }
}

// One of the statistics with the highest weight; *level 0..3 from the weight.
int PokeathlonCourseCeremony_PickIntroStat(const u8 *weights, u8 *level) {
    u8 candidates[29] = { 0 };
    u32 max = 0;
    int count = 0;
    int i;
    u8 *candidate;

    for (i = 0; i < 29; i++) {
        if (max < weights[i]) {
            max = weights[i];
        }
    }
    i = 0;
    candidate = candidates;
    while (i < 29) {
        if (max == weights[i]) {
            *candidate++ = i;
            count++;
        }
        i++;
    }
    for (i = 0; i < 4; i++) {
        if ((u32)max >= gPokeathlonCourseCeremonyIntroStatThresholds[i]) {
            *level = 3 - i;
            break;
        }
    }
    return candidates[MTRandom() % count];
}

void PokeathlonCourseCeremony_ChooseOwnIntro(PokeathlonCourseCeremonyIntro *intro, PokeathlonCourseData *course) {
    PokeathlonStatistics stats = { 0 };
    u8 weights[29] = { 0 };
    u8 level;
    PokeathlonStatistics *records;
    u8 statistic;

    records = PokeathlonSave_GetStatistics2(Save_Pokeathlon_Get(PokeathlonCourse_GetSaveData(course)));
    PokeathlonCourseCeremony_GetIntroStats(&stats, records);
    PokeathlonCourseCeremony_WeighIntroStats(weights, (u32 *)&stats);
    statistic = PokeathlonCourseCeremony_PickIntroStat(weights, &level);
    intro->msgId = level + PokeathlonCourseCeremony_GetIntroMsgBase(statistic);
    intro->value = ((u32 *)&stats)[statistic];
    intro->cheer = PokeathlonCourseCeremony_GetCheerLevel(records->unk70);
}

void PokeathlonCourseCeremony_ChooseLinkIntro(PokeathlonCourseCeremonyIntro *intro, PokeathlonCourseData *course, int team) {
    PokeathlonStatistics stats = { 0 };
    u8 weights[29] = { 0 };
    u8 level;
    PokeathlonStatistics *records;
    u8 statistic;

    records = (PokeathlonStatistics *)PokeathlonCourse_GetField974_AtIndex(course, team);
    PokeathlonCourseCeremony_GetIntroStats(&stats, records);
    PokeathlonCourseCeremony_WeighIntroStats(weights, (u32 *)&stats);
    statistic = PokeathlonCourseCeremony_PickIntroStat(weights, &level);
    intro->msgId = level + PokeathlonCourseCeremony_GetIntroMsgBase(statistic);
    intro->value = ((u32 *)&stats)[statistic];
    intro->cheer = PokeathlonCourseCeremony_GetCheerLevel(records->unk70);
}

typedef struct NpcIntroLocals {
    volatile int team;
    PokeathlonParticipantData *volatile participant;
} NpcIntroLocals;

// NPC team: its own line (msg 311); the cheer from its Pokemon's performance.
#pragma force_active on

void PokeathlonCourseCeremony_ChooseNpcIntro(PokeathlonCourseCeremonyIntro *intro, PokeathlonCourseData *course, int team) {
    NpcIntroLocals locals;
    locals.team = team;
    int cheer;
    int i;
    u32 total = 0;
    locals.participant = PokeathlonCourse_GetParticipantData(course, locals.team);
    for (i = 0; i < 3; i++) {
        u8 *stats = PokeathlonCourse_GetMonStats(course, locals.team, i);
        total += stats[2] + stats[0] + stats[1] + stats[3] + stats[4] + 5;
    }
    cheer = (int)total / 3 - 8;
    if (cheer > 12) {
        cheer = 12;
    } else if (cheer < 0) {
        cheer = 0;
    }
    intro->msgId = PokeathlonCourse_GetNpcTeamMsgId(course, (u8) * (u32 *)locals.participant);
    intro->value = 0;
    intro->cheer = cheer;
}

#pragma force_active reset

// Place of the team, -1 if it is not ranked.
int PokeathlonCourseCeremony_FindPlace(const PokeathlonCourseCeremonyRank *ranks, u8 team) {
    int i = 0;
    const PokeathlonCourseCeremonyRank *rank = ranks;
    while (i < 4) {
        if (team == rank->team) {
            return ranks[i].place;
        }
        i++;
        rank++;
    }
    return -1;
}

// Tied first places: the team with the highest drawing lot wins.
void PokeathlonCourseCeremony_BreakTie(PokeathlonCourseCeremonyRank *ranks) {
    s32 i;
    u32 max = 0;
    s32 winner;
    PokeathlonCourseCeremonyRank *rank;

    for (i = 0, rank = ranks; i < 4; i++, rank++) {
        if (rank->place == 0) {
            rank->place++;
            if (rank->drawingLot > max) {
                max = rank->drawingLot;
                winner = i;
            }
        }
    }
    ranks[winner].place--;
    ranks[winner].points++;
    MATH_QSort(ranks, 4, 4, PokeathlonCourseCeremony_CompareRanks, NULL);
    ranks[0].points--;
}

// Link: the host waits until every player's text is as far as its own. Returns TRUE while the
// players have to wait.
static inline BOOL SyncMessages(PokeathlonCourseData *course, BOOL closing) {
    u32 allFinished;
    u32 waiting;
    u8 *received;
    u8 *sent;
    u8 *copyArea;
    PokeathlonCourseCeremony *ceremony;
    PokeathlonCourseCeremonySync *sync;
    u8 messageCount;
    u8 netId;
    s32 i;

    ceremony = PokeathlonCourse_GetHeapAllocPtr4(course);
    copyArea = PokeathlonCourse_GetDataCopyArea(course);
    sent = PokeathlonCourse_ToBytes(copyArea + 0x28);
    received = PokeathlonCourse_ToBytes(copyArea + 0xF0);
    netId = (u8)PokeathlonCourse_GetNetId(course);
    messageCount = ceremony->messageCount;
    waiting = 0;

    if (netId == 0) {
        allFinished = 1;
        if (closing) {
            ((PokeathlonCourseCeremonyClosingData *)sent)->messageCount = messageCount;
        } else {
            ((PokeathlonCourseCeremonyOpeningData *)sent)->messageCount = messageCount;
        }
        {
            u8 *dest = PokeathlonCourse_ToBytes(copyArea + 0x50);
            u8 *src = PokeathlonCourse_ToBytes(copyArea);
            u32 count = 0x24;

            do {
                *dest++ = *src++;
            } while (--count != 0);
        }
        for (i = 0; i < PokeathlonCourse_GetParticipantCount(course); i++) {
            sync = (PokeathlonCourseCeremonySync *)PokeathlonCourse_ToBytes(copyArea + 0x50 + i * 0x28);
            if (sync->messageCount < messageCount || sync->printing == 1) {
                waiting = 1;
            }
            if (sync->finished == 0) {
                allFinished = 0;
            }
        }
        if (closing) {
            if (allFinished != 0) {
                ((PokeathlonCourseCeremonyClosingData *)sent)->allFinished = 1;
            }
            ((PokeathlonCourseCeremonyClosingData *)sent)->waiting = waiting;
        } else {
            if (allFinished != 0) {
                ((PokeathlonCourseCeremonyOpeningData *)sent)->allFinished = 1;
            }
            ((PokeathlonCourseCeremonyOpeningData *)sent)->waiting = waiting;
        }
    }
    sync = (PokeathlonCourseCeremonySync *)PokeathlonCourse_ToBytes(copyArea);
    sync->messageCount = messageCount;
    sync->printing = PokeathlonCourseMessageBox_IsBusy(ceremony->messageBox);
    sync->finished = ceremony->finished;
    if (closing) {
        if (((PokeathlonCourseCeremonyClosingData *)received)->waiting != 0 && messageCount >= ((PokeathlonCourseCeremonyClosingData *)received)->messageCount) {
            return TRUE;
        }
    } else {
        if (((PokeathlonCourseCeremonyOpeningData *)received)->waiting != 0 && messageCount >= ((PokeathlonCourseCeremonyOpeningData *)received)->messageCount) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL PokeathlonCourseCeremony_SyncOpening(PokeathlonCourseData *course) {
    return SyncMessages(course, FALSE);
}

BOOL PokeathlonCourseCeremony_SyncClosing(PokeathlonCourseData *course) {
    return SyncMessages(course, TRUE);
}
