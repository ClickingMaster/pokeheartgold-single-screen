#include "global.h"

#include "pokeathlon/pokeathlon_snow_throw.h"

#include "assert.h"
#include "heap.h"
#include "math_util.h"

// fx_mtx43.c (not declared in the fx headers here)
void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *mtx, VecFx32 *result);

// Where each Pokemon stands on the field (fx32 field units; the CPU's copy of the table).
extern const VecFx32 ov96_0221CA30[SNOW_THROW_MONS];

// Turns a direction by a random angle of at most `degrees` either way.
void PokeathlonSnowThrow_AddAimSpread(int degrees, VecFx32 *direction) {
    MtxFx43 rotation;
    u16 angle = (u16)(0xB6 * (degrees - (LCRandom() % (degrees * 2))));
    MTX_RotY43(&rotation, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_MultVec43(direction, &rotation, direction);
}

PokeathlonSnowThrowCpu *PokeathlonSnowThrowCpu_New(enum HeapID heapId, int cpuCount, int mode) {
    PokeathlonSnowThrowCpu *cpu = Heap_Alloc(heapId, sizeof(PokeathlonSnowThrowCpu));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonSnowThrowCpu));
    cpu->cpuCount = cpuCount;
    cpu->humanCount = SNOW_THROW_TEAMS - cpuCount;
    cpu->firstCpu = cpu->humanCount;
    cpu->interval = 40;
    cpu->timer = cpu->interval;
    return cpu;
}

void PokeathlonSnowThrowCpu_SetMon(PokeathlonSnowThrowCpu *cpu, u8 index, PokeathlonSnowThrowMon *mon, PokeathlonSnowThrowBall *ball) {
    cpu->mons[index] = mon;
    cpu->balls[index] = ball;
}

void PokeathlonSnowThrowCpu_Delete(PokeathlonSnowThrowCpu *cpu) {
    Heap_Free(cpu);
}

// Called by the host every frame with the frames gone by. Every `interval` frames it looks at one member of each
// CPU team in turn (the next member the next time). Returns the Pokemon looked at, SNOW_THROW_NONE if none.
int PokeathlonSnowThrowCpu_Update(PokeathlonSnowThrowCpu *cpu, int elapsed) {
    int result = SNOW_THROW_NONE;
    int team;
    int mon;

    if (cpu->humanCount == SNOW_THROW_TEAMS) {
        return 0;
    }

    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->thinking = TRUE;
        cpu->timer = 0;
    }

    if (cpu->thinking) {
        team = cpu->firstCpu + cpu->cursor;
        mon = cpu->member + team * SNOW_THROW_MEMBERS;
        if (mon < cpu->firstCpu) {
            GF_AssertFail();
        }
        if ((cpu->balls[mon]->state == SNOW_THROW_BALL_NONE || cpu->balls[mon]->state == SNOW_THROW_BALL_HELD) && cpu->mons[mon]->condition != SNOW_THROW_STUNNED) {
            PokeathlonSnowThrowCpu_Think(cpu, (u8)team, (u8)mon, elapsed);
        }
        result = (u8)mon;
        cpu->cursor++;
        if (cpu->cursor >= cpu->cpuCount) {
            cpu->cursor = 0;
            cpu->member = (cpu->member + 1) % SNOW_THROW_MEMBERS;
            cpu->thinking = FALSE;
        }
    }

    return result;
}

#pragma force_active on

// A CPU Pokemon with enough energy (by chance) makes its snowball bigger, up to 4 taps; otherwise it throws at one
// of the other teams' Pokemon that are best to hit (many hits, a stun, low energy; by chance), with its aim spread
// and at a speed of 1 to 2.
void PokeathlonSnowThrowCpu_Think(PokeathlonSnowThrowCpu *cpu, u8 team, u8 mon, int elapsed) {
    int i;
    u8 count;
    u8 pick;
    f32 speed;
    PokeathlonSnowThrowCpuTarget *targets[SNOW_THROW_MONS];
    PokeathlonSnowThrowCpuTarget *sorted[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
    VecFx32 direction;
    const VecFx32 *home;

    if (cpu->mons[mon]->energy + LCRandom() % 51 <= SNOW_THROW_MAX_ENERGY && cpu->balls[mon]->taps < 4) {
        if (cpu->balls[mon]->state == SNOW_THROW_BALL_NONE) {
            cpu->balls[mon]->taps = 1;
            cpu->balls[mon]->state = SNOW_THROW_BALL_HELD;
            cpu->balls[mon]->position = ov96_0221CA30[mon];
            return;
        }
        GF_ASSERT(cpu->balls[mon]->state == SNOW_THROW_BALL_HELD);
        cpu->balls[mon]->taps++;
        if (cpu->balls[mon]->taps <= 1) {
            cpu->balls[mon]->size = 0;
        } else if (cpu->balls[mon]->taps <= 2) {
            cpu->balls[mon]->size = 1;
        } else if (cpu->balls[mon]->taps <= 3) {
            cpu->balls[mon]->size = 2;
        } else {
            cpu->balls[mon]->size = 3;
        }
        return;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        cpu->targets[i].mon = cpu->mons[i];
        cpu->targets[i].index = i;
        cpu->targets[i].score = cpu->mons[i]->hits / 20 + LCRandom() % 21;
        if (cpu->mons[i]->condition == SNOW_THROW_STUNNED) {
            cpu->targets[i].score++;
        }
        cpu->targets[i].score += (SNOW_THROW_MAX_ENERGY - cpu->mons[i]->energy) / 20;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        targets[i] = &cpu->targets[i];
    }
    count = PokeathlonSnowThrowCpu_SortTargets(team, targets, sorted);
    pick = LCRandom() % count;
    {
        VecFx32 zero = { 0, 0, 0 };
        home = &ov96_0221CA30[mon];
        VEC_Subtract(&ov96_0221CA30[(u8)sorted[pick]->index], home, &direction);
        VEC_Normalize(&direction, &direction);
        PokeathlonSnowThrow_AddAimSpread(3, &direction);
        PokeathlonSnowThrow_AddAimSpread((u8)cpu->mons[mon]->aimSpread, &direction);
        // (always at least 2: the random part is not scaled down)
        speed = (f32)elapsed / 40.0 + ((f32)cpu->mons[mon]->energy / 50.0 + (f32)(LCRandom() % 51));
        if (speed > 2.0) {
            speed = 2.0f;
        } else if (speed < 1.0) {
            speed = 1.0f;
        }
        VEC_MultAdd((fx32)(4096.0f * speed), &direction, &zero, &cpu->balls[mon]->velocity);
        cpu->balls[mon]->state = SNOW_THROW_BALL_FLYING;
        cpu->balls[mon]->position = *home;
    }
}

#pragma force_active reset

// Insertion sort of the other teams' nine Pokemon by score, best first (into a linked list). Returns how many share
// the best score.
u8 PokeathlonSnowThrowCpu_SortTargets(u32 ownTeam, PokeathlonSnowThrowCpuTarget **targets, PokeathlonSnowThrowCpuTarget **sorted) {
    u8 count;
    u8 i;
    u8 bestCount;
    PokeathlonSnowThrowCpuTargetNode nodes[SNOW_THROW_MONS - SNOW_THROW_MEMBERS + 1];

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].target = NULL;
    bestCount = 0;
    count = bestCount;

    for (i = count; i < SNOW_THROW_MONS; i++) {
        if (i / SNOW_THROW_MEMBERS != ownTeam) {
            u8 j;
            PokeathlonSnowThrowCpuTarget *target = targets[i];
            PokeathlonSnowThrowCpuTargetNode *node = &nodes[count + 1];
            PokeathlonSnowThrowCpuTargetNode *cur = &nodes[0];

            node->target = target;
            node->next = NULL;
            node->prev = NULL;
            j = 0;
            if (count > bestCount) {
                u16 score = node->target->score;
                do {
                    cur = cur->next;
                    if (cur->target->score < score) {
                        cur->prev->next = node;
                        node->prev = cur->prev;
                        node->next = cur;
                        cur->prev = node;
                        break;
                    }
                    j++;
                } while (j < count);
            }
            if (j == count) {
                cur->next = node;
                node->prev = cur;
            }
            count++;
        }
    }

    {
        PokeathlonSnowThrowCpuTargetNode *cur = nodes[0].next;
        u16 best = cur->target->score;
        cur = &nodes[0];
        for (i = 0; i < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; i++) {
            cur = cur->next;
            sorted[i] = cur->target;
            if (best == sorted[i]->score) {
                bestCount++;
            }
        }
    }
    if (bestCount == 0) {
        GF_AssertFail();
    }
    return bestCount;
}
