#include "overlay_42.h"
#include "sprite_transfer.h"

const Field2DMapCell sField2DMapCellNone = { 0xFFFFFFFF };

void CharSprite_StartAnimStand(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, arg1);
}

void CharSprite_StartAnimLoop(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, 4);
}

void CharSprite_StartAnimWalk(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, arg1 + 5);
}

void CharSprite_StartAnimTurn(CharSprite *arg0, s32 arg1) {
    Sprite_TryChangeAnimSeq(arg0->sprite, arg0->direction + 5);
    arg0->direction = arg1;
}

void CharSprite_StartAnimRun(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, arg1 + 9);
}

void CharSprite_StartAnimBump(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, arg1 + 5);
}

void CharSprite_StartAnimWalkSlow(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, arg1 + 5);
}

void CharSprite_StartAnimWalkVeryFast(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, arg1 + 5);
}

void CharSprite_StartAnimWalkFast(CharSprite *arg0, s32 arg1) {
    arg0->direction = arg1;
    Sprite_TryChangeAnimSeq(arg0->sprite, arg1 + 5);
}

void CharSprite_UpdateAnimLoop(CharSprite *arg0) {
    Sprite_UpdateAnim(arg0->sprite, 8192);
}

void CharSprite_UpdateAnimWalk(CharSprite *arg0) {
    Sprite_UpdateAnim(arg0->sprite, 8192);
}

void CharSprite_UpdateAnimTurn(CharSprite *arg0) {
    if (arg0->animFrames == 0) {
        Sprite_SetAnimationFrame(arg0->sprite, 1);
    } else {
        Sprite_SetAnimCtrlSeq(arg0->sprite, arg0->direction + 5);
    }
}

void CharSprite_UpdateAnimRun(CharSprite *arg0) {
    Sprite_UpdateAnim(arg0->sprite, 8192);
}

void CharSprite_UpdateAnimBump(CharSprite *arg0) {
    Sprite_UpdateAnim(arg0->sprite, 4096);
}

void CharSprite_UpdateAnimWalkSlow(CharSprite *arg0) {
    Sprite_UpdateAnim(arg0->sprite, 4096);
}

void CharSprite_UpdateAnimWalkVeryFast(CharSprite *arg0) {
    Sprite_UpdateAnim(arg0->sprite, 32768);
}

void CharSprite_UpdateAnimWalkFast(CharSprite *arg0) {
    Sprite_UpdateAnim(arg0->sprite, 24576);
}

s32 CharSprite_IsFree(CharSprite *arg0) {
    if (arg0->sprite == NULL) {
        return 1;
    }
    return 0;
}

void CharSpriteManager_LoadPaletteToBuffer(CharSpriteManager *arg0, SpriteResource *arg1, s32 arg2) {
    NNSG2dPaletteData *sp4 = GF2DGfxResObj_GetPlttDataPtr(arg1);
    s32 temp_r7 = SpriteTransfer_GetPlttOffset(arg1, NNS_G2D_VRAM_TYPE_2DMAIN);
    s32 temp_r6 = SpriteTransfer_GetPlttOffset(arg1, NNS_G2D_VRAM_TYPE_2DSUB);
    if (temp_r7 != -1) {
        PaletteData_LoadPalette(arg0->paletteData, sp4->pRawData, PLTTBUF_MAIN_OBJ, temp_r7 * 16, arg2 << 5);
    }
    if (temp_r6 != -1) {
        PaletteData_LoadPalette(arg0->paletteData, sp4->pRawData, PLTTBUF_SUB_OBJ, temp_r6 * 16, arg2 << 5);
    }
}

void *Field2DMap_New(u16 arg0, u16 arg1, enum HeapID arg2) {
    Field2DMap *temp_r0 = Heap_Alloc(arg2, sizeof(Field2DMap));
    GF_ASSERT(temp_r0);
    temp_r0->width = arg0;
    temp_r0->height = arg1;
    void *temp_r0_2 = Heap_Alloc(arg2, temp_r0->width * temp_r0->height * 4);
    temp_r0->cells = temp_r0_2;
    GF_ASSERT(temp_r0_2);
    memset(temp_r0->cells, 0, temp_r0->width * temp_r0->height * 4);
    return temp_r0;
}

void Field2DMap_Free(Field2DMap *arg0) {
    GF_ASSERT(arg0);
    Heap_Free(arg0->cells);
    Heap_Free(arg0);
}

u16 Field2DMap_GetWidth(Field2DMap *arg0) {
    return arg0->width;
}

u16 Field2DMap_GetHeight(Field2DMap *arg0) {
    return arg0->height;
}

void Field2DMap_Load(Field2DMap *arg0, const void *arg1) {
    GF_ASSERT(arg0);
    memcpy(arg0->cells, arg1, arg0->width * arg0->height * 4);
}

Field2DMapCell Field2DMap_GetCell(Field2DMap *arg0, u32 arg1, u32 arg2) {
    Field2DMapCell sp0 = sField2DMapCellNone;
    GF_ASSERT(arg0);
    if ((arg0->width > arg1) && (arg0->height > arg2)) {
        sp0.value = arg0->cells[arg0->width * arg2 + arg1];
    }
    return sp0;
}
