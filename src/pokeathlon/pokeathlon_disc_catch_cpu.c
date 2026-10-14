#include "global.h"

#include "pokeathlon/pokeathlon_disc_catch_play.h"

#include "assert.h"
#include "heap.h"
#include "math_util.h"

extern const u8 gPokeathlonDiscCatchCpuHomeRowStarts[];                                                  // per member: first home row (tiles) of its band ...
extern const u8 ov96_0221D1B1[];                                                  // ... and the band's rows
extern const int gPokeathlonDiscCatchCpuJumpChances[DISC_CATCH_TEAM_SIZE];                             // per member: chance (%) to jump at a disc in reach
extern const s16 gPokeathlonDiscCatchCpuHomePositions[DISC_CATCH_PARTICIPANTS][DISC_CATCH_TEAM_SIZE][2]; // home x

static inline int GetParticipantCount(PokeathlonCourseData *course) {
    return PokeathlonCourse_GetParticipantCount(course);
}

// The CPU steers the participants after the players.
#pragma force_active on

PokeathlonDiscCatchCpu *PokeathlonDiscCatchCpu_New(PokeathlonCourseData *course, enum HeapID heapId) {
    int i, participant;
    PokeathlonDiscCatchCpu *cpu;
    participant = GetParticipantCount(course);
    cpu = Heap_Alloc(heapId, sizeof(PokeathlonDiscCatchCpu));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonDiscCatchCpu));
    cpu->heapId = heapId;
    cpu->course = course;
    {
        i = 0;
        cpu->count = DISC_CATCH_PARTICIPANTS - participant;
        for (; i < cpu->count; i++) {
            PokeathlonDiscCatchCpuTeam_Init(cpu->teams[i], (u8)participant, course);
            participant++;
        }
    }
    return cpu;
}

#pragma force_active reset

void PokeathlonDiscCatchCpu_Delete(PokeathlonDiscCatchCpu *cpu) {
    Heap_Free(cpu);
}

// NB: each CPU team is compared with the score of team 0, 1, 2 (its own index among the CPU teams),
// not with its own score.
#pragma opt_lifetimes off
#pragma opt_common_subs off
#pragma force_active on

void PokeathlonDiscCatchCpu_Update(PokeathlonDiscCatchCpu *cpu) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(cpu->course);
    int total = 0;
    u32 i = 0;
    for (; (s32)i < DISC_CATCH_PARTICIPANTS; i++) {
        total += PokeathlonDiscCatch_GetTeamScore(work, (u8)i);
    }
    int j = 0;
    if (j < cpu->count) {
        u16 average;
        i = (u32)cpu->teams[0]; // the counter is reused as the team pointer (the original register use)
        average = total / DISC_CATCH_PARTICIPANTS;
        do {
            int score = PokeathlonDiscCatch_GetTeamScore(work, (u8)j);
            PokeathlonDiscCatchCpuTeam_Update((PokeathlonDiscCatchCpuMon *)i, average, score);
            j++;
            i += sizeof(cpu->teams[0]);
        } while (j < cpu->count);
    }
}

#pragma force_active reset
#pragma opt_lifetimes reset
#pragma opt_common_subs reset

void PokeathlonDiscCatchCpuTeam_Init(PokeathlonDiscCatchCpuMon *mons, int participant, PokeathlonCourseData *course) {
    int i;
    int index;
    MI_CpuFill8(mons, 0, sizeof(PokeathlonDiscCatchCpuMon) * DISC_CATCH_TEAM_SIZE);
    for (i = 0, index = participant * DISC_CATCH_TEAM_SIZE; i < DISC_CATCH_TEAM_SIZE; i++) {
        PokeathlonDiscCatchCpuMon_Init(&mons[i], course, index);
        index++;
    }
}

void PokeathlonDiscCatchCpuTeam_Update(PokeathlonDiscCatchCpuMon *mons, u16 average, u16 score) {
    int i;
    for (i = 0; i < DISC_CATCH_TEAM_SIZE; i++) {
        PokeathlonDiscCatchCpuMon_Update(&mons[i], average, score);
    }
}

// A new home spot: a random row of the member's band and the team's column, give or take two tiles.
void PokeathlonDiscCatchCpuMon_PickHome(PokeathlonDiscCatchCpuMon *mon) {
    int band = mon->member * 2;
    mon->targetY = gPokeathlonDiscCatchCpuHomeRowStarts[band] + MTRandom() % ov96_0221D1B1[band];
    mon->targetY = mon->targetY * 8;
    mon->homeX = gPokeathlonDiscCatchCpuHomePositions[mon->participant][mon->member][0];
    mon->homeX = mon->homeX + (MTRandom() % 5 - 2) * 8;
    mon->chasing = FALSE;
    mon->state = 0;
}

void PokeathlonDiscCatchCpuMon_Init(PokeathlonDiscCatchCpuMon *mon, PokeathlonCourseData *course, u8 index) {
    u8 participant = index / DISC_CATCH_TEAM_SIZE;
    u8 member = index % DISC_CATCH_TEAM_SIZE;
    mon->course = course;
    mon->participant = participant;
    mon->member = member;
    mon->thinkTimer = participant * 2 + member * 8;
    mon->homeTimer = participant * 2 + member * 8;
    PokeathlonDiscCatchCpuMon_PickHome(mon);
}

// Every 20 frames: an idle Pokemon goes home or after a disc coming down near it; a chasing one
// follows its disc along the home row until the disc is gone.
void PokeathlonDiscCatchCpuMon_Think(PokeathlonDiscCatchCpuMon *mon) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(mon->course);
    PokeathlonDiscCatchDisc *disc;
    int i;
    PokeathlonDiscCatch_CountTeamsAhead(mon->course, (u8)mon->participant);
    switch (mon->state) {
    case 0:
        GF_ASSERT(mon->chasing == FALSE);
        mon->targetX = mon->homeX;
        for (i = 0; i < DISC_CATCH_DISCS; i++) {
            if (PokeathlonDiscCatch_GetDisc(work, (u8)i)->active) {
                if (PokeathlonDiscCatchCpuMon_IsNearDisc(mon, i, 56)) {
                    mon->disc = i;
                    mon->chasing = TRUE;
                    mon->state = 1;
                }
            }
        }
        break;
    case 1:
        GF_ASSERT(mon->chasing == TRUE);
        if (mon->chasing) {
            disc = PokeathlonDiscCatch_GetDisc(work, (u8)mon->disc);
            if (disc->active) {
                mon->targetX = PokeathlonDiscCatch_GetDiscX((s16)(mon->targetY + 288), disc->path);
            } else {
                mon->chasing = FALSE;
                mon->state = 0;
            }
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    PokeathlonDiscCatch_SetMonTarget(work, &mon->targetX, (u8)mon->participant, (u8)mon->member);
}

// Each frame: a new home every 90 frames, a new decision every 20; jumps at its disc once in reach
// (by the member's chance; never while the team leads the average by more than 20).
void PokeathlonDiscCatchCpuMon_Update(PokeathlonDiscCatchCpuMon *mon, u16 average, u16 score) {
    s16 position[2];
    void *radius;
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(mon->course);
    int roll;
    int chance;
    if (PokeathlonDiscCatch_IsMonInPlay(work, (u8)mon->participant, (u8)mon->member) == FALSE) {
        return;
    }
    mon->homeTimer--;
    if (mon->homeTimer == 0) {
        PokeathlonDiscCatchCpuMon_PickHome(mon);
        mon->homeTimer = 90;
    }
    mon->thinkTimer--;
    if (mon->thinkTimer == 0) {
        PokeathlonDiscCatchCpuMon_Think(mon);
        mon->thinkTimer = 20;
    }
    if (mon->chasing == FALSE) {
        return;
    }
    if (!PokeathlonDiscCatchCpuMon_IsNearDisc(mon, mon->disc, 16)) {
        return;
    }
    roll = MTRandom() % 100;
    PokeathlonDiscCatch_GetMonPosition(work, (u8)mon->participant, (u8)mon->member, position, &radius);
    if (average + 20 < score) {
        chance = 0;
    } else {
        chance = gPokeathlonDiscCatchCpuJumpChances[mon->member];
    }
    if (roll < chance) {
        PokeathlonDiscCatch_MakeMonJump(work, (u8)mon->participant, (u8)mon->member);
    }
}

BOOL PokeathlonDiscCatchCpuMon_IsNearDisc(PokeathlonDiscCatchCpuMon *mon, int index, int reach) {
    s16 position[2];
    void *radius;
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(mon->course);
    PokeathlonDiscCatchDisc *disc = PokeathlonDiscCatch_GetDisc(work, (u8)index);
    if (disc->progress < 288) {
        return FALSE;
    }
    PokeathlonDiscCatch_GetMonPosition(work, (u8)mon->participant, (u8)mon->member, position, &radius);
    return PokeathlonDiscCatch_CirclesOverlap(position[0], position[1], (int)radius, disc->x, disc->progress - 288, reach);
}

BOOL PokeathlonDiscCatch_CirclesOverlap(s16 x0, s16 y0, int radius0, s16 x1, s16 y1, int radius1) {
    if ((radius0 + radius1) * (radius0 + radius1) >= DISC_CATCH_ABS(x0 - x1) * DISC_CATCH_ABS(x0 - x1) + DISC_CATCH_ABS(y0 - y1) * DISC_CATCH_ABS(y0 - y1)) {
        return TRUE;
    }
    return FALSE;
}
