#ifndef POKEHEARTGOLD_GRID_CURSOR_H
#define POKEHEARTGOLD_GRID_CURSOR_H

#include "global.h"

#include "heap.h"

typedef struct GridCursor GridCursor;

typedef struct GridPosition {
    s8 x;
    s8 y;
    u8 index;
    u8 padding;
} GridPosition;

enum GridExit {
    GRID_EXIT_NONE,
    GRID_EXIT_LEFT,
    GRID_EXIT_RIGHT,
    GRID_EXIT_UP,
    GRID_EXIT_DOWN,
};

typedef struct GridMove {
    u32 type;
    u8 value;
} GridMove;

typedef void (*GridMoveCallback)(GridCursor *, GridPosition *, const u8 *, s8, s8);
typedef void (*GridSelectCallback)(GridCursor *, GridPosition *, const u8 *, const GridMove *);

typedef struct GridEntry {
    const u8 *dimensions;
    GridMoveCallback move;
    GridSelectCallback select;
} GridEntry;

struct GridCursor {
    u8 current;
    u8 count;
    u8 canCancel;
    u8 padding;
    u8 *page; // the box mon selector page
    const GridEntry *entries;
    GridPosition *positions;
};

int GridCursor_StepPosition(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridCursor_SelectPanel(GridCursor *cursor, u8 type);
void GridCursor_EnterPanel(GridCursor *cursor, u8 type, const GridMove *move);
GridCursor *GridCursor_New(enum HeapID heapId, u8 *page);
void GridCursor_Init(GridCursor *cursor, u8 canCancel);
void GridCursor_Delete(GridCursor *cursor);
void GridCursor_SetPosition(GridCursor *cursor, u8 type, u8 index);
BOOL GridCursor_Move(GridCursor *cursor, s8 dx, s8 dy);
u8 GridCursor_GetPanel(GridCursor *cursor);
u8 GridCursor_GetIndex(GridCursor *cursor);

#endif
