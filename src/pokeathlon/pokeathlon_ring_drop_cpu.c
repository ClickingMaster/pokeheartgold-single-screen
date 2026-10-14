#include "pokeathlon/pokeathlon_ring_drop_cpu.h"

#include <string.h>

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "math_util.h"

// The computer participants of Ring Drop (the host runs them). Every few frames a computer Pokemon that can act
// decides: dash at an opponent close in front of it, jump when crowded, chase the opponent with the least stamina,
// wait, or walk to a random spot. After a number of decisions it may go to its switch spot and switch, the more
// likely the more tired it is.

#define RING_DROP_PARTICIPANTS 4
// PokeathlonRingDropMon states (pokeathlon_ring_drop.h: RING_DROP_MON_FALL_OUT, RING_DROP_MON_RETURN)
#define RING_DROP_MON_FALLING_OUT 3
#define RING_DROP_MON_RETURNING   4

// One computer participant (0xC).
struct PokeathlonRingDropCpuPlayer {
    PokeathlonCourseData *course; // 0x0
    s8 waitFrames;                // 0x4
    s8 decisions;                 // 0x5: left until it considers switching
    s8 period;                    // 0x6: frames between decisions
    s8 chased;                    // 0x7: participant it follows, -1 none
    u32 participant : 2;          // 0x8
    u32 unk08_2 : 2;
    u32 goingToSwitch : 1;
    u32 chasing : 1;
    u32 unk08_6 : 26;
}; // size: 0xC

struct PokeathlonRingDropCpu {
    enum HeapID heapId;                     // 0x00
    PokeathlonCourseData *course;           // 0x04
    PokeathlonRingDropCpuPlayer players[3]; // 0x08: participants count..3
    volatile u8 count;                      // 0x2C
}; // size: 0x30

// The Ring Drop accessors (pokeathlon_ring_drop.c); the computer passes u8 participants.
s32 PokeathlonRingDrop_GetMonState(PokeathlonCourseData *course, u8 participant);
VecFx32 *PokeathlonRingDrop_GetMonPosition(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_GetMonScreenPosition(PokeathlonCourseData *course, u8 participant, VecFx32 *out);
u8 PokeathlonRingDrop_GetMonDirection(PokeathlonCourseData *course, u8 participant);
u8 PokeathlonRingDrop_GetMonStamina(PokeathlonCourseData *course, u8 participant);
u32 PokeathlonRingDrop_GetMonStaminaPercent(PokeathlonCourseData *course, u8 participant);
int PokeathlonRingDrop_IsMonSwitching(PokeathlonCourseData *course, u8 participant);
int PokeathlonRingDrop_IsMonPushed(PokeathlonCourseData *course, u8 participant);
int PokeathlonRingDrop_CanMonAct(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_SetMonTarget(PokeathlonCourseData *course, u8 participant, const s16 *target);
void PokeathlonRingDrop_RequestJump(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_StartMonSwitch(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_SetMonDestination(PokeathlonCourseData *course, u8 participant, const s16 *position);
int PokeathlonRingDrop_GetDirection(const VecFx32 *vec);

// Chance (percent) of a dash by the best opponent kind (0 none, 1 close, 2 near in front, 3 close in front).
extern const u8 gPokeathlonRingDropCpuDashChances[];
// Chance of a jump by the number of close opponents (1..3).
extern const u8 gPokeathlonRingDropCpuCloseChances[];

// The switch spots (x, y) by participant (y read through the label ov96_0221D98E).
typedef struct PokeathlonRingDropCpuSpot {
    s16 x;
    s16 y;
} PokeathlonRingDropCpuSpot;

extern const PokeathlonRingDropCpuSpot gPokeathlonRingDropCpuSwitchSpots[];
extern const s16 ov96_0221D98E[];

#pragma force_active on

PokeathlonRingDropCpu *PokeathlonRingDropCpu_New(PokeathlonCourseData *course, enum HeapID heapId) {
    int i, participant;
    PokeathlonRingDropCpu *cpu;

    participant = PokeathlonCourse_GetParticipantCount(course);
    cpu = Heap_Alloc(heapId, sizeof(PokeathlonRingDropCpu));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonRingDropCpu));
    cpu->heapId = heapId;
    cpu->course = course;
    i = 0;
    cpu->count = RING_DROP_PARTICIPANTS - participant;
    if (i < cpu->count) {
        PokeathlonRingDropCpuPlayer *player = cpu->players;
        do {
            PokeathlonRingDropCpu_InitPlayer(player, (u8)participant, course);
            i++;
            participant++;
            player++;
        } while (i < cpu->count);
    }
    return cpu;
}

#pragma force_active reset

void PokeathlonRingDropCpu_Free(PokeathlonRingDropCpu *cpu) {
    Heap_Free(cpu);
}

#pragma force_active on

void PokeathlonRingDropCpu_Update(PokeathlonRingDropCpu *cpu) {
    int i = 0;
    if (i < cpu->count) {
        PokeathlonRingDropCpuPlayer *player = cpu->players;
        do {
            PokeathlonRingDropCpu_UpdatePlayer(player);
            i++;
            player++;
        } while (i < cpu->count);
    }
}

#pragma force_active reset

void PokeathlonRingDropCpu_InitPlayer(PokeathlonRingDropCpuPlayer *player, u8 participant, PokeathlonCourseData *course) {
    MI_CpuFill8(player, 0, sizeof(*player));
    player->course = course;
    player->participant = participant;
    player->unk08_2 = 0;
}

BOOL PokeathlonRingDrop_IsNear(const VecFx32 *a, const VecFx32 *b, fx32 distance) {
    VecFx32 delta;
    VEC_Subtract(b, a, &delta);
    if (VEC_Mag(&delta) <= distance) {
        return TRUE;
    }
    return FALSE;
}

BOOL PokeathlonRingDrop_IsInDirection(const VecFx32 *a, const VecFx32 *b, int direction) {
    VecFx32 delta;
    VEC_Subtract(b, a, &delta);
    VEC_Normalize(&delta, &delta);
    if (PokeathlonRingDrop_GetDirection(&delta) == direction) {
        return TRUE;
    }
    return FALSE;
}

// Decide passes the direction as a u8.
#define PokeathlonRingDrop_IsInDirectionU8(a, b, direction) (((BOOL(*)(const VecFx32 *, const VecFx32 *, u8))PokeathlonRingDrop_IsInDirection)((a), (b), (direction)))

#pragma force_active on

void PokeathlonRingDropCpu_Decide(PokeathlonRingDropCpuPlayer *player) {
    int direction;
    VecFx32 *position;
    u8 kinds[4];
    u8 best[4]; // participant, kind
    s16 target[2];
    s16 destination[2];
    VecFx32 *ownPosition;
    u8 nearCount;
    u8 closeCount;
    u8 chance;
    u8 dashChance;
    int i;
    int roll;
    int roll2;
    s8 first;
    s8 second;
    s16 minStamina;
    s16 stamina;
    int state;

    nearCount = 0;
    closeCount = 0;
    memset(kinds, 0, sizeof(kinds));
    ownPosition = PokeathlonRingDrop_GetMonPosition(player->course, player->participant);
    direction = PokeathlonRingDrop_GetMonDirection(player->course, player->participant);
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        if (i == player->participant) {
            continue;
        }
        position = PokeathlonRingDrop_GetMonPosition(player->course, i);
        if (PokeathlonRingDrop_IsNear(ownPosition, position, FX32_CONST(32))) {
            if (PokeathlonRingDrop_IsInDirectionU8(ownPosition, position, direction)) {
                kinds[i] = 3;
            } else {
                kinds[i] = 1;
            }
            closeCount++;
        } else if (PokeathlonRingDrop_IsNear(ownPosition, position, FX32_CONST(48))) {
            if (PokeathlonRingDrop_IsInDirectionU8(ownPosition, position, direction)) {
                kinds[i] = 2;
            }
            nearCount++;
        }
    }
    dashChance = chance = 0;
    memset(best, 0, sizeof(best));
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        if (i != player->participant && best[1] < kinds[i]) {
            best[0] = i;
            best[1] = kinds[i];
        }
    }
    roll = MTRandom() % 100;
    dashChance += gPokeathlonRingDropCpuDashChances[best[1]];
    if (roll < dashChance) {
        position = PokeathlonRingDrop_GetMonPosition(player->course, best[0]);
        target[0] = position->x >> FX32_SHIFT;
        target[1] = position->y >> FX32_SHIFT;
        PokeathlonRingDrop_SetMonTarget(player->course, player->participant, target);
        player->waitFrames = 16;
        return;
    }
    roll = MTRandom() % 100;
    if (nearCount != 0) {
        chance += 5;
    }
    if (closeCount != 0) {
        chance += gPokeathlonRingDropCpuCloseChances[closeCount - 1];
    }
    if (roll < chance) {
        PokeathlonRingDrop_RequestJump(player->course, player->participant);
        player->waitFrames = 20;
        return;
    }
    roll = MTRandom() % 100;
    if (roll < 60) {
        roll2 = MTRandom() % 100;
        if (player->chasing) {
            return;
        }
        first = -1;
        second = -1;
        minStamina = 999;
        for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
            stamina = PokeathlonRingDrop_GetMonStamina(player->course, i);
            state = PokeathlonRingDrop_GetMonState(player->course, i);
            if (i != player->participant && state != RING_DROP_MON_FALLING_OUT && state != RING_DROP_MON_RETURNING && !PokeathlonRingDrop_IsMonSwitching(player->course, player->chased) && minStamina > stamina) {
                second = first;
                first = i;
                minStamina = stamina;
            }
        }
        if (roll2 < 60) {
            player->chased = first;
        } else if (roll2 < 100) {
            player->chased = second;
        } else {
            GF_ASSERT(FALSE);
        }
        if (player->chased != -1) {
            player->chasing = TRUE;
            player->waitFrames = 0;
        }
    } else if (roll < 80) {
        player->waitFrames = 10;
    } else if (roll < 100) {
        destination[0] = MTRandom() % 145 + 56;
        destination[1] = MTRandom() % 80 + 72;
        PokeathlonRingDrop_SetMonDestination(player->course, player->participant, destination);
        player->waitFrames = 10;
    }
}

// Walks to the switch spot and switches there.
void PokeathlonRingDropCpu_GoSwitch(PokeathlonRingDropCpuPlayer *player) {
    const PokeathlonRingDropCpuSpot *spot = &gPokeathlonRingDropCpuSwitchSpots[player->participant];
    VecFx32 position = { 0, 0, 0 };
    VecFx32 spotPosition = { 0, 0, 0 };
    PokeathlonRingDrop_GetMonScreenPosition(player->course, player->participant, &position);
    spotPosition.x = gPokeathlonRingDropCpuSwitchSpots[player->participant].x << FX32_SHIFT;
    spotPosition.y = ov96_0221D98E[player->participant * 2] << FX32_SHIFT;
    PokeathlonRingDrop_SetMonDestination(player->course, player->participant, (const s16 *)spot);
    if (PokeathlonRingDrop_IsNear(&position, &spotPosition, FX32_CONST(64))) {
        PokeathlonRingDrop_StartMonSwitch(player->course, player->participant);
        player->goingToSwitch = FALSE;
    }
}

#pragma force_active reset

// Switches for sure below 10% stamina, never at 50% or more.
BOOL PokeathlonRingDropCpu_ShouldSwitch(PokeathlonRingDropCpuPlayer *player) {
    s32 roll = MTRandom() % 100;
    s32 chance = 0;
    u32 stamina = PokeathlonRingDrop_GetMonStaminaPercent(player->course, (u8)player->participant);
    if (stamina < 10) {
        chance = 100;
    } else if (stamina < 20) {
        chance = 70;
    } else if (stamina < 30) {
        chance = 50;
    } else if (stamina < 40) {
        chance = 30;
    } else if (stamina < 50) {
        chance = 10;
    }
    return roll < chance;
}

#pragma force_active on

void PokeathlonRingDropCpu_UpdatePlayer(PokeathlonRingDropCpuPlayer *player) {
    s16 position[2];

    if (PokeathlonRingDrop_IsMonSwitching(player->course, (u8)player->participant)) {
        return;
    }
    if (player->chasing && !player->goingToSwitch) {
        VecFx32 *chasedPosition;
        PokeathlonRingDrop_GetMonState(player->course, (u8)player->participant);
        if (PokeathlonRingDrop_GetMonState(player->course, (u8)player->chased) == RING_DROP_MON_FALLING_OUT || PokeathlonRingDrop_IsMonSwitching(player->course, (u8)player->chased) || PokeathlonRingDrop_IsMonPushed(player->course, (u8)player->participant)) {
            chasedPosition = PokeathlonRingDrop_GetMonPosition(player->course, (u8)player->participant);
            position[0] = chasedPosition->x >> FX32_SHIFT;
            position[1] = chasedPosition->y >> FX32_SHIFT;
            PokeathlonRingDrop_SetMonDestination(player->course, (u8)player->participant, position);
            player->chasing = FALSE;
        } else {
            chasedPosition = PokeathlonRingDrop_GetMonPosition(player->course, (u8)player->chased);
            position[0] = chasedPosition->x >> FX32_SHIFT;
            position[1] = chasedPosition->y >> FX32_SHIFT;
            PokeathlonRingDrop_SetMonDestination(player->course, (u8)player->participant, position);
        }
    }
    if (player->goingToSwitch) {
        if (PokeathlonRingDrop_CanMonAct(player->course, (u8)player->participant)) {
            int state = PokeathlonRingDrop_GetMonState(player->course, (u8)player->participant);
            PokeathlonRingDropCpu_GoSwitch(player);
            if (state == RING_DROP_MON_FALLING_OUT) {
                player->goingToSwitch = FALSE;
            }
        }
    } else {
        player->waitFrames--;
        if (player->waitFrames <= 0) {
            if (player->waitFrames < 0) {
                player->waitFrames = 0;
            }
            player->period--;
            if (player->period <= 0) {
                player->period = 3;
                if (PokeathlonRingDrop_CanMonAct(player->course, (u8)player->participant)) {
                    PokeathlonRingDropCpu_Decide(player);
                    player->decisions--;
                    if (player->decisions <= 0) {
                        if (PokeathlonRingDropCpu_ShouldSwitch(player)) {
                            player->goingToSwitch = TRUE;
                        }
                        player->decisions = 0;
                    }
                }
            }
        }
    }
}

#pragma force_active reset
