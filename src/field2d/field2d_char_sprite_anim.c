#include "overlay_42.h"
#include "sprite_transfer.h"

const Field2DMapCell sField2DMapCellNone = { 0xFFFFFFFF };

void CharSprite_StartAnimStand(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, direction);
}

void CharSprite_StartAnimLoop(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, 4);
}

void CharSprite_StartAnimWalk(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, direction + 5);
}

void CharSprite_StartAnimTurn(CharSprite *charSprite, s32 direction) {
    Sprite_TryChangeAnimSeq(charSprite->sprite, charSprite->direction + 5);
    charSprite->direction = direction;
}

void CharSprite_StartAnimRun(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, direction + 9);
}

void CharSprite_StartAnimBump(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, direction + 5);
}

void CharSprite_StartAnimWalkSlow(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, direction + 5);
}

void CharSprite_StartAnimWalkVeryFast(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, direction + 5);
}

void CharSprite_StartAnimWalkFast(CharSprite *charSprite, s32 direction) {
    charSprite->direction = direction;
    Sprite_TryChangeAnimSeq(charSprite->sprite, direction + 5);
}

void CharSprite_UpdateAnimLoop(CharSprite *charSprite) {
    Sprite_UpdateAnim(charSprite->sprite, 8192);
}

void CharSprite_UpdateAnimWalk(CharSprite *charSprite) {
    Sprite_UpdateAnim(charSprite->sprite, 8192);
}

void CharSprite_UpdateAnimTurn(CharSprite *charSprite) {
    if (charSprite->animFrames == 0) {
        Sprite_SetAnimationFrame(charSprite->sprite, 1);
    } else {
        Sprite_SetAnimCtrlSeq(charSprite->sprite, charSprite->direction + 5);
    }
}

void CharSprite_UpdateAnimRun(CharSprite *charSprite) {
    Sprite_UpdateAnim(charSprite->sprite, 8192);
}

void CharSprite_UpdateAnimBump(CharSprite *charSprite) {
    Sprite_UpdateAnim(charSprite->sprite, 4096);
}

void CharSprite_UpdateAnimWalkSlow(CharSprite *charSprite) {
    Sprite_UpdateAnim(charSprite->sprite, 4096);
}

void CharSprite_UpdateAnimWalkVeryFast(CharSprite *charSprite) {
    Sprite_UpdateAnim(charSprite->sprite, 32768);
}

void CharSprite_UpdateAnimWalkFast(CharSprite *charSprite) {
    Sprite_UpdateAnim(charSprite->sprite, 24576);
}

s32 CharSprite_IsFree(CharSprite *charSprite) {
    if (charSprite->sprite == NULL) {
        return 1;
    }
    return 0;
}

void CharSpriteManager_LoadPaletteToBuffer(CharSpriteManager *charSpriteManager, SpriteResource *spriteResource, s32 numPalettes) {
    NNSG2dPaletteData *plttData = GF2DGfxResObj_GetPlttDataPtr(spriteResource);
    s32 mainPlttOffset = SpriteTransfer_GetPlttOffset(spriteResource, NNS_G2D_VRAM_TYPE_2DMAIN);
    s32 subPlttOffset = SpriteTransfer_GetPlttOffset(spriteResource, NNS_G2D_VRAM_TYPE_2DSUB);
    if (mainPlttOffset != -1) {
        PaletteData_LoadPalette(charSpriteManager->paletteData, plttData->pRawData, PLTTBUF_MAIN_OBJ, mainPlttOffset * 16, numPalettes << 5);
    }
    if (subPlttOffset != -1) {
        PaletteData_LoadPalette(charSpriteManager->paletteData, plttData->pRawData, PLTTBUF_SUB_OBJ, subPlttOffset * 16, numPalettes << 5);
    }
}

void *Field2DMap_New(u16 width, u16 height, enum HeapID heapID) {
    Field2DMap *map = Heap_Alloc(heapID, sizeof(Field2DMap));
    GF_ASSERT(map);
    map->width = width;
    map->height = height;
    void *cells = Heap_Alloc(heapID, map->width * map->height * 4);
    map->cells = cells;
    GF_ASSERT(cells);
    memset(map->cells, 0, map->width * map->height * 4);
    return map;
}

void Field2DMap_Free(Field2DMap *map) {
    GF_ASSERT(map);
    Heap_Free(map->cells);
    Heap_Free(map);
}

u16 Field2DMap_GetWidth(Field2DMap *map) {
    return map->width;
}

u16 Field2DMap_GetHeight(Field2DMap *map) {
    return map->height;
}

void Field2DMap_Load(Field2DMap *map, const void *cells) {
    GF_ASSERT(map);
    memcpy(map->cells, cells, map->width * map->height * 4);
}

Field2DMapCell Field2DMap_GetCell(Field2DMap *map, u32 x, u32 y) {
    Field2DMapCell cell = sField2DMapCellNone;
    GF_ASSERT(map);
    if ((map->width > x) && (map->height > y)) {
        cell.value = map->cells[map->width * y + x];
    }
    return cell;
}
