#include "grid_cursor.h"

#include <nitro/fs/overlay.h>

#include "global.h"

#include "heap.h"

// StepPosition uses the field overlay's id (1) both to read position->y and as its left edge result.
FS_EXTERN_OVERLAY(field);

static void GridPanel2_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
static void GridPanel2_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
static void GridPanel1_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
static void GridPanel1_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
static void GridPanel3_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
static void GridPanel3_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
static void GridPanel4_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
static void GridPanel4_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
static void GridPanel5_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
static void GridPanel5_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
static void GridPanel6_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
static void GridPanel6_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);

static const u8 sGridPanel2Dimensions[] = { 6, 5, 30, 2 };
static const u8 sGridPanel1Dimensions[] = { 1, 1, 1, 1 };
static const u8 sGridPanel3Dimensions[] = { 1, 3, 3, 3 };
static const u8 sGridPanel5Dimensions[] = { 1, 1, 1, 5 };
static const u8 sGridPanel4Dimensions[] = { 1, 3, 3, 4 };
static const u8 sGridPanel6Dimensions[] = { 6, 1, 6, 6 };

static const GridEntry sGridCursorPanels[] = {
    { sGridPanel2Dimensions, GridPanel2_Move, GridPanel2_Enter },
    { sGridPanel1Dimensions, GridPanel1_Move, GridPanel1_Enter },
    { sGridPanel3Dimensions, GridPanel3_Move, GridPanel3_Enter },
    { sGridPanel4Dimensions, GridPanel4_Move, GridPanel4_Enter },
    { sGridPanel5Dimensions, GridPanel5_Move, GridPanel5_Enter },
    { sGridPanel6Dimensions, GridPanel6_Move, GridPanel6_Enter },
};

static void GridPanel2_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    switch (move.type) {
    default:
        break;
    case GRID_EXIT_LEFT:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, &move);
        return;
    case GRID_EXIT_RIGHT:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, &move);
        return;
    case GRID_EXIT_UP:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, &move);
        return;
    case GRID_EXIT_DOWN:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, &move);
        return;
    }
}

static void GridPanel2_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    switch (move->type) {
    case GRID_EXIT_DOWN:
        position->x = 2;
        position->y = 0;
        position->index = position->x + position->y * dimensions[0];
        break;
    case GRID_EXIT_UP:
        position->x = 2;
        position->y = 4;
        position->index = position->x + position->y * dimensions[0];
        break;
    case GRID_EXIT_LEFT:
        position->x = 5;
        position->y = move->value;
        position->index = position->x + position->y * dimensions[0];
        break;
    case GRID_EXIT_RIGHT:
        position->x = 0;
        position->y = move->value;
        position->index = position->y * dimensions[0];
        break;
    }
}

static void GridPanel1_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = position->x;
    if (move.type != GRID_EXIT_LEFT) {
        if (move.type == GRID_EXIT_UP) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 3, &move);
            return;
        }
    } else {
        if (*(u8 *)cursor->userData == 0x12) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 6, &move);
            return;
        }
        move.value = 4;
        GridCursor_EnterPanel(cursor, 2, &move);
        return;
    }
}

static void GridPanel1_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    position->x = 0;
    position->y = 0;
    position->index = 0;
}

static void GridPanel3_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;
    int type;

    type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.type = type;
    if (position->y == 0) {
        move.value = 0;
    } else if (position->y == 1) {
        move.value = 2;
    } else {
        move.value = 3;
    }
    switch (type) {
    case GRID_EXIT_LEFT:
        if (*(u8 *)cursor->userData == 0x12) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 6, &move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, &move);
        return;
    case GRID_EXIT_RIGHT:
        if (*(u8 *)cursor->userData == 0x12) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 6, &move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, &move);
        return;
    case GRID_EXIT_DOWN:
        if (cursor->canCancel != 0) {
            GridCursor_EnterPanel(cursor, 1, &move);
        }
        return;
    }
}

static void GridPanel3_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    switch (move->type) {
    case GRID_EXIT_UP:
        position->index = ((dimensions[2] - 1) / dimensions[0]) * dimensions[0];
        position->x = position->index % dimensions[0];
        position->y = position->index / dimensions[0];
        break;
    case GRID_EXIT_LEFT:
    case GRID_EXIT_RIGHT:
        if (move->value < 2) {
            position->index = 0;
        } else if (move->value > 2) {
            position->index = 2;
        } else {
            position->index = 1;
        }
        position->x = 0;
        position->y = position->index;
        break;
    }
}

static void GridPanel4_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = 0;
    switch (move.type) {
    default:
        break;
    case GRID_EXIT_UP:
        GridCursor_EnterPanel(cursor, 4, &move);
        return;
    case GRID_EXIT_DOWN:
        GridCursor_EnterPanel(cursor, 4, &move);
        return;
    }
}

static void GridPanel4_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
}

static void GridPanel5_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = position->x;
    switch (move.type) {
    default:
        break;
    case GRID_EXIT_NONE:
        break;
    case GRID_EXIT_LEFT:
        break;
    case GRID_EXIT_RIGHT:
        break;
    case GRID_EXIT_UP:
        if (*(u8 *)cursor->userData == 0x12) {
            GridCursor_EnterPanel(cursor, 6, &move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, &move);
        return;
    case GRID_EXIT_DOWN:
        if (*(u8 *)cursor->userData == 0x12) {
            GridCursor_EnterPanel(cursor, 6, &move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, &move);
        return;
    }
}

static void GridPanel5_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    position->x = 0;
    position->y = 0;
    position->index = 0;
}

static void GridPanel6_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = position->x;
    switch (move.type) {
    default:
        break;
    case GRID_EXIT_LEFT:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, &move);
        return;
    case GRID_EXIT_RIGHT:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, &move);
        return;
    case GRID_EXIT_UP:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, &move);
        return;
    case GRID_EXIT_DOWN:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, &move);
        return;
    }
}

static void GridPanel6_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    switch (move->type) {
    case GRID_EXIT_UP:
    case GRID_EXIT_DOWN:
        position->x = 2;
        position->y = 0;
        position->index = position->x + position->y * dimensions[0];
        break;
    case GRID_EXIT_LEFT:
        position->x = 5;
        position->y = 0;
        position->index = position->x + position->y * dimensions[0];
        break;
    case GRID_EXIT_RIGHT:
        position->x = 0;
        position->y = 0;
        position->index = position->y * dimensions[0];
        break;
    }
}

int GridCursor_StepPosition(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    s8 x;
    s8 y;
    int result = GRID_EXIT_NONE;
    u8 width = dimensions[0];
    u8 height = dimensions[1];

    if (dx != 0) {
        x = position->x + dx;
        y = position->y;
        if (x < 0) {
            result = GRID_EXIT_LEFT;
        } else if (x >= width) {
            result = GRID_EXIT_RIGHT;
        } else if ((u8)(x + y * width) >= dimensions[2]) {
            result = GRID_EXIT_RIGHT;
        }
        if (result != GRID_EXIT_NONE) {
            return result;
        }
        position->x = x;
        position->index = x + y * width;
    }

    if (dy != 0) {
        x = position->x;
        y = position->y + dy;
        if (y < 0) {
            result = GRID_EXIT_UP;
        } else if (y >= height) {
            result = GRID_EXIT_DOWN;
        } else if ((u8)(x + y * width) >= dimensions[2]) {
            result = GRID_EXIT_DOWN;
        }
        if (result != GRID_EXIT_NONE) {
            return result;
        }
        position->y = y;
        position->index = x + y * width;
    }
    return GRID_EXIT_NONE;
}

void GridCursor_SelectPanel(GridCursor *cursor, u8 type) {
    int i;

    for (i = 0; i < cursor->count; i++) {
        if (cursor->entries[i].dimensions[3] == type) {
            cursor->current = i;
            return;
        }
    }
}

void GridCursor_EnterPanel(GridCursor *cursor, u8 type, const GridMove *move) {
    GridCursor_SelectPanel(cursor, type);
    cursor->entries[cursor->current].select(cursor, &cursor->positions[cursor->current], cursor->entries[cursor->current].dimensions, move);
}

GridCursor *GridCursor_New(enum HeapID heapId, void *userData) {
    GridCursor *cursor = Heap_Alloc(heapId, sizeof(GridCursor));
    u32 i;

    {
        u8 *p = (u8 *)cursor;
        for (i = sizeof(GridCursor); i != 0; i--) {
            *p++ = 0;
        }
    }
    cursor->count = 6;
    cursor->positions = Heap_Alloc(heapId, cursor->count * sizeof(GridPosition));
    memset(cursor->positions, 0, cursor->count * sizeof(GridPosition));
    cursor->userData = userData;
    return cursor;
}

void GridCursor_Init(GridCursor *cursor, u8 canCancel) {
    cursor->current = 0;
    cursor->entries = sGridCursorPanels;
    cursor->canCancel = canCancel;
}

void GridCursor_Delete(GridCursor *cursor) {
    Heap_Free(cursor->positions);
    Heap_Free(cursor);
}

void GridCursor_SetPosition(GridCursor *cursor, u8 type, u8 index) {
    GridPosition *position;
    const u8 *dimensions;

    GridCursor_SelectPanel(cursor, type);
    position = &cursor->positions[cursor->current];
    dimensions = cursor->entries[cursor->current].dimensions;
    if (index >= dimensions[2]) {
        GF_AssertFail();
    }
    position->index = index;
    if (position->index >= dimensions[2]) {
        position->index = 0;
    }
    position->x = position->index % dimensions[0];
    position->y = position->index / dimensions[0];
}

BOOL GridCursor_Move(GridCursor *cursor, s8 dx, s8 dy) {
    u8 oldCurrent = cursor->current;
    GridPosition *position = &cursor->positions[oldCurrent];
    const GridEntry *entry = &cursor->entries[oldCurrent];
    u8 oldIndex = position->index;

    entry->move(cursor, position, entry->dimensions, dx, dy);
    if (oldCurrent != cursor->current) {
        return TRUE;
    }
    if (oldIndex != position->index) {
        return TRUE;
    }
    return FALSE;
}

u8 GridCursor_GetPanel(GridCursor *cursor) {
    return cursor->entries[cursor->current].dimensions[3];
}

u8 GridCursor_GetIndex(GridCursor *cursor) {
    return cursor->positions[cursor->current].index;
}
