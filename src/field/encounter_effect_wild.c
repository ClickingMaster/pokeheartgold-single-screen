#include "global.h"

#include "field/encounter_effect.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "screen_fade_legacy.h"
#include "sprite.h"
#include "sys_task_api.h"

// The wild-battle effects 0-5 cover the screen with a grid of black (or white) squares; effect 46 (Red) fills it in
// random order and flashes the palette twice, and effects 2-3 scroll two pictures in with a wavy BG.

enum WildEffectBgLoaderState {
    BG_LOADER_STATE_LOAD,
    BG_LOADER_STATE_LOAD_SCREENS,
    BG_LOADER_STATE_END,
};

enum WildEffectBgLoaderVBlankState {
    BG_LOADER_VBLANK_BG1_CHARS,
    BG_LOADER_VBLANK_BG3_CHARS,
    BG_LOADER_VBLANK_END,
};

#define CELL_GRID_WIDTH  8
#define CELL_GRID_HEIGHT 6
#define CELL_GRID_SIZE   (CELL_GRID_WIDTH * CELL_GRID_HEIGHT)
#define CELL_SIZE        32

// One square that grows from a point (or a line) to CELL_SIZE x CELL_SIZE.
typedef struct WildEffectCell {
    EncounterEffectInterp width;
    EncounterEffectInterp height;
    Window *window;
    s16 x;
    s16 y;
    u8 active;
    u8 fillColor;
} WildEffectCell;

typedef struct ShuffledIndices {
    u16 values[128];
    u32 next;
    u32 count;
} ShuffledIndices;

// Fills the grid cell by cell in random order.
typedef struct WildEffectRandomFill {
    Window *window;
    ShuffledIndices order;
    u8 filled;
    u8 delay;
    s8 timer;
    u8 fillColor;
    u8 active;
} WildEffectRandomFill;

// Grows the grid's cells two at a time (one cell and its mirror image).
typedef struct WildEffectCellGrid {
    Window *window;
    WildEffectCell *cells[CELL_GRID_SIZE];
    u8 started;
    u8 finished;
    u8 delay;
    u8 growFrames;
    s8 timer;
    u8 fillColor;
    u8 active;
    u8 reverse : 1;
} WildEffectCellGrid;

typedef struct WildEffectSine {
    s16 value;
    s16 center;
    s16 amplitude;
    s16 speed;
    u16 angle;
} WildEffectSine;

typedef BOOL (*WildEffectStepFunc)(void *work, u32 *state);

typedef struct WildEffectStep {
    WildEffectStepFunc func;
    u32 state;
} WildEffectStep;

typedef struct RedEffectWork {
    Window *window;
    WildEffectRandomFill *fill;
    HBlankSystem *hBlankSystem;
    int step;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprite;
    WildEffectStep steps[3];
} RedEffectWork;

typedef struct CellGridEffectWork {
    Window *window;
    WildEffectCellGrid *grid;
} CellGridEffectWork;

// Loads the two pictures of the wave effect: graphics during VBlank, then the tilemaps.
typedef struct WildEffectBgLoader {
    EncounterEffectTask *effect;
    void *charFile1;
    void *charFile3;
    NNSG2dCharacterData *charData1;
    NNSG2dCharacterData *charData3;
    SysTask *task;
    SysTask *vBlankTask;
    u16 state;
    u16 vBlankState;
} WildEffectBgLoader;

typedef struct WaveEffectWork {
    EncounterEffectHBlankWave wave;
    EncounterEffectQuadInterp bg1Scroll;
    EncounterEffectQuadInterp bg3Scroll;
    WildEffectSine bg3Sine;
    int timer;
    u8 bg1Scrolling;
    u8 bg3Scrolling;
    u8 bg1Done;
    u8 bg3Done;
    WildEffectBgLoader bgLoader;
} WaveEffectWork;

static WildEffectCell *WildEffectCell_New(enum HeapID heapID);
static void WildEffectCell_Free(WildEffectCell *cell);
static void WildEffectCell_Start(WildEffectCell *cell, int x, int y, int duration, Window *window, s32 startWidth, s32 startHeight, s32 endWidth, s32 endHeight, u8 fillColor);
static BOOL WildEffectCell_Update(WildEffectCell *cell);
static WildEffectRandomFill *WildEffectRandomFill_New(enum HeapID heapID);
static void WildEffectRandomFill_Free(WildEffectRandomFill *fill);
static void WildEffectRandomFill_Start(WildEffectRandomFill *fill, u8 delay, Window *window, u8 fillColor);
static BOOL WildEffectRandomFill_Update(WildEffectRandomFill *fill);
static void ShuffledIndices_Init(ShuffledIndices *indices, u32 count);
static int ShuffledIndices_Next(ShuffledIndices *indices);
static WildEffectCellGrid *WildEffectCellGrid_New(enum HeapID heapID);
static void WildEffectCellGrid_Free(WildEffectCellGrid *grid);
static void WildEffectCellGrid_Start(WildEffectCellGrid *grid, u8 delay, u8 growFrames, Window *window, u8 fillColor, BOOL reverse);
static BOOL WildEffectCellGrid_UpdateRows(WildEffectCellGrid *grid);
static BOOL WildEffectCellGrid_UpdateDiagonals(WildEffectCellGrid *grid);
static void WildEffectSine_Init(WildEffectSine *sine, s16 start, s16 peak, s16 speed);
static void WildEffectSine_Update(WildEffectSine *sine);
static void WildEffectStep_Init(WildEffectStep *step, WildEffectStepFunc func);
static BOOL WildEffectStep_Run(WildEffectStep *step, void *work);
static BOOL RedEffect_FillScreen(RedEffectWork *work, u32 *state);
static BOOL RedEffect_FirstFlash(RedEffectWork *work, u32 *state);
static BOOL RedEffect_SecondFlash(RedEffectWork *work, u32 *state);
static void RedEffect_CreateSprite(NARC *narc, Sprite **sprite, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *spriteResources, int variant);
static void WildEffect_RunCellGridRows(SysTask *task, EncounterEffectTask *effect, BOOL white);
static void WildEffect_RunCellGridDiagonals(SysTask *task, EncounterEffectTask *effect, BOOL white);
static void *WildEffect_LoadCharData(NARC *narc, int member, NNSG2dCharacterData **charData, enum HeapID heapID);
static void WildEffect_LoadScreen(BgConfig *bgConfig, NARC *narc, int member, int bgId);
static void WildEffectBgLoader_VBlankTask(SysTask *task, void *data);
static void WildEffectBgLoader_Task(SysTask *task, void *data);
static void WildEffectBgLoader_Start(WildEffectBgLoader *loader, EncounterEffectTask *effect);
static BOOL WildEffectBgLoader_IsDone(WildEffectBgLoader *loader);
static void WildEffect_RunWave(SysTask *task, EncounterEffectTask *effect, BOOL white);

void SetBothScreensBrightnessByColor(u16 color);

extern const u32 gRedEffectSpriteResources[];
extern const u32 gRedEffectAnimMembers[];
extern const u32 gRedEffectCellMembers[];
extern const u32 gRedEffectPlttMembers[];
extern const BgTemplate gWildEffectWaveBgTemplate_Main1;
extern const BgTemplate gWildEffectWaveBgTemplate_Main3;
extern const u8 gWildEffectCellGridDiagonalsB[];
extern const u8 gWildEffectCellGridRows[];
extern const u8 gWildEffectCellGridDiagonalsA[];
extern const u8 gWildEffectCellGridRowsReverse[];

enum WildEffectState {
    WILD_EFFECT_STATE_INIT,
    WILD_EFFECT_STATE_START_FLASH,
    WILD_EFFECT_STATE_WAIT_FLASH,
    WILD_EFFECT_STATE_START,
    WILD_EFFECT_STATE_RUN,
    WILD_EFFECT_STATE_END,
};

enum RedEffectStepState {
    RED_STEP_STATE_START,
    RED_STEP_STATE_WAIT,
    RED_STEP_STATE_FINISH,
};

enum WaveEffectState {
    WAVE_EFFECT_STATE_LOAD_BG,
    WAVE_EFFECT_STATE_START_FLASH,
    WAVE_EFFECT_STATE_WAIT_FLASH,
    WAVE_EFFECT_STATE_START_WAVE,
    WAVE_EFFECT_STATE_WAIT_BG1_DELAY,
    WAVE_EFFECT_STATE_START_BG1_SCROLL,
    WAVE_EFFECT_STATE_WAIT_BG1_SCROLL,
    WAVE_EFFECT_STATE_END,
};

static WildEffectCell *WildEffectCell_New(enum HeapID heapID) {
    WildEffectCell *cell = Heap_Alloc(heapID, sizeof(WildEffectCell));
    memset(cell, 0, sizeof(WildEffectCell));
    return cell;
}

static void WildEffectCell_Free(WildEffectCell *cell) {
    Heap_Free(cell);
}

static void WildEffectCell_Start(WildEffectCell *cell, int x, int y, int duration, Window *window, s32 startWidth, s32 startHeight, s32 endWidth, s32 endHeight, u8 fillColor) {
    GF_ASSERT(!cell->active);
    EncounterEffectInterp_Init(&cell->width, startWidth, endWidth, duration);
    EncounterEffectInterp_Init(&cell->height, startHeight, endHeight, duration);
    cell->window = window;
    cell->x = x;
    cell->y = y;
    cell->fillColor = fillColor;
    cell->active = TRUE;
}

static BOOL WildEffectCell_Update(WildEffectCell *cell) {
    BOOL done;
    int halfWidth;
    int halfHeight;
    int x;
    int y;

    if (cell->active == FALSE) {
        return TRUE;
    }
    done = EncounterEffectInterp_Update(&cell->width);
    done |= EncounterEffectInterp_Update(&cell->height);
    halfWidth = cell->width.value / 2;
    x = cell->x;
    halfHeight = cell->height.value / 2;
    y = cell->y;
    EncounterEffect_FillWindowRect(cell->window, y - halfHeight, y + halfHeight, x - halfWidth, x + halfWidth, cell->fillColor);
    return done;
}

static WildEffectRandomFill *WildEffectRandomFill_New(enum HeapID heapID) {
    WildEffectRandomFill *fill = Heap_Alloc(heapID, sizeof(WildEffectRandomFill));
    memset(fill, 0, sizeof(WildEffectRandomFill));
    return fill;
}

static void WildEffectRandomFill_Free(WildEffectRandomFill *fill) {
    Heap_Free(fill);
}

static void WildEffectRandomFill_Start(WildEffectRandomFill *fill, u8 delay, Window *window, u8 fillColor) {
    fill->window = window;
    fill->fillColor = fillColor;
    fill->delay = delay;
    fill->filled = 0;
    fill->timer = 0;
    fill->active = TRUE;
    ShuffledIndices_Init(&fill->order, CELL_GRID_SIZE);
}

static BOOL WildEffectRandomFill_Update(WildEffectRandomFill *fill) {
    int index;
    int row;
    int column;
    s16 x;
    s16 y;

    GF_ASSERT(fill != NULL);
    if (fill->active == FALSE) {
        return TRUE;
    }
    if (fill->filled < CELL_GRID_SIZE) {
        fill->timer--;
        if (fill->timer <= 0) {
            fill->timer = fill->delay;
            index = ShuffledIndices_Next(&fill->order);
            column = index % CELL_GRID_WIDTH;
            row = (u32)index / CELL_GRID_WIDTH;
            x = column * CELL_SIZE;
            y = row * CELL_SIZE;
            EncounterEffect_FillWindowRect(fill->window, y, y + CELL_SIZE, x, x + CELL_SIZE, fill->fillColor);
            fill->filled++;
        }
    }
    if (fill->filled >= CELL_GRID_SIZE) {
        fill->active = FALSE;
        return TRUE;
    }
    return FALSE;
}

static void ShuffledIndices_Init(ShuffledIndices *indices, u32 count) {
    u32 i;
    u16 tmp;

    GF_ASSERT(count < 128);
    for (i = 0; i < count; i++) {
        indices->values[i] = i;
    }
    for (i = 0; i < count - 1; i++) {
        u32 other = LCRandom() % count;
        tmp = indices->values[i];
        indices->values[i] = indices->values[other];
        indices->values[other] = tmp;
    }
    indices->next = 0;
    indices->count = count;
}

static int ShuffledIndices_Next(ShuffledIndices *indices) {
    u32 current = indices->next;
    u32 next = current + 1;
    u16 value;
    u32 count;

    indices->next = next;
    value = indices->values[current];
    count = indices->count;
    next = indices->next;
    if (next <= count) {
        count = next;
    }
    indices->next = count;
    return value;
}

static WildEffectCellGrid *WildEffectCellGrid_New(enum HeapID heapID) {
    WildEffectCellGrid *grid = Heap_Alloc(heapID, sizeof(WildEffectCellGrid));
    int i;

    memset(grid, 0, sizeof(WildEffectCellGrid));
    for (i = 0; i < CELL_GRID_SIZE; i++) {
        grid->cells[i] = WildEffectCell_New(heapID);
    }
    return grid;
}

static void WildEffectCellGrid_Free(WildEffectCellGrid *grid) {
    int i;

    GF_ASSERT(grid != NULL);
    for (i = 0; i < CELL_GRID_SIZE; i++) {
        WildEffectCell_Free(grid->cells[i]);
    }
    Heap_Free(grid);
}

static void WildEffectCellGrid_Start(WildEffectCellGrid *grid, u8 delay, u8 growFrames, Window *window, u8 fillColor, BOOL reverse) {
    GF_ASSERT(window != NULL);
    grid->window = window;
    grid->fillColor = fillColor;
    grid->delay = delay;
    grid->growFrames = growFrames;
    grid->started = 0;
    grid->finished = 0;
    grid->timer = delay;
    grid->active = TRUE;
    grid->reverse = reverse;
}

#define CellGrid_StartCell(grid, index, fromLine) WildEffectCell_Start((grid)->cells[index], ((index) % CELL_GRID_WIDTH) * CELL_SIZE + CELL_SIZE / 2, ((index) / CELL_GRID_WIDTH) * CELL_SIZE + CELL_SIZE / 2, (grid)->growFrames, (grid)->window, 0, (fromLine) ? 0 : CELL_SIZE, CELL_SIZE, CELL_SIZE, (grid)->fillColor)

// Rows from the top and the bottom at the same time, alternating direction (snake order).
static BOOL WildEffectCellGrid_UpdateRows(WildEffectCellGrid *grid) {
    int i;
    int index;
    BOOL done;
    int mirror;

    GF_ASSERT(grid != NULL);
    if (grid->active == FALSE) {
        return TRUE;
    }
    if (grid->started < CELL_GRID_SIZE / 2) {
        grid->timer--;
        if (grid->timer <= 0) {
            grid->timer = grid->delay;
            if (grid->reverse) {
                index = gWildEffectCellGridRowsReverse[grid->started];
            } else {
                index = gWildEffectCellGridRows[grid->started];
            }
            CellGrid_StartCell(grid, index, FALSE);
            if (grid->reverse) {
                index = gWildEffectCellGridRowsReverse[grid->started];
            } else {
                index = gWildEffectCellGridRows[grid->started];
            }
            index = CELL_GRID_SIZE - 1 - index;
            CellGrid_StartCell(grid, index, FALSE);
            grid->started++;
        }
    }
    for (i = grid->finished; i < grid->started; i++) {
        if (grid->reverse) {
            index = gWildEffectCellGridRowsReverse[i];
            mirror = CELL_GRID_SIZE - 1 - index;
        } else {
            index = gWildEffectCellGridRows[i];
            mirror = CELL_GRID_SIZE - 1 - index;
        }
        done = WildEffectCell_Update(grid->cells[index]);
        done |= WildEffectCell_Update(grid->cells[mirror]);
        if (done == TRUE) {
            grid->finished++;
        }
    }
    if (grid->finished >= CELL_GRID_SIZE / 2 && done == TRUE) {
        grid->active = FALSE;
        return TRUE;
    }
    return FALSE;
}

// Diagonals from two corners at the same time; the cells grow from a horizontal line.
static BOOL WildEffectCellGrid_UpdateDiagonals(WildEffectCellGrid *grid) {
    int i;
    int index;
    BOOL done;
    int other;

    GF_ASSERT(grid != NULL);
    if (grid->active == FALSE) {
        return TRUE;
    }
    if (grid->started < CELL_GRID_SIZE / 2) {
        grid->timer--;
        if (grid->timer <= 0) {
            grid->timer = grid->delay;
            if (grid->reverse) {
                index = gWildEffectCellGridDiagonalsA[grid->started];
            } else {
                index = CELL_GRID_SIZE - 1 - gWildEffectCellGridDiagonalsA[grid->started];
            }
            CellGrid_StartCell(grid, index, TRUE);
            if (grid->reverse) {
                index = gWildEffectCellGridDiagonalsB[grid->started];
            } else {
                index = CELL_GRID_SIZE - 1 - gWildEffectCellGridDiagonalsB[grid->started];
            }
            CellGrid_StartCell(grid, index, TRUE);
            grid->started++;
        }
    }
    for (i = grid->finished; i < grid->started; i++) {
        if (grid->reverse) {
            index = gWildEffectCellGridDiagonalsA[i];
            other = gWildEffectCellGridDiagonalsB[i];
        } else {
            index = CELL_GRID_SIZE - 1 - gWildEffectCellGridDiagonalsA[i];
            other = CELL_GRID_SIZE - 1 - gWildEffectCellGridDiagonalsB[i];
        }
        done = WildEffectCell_Update(grid->cells[index]);
        done |= WildEffectCell_Update(grid->cells[other]);
        if (done == TRUE) {
            grid->finished++;
        }
    }
    if (grid->finished >= CELL_GRID_SIZE / 2 && done == TRUE) {
        grid->active = FALSE;
        return TRUE;
    }
    return FALSE;
}

static void WildEffectSine_Init(WildEffectSine *sine, s16 start, s16 peak, s16 speed) {
    sine->value = start;
    sine->center = start;
    sine->amplitude = peak - start;
    sine->speed = speed;
    sine->angle = 0;
}

static void WildEffectSine_Update(WildEffectSine *sine) {
    s32 delta;
    u32 index = sine->angle >> 4;

    sine->value = (s16)(sine->center + ((FX_SinCosTable_[index * 2] * sine->amplitude) >> FX32_SHIFT));
    delta = (sine->speed * 0xFFFF) / 360;
    sine->angle += delta;
}

static void WildEffectStep_Init(WildEffectStep *step, WildEffectStepFunc func) {
    step->func = func;
    step->state = 0;
}

static BOOL WildEffectStep_Run(WildEffectStep *step, void *work) {
    return step->func(work, &step->state);
}

void EncounterEffect_PkmnTrainerRed(SysTask *task, EncounterEffectTask *effect) {
    RedEffectWork *work = effect->work;

    switch (effect->state) {
    case WILD_EFFECT_STATE_INIT: {
        u16 black;

        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(RedEffectWork));
        memset(effect->work, 0, sizeof(RedEffectWork));
        work = effect->work;
        work->window = AllocWindows(HEAP_ID_FIELD1, 1);
        AddWindowParameterized(effect->fieldSystem->bgConfig, work->window, GF_BG_LYR_MAIN_3, 0, 0, 32, 32, 0, 0);
        black = 0;
        BG_LoadPlttData(GF_BG_LYR_MAIN_3, &black, sizeof(black), 30);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        EncounterEffect_InitSprites(&work->spriteManager, 2, 1);
        RedEffect_CreateSprite(effect->narc, &work->sprite, &work->spriteManager, &work->spriteResources, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->hBlankSystem = effect->fieldSystem->mapSystems->hBlankSystem;
        WildEffectStep_Init(&work->steps[0], (WildEffectStepFunc)RedEffect_FillScreen);
        WildEffectStep_Init(&work->steps[1], (WildEffectStepFunc)RedEffect_FirstFlash);
        WildEffectStep_Init(&work->steps[2], (WildEffectStepFunc)RedEffect_SecondFlash);
        effect->state = WILD_EFFECT_STATE_START_FLASH;
        break;
    }
    case WILD_EFFECT_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, -16, -16, &effect->fadeDone, 2);
        effect->state = WILD_EFFECT_STATE_WAIT_FLASH;
        break;
    case WILD_EFFECT_STATE_WAIT_FLASH:
        if (effect->fadeDone) {
            effect->state = WILD_EFFECT_STATE_START;
        }
        break;
    case WILD_EFFECT_STATE_START:
        work->step = 0;
        effect->state = WILD_EFFECT_STATE_RUN;
        break;
    case WILD_EFFECT_STATE_RUN:
        if (WildEffectStep_Run(&work->steps[work->step], work)) {
            effect->state = WILD_EFFECT_STATE_END;
        }
        break;
    case WILD_EFFECT_STATE_END:
        SetBothScreensBrightnessByColor(0);
        if (effect->done) {
            *effect->done = TRUE;
        }
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        Sprite_Delete(work->sprite);
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        EncounterEffect_Finish(effect, task);
        return;
    }
    if (effect->state != WILD_EFFECT_STATE_END) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

static BOOL RedEffect_FillScreen(RedEffectWork *work, u32 *state) {
    BOOL done;

    switch (*state) {
    case RED_STEP_STATE_START:
        work->fill = WildEffectRandomFill_New(HEAP_ID_FIELD1);
        WildEffectRandomFill_Start(work->fill, 0, work->window, 15);
        *state = RED_STEP_STATE_WAIT;
        break;
    case RED_STEP_STATE_WAIT:
        done = WildEffectRandomFill_Update(work->fill);
        ScheduleWindowCopyToVram(work->window);
        if (done) {
            *state = RED_STEP_STATE_FINISH;
        }
        break;
    case RED_STEP_STATE_FINISH:
        WildEffectRandomFill_Free(work->fill);
        *state = RED_STEP_STATE_START;
        return TRUE;
    default:
        GF_ASSERT(FALSE);
    }
    return FALSE;
}

static BOOL RedEffect_FirstFlash(RedEffectWork *work, u32 *state) {
    switch (*state) {
    case RED_STEP_STATE_START:
        HBlankSystem_Stop(work->hBlankSystem);
        BeginNormalPaletteFade(3, 42, 42, RGB_BLACK, 28, 1, HEAP_ID_FIELD1);
        *state = RED_STEP_STATE_WAIT;
        break;
    case RED_STEP_STATE_WAIT:
        if (IsPaletteFadeFinished()) {
            *state = RED_STEP_STATE_FINISH;
        }
        break;
    case RED_STEP_STATE_FINISH:
        HBlankSystem_Start(work->hBlankSystem);
        *state = RED_STEP_STATE_START;
        return TRUE;
    default:
        GF_ASSERT(FALSE);
    }
    return FALSE;
}

static BOOL RedEffect_SecondFlash(RedEffectWork *work, u32 *state) {
    switch (*state) {
    case RED_STEP_STATE_START:
        HBlankSystem_Stop(work->hBlankSystem);
        BeginNormalPaletteFade(3, 24, 24, RGB_BLACK, 28, 1, HEAP_ID_FIELD1);
        *state = RED_STEP_STATE_WAIT;
        break;
    case RED_STEP_STATE_WAIT:
        if (IsPaletteFadeFinished()) {
            *state = RED_STEP_STATE_FINISH;
        }
        break;
    case RED_STEP_STATE_FINISH:
        HBlankSystem_Start(work->hBlankSystem);
        *state = RED_STEP_STATE_START;
        return TRUE;
    default:
        GF_ASSERT(FALSE);
    }
    return FALSE;
}

static void RedEffect_CreateSprite(NARC *narc, Sprite **sprite, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *spriteResources, int variant) {
    GF_ASSERT(variant != 2);
    EncounterEffect_LoadSpriteResources(narc, spriteManager, spriteResources, gRedEffectPlttMembers[variant * 4], 1, gRedEffectSpriteResources[variant * 4], gRedEffectCellMembers[variant * 4], gRedEffectAnimMembers[variant * 4], 600000);
    *sprite = EncounterEffect_CreateSprite(spriteManager, spriteResources, FX32_CONST(128), FX32_CONST(96), 0, 0);
    Sprite_SetDrawFlag(*sprite, TRUE);
    Sprite_SetDrawPriority(*sprite, 16);
    Sprite_SetPriority(*sprite, 1);
}

void EncounterEffect_WildRowsWhite(SysTask *task, EncounterEffectTask *effect) {
    WildEffect_RunCellGridRows(task, effect, TRUE);
}

void EncounterEffect_WildRowsBlack(SysTask *task, EncounterEffectTask *effect) {
    WildEffect_RunCellGridRows(task, effect, FALSE);
}

static void WildEffect_RunCellGridRows(SysTask *task, EncounterEffectTask *effect, BOOL white) {
    CellGridEffectWork *work = effect->work;
    u16 black;
    BOOL done;
    int brightness;

    switch (effect->state) {
    case WILD_EFFECT_STATE_INIT: {
        u8 *bytes = Heap_Alloc(HEAP_ID_FIELD1, sizeof(CellGridEffectWork));
        effect->work = bytes;
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        bytes[3] = 0;
        bytes[4] = 0;
        bytes[5] = 0;
        bytes[6] = 0;
        bytes[7] = 0;
        work = effect->work;
        work->window = AllocWindows(HEAP_ID_FIELD1, 1);
        AddWindowParameterized(effect->fieldSystem->bgConfig, work->window, GF_BG_LYR_MAIN_3, 0, 0, 32, 32, 0, 0);
        black = 0;
        BG_LoadPlttData(GF_BG_LYR_MAIN_3, &black, sizeof(black), 30);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        work->grid = WildEffectCellGrid_New(HEAP_ID_FIELD1);
        effect->state = WILD_EFFECT_STATE_START_FLASH;
        break;
    }
    case WILD_EFFECT_STATE_START_FLASH:
        brightness = white ? 16 : -16;
        EncounterEffect_StartFlash(1, brightness, brightness, &effect->fadeDone, 2);
        effect->state = WILD_EFFECT_STATE_WAIT_FLASH;
        break;
    case WILD_EFFECT_STATE_WAIT_FLASH:
        if (effect->fadeDone) {
            effect->state = WILD_EFFECT_STATE_START;
        }
        break;
    case WILD_EFFECT_STATE_START:
        WildEffectCellGrid_Start(work->grid, 0, 4, work->window, 15, white);
        effect->state = WILD_EFFECT_STATE_RUN;
        break;
    case WILD_EFFECT_STATE_RUN:
        done = WildEffectCellGrid_UpdateRows(work->grid);
        ScheduleWindowCopyToVram(work->window);
        if (done) {
            effect->state = WILD_EFFECT_STATE_END;
        }
        break;
    case WILD_EFFECT_STATE_END:
        SetBothScreensBrightnessByColor(0);
        if (effect->done) {
            *effect->done = TRUE;
        }
        WildEffectCellGrid_Free(work->grid);
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        EncounterEffect_Finish(effect, task);
        break;
    }
}

void EncounterEffect_WildDiagonalsWhite(SysTask *task, EncounterEffectTask *effect) {
    WildEffect_RunCellGridDiagonals(task, effect, TRUE);
}

void EncounterEffect_WildDiagonalsBlack(SysTask *task, EncounterEffectTask *effect) {
    WildEffect_RunCellGridDiagonals(task, effect, FALSE);
}

static void WildEffect_RunCellGridDiagonals(SysTask *task, EncounterEffectTask *effect, BOOL white) {
    CellGridEffectWork *work = effect->work;
    u16 black;
    BOOL done;
    int brightness;

    switch (effect->state) {
    case WILD_EFFECT_STATE_INIT: {
        u8 *bytes = Heap_Alloc(HEAP_ID_FIELD1, sizeof(CellGridEffectWork));
        effect->work = bytes;
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        bytes[3] = 0;
        bytes[4] = 0;
        bytes[5] = 0;
        bytes[6] = 0;
        bytes[7] = 0;
        work = effect->work;
        work->window = AllocWindows(HEAP_ID_FIELD1, 1);
        AddWindowParameterized(effect->fieldSystem->bgConfig, work->window, GF_BG_LYR_MAIN_3, 0, 0, 32, 32, 0, 0);
        black = 0;
        BG_LoadPlttData(GF_BG_LYR_MAIN_3, &black, sizeof(black), 30);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        work->grid = WildEffectCellGrid_New(HEAP_ID_FIELD1);
        effect->state = WILD_EFFECT_STATE_START_FLASH;
        break;
    }
    case WILD_EFFECT_STATE_START_FLASH:
        brightness = white ? 16 : -16;
        EncounterEffect_StartFlash(1, brightness, brightness, &effect->fadeDone, 2);
        effect->state = WILD_EFFECT_STATE_WAIT_FLASH;
        break;
    case WILD_EFFECT_STATE_WAIT_FLASH:
        if (effect->fadeDone) {
            effect->state = WILD_EFFECT_STATE_START;
        }
        break;
    case WILD_EFFECT_STATE_START:
        WildEffectCellGrid_Start(work->grid, 0, 4, work->window, 15, white);
        effect->state = WILD_EFFECT_STATE_RUN;
        break;
    case WILD_EFFECT_STATE_RUN:
        done = WildEffectCellGrid_UpdateDiagonals(work->grid);
        ScheduleWindowCopyToVram(work->window);
        if (done) {
            effect->state = WILD_EFFECT_STATE_END;
        }
        break;
    case WILD_EFFECT_STATE_END:
        SetBothScreensBrightnessByColor(0);
        if (effect->done) {
            *effect->done = TRUE;
        }
        WildEffectCellGrid_Free(work->grid);
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        EncounterEffect_Finish(effect, task);
        break;
    }
}

void EncounterEffect_WildWaveWhite(SysTask *task, EncounterEffectTask *effect) {
    WildEffect_RunWave(task, effect, TRUE);
}

void EncounterEffect_WildWaveBlack(SysTask *task, EncounterEffectTask *effect) {
    WildEffect_RunWave(task, effect, FALSE);
}

static void *WildEffect_LoadCharData(NARC *narc, int member, NNSG2dCharacterData **charData, enum HeapID heapID) {
    void *file = GfGfxLoader_LoadFromOpenNarc(narc, member, FALSE, heapID, FALSE);
    NNS_G2dGetUnpackedBGCharacterData(file, charData);
    return file;
}

static void WildEffect_LoadScreen(BgConfig *bgConfig, NARC *narc, int member, int bgId) {
    NNSG2dScreenData *screen;
    void *file = GfGfxLoader_GetScrnDataFromOpenNarc(narc, member, FALSE, &screen, HEAP_ID_FIELD1);

    LoadRectToBgTilemapRect(bgConfig, bgId, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    BgTilemapRectChangePalette(bgConfig, bgId, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3, 0);
    Heap_Free(file);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}

static void WildEffectBgLoader_VBlankTask(SysTask *task, void *data) {
    WildEffectBgLoader *loader = data;
    EncounterEffectTask *effect = loader->effect;
    NNSG2dCharacterData *charData;

    switch (loader->vBlankState) {
    case BG_LOADER_VBLANK_BG1_CHARS:
        charData = loader->charData1;
        BG_LoadCharTilesData(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, charData->pRawData, charData->szByte, 0);
        loader->vBlankState++;
        return;
    case BG_LOADER_VBLANK_BG3_CHARS:
        charData = loader->charData3;
        BG_LoadCharTilesData(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, charData->pRawData, charData->szByte, 0);
        loader->vBlankState++;
        return;
    case BG_LOADER_VBLANK_END:
        loader->vBlankTask = NULL;
        SysTask_Destroy(task);
        return;
    default:
        GF_ASSERT(FALSE);
    }
}

static void WildEffectBgLoader_Task(SysTask *task, void *data) {
    WildEffectBgLoader *loader = data;
    EncounterEffectTask *effect = loader->effect;

    GF_ASSERT(effect->narc != NULL);
    switch (loader->state) {
    case BG_LOADER_STATE_LOAD:
        GfGfxLoader_GXLoadPalFromOpenNarc(effect->narc, 160, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_FIELD1);
        loader->charFile1 = WildEffect_LoadCharData(effect->narc, 161, &loader->charData1, HEAP_ID_FIELD1);
        loader->charFile3 = WildEffect_LoadCharData(effect->narc, 164, &loader->charData3, HEAP_ID_FIELD1);
        loader->vBlankState = BG_LOADER_VBLANK_BG1_CHARS;
        loader->vBlankTask = SysTask_CreateOnVBlankQueue(WildEffectBgLoader_VBlankTask, loader, 0);
        loader->state++;
        return;
    case BG_LOADER_STATE_LOAD_SCREENS:
        if (loader->vBlankTask != NULL) {
            return;
        }
        WildEffect_LoadScreen(effect->fieldSystem->bgConfig, effect->narc, 162, GF_BG_LYR_MAIN_1);
        WildEffect_LoadScreen(effect->fieldSystem->bgConfig, effect->narc, 165, GF_BG_LYR_MAIN_3);
        loader->state++;
        return;
    case BG_LOADER_STATE_END:
        Heap_Free(loader->charFile1);
        Heap_Free(loader->charFile3);
        loader->task = NULL;
        SysTask_Destroy(task);
        return;
    default:
        GF_ASSERT(FALSE);
    }
}

static void WildEffectBgLoader_Start(WildEffectBgLoader *loader, EncounterEffectTask *effect) {
    loader->effect = effect;
    loader->state = BG_LOADER_STATE_LOAD;
    loader->task = SysTask_CreateOnMainQueue(WildEffectBgLoader_Task, loader, 1);
}

static BOOL WildEffectBgLoader_IsDone(WildEffectBgLoader *loader) {
    GF_ASSERT(loader != NULL);
    return loader->task == NULL;
}

static void WildEffect_RunWave(SysTask *task, EncounterEffectTask *effect, BOOL white) {
    WaveEffectWork *work = effect->work;

    switch (effect->state) {
    case WAVE_EFFECT_STATE_LOAD_BG:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(WaveEffectWork));
        memset(effect->work, 0, sizeof(WaveEffectWork));
        work = effect->work;
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFFE0FF) | 0x1500;
        FreeBgTilemapBuffer(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
        InitBgFromTemplate(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, &gWildEffectWaveBgTemplate_Main1, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
        FreeBgTilemapBuffer(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        InitBgFromTemplate(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, &gWildEffectWaveBgTemplate_Main3, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(8, 0);
        GfGfx_EngineASetPlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
        WildEffectBgLoader_Start(&work->bgLoader, effect);
        effect->state = WAVE_EFFECT_STATE_START_FLASH;
        break;
    case WAVE_EFFECT_STATE_START_FLASH:
        if (!WildEffectBgLoader_IsDone(&work->bgLoader)) {
            break;
        }
        BG_SetMaskColor(GF_BG_LYR_MAIN_1, 0);
        EncounterEffectQuadInterp_Init(&work->bg1Scroll, 0, FX32_CONST(320), 0, 18);
        EncounterEffectQuadInterp_Init(&work->bg3Scroll, 0, FX32_CONST(320), FX32_CONST(8), 30);
        WildEffectSine_Init(&work->bg3Sine, 0, 16, 15);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, work->bg1Scroll.value >> FX32_SHIFT);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, work->bg3Scroll.value >> FX32_SHIFT);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        {
            int brightness;

            if (white) {
                brightness = 16;
            } else {
                brightness = -16;
            }
            EncounterEffect_StartFlash(1, brightness, brightness, &effect->fadeDone, 2);
        }
        effect->state = WAVE_EFFECT_STATE_WAIT_FLASH;
        break;
    case WAVE_EFFECT_STATE_WAIT_FLASH:
        if (effect->fadeDone) {
            EncounterEffectHBlankWave_Init(&work->wave, 4);
            effect->state = WAVE_EFFECT_STATE_START_WAVE;
        }
        break;
    case WAVE_EFFECT_STATE_START_WAVE:
        work->timer = 10;
        EncounterEffectHBlankWave_Start(&work->wave, 0, 191, 3410, 0x3000, 400, (void *)&reg_G2_BG0HOFS, 0, 4);
        work->bg3Scrolling = TRUE;
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        effect->state = WAVE_EFFECT_STATE_WAIT_BG1_DELAY;
        // fallthrough
    case WAVE_EFFECT_STATE_WAIT_BG1_DELAY:
        if (work->timer-- == 0) {
            effect->state = WAVE_EFFECT_STATE_START_BG1_SCROLL;
        }
        break;
    case WAVE_EFFECT_STATE_START_BG1_SCROLL:
        work->bg1Scrolling = TRUE;
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        effect->state = WAVE_EFFECT_STATE_WAIT_BG1_SCROLL;
        break;
    case WAVE_EFFECT_STATE_WAIT_BG1_SCROLL:
        if (work->bg1Done) {
            effect->state = WAVE_EFFECT_STATE_END;
        }
        break;
    case WAVE_EFFECT_STATE_END:
        SetBothScreensBrightnessByColor(0);
        if (effect->done) {
            *effect->done = TRUE;
        }
        EncounterEffectHBlankWave_Free(&work->wave);
        EncounterEffect_Finish(effect, task);
        return;
    }
    if (work->bg1Scrolling) {
        work->bg1Done = EncounterEffectQuadInterp_Update(&work->bg1Scroll);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, work->bg1Scroll.value >> FX32_SHIFT);
    }
    if (work->bg3Scrolling) {
        work->bg3Done = EncounterEffectQuadInterp_Update(&work->bg3Scroll);
        WildEffectSine_Update(&work->bg3Sine);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, work->bg3Scroll.value >> FX32_SHIFT);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, work->bg3Sine.value);
    }
}
