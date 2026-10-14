#include "pokeathlon/pokeathlon_relay_run_cpu.h"

#include "global.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "sprite_transfer.h"

typedef char PokeathlonRelayRunCpuSizeCheck[(sizeof(PokeathlonRelayRunCpu) == 0x3E8 && offsetof(PokeathlonRelayRunCpu, cells) == 0x29C && offsetof(PokeathlonRelayRunCpu, thinking) == 0x3DC) ? 1 : -1];

// Course sprite and object helpers (pokeathlon_course_objects.c), with untyped entries as the event uses them.
void PokeathlonCourseObject_GroundToCenter(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
void PokeathlonCourseObject_GetCollisionCenter(void *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY);
void **PokeathlonCourseSpritePool_GetResourceSlot(void *pool, u32 id, u32 kind);

// Sprite resource ids of the touch screen (pairs: the two Pokemon pictures are entries 6 and 7).
extern const u32 gPokeathlonRelayRunPicIds[];
extern const WindowTemplate ov96_0221C760;
extern const WindowTemplate ov96_0221C768;
// Base score of each cell ahead of the runner: the farther, the better.
extern const s32 ov96_0221C770[RELAY_RUN_CPU_CELLS];

// The touch screen: where the two picture slots are in VRAM.
void PokeathlonRelayRunHud_GetPicVramOffsets(void *pool, PokeathlonRelayRunHud *hud) {
    u32 ids[2] = { gPokeathlonRelayRunPicIds[6], gPokeathlonRelayRunPicIds[7] };
    u8 i;

    for (i = 0; i < 2; i++) {
        void *charResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, ids[i], 0);
        void *plttResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, ids[i], 1);
        NNSG2dImageProxy *image = SpriteTransfer_GetCharProxy(charResource);
        NNSG2dImagePaletteProxy *palette = SpriteTransfer_GetPaletteProxy(plttResource, image);

        hud->picCharLocations[i] = NNS_G2dGetImageLocation(image, NNS_G2D_VRAM_TYPE_2DSUB);
        hud->picPaletteLocations[i] = NNS_G2dGetImagePaletteLocation(palette, NNS_G2D_VRAM_TYPE_2DSUB);
    }
}

void PokeathlonRelayRunHud_InitWindows(PokeathlonRelayRunHud *hud) {
    AddWindow(hud->bgConfig, &hud->nameWindow, &ov96_0221C760);
    AddWindow(hud->bgConfig, &hud->lapsWindow, &ov96_0221C768);
    BG_FillCharDataRange(hud->bgConfig, GF_BG_LYR_SUB_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, hud->heapId);
}

PokeathlonRelayRunCpu *PokeathlonRelayRunCpu_New(enum HeapID heapId, int cpuCount, int mode) {
    PokeathlonRelayRunCpu *cpu = Heap_Alloc(heapId, sizeof(PokeathlonRelayRunCpu));

    MI_CpuFill8(cpu, 0, sizeof(PokeathlonRelayRunCpu));
    cpu->cpuCount = cpuCount;
    cpu->humanCount = RELAY_RUN_TEAMS - cpuCount;
    cpu->firstCpu = cpu->humanCount;
    cpu->interval = 10;
    cpu->timer = cpu->interval;
    return cpu;
}

void PokeathlonRelayRunCpu_SetTeam(PokeathlonRelayRunCpu *cpu, u8 index, PokeathlonRelayRunTeam *team) {
    cpu->teams[index].team = team;
}

// The obstacle layout as sent by the host (kind | y << 8 | x << 16 each; kind 0 ends it). Obstacles are squares
// (walls 32 or 48 pixels, slow patches 64) centred on x, y (the track starts 32 pixels down).
#pragma force_active on

void PokeathlonRelayRunCpu_SetObstacles(PokeathlonRelayRunCpu *cpu, s32 *layout) {
    int i;

    // (steps cpu itself through the obstacles, as built)
    for (i = 0; i < RELAY_RUN_OBSTACLES; i++, layout++, cpu = (PokeathlonRelayRunCpu *)((u8 *)cpu + sizeof(PokeathlonRelayRunCpuObstacle))) {
        int kind, y, x;

        kind = (u8)*layout;
        cpu->obstacles[0].kind = kind;
        y = (u8)(*layout >> 8);
        x = (u16)(*layout >> 16);
        if (kind != 0) {
            cpu->obstacles[0].active = TRUE;
            if (cpu->obstacles[0].kind == RELAY_RUN_OBSTACLE_WALL) {
                cpu->obstacles[0].size = 32;
            } else if (cpu->obstacles[0].kind == RELAY_RUN_OBSTACLE_BIG_WALL) {
                cpu->obstacles[0].size = 48;
            } else {
                cpu->obstacles[0].size = 64;
            }
            cpu->obstacles[0].x = x - cpu->obstacles[0].size / 2;
            cpu->obstacles[0].y = y + 32 - cpu->obstacles[0].size / 2;
        } else {
            cpu->obstacles[0].active = FALSE;
        }
    }
}

#pragma force_active reset

void PokeathlonRelayRunCpu_Delete(PokeathlonRelayRunCpu *cpu) {
    Heap_Free(cpu);
}

// Called by the host every frame. Returns the team looked at, RELAY_RUN_CPU_NONE if none (0 without CPU teams).
u8 PokeathlonRelayRunCpu_Update(PokeathlonCourseData *course, PokeathlonRelayRunCpu *cpu, s32 timer) {
    u8 result = RELAY_RUN_CPU_NONE;

    if (cpu->humanCount == RELAY_RUN_TEAMS) {
        return 0;
    }
    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->thinking = TRUE;
        cpu->timer = 0;
    }
    if (cpu->thinking) {
        int team = cpu->firstCpu + cpu->cursor;

        GF_ASSERT(team >= cpu->firstCpu);
        PokeathlonRelayRunCpu_Think(course, cpu, team, timer);
        result = team;
        cpu->cursor++;
        if (cpu->cursor >= cpu->cpuCount) {
            cpu->cursor = 0;
            cpu->thinking = FALSE;
        }
    }
    return result;
}

// Rates the cells ahead of team `index`'s runner and flicks it to one of the best, or hands over to the
// next member when the best is below the threshold (raised while the runner is tired).
#pragma force_active on

void PokeathlonRelayRunCpu_Think(PokeathlonCourseData *course, PokeathlonRelayRunCpu *cpu, u8 index, s32 timer) {
    PokeathlonRelayRunCpuObstacle obstacles[RELAY_RUN_OBSTACLES];
    PokeathlonRelayRunCpuTeam teams[RELAY_RUN_TEAMS];
    int laneScores[RELAY_RUN_TEAMS];
    int groundX, groundY;
    fx32 centerX, centerY;
    int fromX, fromY;
    int threshold;
    int *laneScore;
    PokeathlonRelayRunCpuTeam *self;
    int i, j, lane, firstCell, laneTop;
    PokeathlonRelayRunTeam *team;
    PokeathlonRelayRunCpuCell *cell;
    u8 column, next;
    int distance, cellX, cellY;

    team = cpu->teams[index].team;
    if (team->swapping || team->members[team->current].condition == RELAY_RUN_EXHAUSTED || team->stunTimer) {
        return;
    }
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        laneScores[i] = 0;
    }
    threshold = 0;
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        PokeathlonRelayRunTeam *other;

        teams[i] = cpu->teams[i];
        other = teams[i].team;
        PokeathlonCourseObject_GroundToCenter(other->monObjects[other->current], other->position.x / FX32_ONE, other->position.y / FX32_ONE, &groundX, &groundY);
        PokeathlonCourseObject_GetCollisionCenter(other->monObjects[other->current], groundX << FX32_SHIFT, groundY << FX32_SHIFT, &centerX, &centerY);
        teams[i].x = centerX / FX32_ONE;
        teams[i].y = centerY / FX32_ONE;
        cpu->lanes[i] = (teams[i].y - 32) / RELAY_RUN_CPU_CELL_SIZE;
        if (cpu->lanes[i] > 4) {
            GF_AssertFail();
        }
    }
    self = &teams[index];
    for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
        obstacles[i] = cpu->obstacles[i];
    }
    // near the end of the lap: look at everything shifted back so that the lookahead doesn't wrap
    if (self->x >= RELAY_RUN_TRACK_LENGTH - RELAY_RUN_CPU_LOOKAHEAD) {
        for (i = 0; i < RELAY_RUN_TEAMS; i++) {
            PokeathlonRelayRunCpu_WrapX(&teams[i].x, -RELAY_RUN_CPU_LOOKAHEAD);
        }
        for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
            PokeathlonRelayRunCpu_WrapX(&obstacles[i].x, RELAY_RUN_CPU_LOOKAHEAD / 2);
        }
    }
    for (i = 0; i < RELAY_RUN_CPU_CELLS; i++) {
        cpu->cells[i].index = i;
        cellX = (i % 4) * RELAY_RUN_CPU_CELL_SIZE;
        cpu->cells[i].x = self->x + RELAY_RUN_CPU_CELL_SIZE / 2 + cellX;
        cellY = (i / 4) * RELAY_RUN_CPU_CELL_SIZE;
        cpu->cells[i].y = cellY + 32 + RELAY_RUN_CPU_CELL_SIZE / 2;
        cpu->cells[i].targetX = self->x + cellX + LCRandom() % RELAY_RUN_CPU_CELL_SIZE;
        cpu->cells[i].targetY = cellY + 32 + LCRandom() % RELAY_RUN_CPU_CELL_SIZE;
    }
    for (i = 0; i < RELAY_RUN_CPU_CELLS; i++) {
        cpu->cells[i].score = ov96_0221C770[i];
    }
    for (lane = 0, laneScore = laneScores, firstCell = 0, laneTop = 32; lane < 4; lane++, laneScore++, firstCell += 4, laneTop += RELAY_RUN_CPU_CELL_SIZE) {
        // other runners: in the own lane ahead -4, in another lane ahead -2, the cell they are in -3;
        // close behind in the own lane +2
        for (j = 0; j < RELAY_RUN_TEAMS; j++) {
            if (j == index) {
                continue;
            }
            if (self->x + RELAY_RUN_CPU_LOOKAHEAD < teams[j].x) {
                continue;
            }
            distance = teams[j].x - self->x;
            if (distance >= 0 && distance < RELAY_RUN_CPU_LOOKAHEAD) {
                if (lane == cpu->lanes[j]) {
                    if (lane == cpu->lanes[index]) {
                        *laneScore -= 4;
                    } else {
                        *laneScore -= 2;
                    }
                }
                column = distance / RELAY_RUN_CPU_CELL_SIZE;
                if (column >= 4) {
                    GF_AssertFail();
                } else {
                    cpu->cells[firstCell + column].score -= 3;
                }
            } else if (distance >= -RELAY_RUN_CPU_LOOKAHEAD / 2 && lane == cpu->lanes[j] && lane == cpu->lanes[index]) {
                *laneScore += 2;
            }
        }
        // obstacles ahead overlapping the lane (NB: the penalty goes to laneScores[obstacle index], as built)
        for (j = 0; j < RELAY_RUN_OBSTACLES; j++) {
            if (!obstacles[j].active) {
                break;
            }
            if (self->x + RELAY_RUN_CPU_LOOKAHEAD >= obstacles[j].x) {
                distance = obstacles[j].x - self->x;
                if (distance > 0 && distance <= RELAY_RUN_CPU_LOOKAHEAD && laneTop < obstacles[j].y + obstacles[j].size && obstacles[j].y < laneTop + RELAY_RUN_CPU_CELL_SIZE) {
                    if (obstacles[j].kind == RELAY_RUN_OBSTACLE_SLOW_PATCH) {
                        laneScores[j] -= 1;
                    } else {
                        laneScores[j] -= 6;
                    }
                }
            }
        }
    }
    for (i = 0; i < RELAY_RUN_CPU_CELLS; i++) {
        cpu->cells[i].score += laneScores[i / 4];
    }
    if (timer < RELAY_RUN_DURATION) {
        for (i = 0; i < 4; i++) {
            cpu->cells[i * 4 + 2].score += 3;
            cpu->cells[i * 4 + 3].score += 3;
        }
    }
    if (self->team->members[self->team->current].energy <= 20.0f) {
        for (i = 0; i < 4; i++) {
            cpu->cells[i * 4 + 2].score -= 2;
            cpu->cells[i * 4 + 3].score -= 2;
        }
        threshold += 8;
    }
    team = self->team;
    next = (team->current + 1) % RELAY_RUN_MEMBERS;
    if (team->members[next].condition == RELAY_RUN_TIRED) {
        threshold -= 5;
    }
    cell = PokeathlonRelayRunCpu_PickBestCell(cpu);
    if (cell->score >= threshold) {
        if (index < cpu->firstCpu) {
            return;
        }
        PokeathlonCourseObject_GroundToCenter(self->team->monObjects[self->team->current], self->x, self->y, &fromX, &fromY);
        self->team->dragStart.x = fromX << FX32_SHIFT;
        self->team->dragStart.y = fromY << FX32_SHIFT;
        self->team->dragStart.z = 0;
        self->team->dragEnd.x = cell->targetX << FX32_SHIFT;
        self->team->dragEnd.y = cell->targetY << FX32_SHIFT;
        self->team->dragEnd.z = 0;
        self->team->flicked = TRUE;
    } else {
        if (index < cpu->firstCpu) {
            return;
        }
        self->team->swapping = TRUE;
        self->team->swapPhase = 1;
        PokeathlonCourse_AddMonCounter(course, self->team->owner, self->team->current, 7, 1);
    }
}

#pragma force_active reset

// Moves x along the lap, wrapping at its ends.
void PokeathlonRelayRunCpu_WrapX(s32 *x, s32 delta) {
    *x += delta;
    if (*x >= RELAY_RUN_TRACK_LENGTH) {
        *x -= RELAY_RUN_TRACK_LENGTH;
    } else if (*x < 0) {
        *x += RELAY_RUN_TRACK_LENGTH;
    }
}

// A random one of the cells with the best score.
PokeathlonRelayRunCpuCell *PokeathlonRelayRunCpu_PickBestCell(PokeathlonRelayRunCpu *cpu) {
    PokeathlonRelayRunCpuCell *sorted[RELAY_RUN_CPU_CELLS];
    PokeathlonRelayRunCpuCell *list[RELAY_RUN_CPU_CELLS];
    int i;
    s32 count;
    u8 pick;

    for (i = 0; i < RELAY_RUN_CPU_CELLS; i++) {
        list[i] = &cpu->cells[i];
    }
    count = PokeathlonRelayRunCpu_SortCells(list, sorted);
    pick = (s32)LCRandom() % count;
    return sorted[pick];
}

// Insertion sort of the cells by score, best first (into a linked list). Returns how many share the best score.
#pragma force_active on

u8 PokeathlonRelayRunCpu_SortCells(PokeathlonRelayRunCpuCell **cells, PokeathlonRelayRunCpuCell **out) {
    PokeathlonRelayRunCpuCellNode nodes[RELAY_RUN_CPU_CELLS + 1];
    u8 count, i, j;

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    count = 0;
    nodes[0].cell = NULL;
    for (i = 0; i < RELAY_RUN_CPU_CELLS; i++) {
        PokeathlonRelayRunCpuCellNode *node = &nodes[i + 1];
        PokeathlonRelayRunCpuCellNode *cur;

        node->cell = cells[i];
        node->next = NULL;
        node->prev = NULL;
        cur = &nodes[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (cur->cell->score < node->cell->score) {
                cur->prev->next = node;
                node->prev = cur->prev;
                node->next = cur;
                cur->prev = node;
                break;
            }
        }
        if (j == i) {
            cur->next = node;
            node->prev = cur;
        }
    }
    {
        PokeathlonRelayRunCpuCellNode *first = nodes[0].next;
        PokeathlonRelayRunCpuCellNode *it;
        s16 best;

        i = 0;
        best = first->cell->score;
        it = &nodes[0];
        for (; i < RELAY_RUN_CPU_CELLS; i++) {
            it = it->next;
            out[i] = it->cell;
            if (best == out[i]->score) {
                count++;
            }
        }
    }
    GF_ASSERT(count);
    return count;
}

#pragma force_active reset
