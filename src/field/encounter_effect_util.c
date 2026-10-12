#include "field/encounter_effect.h"

#include "global.h"

#include "battle/battle_setup.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"

// Helpers shared by the encounter effects: sprite setup, filled rectangles moved over a window (trails and the
// falling-block wipe), the zigzag window wipe, the default wild/trainer effect and main screen brightness.

#define BLOCK_WIPE_COLUMNS 8
#define BLOCK_WIPE_ROWS    6
#define BLOCK_WIPE_BLOCKS  (BLOCK_WIPE_COLUMNS * BLOCK_WIPE_ROWS)
#define BLOCK_WIPE_SIZE    32

// Defined in hblank_system_1.c; no header declares them yet.
typedef void (*HBlankTaskFunc)(void *, void *);
void *HBlankSystem_StartTask(HBlankSystem *hBlankSystem, HBlankTaskFunc func, void *data);
void HBlankTask_Delete(void *task);

// NitroSDK gx.c; the SDK headers in lib/include only declare the getter.
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

// A width x height rectangle filled with fill, moved from (x.start, y.start) to (x.end, y.end); positions are
// the rectangle's centre in pixels.
typedef struct FilledRect {
    EncounterEffectInterp x; // 0x00
    EncounterEffectInterp y; // 0x14
    Window *window;          // 0x28
    u8 width;                // 0x2C
    u8 height;               // 0x2D
    u8 active;               // 0x2E
    u8 fill;                 // 0x2F
} FilledRect;

// 32x32 blocks fall one after another (every delay frames, columns in sBlockWipeColumnOrder, bottom row first)
// and stack up until they cover the window.
typedef struct BlockWipe {
    Window *window;                       // 0x00
    FilledRect *blocks[BLOCK_WIPE_BLOCKS]; // 0x04
    u8 started;                           // 0xC4
    u8 finished;                          // 0xC5
    u8 duration;                          // 0xC6
    u8 delay;                             // 0xC7
    s8 timer;                             // 0xC8
    u8 fill;                              // 0xC9
    u8 active;                            // 0xCA
} BlockWipe;

// Window 0 is closed from the left; its left edge is a zigzag (lines[], period lines per stroke) that moves
// with the interpolated value.
typedef struct ZigzagWipe {
    EncounterEffectInterp edge; // 0x00
    u32 state;                  // 0x14
    u8 lines[192];              // 0x18
    HBlankSystem *hBlankSystem; // 0xD8
    void *hBlankTask;           // 0xDC
    SysTask *task;              // 0xE0
    BOOL *done;                 // 0xE4
} ZigzagWipe;

extern u8 sBlockWipeColumnOrder[];

// The trainer and leader effects keep these objects as untyped pointers and declare the functions locally.
FilledRect *RectTrail_New(enum HeapID heapID);
void RectTrail_Free(FilledRect *rect);
void RectTrail_Start(FilledRect *rect, int startX, int endX, int startY, int endY, s32 duration, Window *window, int width, int height, u8 fill);
BOOL RectTrail_Update(FilledRect *rect);
FilledRect *MovingRect_New(enum HeapID heapID);
void MovingRect_Free(FilledRect *rect);
void MovingRect_Start(FilledRect *rect, int startX, int endX, int startY, int endY, s32 duration, Window *window, int width, int height, u8 fill);
BOOL MovingRect_Update(FilledRect *rect);
BlockWipe *BlockWipe_New(enum HeapID heapID);
void BlockWipe_Free(BlockWipe *wipe);
void BlockWipe_Start(BlockWipe *wipe, u8 duration, u8 delay, Window *window, u8 fill);
BOOL BlockWipe_Update(BlockWipe *wipe);
ZigzagWipe *ZigzagWipe_New(void);
void ZigzagWipe_Free(ZigzagWipe *wipe);
SysTask *ZigzagWipe_Start(EncounterEffectTask *effect, ZigzagWipe *wipe, s32 duration, u32 period, volatile u32 width, u32 insidePlanes, u32 outsidePlanes);
void Task_ZigzagWipe_Begin(SysTask *task, void *data);
void Task_ZigzagWipe_Update(SysTask *task, void *data);
void HBlank_ZigzagWipe(void *unused, void *data);
void ZigzagWipe_End(ZigzagWipe *wipe);
int EncounterEffect_GetDefaultEffect(BattleSetup *setup);
void Task_ApplyMainBrightness(SysTask *task, void *data);

void EncounterEffect_InitSprites(EncounterEffectSpriteManager *spriteManager, int maxSprites, int resourceCount) {
    int i;
    spriteManager->spriteList = G2dRenderer_Init(maxSprites, &spriteManager->renderer, HEAP_ID_FIELD1);
    for (i = 0; i < 4; i++) {
        spriteManager->resourceManagers[i] = Create2DGfxResObjMan(resourceCount, (GfGfxResType)i, HEAP_ID_FIELD1);
    }
}

void EncounterEffect_FreeSprites(EncounterEffectSpriteManager *spriteManager) {
    int i;
    SpriteList_Delete(spriteManager->spriteList);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(spriteManager->resourceManagers[i]);
    }
}

void EncounterEffect_LoadSpriteResources(NARC *narc, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, int plttMember, int plttCount, int charMember, int cellMember, int animMember, int resourceId) {
    resources->resourceObjects[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarcWithAtEndFlag(spriteManager->resourceManagers[GF_GFX_RES_TYPE_CHAR], narc, charMember, FALSE, resourceId, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_FIELD1, TRUE);
    resources->resourceObjects[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(spriteManager->resourceManagers[GF_GFX_RES_TYPE_PLTT], narc, plttMember, FALSE, resourceId, NNS_G2D_VRAM_TYPE_2DMAIN, plttCount, HEAP_ID_FIELD1);
    resources->resourceObjects[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(spriteManager->resourceManagers[GF_GFX_RES_TYPE_CELL], narc, cellMember, FALSE, resourceId, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    resources->resourceObjects[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(spriteManager->resourceManagers[GF_GFX_RES_TYPE_ANIM], narc, animMember, FALSE, resourceId, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->resourceObjects[GF_GFX_RES_TYPE_CHAR]);
    GF2DGfxResObj_FreeData(resources->resourceObjects[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(resources->resourceObjects[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&resources->header, resourceId, resourceId, resourceId, resourceId, -1, -1, FALSE, 0, spriteManager->resourceManagers[GF_GFX_RES_TYPE_CHAR], spriteManager->resourceManagers[GF_GFX_RES_TYPE_PLTT], spriteManager->resourceManagers[GF_GFX_RES_TYPE_CELL], spriteManager->resourceManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

void EncounterEffect_FreeSpriteResources(EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources) {
    int i;
    SpriteTransfer_DeleteCharTransferTask(resources->resourceObjects[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(resources->resourceObjects[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(spriteManager->resourceManagers[i], resources->resourceObjects[i]);
    }
}

Sprite *EncounterEffect_CreateSprite(EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, fx32 x, fx32 y, fx32 z, int priority) {
    SimpleSpriteTemplate template;
    Sprite *sprite;
    template.spriteList = spriteManager->spriteList;
    template.header = &resources->header;
    template.position.x = x;
    template.position.y = y;
    template.position.z = z;
    template.priority = priority;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_FIELD1;
    sprite = Sprite_Create(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    return sprite;
}

void EncounterEffect_SetVecFx32(VecFx32 *vec, fx32 x, fx32 y, fx32 z) {
    VecFx32 value = { x, y, z };
    *vec = value;
}

FilledRect *RectTrail_New(enum HeapID heapID) {
    FilledRect *rect = Heap_Alloc(heapID, sizeof(FilledRect));
    memset(rect, 0, sizeof(FilledRect));
    return rect;
}

void RectTrail_Free(FilledRect *rect) {
    Heap_Free(rect);
}

void RectTrail_Start(FilledRect *rect, int startX, int endX, int startY, int endY, s32 duration, Window *window, int width, int height, u8 fill) {
    if (rect->active) {
        GF_ASSERT(FALSE);
    }
    EncounterEffectInterp_Init(&rect->x, startX, endX, duration);
    EncounterEffectInterp_Init(&rect->y, startY, endY, duration);
    rect->window = window;
    rect->width = width;
    rect->height = height;
    rect->fill = fill;
    rect->active = TRUE;
}

// Draws the rectangle at its new position without clearing the old one, leaving a trail.
BOOL RectTrail_Update(FilledRect *rect) {
    if (rect->active == FALSE) {
        return TRUE;
    }
    BOOL done = EncounterEffectInterp_Update(&rect->x);
    EncounterEffectInterp_Update(&rect->y);
    u32 width = rect->width;
    s16 left = rect->x.value - (width >> 1);
    s16 right = left + width;
    u32 height = rect->height;
    s16 top = rect->y.value - (height >> 1);
    s16 bottom = top + height;
    EncounterEffect_FillWindowRect(rect->window, top, bottom, left, right, rect->fill);
    return done;
}

FilledRect *MovingRect_New(enum HeapID heapID) {
    FilledRect *rect = Heap_Alloc(heapID, sizeof(FilledRect));
    memset(rect, 0, sizeof(FilledRect));
    return rect;
}

void MovingRect_Free(FilledRect *rect) {
    Heap_Free(rect);
}

void MovingRect_Start(FilledRect *rect, int startX, int endX, int startY, int endY, s32 duration, Window *window, int width, int height, u8 fill) {
    if (rect->active) {
        GF_ASSERT(FALSE);
    }
    EncounterEffectInterp_Init(&rect->x, startX, endX, duration);
    EncounterEffectInterp_Init(&rect->y, startY, endY, duration);
    rect->window = window;
    rect->width = width;
    rect->height = height;
    rect->fill = fill;
    rect->active = TRUE;
}

// Clears the rectangle (colour 0) at its old position, then draws it at the new one.
BOOL MovingRect_Update(FilledRect *rect) {
    if (rect->active == FALSE) {
        return TRUE;
    }
    u32 width = rect->width;
    s16 left = rect->x.value - (width >> 1);
    s16 right = left + width;
    u32 height = rect->height;
    s16 top = rect->y.value - (height >> 1);
    s16 bottom = top + height;
    EncounterEffect_FillWindowRect(rect->window, top, bottom, left, right, 0);
    BOOL done = EncounterEffectInterp_Update(&rect->x);
    EncounterEffectInterp_Update(&rect->y);
    width = rect->width;
    left = rect->x.value - (width >> 1);
    right = left + width;
    height = rect->height;
    top = rect->y.value - (height >> 1);
    bottom = top + height;
    EncounterEffect_FillWindowRect(rect->window, top, bottom, left, right, rect->fill);
    return done;
}

// Fills the part of the rectangle that lies on a 256x256 window.
void EncounterEffect_FillWindowRect(Window *window, s16 top, s16 bottom, s16 left, s16 right, u8 fill) {
    if (right <= 0 || bottom <= 0 || left == right || top == bottom) {
        return;
    }
    if (left < 0) {
        left = 0;
    }
    if (right > 256) {
        right = 256;
    }
    if (top < 0) {
        top = 0;
    }
    if (bottom > 256) {
        bottom = 256;
    }
    FillWindowPixelRect(window, fill, (u16)left, (u16)top, (u16)(right - left), (u16)(bottom - top));
}

BlockWipe *BlockWipe_New(enum HeapID heapID) {
    BlockWipe *wipe = Heap_Alloc(HEAP_ID_FIELD1, sizeof(BlockWipe));
    int i;
    memset(wipe, 0, sizeof(BlockWipe));
    for (i = 0; i < BLOCK_WIPE_BLOCKS; i++) {
        wipe->blocks[i] = MovingRect_New(heapID);
    }
    return wipe;
}

void BlockWipe_Free(BlockWipe *wipe) {
    int i;
    for (i = 0; i < BLOCK_WIPE_BLOCKS; i++) {
        MovingRect_Free(wipe->blocks[i]);
    }
    Heap_Free(wipe);
}

void BlockWipe_Start(BlockWipe *wipe, u8 duration, u8 delay, Window *window, u8 fill) {
    wipe->window = window;
    wipe->fill = fill;
    wipe->duration = duration;
    wipe->delay = delay;
    wipe->started = 0;
    wipe->finished = 0;
    wipe->timer = 0;
    wipe->active = TRUE;
}

#pragma force_active on

BOOL BlockWipe_Update(BlockWipe *wipe) {
    int i;
    int result;
    if (!wipe->active) {
        return TRUE;
    }
    if (wipe->started < BLOCK_WIPE_BLOCKS) {
        wipe->timer--;
        if (wipe->timer <= 0) {
            int block, x, y;
            wipe->timer = wipe->delay;
            block = wipe->started;
            x = sBlockWipeColumnOrder[block % BLOCK_WIPE_COLUMNS] * BLOCK_WIPE_SIZE + BLOCK_WIPE_SIZE / 2;
            y = ((u32)block / BLOCK_WIPE_COLUMNS) * BLOCK_WIPE_SIZE;
            MovingRect_Start(wipe->blocks[block], x, x, -16 - y, 176 - y, wipe->duration, wipe->window, BLOCK_WIPE_SIZE, BLOCK_WIPE_SIZE, wipe->fill);
            wipe->started++;
        }
    }
    for (i = wipe->finished; i < wipe->started; i++) {
        result = MovingRect_Update(wipe->blocks[i]);
        if (result == TRUE) {
            wipe->finished++;
        }
    }
    if (wipe->finished >= BLOCK_WIPE_BLOCKS && result == TRUE) {
        wipe->active = FALSE;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

ZigzagWipe *ZigzagWipe_New(void) {
    ZigzagWipe *wipe = Heap_Alloc(HEAP_ID_FIELD1, sizeof(ZigzagWipe));
    memset(wipe, 0, sizeof(ZigzagWipe));
    return wipe;
}

void ZigzagWipe_Free(ZigzagWipe *wipe) {
    if (wipe->task) {
        ZigzagWipe_End(wipe);
    }
    Heap_Free(wipe);
}

#pragma force_active on

// width is volatile: the generated code keeps it on the stack.
SysTask *ZigzagWipe_Start(EncounterEffectTask *effect, ZigzagWipe *wipe, s32 duration, u32 period, volatile u32 width, u32 insidePlanes, u32 outsidePlanes) {
    int i;
    width = width;
    if (wipe->task) {
        GF_AssertFail();
    }
    effect->unk_18 = FALSE;
    wipe->hBlankSystem = effect->fieldSystem->mapSystems->hBlankSystem;
    wipe->state = 0;
    wipe->done = &effect->unk_18;
    EncounterEffectInterp_Init(&wipe->edge, 255, 0, duration);
    for (i = 0; i < 192; i++) {
        u32 step = i % period;
        u32 product = step * width;
        u32 x = product / period;
        if ((i / period & 1) == 0) {
            wipe->lines[i] = x;
        } else {
            wipe->lines[i] = width - x;
        }
    }
    reg_G2_WININ = (reg_G2_WININ & ~0x3F) | insidePlanes | 0x20;
    reg_G2_WINOUT = (reg_G2_WINOUT & ~0x3F) | outsidePlanes;
    reg_G2_WIN0H = 0;
    reg_G2_WIN0V = 192;
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    return SysTask_CreateOnVWaitQueue(Task_ZigzagWipe_Begin, wipe, 1024);
}

#pragma force_active reset

void Task_ZigzagWipe_Begin(SysTask *task, void *data) {
    ZigzagWipe *wipe = data;
    wipe->hBlankTask = HBlankSystem_StartTask(wipe->hBlankSystem, HBlank_ZigzagWipe, wipe);
    wipe->task = SysTask_CreateOnVWaitQueue(Task_ZigzagWipe_Update, wipe, 1024);
    SysTask_Destroy(task);
}

void Task_ZigzagWipe_Update(SysTask *task, void *data) {
    ZigzagWipe *wipe = data;
    switch (wipe->state) {
    case 0:
        if (EncounterEffectInterp_Update(&wipe->edge) == TRUE) {
            wipe->state++;
        }
        break;
    case 1:
        ZigzagWipe_End(wipe);
        break;
    }
}

void HBlank_ZigzagWipe(void *unused, void *data) {
    ZigzagWipe *wipe = data;
    s32 line = reg_GX_VCOUNT;
    s32 left;
    if (line >= 192) {
        return;
    }
    left = wipe->edge.value - wipe->lines[line];
    if (left < 0) {
        left = 0;
    }
    reg_G2_WIN0H = (u16)(0xFF | ((left << 8) & 0xFF00));
    reg_G2_WIN0V = 192;
}

void ZigzagWipe_End(ZigzagWipe *wipe) {
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    *wipe->done = TRUE;
    HBlankTask_Delete(wipe->hBlankTask);
    wipe->hBlankTask = NULL;
    SysTask_Destroy(wipe->task);
    wipe->task = NULL;
}

// The effect used when the battle's intro table entry has none: rows (0-1) by default, waves (2-3) on water,
// diagonals (4-5) in caves; the black variant (odd ids) at night; trainer battles use the trainer range (6-11).
int EncounterEffect_GetDefaultEffect(BattleSetup *setup) {
    u32 battleType = setup->battleType;
    int trainer = TRUE;
    int effect;
    if ((battleType & BATTLE_TYPE_TRAINER) == 0) {
        if ((battleType & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_ROAMER | BATTLE_TYPE_PAL_PARK | BATTLE_TYPE_TUTORIAL | BATTLE_TYPE_BUG_CONTEST)) != 0 || battleType == BATTLE_TYPE_NONE) {
            trainer = FALSE;
        } else {
            GF_AssertFail();
            trainer = FALSE;
        }
    }
    switch (setup->terrain) {
    case TERRAIN_PLAIN:
    case TERRAIN_SAND:
    case TERRAIN_GRASS:
    case TERRAIN_PUDDLE:
    case TERRAIN_MOUNTAIN:
    case TERRAIN_SNOW:
    case TERRAIN_ICE:
    case TERRAIN_BUILDING:
    case TERRAIN_GREAT_MARSH:
    case TERRAIN_UNKNOWN:
    case TERRAIN_MAX:
        effect = 0;
        break;
    case TERRAIN_WATER:
        effect = 2;
        break;
    case TERRAIN_CAVE:
        effect = 4;
        break;
    }
    if ((u32)setup->timeOfDay - RTC_TIMEOFDAY_NITE <= 1) {
        effect++;
    }
    return effect + 6 * trainer;
}

void Task_ApplyMainBrightness(SysTask *task, void *data) {
    EncounterEffectInterp *brightness = data;
    GXx_SetMasterBrightness_(&reg_GX_MASTER_BRIGHT, brightness->value);
    SysTask_Destroy(task);
}

SysTask *EncounterEffect_ApplyMainBrightness(EncounterEffectInterp *brightness) {
    return SysTask_CreateOnVWaitQueue(Task_ApplyMainBrightness, brightness, 1024);
}
