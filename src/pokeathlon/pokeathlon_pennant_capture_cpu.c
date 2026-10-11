#include "global.h"

#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sprite_transfer.h"

// math_util.c: declared here as returning u32, as the calls were built (math_util.h says u16)
u32 LCRandom(void);

// Course sprite and object helpers (OVY_96, not yet tidied).
void ov96_021EB03C(void *object, int x, int y, int *outX, int *outY);
void ov96_021EB0A4(void *object, int x, int y, int *outX, int *outY);
void ov96_021EB52C(void *sprite, int a1, int show);
void **ov96_021EB5EC(void *pool, int id, int kind);
// the CPU's cell checks, next in link order (overlay96_course_scene_helpers.c)
BOOL ov96_022090A8(PokeathlonPennantCaptureCpuCell *cell, s32 x, s32 y);
BOOL ov96_022090D8(PokeathlonPennantCaptureCpuCell *cell);

extern const WindowTemplate ov96_0221CBEC;
extern const PokeathlonPennantCaptureSpriteIds ov96_0221CBCC[];

typedef struct PokeathlonPennantCaptureCpuPoint {
    int y;
    int x;
} PokeathlonPennantCaptureCpuPoint;

typedef struct PokeathlonPennantCaptureCellNode {
    struct PokeathlonPennantCaptureCellNode *prev;
    struct PokeathlonPennantCaptureCellNode *next;
    PokeathlonPennantCaptureCpuCell *cell;
} PokeathlonPennantCaptureCellNode;

#pragma force_active on

void PokeathlonPennantCaptureDisplay_GetPicVramOffsets(void *pool, PokeathlonPennantCaptureDisplay *display) {
    PokeathlonPennantCaptureSpriteIds ids = ov96_0221CBCC[3];
    u8 i;
    for (i = 0; i < 2; i++) {
        int id = ((int *)&ids)[i];
        void *charResource = *ov96_021EB5EC(pool, id, 0);
        void *plttResource = *ov96_021EB5EC(pool, id, 1);
        NNSG2dImageProxy *image = SpriteTransfer_GetCharProxy(charResource);
        NNSG2dImagePaletteProxy *palette = SpriteTransfer_GetPaletteProxy(plttResource, image);
        display->picCharOffsets[i] = NNS_G2dGetImageLocation(image, NNS_G2D_VRAM_TYPE_2DSUB);
        display->picPlttOffsets[i] = NNS_G2dGetImagePaletteLocation(palette, NNS_G2D_VRAM_TYPE_2DSUB);
    }
}

#pragma force_active reset

void PokeathlonPennantCaptureDisplay_InitWindow(PokeathlonPennantCaptureDisplay *display) {
    AddWindow(display->bgConfig, &display->window, &ov96_0221CBEC);
    BG_FillCharDataRange(display->bgConfig, GF_BG_LYR_SUB_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, display->heapId);
}

void PokeathlonPennantCaptureDisplay_ShowStatus(void **sprites, int show) {
    ov96_021EB52C(sprites[0], 1, show);
    ov96_021EB52C(sprites[2], 1, show);
    ov96_021EB52C(sprites[1], 1, show);
}

PokeathlonPennantCaptureCpu *PokeathlonPennantCaptureCpu_New(enum HeapID heapId, int cpuCount) {
    PokeathlonPennantCaptureCpu *cpu = Heap_Alloc(heapId, sizeof(PokeathlonPennantCaptureCpu));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonPennantCaptureCpu));
    cpu->cpuCount = cpuCount;
    cpu->humanCount = 4 - cpuCount;
    cpu->firstCpu = cpu->humanCount;
    cpu->interval = 6;
    cpu->timer = cpu->interval;
    return cpu;
}

void PokeathlonPennantCaptureCpu_SetPlayer(PokeathlonPennantCaptureCpu *cpu, int participant, PokeathlonPennantCapturePlayer *player) {
    cpu->players[participant].player = player;
}

void PokeathlonPennantCaptureCpu_SetPennants(PokeathlonPennantCaptureCpu *cpu, PokeathlonPennantCapturePennant *pennants) {
    cpu->pennants = pennants;
}

#pragma force_active on

// The obstacle spots: 20 (x, y) pairs in units of 8 (0, 0 = none).
void PokeathlonPennantCaptureCpu_SetObstacles(PokeathlonPennantCaptureCpu *cpu, const u8 *spots) {
    int i;
    for (i = 0; i < 20; i++) {
        u16 x = spots[0] << 3, y = spots[1] << 3;
        if (x && y) {
            cpu->obstacles[i].active = 1;
            cpu->obstacles[i].x = x;
            cpu->obstacles[i].y = y;
        } else {
            cpu->obstacles[i].active = 0;
        }
        spots += 2;
    }
}

#pragma force_active reset

void PokeathlonPennantCaptureCpu_Delete(PokeathlonPennantCaptureCpu *cpu) {
    Heap_Free(cpu);
}

// Every `interval` frames the CPUs pick new targets, one CPU per frame. Returns the participant that
// decided (4 = none, 0 = no CPUs).
int PokeathlonPennantCaptureCpu_Update(PokeathlonPennantCaptureCpu *cpu) {
    s32 participant;
    int result = 4;

    if (cpu->humanCount == 4) {
        return 0;
    }

    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->thinking = 1;
        cpu->timer = 0;
    }

    if (cpu->thinking != 0) {
        participant = cpu->firstCpu + cpu->cursor;
        GF_ASSERT(participant >= cpu->firstCpu);
        PokeathlonPennantCaptureCpu_ChooseTarget(cpu, participant);
        result = (u8)participant;
        cpu->cursor++;
        if (cpu->cursor >= cpu->cpuCount) {
            cpu->cursor = 0;
            cpu->thinking = 0;
        }
    }
    return result;
}

#pragma force_active on

// Rates the cells around the Pokemon (pennants +7, field items +2, other Pokemon -3, obstacles -2,
// off the field -100; towards the goal rows when carrying pennants) and heads for one of the best.
void PokeathlonPennantCaptureCpu_ChooseTarget(PokeathlonPennantCaptureCpu *cpu, u8 who) {
    PokeathlonPennantCaptureCpuObject obstacles[20], fieldItems[10];
    PokeathlonPennantCaptureCpuPlayer players[4];
    PokeathlonPennantCaptureCpuPlayer *self;
    PokeathlonPennantCapturePennant *pennants;
    PokeathlonPennantCapturePlayer *player;
    PokeathlonPennantCaptureCpuPoint raw, screen, chosen;
    PokeathlonPennantCaptureCpuCell *best;
    int i, j, k;
    player = cpu->players[who].player;
    if (player->depositing || player->knockedDown == 1 || player->freezeTimer) {
        return;
    }
    for (i = 0; i < 4; i++) {
        PokeathlonPennantCapturePlayer *other;
        players[i] = cpu->players[i];
        other = players[i].player;
        ov96_021EB0A4(other->monObjects[other->member], other->position.x / 4096, other->position.y / 4096, &raw.x, &raw.y);
        ov96_021EB03C(other->monObjects[other->member], raw.x << 12, raw.y << 12, &screen.x, &screen.y);
        players[i].x = screen.x / 4096;
        players[i].y = screen.y / 4096;
    }
    self = &players[who];
    for (i = 0; i < 20; i++) {
        obstacles[i] = cpu->obstacles[i];
    }
    for (i = 0; i < 10; i++) {
        fieldItems[i] = cpu->fieldItems[i];
    }
    pennants = cpu->pennants;
    for (i = 0; i < 48; i++) {
        cpu->cells[i].index = i;
        cpu->cells[i].x = self->x - 80 + (i % 6) * 32;
        cpu->cells[i].y = self->y - 48 + (i / 6) * 32;
        cpu->cells[i].targetX = self->x - 96 + (i % 6) * 32 + (int)LCRandom() % 32;
        cpu->cells[i].targetY = self->y - 128 + (i / 6) * 32 + (int)LCRandom() % 32;
    }
    for (i = 0; i < 48; i++) {
        cpu->cells[i].score = 0;
    }
    for (k = 0; k < 2; k++) {
        for (j = 0; j < 4; j++) {
            u8 id = j + 19;
            cpu->cells[id].score += 2;
        }
    }
    for (i = 0; i < 48; i++) {
        PokeathlonPennantCaptureCpuCell *cell = &cpu->cells[i];
        {
            PokeathlonPennantCapturePennant *pennant = pennants;
            for (j = 0; j < 5; j++, pennant++) {
                if (pennant->active && ov96_022090A8(cell, pennant->x, pennant->y)) {
                    cell->score += 7;
                }
            }
        }
        for (j = 0; j < 4; j++) {
            if (j != who && ov96_022090A8(cell, players[j].x, players[j].y)) {
                cell->score -= 3;
            }
        }
        for (j = 0; j < 20; j++) {
            if (!obstacles[j].active) {
                break;
            }
            if (ov96_022090A8(cell, (u16)obstacles[j].x, (u16)obstacles[j].y)) {
                cell->score -= 2;
            }
        }
        for (j = 0; j < 10; j++) {
            if (!fieldItems[j].active) {
                break;
            }
            if (ov96_022090A8(cell, (u16)fieldItems[j].x, (u16)fieldItems[j].y)) {
                cell->score += 2;
            }
        }
        if (!ov96_022090D8(cell)) {
            cell->score -= 100;
        }
    }
    {
        u16 y = player->position.y / 4096;
        if (y > 256) {
            for (i = 0; i < 6; i++) {
                cpu->cells[i + 30].score += 3;
            }
        } else if (y < 256) {
            for (i = 0; i < 6; i++) {
                cpu->cells[i].score += 3;
            }
        }
    }
    if (player->pennants) {
        for (i = 0; i < 6; i++) {
            cpu->cells[i + 30].score += player->pennants;
        }
        {
            u16 x = player->position.x / 4096;
            if (x > 256) {
                for (i = 0; i < 3; i++) {
                    cpu->cells[i + 30].score += 2;
                }
            } else if (x < 256) {
                for (i = 0; i < 3; i++) {
                    cpu->cells[i + 33].score += 2;
                }
            }
        }
    }
    best = PokeathlonPennantCaptureCpu_PickBestCell(cpu);
    if ((u32)who >= cpu->firstCpu) {
        PokeathlonPennantCapturePlayer *mon = self->player;
        ov96_021EB0A4(mon->monObjects[mon->member], self->x, self->y, &chosen.x, &chosen.y);
        self->player->cpuFrom.x = chosen.x << 12;
        self->player->cpuFrom.y = chosen.y << 12;
        self->player->cpuFrom.z = 0;
        self->player->cpuTarget.x = best->targetX << 12;
        self->player->cpuTarget.y = best->targetY << 12;
        self->player->cpuTarget.z = 0;
        self->player->cpuHasTarget = 1;
    }
}

#pragma force_active reset

PokeathlonPennantCaptureCpuCell *PokeathlonPennantCaptureCpu_PickBestCell(PokeathlonPennantCaptureCpu *cpu) {
    PokeathlonPennantCaptureCpuCell *sorted[48];
    PokeathlonPennantCaptureCpuCell *list[48];
    s32 i;
    s32 count;
    u8 index;

    for (i = 0; i < 48; i++) {
        list[i] = &cpu->cells[i];
    }
    count = PokeathlonPennantCaptureCpu_SortCells(list, sorted);
    index = (int)LCRandom() % count;
    return sorted[index];
}

#pragma force_active on

// Sorts the cells by score (insertion into a linked list, best first); returns how many share the best score.
u8 PokeathlonPennantCaptureCpu_SortCells(PokeathlonPennantCaptureCpuCell **cells, PokeathlonPennantCaptureCpuCell **out) {
    PokeathlonPennantCaptureCellNode nodes[49];
    u8 count;
    u8 i, j;
    PokeathlonPennantCaptureCellNode *node;
    PokeathlonPennantCaptureCellNode *cur;

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].cell = NULL;
    count = 0;
    for (i = 0; i < 48; i++) {
        node = &nodes[i + 1];
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
        PokeathlonPennantCaptureCellNode *it;
        s16 first;
        first = nodes[0].next->cell->score;
        for (j = 0, it = &nodes[0]; j < 48; j++) {
            it = it->next;
            out[j] = it->cell;
            if (first == out[j]->score) {
                count++;
            }
        }
    }
    GF_ASSERT(count != 0);
    return count;
}

#pragma force_active reset
