#include "grid_cursor.h"

#include <nitro/fs/overlay.h>

#include "global.h"

#include "heap.h"

// StepPosition reads position->y and returns 1 through the field overlay's id (1): kept, it is how the code matches.
FS_EXTERN_OVERLAY(field);

extern void GF_AssertFail(void);

void GridPanel2_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    switch (move.type) {
    default:
        break;
    case 1:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, (int)&move);
        return;
    case 2:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, (int)&move);
        return;
    case 3:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, (int)&move);
        return;
    case 4:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, (int)&move);
        return;
    }
}

void GridPanel2_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    switch (move->type) {
    case 4:
        position->x = 2;
        position->y = 0;
        position->index = position->x + position->y * dimensions[0];
        break;
    case 3:
        position->x = 2;
        position->y = 4;
        position->index = position->x + position->y * dimensions[0];
        break;
    case 1:
        position->x = 5;
        position->y = move->value;
        position->index = position->x + position->y * dimensions[0];
        break;
    case 2:
        position->x = 0;
        position->y = move->value;
        position->index = position->y * dimensions[0];
        break;
    }
}

void GridPanel1_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = position->x;
    if (move.type != 1) {
        if (move.type == 3) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 3, (int)&move);
            return;
        }
    } else {
        if (*(u8 *)cursor->userData == 0x12) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 6, (int)&move);
            return;
        }
        move.value = 4;
        GridCursor_EnterPanel(cursor, 2, (int)&move);
        return;
    }
}

void GridPanel1_Enter(void *unused, GridPosition *position) {
    position->x = 0;
    position->y = 0;
    position->index = 0;
}

void GridPanel3_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
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
    case 1:
        if (*(u8 *)cursor->userData == 0x12) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 6, (int)&move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, (int)&move);
        return;
    case 2:
        if (*(u8 *)cursor->userData == 0x12) {
            move.value = 0;
            GridCursor_EnterPanel(cursor, 6, (int)&move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, (int)&move);
        return;
    case 4:
        if (cursor->unk_02 != 0) {
            GridCursor_EnterPanel(cursor, 1, (int)&move);
        }
        return;
    }
}

void GridPanel3_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    switch (move->type) {
    case 3:
        position->index = ((dimensions[2] - 1) / dimensions[0]) * dimensions[0];
        position->x = position->index % dimensions[0];
        position->y = position->index / dimensions[0];
        break;
    case 1:
    case 2:
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

void GridPanel4_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = 0;
    switch (move.type) {
    default:
        break;
    case 3:
        GridCursor_EnterPanel(cursor, 4, (int)&move);
        return;
    case 4:
        GridCursor_EnterPanel(cursor, 4, (int)&move);
        return;
    }
}

void GridPanel4_Enter(void) {
}

void GridPanel5_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = position->x;
    switch (move.type) {
    default:
        break;
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        if (*(u8 *)cursor->userData == 0x12) {
            GridCursor_EnterPanel(cursor, 6, (int)&move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, (int)&move);
        return;
    case 4:
        if (*(u8 *)cursor->userData == 0x12) {
            GridCursor_EnterPanel(cursor, 6, (int)&move);
            return;
        }
        GridCursor_EnterPanel(cursor, 2, (int)&move);
        return;
    }
}

void GridPanel5_Enter(void *unused, GridPosition *position) {
    position->x = 0;
    position->y = 0;
    position->index = 0;
}

void GridPanel6_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    GridMove move;

    move.type = GridCursor_StepPosition(cursor, position, dimensions, dx, dy);
    move.value = position->x;
    switch (move.type) {
    default:
        break;
    case 1:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, (int)&move);
        return;
    case 2:
        move.value = position->y;
        GridCursor_EnterPanel(cursor, 3, (int)&move);
        return;
    case 3:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, (int)&move);
        return;
    case 4:
        move.value = position->x;
        GridCursor_EnterPanel(cursor, 5, (int)&move);
        return;
    }
}

void GridPanel6_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move) {
    switch (move->type) {
    case 3:
    case 4:
        position->x = 2;
        position->y = 0;
        position->index = position->x + position->y * dimensions[0];
        break;
    case 1:
        position->x = 5;
        position->y = 0;
        position->index = position->x + position->y * dimensions[0];
        break;
    case 2:
        position->x = 0;
        position->y = 0;
        position->index = position->y * dimensions[0];
        break;
    }
}

int GridCursor_StepPosition(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy) {
    s8 x;
    s8 y;
    int result = 0;
    u8 width = dimensions[0];
    u8 height = dimensions[1];

    if (dx != 0) {
        x = position->x + dx;
        y = ((s8 *)position)[FS_OVERLAY_ID(field)];
        if (x < 0) {
            result = FS_OVERLAY_ID(field);
        } else if (x >= width) {
            result = 2;
        } else if ((u8)(x + y * width) >= dimensions[2]) {
            result = 2;
        }
        if (result != 0) {
            goto done;
        }
        position->x = x;
        position->index = x + y * width;
    }

    if (dy != 0) {
        x = position->x;
        y = position->y + dy;
        if (y < 0) {
            result = 3;
        } else if (y >= height) {
            result = 4;
        } else if ((u8)(x + y * width) >= dimensions[2]) {
            result = 4;
        }
        if (result != 0) {
            goto done;
        }
        position->y = y;
        position->index = x + y * width;
    }
    return 0;

done:
    return result;
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

void GridCursor_EnterPanel(GridCursor *cursor, u8 type, int arg) {
    GridCursor_SelectPanel(cursor, type);
    cursor->entries[cursor->current].select(cursor, &cursor->positions[cursor->current], cursor->entries[cursor->current].dimensions, arg);
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

void GridCursor_Init(GridCursor *cursor, u8 value) {
    cursor->current = 0;
    cursor->entries = gGridCursorPanels;
    cursor->unk_02 = value;
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

BOOL GridCursor_Move(GridCursor *cursor, int arg1, int arg2) {
    volatile int savedArg2 = arg2;
    u8 oldCurrent = ((volatile GridCursor *)cursor)->current;
    GridPosition *position = &cursor->positions[oldCurrent];
    const GridEntry *entry = &cursor->entries[oldCurrent];
    u8 oldIndex = position->index;

    entry->move(cursor, position, entry->dimensions, arg1, savedArg2);
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
