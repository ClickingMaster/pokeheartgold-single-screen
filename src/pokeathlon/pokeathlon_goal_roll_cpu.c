#include "global.h"

#include "pokeathlon/pokeathlon_goal_roll.h"

#include "assert.h"
#include "bg_window.h"
#include "heap.h"
#include "math_util.h"
#include "sprite_system.h"
#include "unk_02009D48.h"

// Course helpers and the sprite pool (OVY_96, not yet tidied).
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int event, int value);
s32 PokeathlonCourseObject_GetCollisionRadius(void *object);
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonGoalRollSprite *sprite, BOOL shown, BOOL show);

// Per goal (indexed by participant - 1 or by 3 - quadrant): three guard points (x, y) in front of it.
extern const u16 ov96_0221D678[][3][2];
extern const u16 ov96_0221D67A[][3][2];
extern const u16 ov96_0221D67C[][3][2];
extern const u16 ov96_0221D680[][3][2];

// The timer on the top screen: seconds left; "Second Wind!" from the last 900 frames on.
void PokeathlonGoalRollDisplay_UpdateTimer(PokeathlonGoalRollDisplay *display, s32 timer) {
    u8 seconds;

    if (timer <= 900) {
        Sprite_SetDrawFlag(display->timerDigits[0], FALSE);
        Sprite_SetDrawFlag(display->timerDigits[1], FALSE);
        PokeathlonCourseSpritePoolSprite_Show(display->timerLabelSprite, TRUE, FALSE);
        if (display->secondWindShown == FALSE) {
            PokeathlonGoalRollDisplay_PrintSecondWind(display);
            display->secondWindShown = TRUE;
        }
    } else {
        seconds = (u8)(timer / 30);
        Sprite_SetAnimCtrlSeq(display->timerDigits[0], (u8)(seconds / 10) + 1);
        Sprite_SetAnimCtrlSeq(display->timerDigits[1], (u8)(seconds % 10) + 1);
    }
}

// The own score in big digits (cut from the digit screen: 5 digits per row, 4 x 8 tiles each).
void PokeathlonGoalRollDisplay_DrawScore(PokeathlonGoalRollDisplay *display, s32 score) {
    u8 srcX[2];
    u8 srcY[2];
    u8 digit;
    u8 i;

    digit = (u8)(score / 10);
    srcX[0] = (digit % 5) * 4;
    srcY[0] = (digit / 5) * 8;
    digit = (u8)(score % 10);
    srcX[1] = (digit % 5) * 4;
    srcY[1] = (digit / 5) * 8;

    for (i = 0; i < 2; i++) {
        CopyRectToBgTilemapRect(display->bgConfig, GF_BG_LYR_SUB_2, (u8)(i * 4 + 6), 10, 4, 8, display->digitScreen->rawData, srcX[i], srcY[i], 20, 16);
    }
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_2);
}

PokeathlonGoalRollCpu *PokeathlonGoalRollCpu_New(enum HeapID heapId, u8 cpuCount) {
    PokeathlonGoalRollCpu *cpu = Heap_Alloc(heapId, sizeof(PokeathlonGoalRollCpu));

    MI_CpuFill8(cpu, 0, sizeof(PokeathlonGoalRollCpu));
    cpu->cpuCount = cpuCount;
    cpu->humanCount = 4 - cpuCount;
    cpu->firstCpu = cpu->humanCount;
    cpu->interval = 3;
    cpu->timer = cpu->interval;
    return cpu;
}

void PokeathlonGoalRollCpu_SetMon(PokeathlonGoalRollCpuTeam *teams, s32 index, PokeathlonGoalRollMon *mon) {
    teams[(u8)(index / 3)].mons[(u8)(index % 3)] = mon;
}

void PokeathlonGoalRollCpu_ChooseAllRoles(PokeathlonGoalRollCpuTeam *teams) {
    u8 i;

    for (i = 0; i < 4; i++) {
        PokeathlonGoalRollCpu_ChooseRoles(&teams[i]);
    }
}

// The member best at kicking and with most power moves first; the others follow by kick strength.
void PokeathlonGoalRollCpu_ChooseRoles(PokeathlonGoalRollCpuTeam *team) {
    u8 first[3];
    u8 second[3];
    u8 selected;
    u8 i;
    u8 a;
    u8 b;

    for (i = 0; i < 3; i++) {
        first[i] = team->mons[i]->kickRow + team->mons[i]->maxPower;
        second[i] = team->mons[i]->kickRow + team->mons[i]->kickLevel;
    }

    selected = 0;
    for (i = 1; i < 3; i++) {
        if (first[i - 1] < first[i]) {
            selected = i;
        }
    }

    if (selected == 0) {
        if (second[1] >= second[2]) {
            a = 1;
            b = 2;
        } else {
            a = 2;
            b = 1;
        }
    } else if (selected == 1) {
        if (second[0] >= second[2]) {
            a = 0;
            b = 2;
        } else {
            a = 2;
            b = 0;
        }
    } else {
        if (second[0] >= second[1]) {
            a = 0;
            b = 1;
        } else {
            a = 1;
            b = 0;
        }
    }

    team->order[0].mon = team->mons[selected];
    team->order[1].mon = team->mons[a];
    team->order[2].mon = team->mons[b];
}

void PokeathlonGoalRollCpu_SetBalls(PokeathlonGoalRollCpu *cpu, PokeathlonGoalRollBall *balls) {
    cpu->balls = balls;
}

void PokeathlonGoalRollCpu_Free(PokeathlonGoalRollCpu *cpu) {
    Heap_Free(cpu);
}

// Every `interval` frames the CPU participants each move one Pokemon, in their order.
void PokeathlonGoalRollCpu_Update(PokeathlonCourseData *course, PokeathlonGoalRollCpu *cpu) {
    PokeathlonGoalRollCpuSlot *slot;

    if (cpu->humanCount == 4) {
        return;
    }
    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->run = TRUE;
        cpu->timer = 0;
    }
    if (cpu->run == FALSE) {
        return;
    }
    slot = &cpu->teams[cpu->firstCpu + cpu->current].order[cpu->orderSlot];
    if (slot->wait == 0) {
        PokeathlonGoalRollCpu_Think(course, cpu, slot);
    } else {
        slot->wait--;
    }
    cpu->current++;
    if (cpu->current >= cpu->cpuCount) {
        cpu->current = 0;
        cpu->orderSlot = (cpu->orderSlot + 1) % 3;
        cpu->run = FALSE;
    }
}

#pragma force_active on

int PokeathlonGoalRollCpu_ThinkChaser(PokeathlonCourseData *course, u8 participant, u8 member, PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *balls) {
    VecFx32 *position;
    int radius, quadrant;

    struct {
        u8 values[2];
    } decisions;

    u8 i;
    int result;

    if (mon->mode) {
        return 0;
    }
    if (mon->motion) {
        return 0;
    }
    quadrant = PokeathlonGoalRollCpu_GetQuadrant(128, 96, mon->position.x / 4096, mon->position.y / 4096);
    i = 0;
    position = &mon->position;
    for (; i < 2; i++) {
        PokeathlonGoalRollBall *ball = &balls[i];
        int ballQuadrant;
        if (!ball->state && ball->kind) {
            ballQuadrant = PokeathlonGoalRollCpu_GetQuadrant(128, 96, ball->position.x / 4096, ball->position.y / 4096);
        } else {
            ballQuadrant = 4;
        }
        if (ballQuadrant != 4) {
            radius = 0;
            if (ball->kind == GOAL_ROLL_BALL_SMALL) {
                radius = 8;
            } else if (ball->kind == GOAL_ROLL_BALL_BIG) {
                radius = 12;
            } else {
                GF_AssertFail();
            }
            if (PokeathlonGoalRollCpu_CirclesTouch(position, (u32)PokeathlonCourseObject_GetCollisionRadius(mon->object) << 12, &ball->position, radius << 12)) {
                decisions.values[i] = 1;
            } else if (quadrant == ballQuadrant) {
                if (PokeathlonGoalRollCpu_IsMonInTargetQuadrantOfBall(ball->position.x / 4096, ball->position.y / 4096, mon->position.x / 4096, mon->position.y / 4096, quadrant, participant)) {
                    decisions.values[i] = 4;
                } else {
                    decisions.values[i] = 3;
                }
            } else {
                decisions.values[i] = 2;
            }
        } else {
            decisions.values[i] = 0;
        }
    }
    if (decisions.values[0] >= decisions.values[1]) {
        result = PokeathlonGoalRollCpu_ApplyChaserDecision(mon, balls, decisions.values[0], quadrant);
    } else {
        balls++;
        result = PokeathlonGoalRollCpu_ApplyChaserDecision(mon, balls, decisions.values[1], quadrant);
    }
    if (mon->mode == GOAL_ROLL_MODE_DASH) {
        PokeathlonCourse_AddMonCounter(course, participant, member, 6, 1);
    }
    return result;
}

#pragma force_active reset

int PokeathlonGoalRollCpu_ApplyChaserDecision(PokeathlonGoalRollMon *mon, const PokeathlonGoalRollBall *ball, u8 decision, int quadrant) {
    u8 random = (s32)LCRandom() % 100;

    switch (decision) {
    case 2:
        if (random < 60) {
            return 0;
        }
        if (random < 80) {
            mon->target = ball->position;
            return 10;
        }
        mon->target = ball->position;
        mon->mode = GOAL_ROLL_MODE_DASH;
        return 10;
    case 3:
        if (random < 20) {
            return 0;
        }
        if (random < 90) {
            mon->target = ball->position;
            return 3;
        }
        mon->target = ball->position;
        mon->mode = GOAL_ROLL_MODE_DASH;
        return 3;
    case 4:
    case 13:
        mon->target = ball->position;
        mon->mode = GOAL_ROLL_MODE_DASH;
        break;
    case 1:
    case 9:
        if (quadrant != 4) {
            u8 goal = 3 - quadrant;
            mon->target.x = ov96_0221D678[goal][0][0] << 12;
            mon->target.y = ov96_0221D67A[goal][0][0] << 12;
        } else {
            GF_AssertFail();
        }
        break;
    case 0:
    case 8:
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

#pragma force_active on

int PokeathlonGoalRollCpu_ThinkGuard(PokeathlonCourseData *course, u8 participant, u8 member, PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *balls) {
    int quadrant, result;
    u32 ballQuadrant;
    u8 i, decisions[2];
    VecFx32 delta;

    if (mon->mode) {
        return 0;
    }
    if (mon->motion) {
        return 0;
    }
    quadrant = PokeathlonGoalRollCpu_GetQuadrant(128, 96, mon->position.x / 4096, mon->position.y / 4096);
    for (i = 0; i < 2; i++) {
        PokeathlonGoalRollBall *ball = &balls[i];
        if (!ball->state && ball->kind) {
            ballQuadrant = PokeathlonGoalRollCpu_GetQuadrant(128, 96, ball->position.x / 4096, ball->position.y / 4096);
        } else {
            ballQuadrant = 4;
        }
        if (ballQuadrant != 4) {
            if (participant == ballQuadrant) {
                if (PokeathlonGoalRollCpu_IsMonNotOppositeOfBall(ball->position.x / 4096, ball->position.y / 4096, mon->position.x / 4096, mon->position.y / 4096, quadrant, participant)) {
                    decisions[i] = 7;
                } else {
                    decisions[i] = 6;
                }
            } else {
                decisions[i] = 6;
            }
        } else {
            decisions[i] = 5;
        }
    }
    ballQuadrant = decisions[1];
    if ((decisions[0] == 7) == (ballQuadrant == 7)) {
        int distance;
        VEC_Subtract(&mon->position, &balls[0].position, &delta);
        distance = VEC_Mag(&delta);
        VEC_Subtract(&mon->position, &balls[1].position, &delta);
        if (distance >= VEC_Mag(&delta)) {
            result = PokeathlonGoalRollCpu_ApplyGuardDecision(mon, balls, decisions[0], participant);
        } else {
            balls++;
            result = PokeathlonGoalRollCpu_ApplyGuardDecision(mon, balls, ballQuadrant, participant);
        }
    } else {
        if (decisions[0] >= ballQuadrant) {
            result = PokeathlonGoalRollCpu_ApplyGuardDecision(mon, balls, decisions[0], participant);
        } else {
            balls++;
            result = PokeathlonGoalRollCpu_ApplyGuardDecision(mon, balls, ballQuadrant, participant);
        }
    }
    if (mon->mode == GOAL_ROLL_MODE_DASH) {
        PokeathlonCourse_AddMonCounter(course, participant, member, 6, 1);
    }
    return result;
}

#pragma force_active reset

int PokeathlonGoalRollCpu_ApplyGuardDecision(PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *ball, int decision, int participant) {
    volatile u16 point[2];

    switch (decision) {
    case 6:
    case 11: {
        const u16(*points)[2] = ov96_0221D678[participant - 1];

        if (PokeathlonGoalRollCpu_IsNearPoint(mon->position.x / FX32_ONE, mon->position.y / FX32_ONE, points)) {
            return 0;
        }
        if ((s32)LCRandom() % 2 == 0) {
            point[0] = points[0][0];
            point[1] = points[0][1];
        } else if ((s32)LCRandom() % 2 == 0) {
            const u16 *other = ov96_0221D67C[participant - 1][0];
            point[0] = other[0];
            point[1] = other[1];
        } else {
            const u16 *other = ov96_0221D680[participant - 1][0];
            point[0] = other[0];
            point[1] = other[1];
        }
        point[0] += 8 - (s32)LCRandom() % 17;
        point[1] += 8 - (s32)LCRandom() % 17;
        mon->target.x = point[0] << 12;
        mon->target.y = point[1] << 12;
        return 6;
    }
    case 7:
    case 12:
        mon->target = ball->position;
        if ((s32)LCRandom() % 100 < 80) {
            mon->mode = GOAL_ROLL_MODE_DASH;
        }
        return 10;
    default:
        GF_AssertFail();
    case 5:
    case 8:
        return 0;
    }
}

#pragma force_active on

int PokeathlonGoalRollCpu_ThinkFlex(PokeathlonCourseData *course, u8 participant, u8 member, PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *balls, PokeathlonGoalRollCpuTeam *teams) {
    u8 i;
    int ballQuadrant;
    int quadrant;
    int radius;
    int result;
    fx32 distance;
    PokeathlonGoalRollBall *ball;
    u8 decisions[2];
    u8 quadrants[2];
    VecFx32 delta;
    u8 decision0;
    u8 decision1;
    BOOL isChase0;
    BOOL isChase1;

    if (mon->mode) {
        return 0;
    }
    if (mon->motion) {
        return 0;
    }
    quadrant = PokeathlonGoalRollCpu_GetQuadrant(128, 96, mon->position.x / FX32_ONE, mon->position.y / FX32_ONE);
    for (i = 0; i < 2; i++) {
        ball = &balls[i];
        if (ball->state == GOAL_ROLL_BALL_ROLLING && ball->kind != GOAL_ROLL_BALL_NONE) {
            quadrants[i] = PokeathlonGoalRollCpu_GetQuadrant(128, 96, ball->position.x / FX32_ONE, ball->position.y / FX32_ONE);
        } else {
            quadrants[i] = 4;
        }
    }
    for (i = 0; i < 2; i++) {
        ball = &balls[i];
        decisions[i] = 8;
        ballQuadrant = quadrants[i];
        if (ballQuadrant == 4) {
            continue;
        }
        radius = 0;
        if (ball->kind == GOAL_ROLL_BALL_SMALL) {
            radius = 8;
        } else if (ball->kind == GOAL_ROLL_BALL_BIG) {
            radius = 12;
        } else {
            GF_AssertFail();
        }
        if (PokeathlonGoalRollCpu_CirclesTouch(&mon->position, PokeathlonCourseObject_GetCollisionRadius(mon->object) << 12, &ball->position, radius << 12)) {
            decisions[i] = 8;
        } else if (quadrant == ballQuadrant) {
            if (PokeathlonGoalRollCpu_IsMonInTargetQuadrantOfBall(ball->position.x / FX32_ONE, ball->position.y / FX32_ONE, mon->position.x / FX32_ONE, mon->position.y / FX32_ONE, quadrant, participant)) {
                decisions[i] = 13;
            }
        }
    }
    if (decisions[0] == 13) {
        return PokeathlonGoalRollCpu_ApplyChaserDecision(mon, balls, decisions[0], quadrant);
    }
    if (decisions[1] == 13) {
        balls++;
        return PokeathlonGoalRollCpu_ApplyChaserDecision(mon, balls, decisions[1], quadrant);
    }
    if (teams[participant].order[0].mon->power >= 50) {
        return PokeathlonGoalRollCpu_MoveToEmptierQuadrant(mon, teams, participant);
    }
    for (i = 0; i < 2; i++) {
        ball = &balls[i];
        if (quadrants[i] != 4) {
            if (participant == quadrants[i]) {
                if (PokeathlonGoalRollCpu_IsMonInTargetQuadrantOfBall(ball->position.x / FX32_ONE, ball->position.y / FX32_ONE, mon->position.x / FX32_ONE, mon->position.y / FX32_ONE, quadrant, participant)) {
                    decisions[i] = 12;
                } else {
                    decisions[i] = 11;
                }
            } else {
                decisions[i] = 11;
            }
        } else {
            decisions[i] = 10;
        }
    }
    decision0 = decisions[0];
    decision1 = decisions[1];
    isChase0 = decision0 == 7 ? TRUE : FALSE;
    isChase1 = decision1 == 7 ? TRUE : FALSE;
    if (isChase0 == isChase1) {
        VEC_Subtract(&mon->position, &balls[0].position, &delta);
        distance = VEC_Mag(&delta);
        VEC_Subtract(&mon->position, &balls[1].position, &delta);
        if (distance >= VEC_Mag(&delta)) {
            result = PokeathlonGoalRollCpu_ApplyFlexDecision(mon, balls, decisions[0], participant, teams);
        } else {
            balls++;
            result = PokeathlonGoalRollCpu_ApplyFlexDecision(mon, balls, decision1, participant, teams);
        }
    } else if (decision0 >= decision1) {
        result = PokeathlonGoalRollCpu_ApplyFlexDecision(mon, balls, decision0, participant, teams);
    } else {
        balls++;
        result = PokeathlonGoalRollCpu_ApplyFlexDecision(mon, balls, decision1, participant, teams);
    }
    if (mon->mode == GOAL_ROLL_MODE_DASH) {
        PokeathlonCourse_AddMonCounter(course, participant, member, 6, 1);
    }
    return result;
}

#pragma force_active reset

// PokeathlonGoalRollCpu_MoveToEmptierQuadrant with the participant passed as int (this call was built against such a declaration).
typedef int (*PokeathlonGoalRollCpuSpreadFunc)(PokeathlonGoalRollMon *mon, PokeathlonGoalRollCpuTeam *teams, int participant);

int PokeathlonGoalRollCpu_ApplyFlexDecision(PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *ball, int decision, int participant, PokeathlonGoalRollCpuTeam *teams) {
    if (decision == 10) {
        return ((PokeathlonGoalRollCpuSpreadFunc)PokeathlonGoalRollCpu_MoveToEmptierQuadrant)(mon, teams, participant);
    }
    return PokeathlonGoalRollCpu_ApplyGuardDecision(mon, ball, decision, participant);
}

#pragma force_active on

// Sends the Pokemon to a random spot in a quadrant with fewer Pokemon than the one before it.
int PokeathlonGoalRollCpu_MoveToEmptierQuadrant(PokeathlonGoalRollMon *mon, PokeathlonGoalRollCpuTeam *teams, u8 excluded) {
    u8 counts[4] = { 0, 0, 0, 0 };
    u8 i, j;
    int chosen, x, y;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            PokeathlonGoalRollMon *other = teams[i].order[j].mon;
            int quadrant = PokeathlonGoalRollCpu_GetQuadrant(128, 96, other->position.x / 4096, other->position.y / 4096);
            if (quadrant == 4) {
                goto next;
            }
            counts[quadrant]++;
next:;
        }
    }
    chosen = 0;
    for (i = 1; i < 4; i++) {
        if (counts[i - 1] > counts[i] && i != excluded) {
            chosen = i;
        }
    }
    x = (int)LCRandom() % 64;
    y = (int)LCRandom() % 32;
    switch (chosen) {
    case 0:
        mon->target.x = (128 - x) << 12;
        mon->target.y = (96 - y) << 12;
        break;
    case 1:
        mon->target.x = (128 + x) << 12;
        mon->target.y = (96 - y) << 12;
        break;
    case 2:
        mon->target.x = (128 - x) << 12;
        mon->target.y = (96 + y) << 12;
        break;
    case 3:
        mon->target.x = (128 + x) << 12;
        mon->target.y = (96 + y) << 12;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 10;
}

// Lets the Pokemon in `slot` act according to its role; the result is how long it waits.
void PokeathlonGoalRollCpu_Think(PokeathlonCourseData *course, PokeathlonGoalRollCpu *cpu, PokeathlonGoalRollCpuSlot *slot) {
    u8 participant = cpu->firstCpu + cpu->current;
    u8 role = cpu->orderSlot;
    switch (role) {
    case 0:
        slot->wait = (u32)PokeathlonGoalRollCpu_ThinkGuard(course, participant, role, slot->mon, cpu->balls);
        break;
    case 1:
        slot->wait = (u32)PokeathlonGoalRollCpu_ThinkChaser(course, participant, role, slot->mon, cpu->balls);
        break;
    case 2:
        slot->wait = PokeathlonGoalRollCpu_ThinkFlex(course, participant, role, slot->mon, cpu->balls, cpu->teams);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

#pragma force_active reset

// 0 top left, 1 top right, 2 bottom left, 3 bottom right of (x, y)
int PokeathlonGoalRollCpu_GetQuadrant(int x, int y, int pointX, int pointY) {
    if (pointX < x && pointY < y) {
        return 0;
    }
    if (pointX >= x && pointY < y) {
        return 1;
    }
    if (pointX < x && pointY >= y) {
        return 2;
    }
    if (pointX >= x && pointY >= y) {
        return 3;
    }
    GF_AssertFail();
    return 4;
}

BOOL PokeathlonGoalRollCpu_IsMonInTargetQuadrantOfBall(int x1, int y1, int x2, int y2, int quadrantInput, int participantInput) {
    int result = PokeathlonGoalRollCpu_GetQuadrant(x1, y1, x2, y2);
    u8 participant = *(const u8 *)&participantInput;
    u8 quadrant = *(const u8 *)&quadrantInput;
    u8 target;

    if (participant != quadrant) {
        target = 3 - quadrant;
    } else {
        target = participant;
    }
    return result == target;
}

BOOL PokeathlonGoalRollCpu_IsMonNotOppositeOfBall(int x1, int y1, int x2, int y2, int quadrantInput, int participantInput) {
    int result = PokeathlonGoalRollCpu_GetQuadrant(x1, y1, x2, y2);
    u8 quadrant = *(const u8 *)&quadrantInput;
    u8 participant = *(const u8 *)&participantInput;

    if (participant == quadrant) {
        return (3 - participant) != result;
    }
    return FALSE;
}

BOOL PokeathlonGoalRollCpu_IsNearPoint(int x, int y, const u16 (*points)[2]) {
    u8 i;

    for (i = 0; i < 3; i++) {
        int dx = x - points[i][0];
        int dy = y - points[i][1];

        if (dx >= -8 && dx <= 8 && dy >= -8 && dy <= 8) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL PokeathlonGoalRollCpu_CirclesTouch(const VecFx32 *a, fx32 radiusA, const VecFx32 *b, fx32 radiusB) {
    VecFx32 delta;

    VEC_Subtract(a, b, &delta);
    return VEC_Mag(&delta) < radiusA + radiusB;
}

// Screen shake task (started by PokeathlonScreenShake_Start in the next event's code).
void PokeathlonScreenShake_Task(SysTask *task, PokeathlonScreenShake *shake) {
    int sign;
    int x;
    int y;

    if (shake->direction == 0) {
        sign = 1;
    } else {
        sign = -1;
    }
    y = (shake->offset >> 12) * sign;
    x = y / 2;
    switch (shake->frame % 3) {
    case 0:
        x *= -1;
        break;
    case 1:
        x = 0;
        break;
    case 2:
        break;
    default:
        GF_AssertFail();
        break;
    }

    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, y);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, y);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, y);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, y);
    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(shake->spriteSystem), x << 12, y << 12);

    shake->offset -= shake->step;
    shake->direction ^= 1;
    shake->frame++;
    if (shake->frame >= shake->duration) {
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
        G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(shake->spriteSystem), 0, 0);
        shake->task = NULL;
        SysTask_Destroy(task);
    }
}
