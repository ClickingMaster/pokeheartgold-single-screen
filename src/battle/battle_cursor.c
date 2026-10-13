#include "battle/battle_cursor.h"

#include "global.h"

#include "sprite_system.h"

static void BattleCursor_Update(SysTask *task, void *data);

void BattleCursor_LoadResources(SpriteSystem *renderer, SpriteManager *gfxHandler, PaletteData *plttData, enum HeapID heapID, u32 character, u32 pal, u32 cell, u32 animation) {
    NARC *narc = NARC_New(NARC_a_0_0_8, heapID);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_SUB_OBJ, renderer, gfxHandler, narc, 80, 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, pal);
    SpriteSystem_LoadCharResObjFromOpenNarc(renderer, gfxHandler, narc, 250, 1, NNS_G2D_VRAM_TYPE_2DSUB, character);
    SpriteSystem_LoadCellResObjFromOpenNarc(renderer, gfxHandler, narc, 251, 1, cell);
    SpriteSystem_LoadAnimResObjFromOpenNarc(renderer, gfxHandler, narc, 252, 1, animation);
    NARC_Delete(narc);
}

void BattleCursor_FreeResources(SpriteManager *gfxHandler, u32 character, u32 pal, u32 cell, u32 animation) {
    SpriteManager_UnloadCharObjById(gfxHandler, character);
    SpriteManager_UnloadPlttObjById(gfxHandler, pal);
    SpriteManager_UnloadCellObjById(gfxHandler, cell);
    SpriteManager_UnloadAnimObjById(gfxHandler, animation);
}

static const ManagedSpriteTemplate sBattleCursorSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 0,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0, 0, 0, 0, -1, -1 },
    .bgPriority = 0,
    .vramTransfer = 0
};

BattleCursor *BattleCursor_New(SpriteSystem *renderer, SpriteManager *gfxHandler, enum HeapID heapID, u32 character, u32 pal, u32 cell, u32 animation, u32 drawPriority, u32 bgPriority) {
    BattleCursor *cursor;
    ManagedSpriteTemplate unkStruct;
    int i;

    unkStruct = sBattleCursorSpriteTemplate;
    unkStruct.resIdList[0] = character;
    unkStruct.resIdList[1] = pal;
    unkStruct.resIdList[2] = cell;
    unkStruct.resIdList[3] = animation;
    unkStruct.drawPriority = drawPriority;
    unkStruct.bgPriority = bgPriority;

    cursor = Heap_Alloc(heapID, sizeof(BattleCursor));
    MI_CpuFill8(cursor, 0, sizeof(BattleCursor));

    for (i = 0; i < 5; i++) {
        cursor->sprites[i] = SpriteSystem_NewSprite(renderer, gfxHandler, &unkStruct);
        ManagedSprite_SetDrawFlag(cursor->sprites[i], FALSE);
    }

    cursor->task = SysTask_CreateOnMainQueue(BattleCursor_Update, cursor, 0x9C40);

    return cursor;
}

void BattleCursor_Delete(BattleCursor *cursor) {
    for (int i = 0; i < 5; i++) {
        Sprite_DeleteAndFreeResources(cursor->sprites[i]);
    }

    SysTask_Destroy(cursor->task);
    Heap_Free(cursor);
}

void BattleCursor_SetCornersWithOffset(BattleCursor *cursor, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, fx32 yOffset) {
    int i;

    ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[0], x0, y0, yOffset);
    ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[1], x1, y1, yOffset);
    ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[2], x2, y2, yOffset);
    ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[3], x3, y3, yOffset);

    ManagedSprite_SetAnim(cursor->sprites[0], 0);
    ManagedSprite_SetAnim(cursor->sprites[1], 1);
    ManagedSprite_SetAnim(cursor->sprites[2], 2);
    ManagedSprite_SetAnim(cursor->sprites[3], 3);

    for (i = 0; i < 4; i++) {
        ManagedSprite_SetDrawFlag(cursor->sprites[i], TRUE);
    }
}

void BattleCursor_SetCorners(BattleCursor *cursor, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
    BattleCursor_SetCornersWithOffset(cursor, x0, y0, x1, y1, x2, y2, x3, y3, (fx32)0xC0000);
}

void BattleCursor_SetRect(BattleCursor *cursor, int left, int right, int top, int bottom) {
    BattleCursor_SetCorners(cursor, left, top, right, top, left, bottom, right, bottom);
}

void BattleCursor_SetRectWithOffset(BattleCursor *cursor, int left, int right, int top, int bottom, fx32 yOffset) {
    BattleCursor_SetCornersWithOffset(cursor, left, top, right, top, left, bottom, right, bottom, yOffset);
}

void BattleCursor_ShowExtraSpriteWithOffset(BattleCursor *cursor, int x, int y, int anim, fx32 yOffset) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[4], x, y, yOffset);
    ManagedSprite_SetAnim(cursor->sprites[4], anim);
    ManagedSprite_SetDrawFlag(cursor->sprites[4], TRUE);
}

void BattleCursor_Disable(BattleCursor *cursor) {
    for (int i = 0; i < 5; i++) {
        ManagedSprite_SetDrawFlag(cursor->sprites[i], FALSE);
    }
}

void BattleCursor_HideExtraSprite(BattleCursor *cursor) {
    ManagedSprite_SetDrawFlag(cursor->sprites[4], FALSE);
}

static void BattleCursor_Update(SysTask *task, void *data) {
    BattleCursor *cursor = data;
    int i;

    if (!ManagedSprite_GetDrawFlag(cursor->sprites[0])) {
        return;
    }

    for (i = 0; i < 4; i++) {
        ManagedSprite_TickFrame(cursor->sprites[i]);
    }
    if (ManagedSprite_GetDrawFlag(cursor->sprites[4]) == TRUE) {
        ManagedSprite_TickFrame(cursor->sprites[4]);
    }
}
