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

typedef void (*GridMoveCallback)(GridCursor *, GridPosition *, const u8 *, int, int);
typedef void (*GridSelectCallback)(GridCursor *, GridPosition *, const u8 *, int);

typedef struct GridEntry {
    const u8 *dimensions;
    GridMoveCallback move;
    GridSelectCallback select;
} GridEntry;

struct GridCursor {
    u8 current;
    u8 count;
    u8 unk_02;
    u8 padding;
    void *userData;
    const GridEntry *entries;
    GridPosition *positions;
};

typedef struct GridMove {
    u32 type;
    u8 value;
} GridMove;

extern const GridEntry gGridCursorPanels[6];

void GridPanel2_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridPanel2_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
void GridPanel1_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridPanel1_Enter(void *unused, GridPosition *position);
void GridPanel3_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridPanel3_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
void GridPanel4_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridPanel4_Enter(void);
void GridPanel5_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridPanel5_Enter(void *unused, GridPosition *position);
void GridPanel6_Move(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridPanel6_Enter(GridCursor *cursor, GridPosition *position, const u8 *dimensions, const GridMove *move);
int GridCursor_StepPosition(GridCursor *cursor, GridPosition *position, const u8 *dimensions, s8 dx, s8 dy);
void GridCursor_SelectPanel(GridCursor *cursor, u8 type);
void GridCursor_EnterPanel(GridCursor *cursor, u8 type, int arg);
GridCursor *GridCursor_New(enum HeapID heapId, void *userData);
void GridCursor_Init(GridCursor *cursor, u8 value);
void GridCursor_Delete(GridCursor *cursor);
void GridCursor_SetPosition(GridCursor *cursor, u8 type, u8 index);
BOOL GridCursor_Move(GridCursor *cursor, int dx, int dy);
u8 GridCursor_GetPanel(GridCursor *cursor);
u8 GridCursor_GetIndex(GridCursor *cursor);

#endif
