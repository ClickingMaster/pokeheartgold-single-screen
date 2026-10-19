#include <string.h>

#include "global.h"

#include "application/choose_starter/choose_starter_sub_res.naix"
#include "msgdata/msg.naix"
#include "pokeathlon/pokeathlon_circle_push.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"

extern const u32 gPokeathlonCirclePushNameplateCoords[];

typedef struct CirclePushSortNode {
    struct CirclePushSortNode *prev;
    struct CirclePushSortNode *next;
    PokeathlonCirclePushAiCell *cell;
} CirclePushSortNode;

static BOOL PokeathlonCirclePush_IsInCircle(int x, int y, int centerX, int centerY, u16 radius);
static void PokeathlonCirclePushAi_ChooseTarget(PokeathlonCirclePushAi *ai, u8 index, u8 layout);
static void PokeathlonCirclePushAi_PlaceInCells(u8 count, PokeathlonCirclePushAiCell **cells);
static void PokeathlonCirclePushAi_ScoreCells(PokeathlonCirclePushAi *ai, u8 count, PokeathlonCirclePushAiZone *zones, PokeathlonCirclePushAiMon *mon, PokeathlonCirclePushAiCell **cells);
static int PokeathlonCirclePushAi_GetCell(int x, int y);
static int PokeathlonCirclePushAi_ScoreCell(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, int cell, PokeathlonCirclePushAiMon *mon, int x, int y, BOOL *inZone);
static int PokeathlonCirclePushAi_CountRivals(const PokeathlonCirclePushAiMon *mons, const PokeathlonCirclePushAiMon *mon, int cell);
static BOOL PokeathlonCirclePushAi_IsInZone(PokeathlonCirclePushAiZone *zones, fx32 x, fx32 y);
static BOOL PokeathlonCirclePushAi_IsZoneCell(PokeathlonCirclePushAi *ai, u8 cell);
static void PokeathlonCirclePushAi_SnapToZone(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, s32 cell, s32 *x, s32 *y);
static BOOL PokeathlonCirclePushAi_GetColumnRow(s32 x, s32 y, s16 *column, s16 *row);
static u8 PokeathlonCirclePushAi_SortCells(u32 count, PokeathlonCirclePushAiCell **cells, PokeathlonCirclePushAiCell **sorted);

static const u16 sPokeathlonCirclePushZoneKind2[2] = { 32, 2 };

static const u16 sPokeathlonCirclePushZoneKind1[2] = { 56, 1 };

static const u16 sPokeathlonCirclePushZoneKind3[2] = { 24, 3 };

static const u16 sPokeathlonCirclePushZoneKind4[2] = { 16, 5 };

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout0[4] = {
    { 1, 0x80, 0x60 },
    { 0, 0,    0    },
    { 0, 0,    0    },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout8[4] = {
    { 2, 0xC0, 0x40 },
    { 3, 0x40, 0x60 },
    { 4, 0xB8, 0x90 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout1[4] = {
    { 3, 0x40, 0x40 },
    { 2, 0x90, 0x68 },
    { 0, 0,    0    },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout2[4] = {
    { 3, 0xC8, 0x40 },
    { 2, 0x60, 0x70 },
    { 0, 0,    0    },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout3[4] = {
    { 3, 0x80, 0x38 },
    { 2, 0x80, 0x80 },
    { 0, 0,    0    },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout4[4] = {
    { 2, 0xC0, 0x40 },
    { 3, 0x88, 0x38 },
    { 0, 0,    0    },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout5[4] = {
    { 2, 0x58, 0x50 },
    { 3, 0xC8, 0x80 },
    { 0, 0,    0    },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout6[4] = {
    { 2, 0x40, 0x40 },
    { 2, 0xC0, 0x40 },
    { 4, 0x38, 0x88 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout7[4] = {
    { 2, 0x48, 0x60 },
    { 3, 0x90, 0x60 },
    { 4, 0xC8, 0x60 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout12[4] = {
    { 4, 0xC8, 0x30 },
    { 3, 0x48, 0x60 },
    { 4, 0xB8, 0x90 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout9[4] = {
    { 3, 0x90, 0x38 },
    { 2, 0x60, 0x88 },
    { 4, 0xB8, 0x68 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout10[4] = {
    { 4, 0xD0, 0x30 },
    { 2, 0x80, 0x60 },
    { 4, 0x30, 0x90 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout11[4] = {
    { 4, 0x80, 0x38 },
    { 3, 0x80, 0x60 },
    { 4, 0x80, 0x88 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout14[4] = {
    { 4, 0x30, 0x30 },
    { 4, 0xD0, 0x30 },
    { 3, 0x80, 0x80 },
    { 0, 0,    0    },
};

static const PokeathlonCirclePushZone sPokeathlonCirclePushZones_Layout13[4] = {
    { 4, 0x68, 0x30 },
    { 3, 0xA8, 0x70 },
    { 4, 0x30, 0x90 },
    { 0, 0,    0    },
};

static const u16 *sPokeathlonCirclePushZoneKinds[4] = {
    sPokeathlonCirclePushZoneKind1,
    sPokeathlonCirclePushZoneKind2,
    sPokeathlonCirclePushZoneKind3,
    sPokeathlonCirclePushZoneKind4,
};

static const PokeathlonCirclePushZone *sPokeathlonCirclePushLayoutZones[15] = {
    sPokeathlonCirclePushZones_Layout0,
    sPokeathlonCirclePushZones_Layout1,
    sPokeathlonCirclePushZones_Layout2,
    sPokeathlonCirclePushZones_Layout3,
    sPokeathlonCirclePushZones_Layout4,
    sPokeathlonCirclePushZones_Layout5,
    sPokeathlonCirclePushZones_Layout6,
    sPokeathlonCirclePushZones_Layout7,
    sPokeathlonCirclePushZones_Layout8,
    sPokeathlonCirclePushZones_Layout9,
    sPokeathlonCirclePushZones_Layout10,
    sPokeathlonCirclePushZones_Layout11,
    sPokeathlonCirclePushZones_Layout12,
    sPokeathlonCirclePushZones_Layout13,
    sPokeathlonCirclePushZones_Layout14,
};

PokeathlonCirclePushZoneMarkers *PokeathlonCirclePushZoneMarkers_New(enum HeapID heapId) {
    PokeathlonCirclePushZoneMarkers *markers = Heap_Alloc(heapId, sizeof(PokeathlonCirclePushZoneMarkers));
    MI_CpuFill8(markers, 0, sizeof(PokeathlonCirclePushZoneMarkers));
    return markers;
}

void PokeathlonCirclePushZoneMarkers_Free(PokeathlonCirclePushZoneMarkers *markers) {
    Heap_Free(markers);
}

void PokeathlonCirclePushZoneMarkers_CreateSprites(PokeathlonCirclePushZoneMarkers *markers, PokeathlonCourseSpritePool *pool) {
    int i;
    PokeathlonCirclePushZoneMarker *marker = markers->markers;
    for (i = 0; i < 8; i++, marker++) {
        marker->sprite = PokeathlonCourseSpritePool_Take(pool, 101, 2);
        Sprite *sprite = PokeathlonCourseSpritePoolSprite_GetSprite(marker->sprite);
        Sprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
        Sprite_SetDrawPriority(sprite, 100);
    }
}

// The next zone layout: one of the layouts of the round's kind (kind 0: layout 0; 1: 1..5,
// 2: 6..9, 3: 10..14), not the current one.
u8 PokeathlonCirclePush_ChooseLayout(u8 layout, int kind) {
    u8 first;
    u8 next;
    int count;
    int currentKind;
    if (layout == 15) {
        return 0;
    }
    if (layout < 1) {
        currentKind = 0;
    } else if (layout < 6) {
        currentKind = 1;
    } else if (layout < 10) {
        currentKind = 2;
    } else {
        currentKind = 3;
    }
    if (kind == 1) {
        first = 1;
        count = 5;
    } else if (kind == 2) {
        first = 6;
        count = 4;
    } else {
        first = 10;
        count = 5;
    }
    if (kind == currentKind) {
        u32 step;
        GF_ASSERT(layout >= first);
        step = LCRandom() % (count - 1);
        next = (u8)(layout - first + 1);
        next = (u8)(next + step);
        next = (u8)(next % (u32)count);
        return (u8)(next + first);
    }
    return (u8)(first + LCRandom() % count);
}

// Points of the bonus zone of layout that (x, y) lies in (0: none); *zone: its index.
int PokeathlonCirclePush_GetZonePoints(u8 layout, u16 x, u16 y, u8 *zone) {
    u8 i;
    u32 points;
    if (layout == 15) {
        return 0;
    }
    i = 0;
    points = 0;
    for (; i < 4; i++) {
        const PokeathlonCirclePushZone *zones = sPokeathlonCirclePushLayoutZones[layout];
        const PokeathlonCirclePushZone *entry = &zones[i];
        u32 kind = entry->kind;
        if (!kind) {
            break;
        }
        {
            const u16 *kindData = sPokeathlonCirclePushZoneKinds[kind - 1];
            if (PokeathlonCirclePush_IsInCircle(x, y, entry->x, entry->y, kindData[0])) {
                points = (u8)kindData[1];
                *zone = i;
                break;
            }
        }
    }
    return points;
}

// Shows the markers of the bonus zones of layout.
void PokeathlonCirclePushZoneMarkers_Show(PokeathlonCirclePushZoneMarkers *markers, u8 layout) {
    vs32 savedLayout = layout;
    int i = 0;
    const PokeathlonCirclePushZone *zone = sPokeathlonCirclePushLayoutZones[savedLayout];
    for (; i < 4; i++, zone++) {
        if (zone->kind) {
            Sprite *sprite;
            VecFx32 position;
            {
                PokeathlonCirclePushZoneMarker *marker = &markers->markers[markers->next];
                GF_ASSERT(marker->shown != TRUE);
                marker->shown = TRUE;
                marker->layout = savedLayout;
                marker->zone = zone;
            }
            sprite = PokeathlonCourseSpritePoolSprite_GetSprite(markers->markers[markers->next].sprite);
            PokeathlonCourseSpritePoolSprite_Show(markers->markers[markers->next].sprite, TRUE, TRUE);
            Sprite_SetAnimCtrlSeq(sprite, zone->kind - 1);
            position.z = 0;
            position.x = zone->x << FX32_SHIFT;
            position.y = zone->y << FX32_SHIFT;
            Sprite_SetMatrix(sprite, &position);
            {
                s32 next = *(vs32 *)&markers->next + 1;
                markers->next = next % 8;
            }
        }
    }
}

void PokeathlonCirclePushZoneMarkers_Hide(PokeathlonCirclePushZoneMarkers *markers, u8 layout) {
    int i;
    PokeathlonCirclePushZoneMarker *marker = markers->markers;
    for (i = 0; i < 8; i++, marker++) {
        if (layout == marker->layout) {
            marker->layout = 15;
            marker->zone = NULL;
            marker->shown = FALSE;
            PokeathlonCourseSpritePoolSprite_Show(marker->sprite, FALSE, FALSE);
        }
    }
}

// Effect (sprite animation) of the points 1, 2, 3, 5.
int PokeathlonCirclePush_GetPointsEffect(int points) {
    int effect = 0;
    switch (points) {
    case 1:
        effect = 1;
        break;
    case 2:
        effect = 2;
        break;
    case 3:
        effect = 3;
        break;
    case 5:
        effect = 4;
        break;
    default:
        GF_ASSERT(FALSE);
    }
    return effect;
}

static BOOL PokeathlonCirclePush_IsInCircle(int x, int y, int centerX, int centerY, u16 radius) {
    VecFx32 a;
    VecFx32 b;
    a.x = x << FX32_SHIFT;
    a.y = y << FX32_SHIFT;
    a.z = 0;
    b.x = centerX << FX32_SHIFT;
    b.y = centerY << FX32_SHIFT;
    b.z = 0;
    VEC_Subtract(&a, &b, &a);
    if ((radius << FX32_SHIFT) > VEC_Mag(&a)) {
        return TRUE;
    }
    return FALSE;
}

// The bonus zones of layout as circles for the CPU players; returns how many there are.
static u8 PokeathlonCirclePush_GetZoneCircles(int layout, PokeathlonCirclePushAiZone *circles) {
    int i;
    u8 count = 0;
    for (i = 0; i < 4; i++, circles++) {
        const PokeathlonCirclePushZone *zones = sPokeathlonCirclePushLayoutZones[layout];
        const PokeathlonCirclePushZone *zone = &zones[i];
        u32 kind = zone->kind;
        if (!kind) {
            circles->radius = 0;
            circles->x = 0;
            circles->y = 0;
        } else {
            const u16 *kindData = sPokeathlonCirclePushZoneKinds[kind - 1];
            circles->radius = kindData[0];
            circles->x = zone->x;
            circles->y = zone->y;
            count++;
        }
    }
    return count;
}

// teamCount CPU teams, the last ones; they act more often in a link course.
PokeathlonCirclePushAi *PokeathlonCirclePushAi_New(enum HeapID heapId, int teamCount, int linkMode) {
    PokeathlonCirclePushAi *ai = Heap_Alloc(heapId, sizeof(PokeathlonCirclePushAi));
    MI_CpuFill8(ai, 0, sizeof(PokeathlonCirclePushAi));
    ai->teamCount = teamCount;
    ai->firstTeam2 = 4 - teamCount;
    ai->firstTeam = ai->firstTeam2;
    if (linkMode == 0) {
        ai->interval = 20;
    } else {
        ai->interval = 10;
    }
    ai->timer = ai->interval;
    return ai;
}

void PokeathlonCirclePushAi_SetMon(PokeathlonCirclePushAi *ai, u8 slot, u16 speed, u16 *stamina, VecFx32 *position, u32 *state, VecFx32 *target, PokeathlonCourseMonObject *object) {
    PokeathlonCirclePushAiMon *mons = ai->mons;
    mons[slot].speed = speed;
    mons[slot].stamina = stamina;
    mons[slot].position = position;
    mons[slot].state = state;
    mons[slot].cell = -1;
    mons[slot].slot = slot;
    mons[slot].object = object;
    if (slot / 3 >= ai->firstTeam) {
        mons[slot].target = target;
    } else {
        mons[slot].target = NULL;
    }
}

void PokeathlonCirclePushAi_Free(PokeathlonCirclePushAi *ai) {
    Heap_Free(ai);
}

// Every interval frames one Pokemon of each CPU team (in turn) gets a new target. Returns it
// (12: none).
int PokeathlonCirclePushAi_Update(PokeathlonCirclePushAi *ai, u8 layout) {
    int result = 12;
    int mon;

    if (ai->firstTeam2 == 4) {
        return result;
    }
    ai->timer++;
    if (ai->timer >= ai->interval) {
        u8 i;
        ai->moving = TRUE;
        ai->timer = 0;
        for (i = 0; i < ai->teamCount; i++) {
            ai->skipped[i] = 0;
        }
    }
    if (ai->moving != FALSE) {
        u8 firstTeam = ai->firstTeam;
        mon = 2 - ai->member + (firstTeam + ai->team) * 3;
        if (mon / 3 < firstTeam) {
            GF_AssertFail();
        }
        PokeathlonCirclePushAi_ChooseTarget(ai, (u8)mon, layout);
        result = (u8)mon;
        ai->team++;
        if (ai->team >= ai->teamCount) {
            ai->team = 0;
            ai->member = (ai->member + 1) % 3;
            ai->moving = FALSE;
        }
    }
    return result;
}

// A CPU Pokemon picks a target cell around it (or anywhere, if no cell nearby is good):
// bonus zones, cells nobody else wants, staying away from stronger Pokemon.

static void PokeathlonCirclePushAi_ChooseTarget(PokeathlonCirclePushAi *ai, u8 index, u8 layout) {
    PokeathlonCirclePushAiCell *all[25];
    PokeathlonCirclePushAiCell *choices[25];
    PokeathlonCirclePushAiZone zones[4];
    PokeathlonCirclePushAiCell *near[9];
    PokeathlonCirclePushAiCell *far[16];
    int ownX;
    int ownY;
    int x;
    int y;
    s32 radius;
    int nearX;
    int nearY;
    int farX;
    int farY;
    s16 column;
    s16 row;
    PokeathlonCirclePushAi *work = ai;
    PokeathlonCirclePushAiMon *mon = &work->mons[index];
    int nearCount;
    int farCount;
    int i;
    int count;
    PokeathlonCirclePushAiCell *choice;

    if (!*mon->state) {
        return;
    }
    if (!mon->target) {
        GF_ASSERT(0);
        return;
    }
    if (work->skipped[work->member] < 3 && (int)LCRandom() % 100 < 30) {
        work->skipped[work->member]++;
        return;
    }
    for (i = 0; i < 12; i++) {
        PokeathlonCirclePushAiMon *other = &work->mons[i];

        if (*other->state) {
            PokeathlonCourseObject_GetCollisionCircle(other->object, other->position->x, other->position->y, &x, &y, &radius);
            other->cell = PokeathlonCirclePushAi_GetCell(x / FX32_ONE, y / FX32_ONE);
        } else {
            other->cell = -1;
        }
    }
    PokeathlonCourseObject_CenterToGround(mon->object, mon->position->x / FX32_ONE, mon->position->y / FX32_ONE, &ownX, &ownY);
    mon->cell = PokeathlonCirclePushAi_GetCell(ownX, ownY);
    PokeathlonCirclePush_GetZoneCircles(layout, zones);
    for (i = 0; i < 4; i++) {
        if (zones[i].radius > 0) {
            work->zoneCells[i] = PokeathlonCirclePushAi_GetCell(zones[i].x, zones[i].y);
        } else {
            work->zoneCells[i] = -1;
        }
    }
    if (!PokeathlonCirclePushAi_GetColumnRow((u16)ownX, (u16)ownY, &column, &row)) {
        GF_ASSERT(0);
    }
    nearCount = 0;
    farCount = 0;
    for (i = 0; i < 25; i++) {
        int dx;
        int dy;

        work->cells[i].index = i;
        work->cells[i].x = i % 5 - 2 + column;
        work->cells[i].y = i / 5 - 2 + row;
        if (work->cells[i].x < 0 || work->cells[i].y < 0 || work->cells[i].x >= 7 || work->cells[i].y >= 5) {
            work->cells[i].inRing = FALSE;
        } else {
            work->cells[i].inRing = TRUE;
        }
        dx = work->cells[i].x - column;
        dy = work->cells[i].y - row;
        all[i] = &work->cells[i];
        if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
            near[nearCount++] = &work->cells[i];
        } else {
            far[farCount++] = &work->cells[i];
        }
    }
    GF_ASSERT(nearCount == 9);
    GF_ASSERT(farCount == 16);
    PokeathlonCirclePushAi_PlaceInCells(9, near);
    near[4]->pixelX = ownX;
    near[4]->pixelY = ownY;
    PokeathlonCirclePushAi_ScoreCells(work, 9, zones, mon, near);
    count = PokeathlonCirclePushAi_SortCells(9, near, choices);
    if (choices[0]->score >= 3) {
        choice = choices[(u8)((int)LCRandom() % count)];
        PokeathlonCourseObject_GroundToCenter(mon->object, choice->pixelX, choice->pixelY, &nearX, &nearY);
        mon->target->x = (u32)nearX << FX32_SHIFT;
        mon->target->y = (u32)nearY << FX32_SHIFT;
        mon->target->z = 0;
        return;
    }
    PokeathlonCirclePushAi_PlaceInCells(16, far);
    PokeathlonCirclePushAi_ScoreCells(work, 25, zones, mon, all);
    count = PokeathlonCirclePushAi_SortCells(25, all, choices);
    choice = choices[(u8)((int)LCRandom() % count)];
    PokeathlonCourseObject_GroundToCenter(mon->object, choice->pixelX, choice->pixelY, &farX, &farY);
    mon->target->x = (u32)farX << FX32_SHIFT;
    mon->target->y = (u32)farY << FX32_SHIFT;
    mon->target->z = 0;
}

// A random point of each cell (cells are 32 pixels, from (16, 16)).
static void PokeathlonCirclePushAi_PlaceInCells(u8 count, PokeathlonCirclePushAiCell **cells) {
    u8 i;

    for (i = 0; i < count; i++) {
        PokeathlonCirclePushAiCell *cell = cells[i];
        if (cell->inRing) {
            cell->pixelX = cell->x * 32 + 16 + LCRandom() % 32;
            cell = cells[i];
            cell->pixelY = cell->y * 32 + 16 + LCRandom() % 32;
            cell = cells[i];
            cell->cellIndex = cell->x + cell->y * 7;
        } else {
            cell->pixelX = -1;
            cells[i]->pixelY = -1;
            cells[i]->cellIndex = -1;
        }
    }
}

static void PokeathlonCirclePushAi_ScoreCells(PokeathlonCirclePushAi *ai, u8 count, PokeathlonCirclePushAiZone *zones, PokeathlonCirclePushAiMon *mon, PokeathlonCirclePushAiCell **cells) {
    u8 i;

    for (i = 0; i < count; i++) {
        if (!cells[i]->inRing) {
            cells[i]->score = -100;
        } else {
            BOOL inZone;
            cells[i]->score = PokeathlonCirclePushAi_ScoreCell(ai, zones, cells[i]->cellIndex, mon, cells[i]->pixelX, cells[i]->pixelY, &inZone);
            if (inZone != FALSE) {
                PokeathlonCirclePushAi_SnapToZone(ai, zones, (u8)cells[i]->cellIndex, &cells[i]->pixelX, &cells[i]->pixelY);
            }
        }
    }
}

static int PokeathlonCirclePushAi_GetCell(int x, int y) {
    int column = (x - 16) / 32;
    int row = (y - 16) / 32;
    int cell;

    if (column < 0 || row < 0) {
        return -1;
    }
    cell = row * 7 + column;
    if (cell > 34) {
        cell = -1;
    }
    return cell;
}

static int PokeathlonCirclePushAi_ScoreCell(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, int cell, PokeathlonCirclePushAiMon *mon, int x, int y, BOOL *inZone) {
    int score = 0;
    if (cell < 0) {
        return score;
    }
    *inZone = FALSE;
    if (*mon->stamina != 0 && *mon->state == CIRCLE_PUSH_MON_IN_RING) {
        if (PokeathlonCirclePushAi_IsInZone(zones, x << FX32_SHIFT, y << FX32_SHIFT)) {
            score += 5;
        }
        if (PokeathlonCirclePushAi_IsZoneCell(ai, (u8)cell)) {
            score += 4;
            *inZone = TRUE;
        }
        score += PokeathlonCirclePushAi_CountRivals(ai->mons, mon, cell);
    } else {
        int rivals;
        if (PokeathlonCirclePushAi_IsInZone(zones, x << FX32_SHIFT, y << FX32_SHIFT)) {
            score += 5;
        }
        if (PokeathlonCirclePushAi_IsZoneCell(ai, (u8)cell)) {
            score += 4;
            *inZone = TRUE;
        }
        rivals = PokeathlonCirclePushAi_CountRivals(ai->mons, mon, cell);
        if (rivals == 0) {
            score += 2;
        }
        if (mon->cell == cell) {
            score += 2;
        }
        if (rivals != 0) {
            score = 0;
        }
        if (PokeathlonCirclePushAi_IsInZone(zones, mon->position->x, mon->position->y)) {
            score += 4;
        }
    }
    return score;
}

#pragma opt_loop_invariants off

// Minus one for every other Pokemon in the cell, two if it is a faster rival.

static int PokeathlonCirclePushAi_CountRivals(const PokeathlonCirclePushAiMon *mons, const PokeathlonCirclePushAiMon *mon, int cell) {
    int result;
    int i;
    int slot;
    if (mon->cell == -1) {
        return 0;
    }
    slot = mon->slot;
    result = 0;
    for (i = 0; i < 12; i++, mons++) {
        if (i != slot && cell == mons->cell) {
            if (mon->speed < mons->speed) {
                if (slot / 3 == mons->slot / 3) {
                    result--;
                } else {
                    result -= 2;
                }
            } else {
                result--;
            }
        }
    }
    return result;
}

#pragma opt_loop_invariants reset

static BOOL PokeathlonCirclePushAi_IsInZone(PokeathlonCirclePushAiZone *zones, fx32 x, fx32 y) {
    VecFx32 point;
    VecFx32 delta;
    s32 i;

    point.x = x;
    point.y = y;
    point.z = 0;
    for (i = 0; i < 4; i++, zones++) {
        if (zones->radius <= 0) {
            break;
        }
        delta.x = zones->x << FX32_SHIFT;
        delta.y = zones->y << FX32_SHIFT;
        delta.z = 0;
        VEC_Subtract(&point, &delta, &delta);
        if (VEC_Mag(&delta) < zones->radius << FX32_SHIFT) {
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL PokeathlonCirclePushAi_IsZoneCell(PokeathlonCirclePushAi *ai, u8 cell) {
    s32 i;
    for (i = 0; i < 4; i++) {
        s32 zoneCell = ai->zoneCells[i];
        if (zoneCell == -1) {
            break;
        }
        if (cell == zoneCell) {
            return TRUE;
        }
    }
    return FALSE;
}

static void PokeathlonCirclePushAi_SnapToZone(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, s32 cell, s32 *x, s32 *y) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (cell == ai->zoneCells[i]) {
            *x = zones[i].x;
            *y = zones[i].y;
            return;
        }
    }
}

static BOOL PokeathlonCirclePushAi_GetColumnRow(s32 x, s32 y, s16 *column, s16 *row) {
    *column = (x - 16) / 32;
    *row = (y - 16) / 32;
    if (*column < 0 || *row < 0) {
        return FALSE;
    }
    return TRUE;
}

// Sorts the cells by score (insertion into a list); returns how many share the best score.

static u8 PokeathlonCirclePushAi_SortCells(u32 count, PokeathlonCirclePushAiCell **cells, PokeathlonCirclePushAiCell **sorted) {
    u8 best;
    CirclePushSortNode nodes[26];
    u8 i;
    u8 j;
    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    best = 0;
    nodes[0].cell = NULL;
    for (i = 0; i < count; i++) {
        CirclePushSortNode *node = &nodes[i + 1];
        CirclePushSortNode *pos;
        node->cell = cells[i];
        node->next = NULL;
        node->prev = NULL;
        pos = &nodes[0];
        for (j = 0; j < i; j++) {
            pos = pos->next;
            if (pos->cell->score < node->cell->score) {
                pos->prev->next = node;
                node->prev = pos->prev;
                node->next = pos;
                pos->prev = node;
                break;
            }
        }
        if (j == i) {
            pos->next = node;
            node->prev = pos;
        }
    }
    {
        CirclePushSortNode *pos = nodes[0].next;
        int bestScore = pos->cell->score;
        pos = &nodes[0];
        for (i = 0; i < count; i++) {
            pos = pos->next;
            sorted[i] = pos->cell;
            if (bestScore == sorted[i]->score) {
                best++;
            }
        }
    }
    if (best == 0) {
        GF_AssertFail();
    }
    return best;
}

PokeathlonCirclePushDisplay *PokeathlonCirclePushDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonCirclePushDisplay *display = Heap_Alloc(heapId, sizeof(PokeathlonCirclePushDisplay));
    MI_CpuFill8(display, 0, sizeof(PokeathlonCirclePushDisplay));
    display->heapId = heapId;
    display->bgConfig = bgConfig;
    display->course = course;
    PokeathlonCirclePushDisplay_CreateWindows(display);
    display->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, heapId);
    display->msgFormat = MessageFormat_New(heapId);
    return display;
}

void PokeathlonCirclePushDisplay_Free(PokeathlonCirclePushDisplay *display) {
    s32 i;
    for (i = 0; i < 12; i++) {
        Heap_Free(display->anim.monPics[i].charData);
        Heap_Free(display->anim.monPics[i].paletteData);
    }
    {
        PokeathlonCirclePushDisplay *fileView;
        s32 j;
        j = 0;
        fileView = display;
        for (; j < 4; j++, fileView = (PokeathlonCirclePushDisplay *)((u8 *)fileView + sizeof(void *))) {
            Heap_Free(fileView->anim.screenFiles[0]);
        }
        Heap_Free(display->anim.screenFiles[4]);
        {
            PokeathlonCirclePushDisplay *lastView;
            s32 k;
            k = 0;
            lastView = display;
            for (; k < 2; k++, lastView = (PokeathlonCirclePushDisplay *)((u8 *)lastView + sizeof(void *))) {
                Heap_Free(lastView->anim.screenFiles[5]);
            }
            MessageFormat_Delete(display->msgFormat);
            DestroyMsgData(display->msgData);
            {
                Window *window;
                s32 n = 0;
                window = display->windows;
                for (; n < 5; n++, window++) {
                    RemoveWindow(window);
                }
            }
        }
    }
    Heap_Free(display);
}

// Bottom screen frame (sub BG 1) and the screens of the top screen's animations.

void PokeathlonCirclePushDisplay_LoadScreens(PokeathlonCirclePushDisplay *display) {
    NNSG2dScreenData *screen;
    void *file = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 13, FALSE, &screen, display->heapId);
    BG_LoadScreenTilemapData(display->bgConfig, GF_BG_LYR_SUB_1, screen->rawData, screen->szByte);
    BgTilemapRectChangePalette(display->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 32, 24, 2);
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(file);
    display->anim.screenFiles[5] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 23, FALSE, &display->anim.zoomBg3Screen, display->heapId);
    display->anim.screenFiles[6] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 24, FALSE, &display->anim.matchupBg3Screen, display->heapId);
    BG_LoadScreenTilemapData(display->bgConfig, GF_BG_LYR_SUB_3, display->anim.zoomBg3Screen->rawData, display->anim.zoomBg3Screen->szByte);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    display->anim.screenFiles[0] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 18, FALSE, &display->anim.scoreboardScreen, display->heapId);
    display->anim.screenFiles[1] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 19, FALSE, &display->anim.matchupBg2Screen, display->heapId);
    display->anim.screenFiles[2] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 17, FALSE, &display->anim.zoomBg2Screen, display->heapId);
    display->anim.screenFiles[3] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 20, FALSE, &display->anim.monPicScreen, display->heapId);
    display->anim.screenFiles[4] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 16, FALSE, &display->anim.digitsScreen, display->heapId);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.scoreboardScreen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.matchupBg2Screen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.monPicScreen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.zoomBg2Screen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.digitsScreen);
    BG_LoadScreenTilemapData(display->bgConfig, GF_BG_LYR_SUB_2, display->anim.scoreboardScreen->rawData, display->anim.scoreboardScreen->szByte);
    PokeathlonCirclePushDisplayAnim_PrintScore(&display->anim, display->bgConfig);
    BgTilemapRectChangePalette(display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 24, 3);
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_2);
}

void PokeathlonCirclePushDisplay_LoadPics(PokeathlonCirclePushDisplay *display) {
    PokeathlonCirclePushDisplay_LoadMonPics(display);
}

// Resources of the two picture sprites (NARC 93).
void PokeathlonCirclePushDisplay_LoadSpriteResources(PokeathlonCourseSpritePool *pool, PokeathlonCirclePushDisplay *display) {
    u32 resIds[2];
    u32 *resId = resIds;
    int i;
    resIds[0] = gPokeathlonCirclePushNameplateCoords[2];
    resIds[1] = gPokeathlonCirclePushNameplateCoords[3];
    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePool_LoadChar(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000009_NCGR, *resId, 2);
        PokeathlonCourseSpritePool_LoadPltt(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000006_NCLR, *resId, 2, 1);
        PokeathlonCourseSpritePool_LoadCell(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000010_NCER, *resId);
        PokeathlonCourseSpritePool_LoadAnim(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000010_NCER, *resId);
        resId++;
    }
}
