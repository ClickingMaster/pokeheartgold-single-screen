#include "pokeathlon/pokeathlon_event_points.h"

#include "global.h"

#include "assert.h"
#include "math_util.h"

// The course points (at most 200) every participant gets for an event: events 0-8 convert each participant's
// event score (gPokeathlonEventScorePointsFuncs[event]), event 9 (Goal Roll) gives points by rank.

// PokeathlonCourseScoreList_GetRanked entries (pokeathlon_course_internal.h)
void *PokeathlonCourseScoreList_GetRanked(void *list, u8 index);

typedef u32 (*PokeathlonEventPointsFunc)(u32 score);
extern void (*const gPokeathlonEventPointsCalcFuncs[])(void *list, int event);
extern const PokeathlonEventPointsFunc gPokeathlonEventScorePointsFuncs[];

#pragma force_active on

void PokeathlonEvent_CalcPoints(void *list, int event) {
    gPokeathlonEventPointsCalcFuncs[event](list, event);
}

#pragma force_active reset

void PokeathlonEvent_CalcPointsFromScores(void *list, int event) {
    PokeathlonEventPointsFunc func = gPokeathlonEventScorePointsFuncs[event];
    u8 i;
    if (func != NULL) {
        for (i = 0; i < 4; i++) {
            PokeathlonEventPointsEntry *entry = PokeathlonCourseScoreList_GetRanked(list, i);
            entry->points = (u16)func((u16)entry->score);
        }
    } else {
        GF_AssertFail();
    }
}

void PokeathlonEvent_CalcPointsFromRanks(void *list, int event) {
    if (gPokeathlonEventScorePointsFuncs[event] == NULL) {
        PokeathlonEvent_CalcRankPoints(list);
    } else {
        GF_AssertFail();
    }
}

#pragma force_active on

// 30 points at 0, approaching 150.
float PokeathlonEvent_Saturate(u32 score, float rate) {
    return (float)(150.0 - 120.0 / (1.0 + (float)score * rate));
}

#pragma force_active reset

u8 PokeathlonEvent_ClampPoints(float points) {
    u32 n = (u32)points;
    if ((double)n > 200.0) {
        n = 200;
    }
    return (u8)n;
}

#pragma force_active on

// Event 0 (Hurdle Dash): the score is the time in frames.
u8 PokeathlonEvent_HurdleDashPoints(u32 frames) {
    return PokeathlonEvent_ClampPoints((float)(11500.0 / (double)((float)frames / 30.0f)));
}

#pragma force_active reset

// Event 1 (Pennant Capture)
u8 PokeathlonEvent_PennantCapturePoints(int score) {
    return PokeathlonEvent_ClampPoints((float)(score * 3));
}

// Event 2 (Circle Push)
u8 PokeathlonEvent_CirclePushPoints(int score) {
    return PokeathlonEvent_ClampPoints((float)(score * 3));
}

// Event 3 (Block Smash)
u8 PokeathlonEvent_BlockSmashPoints(u32 score) {
    return PokeathlonEvent_ClampPoints((float)score);
}

// Event 4 (Disc Catch). The score goes to PokeathlonEvent_Saturate as a float (through a float, float type).
u8 PokeathlonEvent_DiscCatchPoints(float score) {
    return PokeathlonEvent_ClampPoints(((float (*)(float, float))PokeathlonEvent_Saturate)(score, 0.08f));
}

// Event 5 (Lamp Jump)
u8 PokeathlonEvent_LampJumpPoints(u32 score) {
    return PokeathlonEvent_ClampPoints((float)((double)(float)score / 3.5));
}

u8 PokeathlonEvent_RelayRunPoints(u32 score) {
    return PokeathlonEvent_ClampPoints((float)(((double)(float)score / 1024.0) * 10.0));
}

// Event 7 (Ring Drop)
u8 PokeathlonEvent_RingDropPoints(u32 score) {
    double points = (float)score;
    points *= 1.5;
    return PokeathlonEvent_ClampPoints((float)points);
}

u8 PokeathlonEvent_SnowThrowPoints(int score) {
    return PokeathlonEvent_ClampPoints((float)(score * 3));
}
