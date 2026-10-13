#include "global.h"

#include "constants/species.h"

#include "pokeathlon/pokeathlon_course_internal.h"

#include "heap.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"

#pragma force_active on

// Allocates the course sprite resources; flags selects which of the 12 sprite groups get loaded.
PokeathlonCourseSpriteResources *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, u32 flags, int charVram) {
    u8 counts[4];
    PokeathlonCourseSpriteResources *resources;
    int i;
    for (i = 0; i < 4; i++) {
        counts[i] = 0;
    }
    if (flags & 0x001) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x004) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x002) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x008) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x010) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x020) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x040) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x080) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x100) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x200) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x400) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    if (flags & 0x800) {
        counts[GF_GFX_RES_TYPE_CHAR]++;
        counts[GF_GFX_RES_TYPE_CELL]++;
        counts[GF_GFX_RES_TYPE_ANIM]++;
    }
    counts[GF_GFX_RES_TYPE_PLTT] = 2;
    resources = Heap_Alloc(heapId, sizeof(PokeathlonCourseSpriteResources));
    resources->heapId = heapId;
    resources->flags = flags;
    resources->charVram = charVram;
    for (i = 0; i < 4; i++) {
        resources->resMans[i] = Create2DGfxResObjMan(counts[i], (GfGfxResType)i, resources->heapId);
    }
    PokeathlonCourseSprites_LoadResources(resources);
    return resources;
}

void PokeathlonCourseSprites_FreeResources(PokeathlonCourseSpriteResources *resources) {
    int i;

    if (resources->flags & 0x001) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[0]);
    }
    if (resources->flags & 0x004) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[1]);
    }
    if (resources->flags & 0x002) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[2]);
    }
    if (resources->flags & 0x008) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[3]);
    }
    if (resources->flags & 0x010) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[4]);
    }
    if (resources->flags & 0x020) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[5]);
    }
    if (resources->flags & 0x040) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[6]);
    }
    if (resources->flags & 0x080) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[7]);
    }
    if (resources->flags & 0x100) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[8]);
    }
    if (resources->flags & 0x200) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[9]);
    }
    if (resources->flags & 0x400) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[10]);
    }
    if (resources->flags & 0x800) {
        SpriteTransfer_DeleteCharTransferTask(resources->chars[11]);
    }
    SpriteTransfer_DeletePlttTransferTask(resources->palette);
    if (resources->extraPalette) {
        SpriteTransfer_DeletePlttTransferTask(resources->extraPalette);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(resources->resMans[i]);
    }
    Heap_Free(resources);
}

#pragma force_active reset

u8 PokeathlonCourseSprites_HasGroup(PokeathlonCourseSpriteResources *resources, u32 mask) {
    if (resources == NULL) {
        return 0;
    }
    return resources->flags & mask;
}

GF_2DGfxResMan *PokeathlonCourseSprites_GetResMan(PokeathlonCourseSpriteResources *resources, int type) {
    return resources->resMans[type];
}

#pragma force_active on

// Loads the character, cell and animation files of each selected group (NARC a/1/5/3) and the palettes.
void PokeathlonCourseSprites_LoadResources(PokeathlonCourseSpriteResources *resources) {
    int vram;

    if (resources->flags & 0x001) {
        resources->chars[0] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 3, FALSE, 0x14, 1, resources->heapId);
        resources->cells[0] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 2, FALSE, 0x14, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[0] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 1, FALSE, 0x14, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[0]);
    }
    if (resources->flags & 0x004) {
        resources->chars[1] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 6, FALSE, 0x15, resources->charVram, resources->heapId);
        resources->cells[1] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 5, FALSE, 0x15, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[1] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 4, FALSE, 0x15, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[1]);
    }
    if (resources->flags & 0x002) {
        resources->chars[2] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 9, FALSE, 0x16, resources->charVram, resources->heapId);
        resources->cells[2] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 8, FALSE, 0x16, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[2] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 7, FALSE, 0x16, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[2]);
    }
    if (resources->flags & 0x010) {
        resources->chars[4] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 16, FALSE, 0x1A, 1, resources->heapId);
        resources->cells[4] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 15, FALSE, 0x1A, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[4] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 14, FALSE, 0x1A, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[4]);
    }
    if (resources->flags & 0x020) {
        resources->chars[5] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 19, FALSE, 0x1B, 1, resources->heapId);
        resources->cells[5] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 18, FALSE, 0x1B, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[5] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 17, FALSE, 0x1B, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[5]);
    }
    if (resources->flags & 0x008) {
        resources->chars[3] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 13, FALSE, 0x19, 2, resources->heapId);
        resources->cells[3] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 12, FALSE, 0x19, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[3] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 11, FALSE, 0x19, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[3]);
    }
    if (resources->flags & 0x040) {
        resources->chars[6] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 22, FALSE, 0x1C, 1, resources->heapId);
        resources->cells[6] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 21, FALSE, 0x1C, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[6] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 20, FALSE, 0x1C, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[6]);
    }
    if (resources->flags & 0x080) {
        resources->chars[7] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 25, FALSE, 0x1D, 1, resources->heapId);
        resources->cells[7] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 24, FALSE, 0x1D, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[7] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 23, FALSE, 0x1D, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[7]);
    }
    if (resources->flags & 0x100) {
        resources->chars[8] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 28, FALSE, 0x1E, 2, resources->heapId);
        resources->cells[8] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 27, FALSE, 0x1E, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[8] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 26, FALSE, 0x1E, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[8]);
    }
    if (resources->flags & 0x200) {
        resources->chars[9] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 31, FALSE, 0x1F, 1, resources->heapId);
        resources->cells[9] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 30, FALSE, 0x1F, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[9] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 29, FALSE, 0x1F, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[9]);
    }
    if (resources->flags & 0x400) {
        resources->chars[10] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 34, FALSE, 0x1F, 2, resources->heapId);
        resources->cells[10] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 33, FALSE, 0x1F, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[10] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 32, FALSE, 0x1F, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[10]);
    }
    if (resources->flags & 0x800) {
        resources->chars[11] = AddCharResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_3, 37, FALSE, 0x20, 1, resources->heapId);
        resources->cells[11] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_3, 36, FALSE, 0x20, GF_GFX_RES_TYPE_CELL, resources->heapId);
        resources->anims[11] = AddCellOrAnimResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_3, 35, FALSE, 0x20, GF_GFX_RES_TYPE_ANIM, resources->heapId);
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources->chars[11]);
    }
    vram = resources->charVram;
    if (vram == 2) {
        vram = 3;
    }
    resources->palette = AddPlttResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_1_5_3, 0, FALSE, 0x17, vram, 1, resources->heapId);
    SpriteTransfer_CreateExtPlttTransferTask(resources->palette);
    resources->extraPalette = NULL;
    if ((resources->flags & 0x008) || (resources->flags & 0x100) || (resources->flags & 0x400)) {
        resources->extraPalette = AddPlttResObjFromNarc(resources->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_1_5_3, 10, FALSE, 0x18, 2, 1, resources->heapId);
        SpriteTransfer_CreateExtPlttTransferTask(resources->extraPalette);
    }
}

#pragma force_active reset

// "3, 2, 1" countdown and finish sprite (group 0x14).
Sprite *PokeathlonCourseSprites_CreateCountdown(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, NNS_G2D_VRAM_TYPE whichScreen, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x14, 0x17, 0x14, 0x14, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = whichScreen;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

Sprite *PokeathlonCourseSprites_CreateTimerDigit(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x19, 0x18, 0x19, 0x19, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

Sprite *ov96_021EA374(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x1C, 0x17, 0x1C, 0x1C, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.drawPriority = 0;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

Sprite *PokeathlonCourseSprites_CreateLapCounterDigit(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x1A, 0x17, 0x1A, 0x1A, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.drawPriority = 0;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

// "New record" sprite (group 0x1B).
Sprite *PokeathlonCourseSprites_CreateNewRecord(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x1B, 0x17, 0x1B, 0x1B, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.drawPriority = 0;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

// Countdown of the last seconds of an event (group 0x1D).
Sprite *PokeathlonCourseSprites_CreateTimeUpCountdown(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x1D, 0x17, 0x1D, 0x1D, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.drawPriority = 0;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

Sprite *ov96_021EA634(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x1E, 0x18, 0x1E, 0x1E, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.drawPriority = 0;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

#pragma force_active on

// "Start" sprite (group 0x1F) on the given screen, with that screen's palette.
Sprite *PokeathlonCourseSprites_CreateStart(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, NNS_G2D_VRAM_TYPE whichScreen, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;
    Sprite *sprite;
    int paletteId = whichScreen == NNS_G2D_VRAM_TYPE_2DMAIN ? 0x17 : 0x18;
    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x1F, paletteId, 0x1F, 0x1F, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 1;
    template.whichScreen = whichScreen;
    sprite = Sprite_CreateAffine(&template);
    if (!sprite) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

#pragma force_active reset

// Marker that follows a course object (group 0x20), drawn behind the other sprites.
Sprite *PokeathlonCourseSprites_CreateMarker(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, 0x20, 0x17, 0x20, 0x20, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    template.spriteList = spriteList;
    template.header = &header;
    template.heapID = heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 2;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    sprite = Sprite_CreateAffine(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

PokeathlonCourseMonObjects *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int count, int baseDrawPriority, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList) {
    PokeathlonCourseMonObjects *objects = Heap_Alloc(heapId, sizeof(PokeathlonCourseMonObjects));
    MI_CpuFill8(objects, 0, sizeof(PokeathlonCourseMonObjects));
    objects->heapId = heapId;
    objects->count = count;
    objects->resources = resources;
    objects->spriteList = spriteList;
    objects->baseDrawPriority = baseDrawPriority;
    objects->gfxManager = PokeathlonCourseMonGfx_AllocManager(heapId);
    PokeathlonCourseMonGfx_InitManager(objects->gfxManager, count, spriteList);
    return objects;
}

void PokeathlonCourseMonObjects_Free(PokeathlonCourseMonObjects *objects) {
    PokeathlonCourseMonGfx_FreeManager(objects->gfxManager);
    Heap_Free(objects);
}

#pragma force_active on

// Sets up the shadow and marker sprites of each Pokemon (if their groups are loaded) and starts loading the
// Pokemon sprites; PokeathlonCourseMonObjects_FinishLoad links them once loaded.
void PokeathlonCourseMonObjects_Load(PokeathlonCourseMonObjects *objects, int count, PokeathlonCourseMonInfo *infos, PokeathlonCourseMonGfxTemplate *template, u32 priority, int baseIndex) {
    int i;
    int taskPriority = priority;
    for (i = 0; i < objects->count; i++) {
        objects->objects[i].halfHeight = objects->objects[i].halfWidth = infos[i].isLarge ? 32 : 16;
        switch (template->sizeTypes[i]) {
        case 1:
            objects->objects[i].shadowOffsetY = 3;
            break;
        case 2:
            objects->objects[i].shadowOffsetY = 4;
            break;
        case 3:
            objects->objects[i].shadowOffsetY = 5;
            break;
        default:
            GF_AssertFail();
            objects->objects[i].shadowOffsetY = 3;
            break;
        }
        if (PokeathlonCourseSprites_HasGroup(objects->resources, 0x002)) {
            objects->objects[i].shadowSprite = PokeathlonCourseMonObjects_CreateSprite(objects, template->priority, template->whichScreen, 0x16);
            if (infos[i].isLarge) {
                Sprite_SetAnimCtrlSeq(objects->objects[i].shadowSprite, 1);
            } else {
                Sprite_SetAnimCtrlSeq(objects->objects[i].shadowSprite, 0);
            }
            if (infos[i].species == SPECIES_DIGLETT || infos[i].species == SPECIES_DUGTRIO) {
                objects->objects[i].shadowHidden = TRUE;
                Sprite_SetDrawFlag(objects->objects[i].shadowSprite, FALSE);
            } else {
                objects->objects[i].shadowHidden = FALSE;
            }
        }
        if (PokeathlonCourseSprites_HasGroup(objects->resources, 0x004)) {
            objects->objects[i].markerSprite = PokeathlonCourseMonObjects_CreateSprite(objects, template->priority, template->whichScreen, 0x15);
            GF_ASSERT(infos[i].participant < 4);
            Sprite_SetAnimCtrlSeq(objects->objects[i].markerSprite, infos[i].participant);
            switch (template->sizeTypes[i]) {
            case 1:
                objects->objects[i].markerOffsetY = 3;
                break;
            case 2:
                objects->objects[i].markerOffsetY = 4;
                break;
            case 3:
                objects->objects[i].markerOffsetY = 5;
                break;
            default:
                GF_AssertFail();
                objects->objects[i].markerOffsetY = 3;
                break;
            }
        }
        objects->objects[i].drawPriorityStep = 12;
    }
    if (taskPriority <= 0) {
        taskPriority = 1;
    }
    PokeathlonCourseMonGfx_StartLoad(objects->gfxManager, count, infos, template, taskPriority - 1, baseIndex);
    SysTask_CreateOnMainQueue(PokeathlonCourseMonObjects_FinishLoad, objects, taskPriority);
}

#pragma force_active reset

BOOL PokeathlonCourseMonObjects_IsLoaded(PokeathlonCourseMonObjects *objects) {
    return objects->loaded;
}

#pragma force_active on

PokeathlonCourseMonObject *PokeathlonCourseMonObjects_Get(PokeathlonCourseMonObjects *objects, int index) {
    GF_ASSERT(index < objects->count);
    return &objects->objects[index];
}

#pragma force_active reset

PokeathlonCourseMonGfx *PokeathlonCourseMonObject_GetGfx(PokeathlonCourseMonObject *object) {
    return object->gfx;
}

void PokeathlonCourseMonObjects_FinishLoad(SysTask *task, void *data) {
    PokeathlonCourseMonObjects *objects = data;
    int i;
    int drawPriority;
    if (PokeathlonCourseMonGfx_IsLoaded(objects->gfxManager) == FALSE) {
        return;
    }
    drawPriority = objects->baseDrawPriority;
    for (i = 0; i < objects->count; i++) {
        objects->objects[i].gfx = PokeathlonCourseMonGfx_Get(objects->gfxManager, (u8)i);
        PokeathlonCourseMonObject_SetDrawPriority(&objects->objects[i], drawPriority);
        drawPriority++;
        objects->objects[i].topRow = PokeathlonCourseMonGfx_GetTopRow(objects->objects[i].gfx);
    }
    objects->loaded = TRUE;
    SysTask_Destroy(task);
}

Sprite *PokeathlonCourseMonObjects_CreateSprite(PokeathlonCourseMonObjects *objects, int priority, NNS_G2D_VRAM_TYPE whichScreen, int resourceId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;

    charMan = PokeathlonCourseSprites_GetResMan(objects->resources, GF_GFX_RES_TYPE_CHAR);
    plttMan = PokeathlonCourseSprites_GetResMan(objects->resources, GF_GFX_RES_TYPE_PLTT);
    cellMan = PokeathlonCourseSprites_GetResMan(objects->resources, GF_GFX_RES_TYPE_CELL);
    animMan = PokeathlonCourseSprites_GetResMan(objects->resources, GF_GFX_RES_TYPE_ANIM);
    CreateSpriteResourcesHeader(&header, resourceId, 0x17, resourceId, resourceId, -1, -1, 0, priority, charMan, plttMan, cellMan, animMan, NULL, NULL);
    if (whichScreen == NNS_G2D_VRAM_TYPE_MAX) {
        whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
    template.spriteList = objects->spriteList;
    template.header = &header;
    template.heapID = objects->heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = whichScreen;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

void PokeathlonCourseMonObject_SetDrawFlag(PokeathlonCourseMonObject *object, BOOL draw) {
    Sprite *sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);
    Sprite_SetDrawFlag(sprite, draw);
    if (object->shadowSprite != NULL) {
        if (object->shadowHidden != FALSE) {
            Sprite_SetDrawFlag(object->shadowSprite, FALSE);
        } else {
            Sprite_SetDrawFlag(object->shadowSprite, draw);
        }
    }
    if (object->markerSprite != NULL) {
        Sprite_SetDrawFlag(object->markerSprite, draw);
    }
}

void PokeathlonCourseMonObject_SetShadowDrawFlag(PokeathlonCourseMonObject *object, BOOL draw) {
    Sprite *sprite = object->shadowSprite;
    if (sprite != NULL) {
        if (object->shadowHidden != FALSE) {
            Sprite_SetDrawFlag(sprite, FALSE);
        } else {
            Sprite_SetDrawFlag(sprite, draw);
        }
    }
}

void PokeathlonCourseMonObject_SetMonDrawFlag(PokeathlonCourseMonObject *object, BOOL draw) {
    Sprite_SetDrawFlag(PokeathlonCourseMonGfx_GetSprite(object->gfx), draw);
}

#pragma force_active on

// The shadow is drawn 2 steps and the marker 1 step behind the Pokemon.
void PokeathlonCourseMonObject_SetDrawPriority(PokeathlonCourseMonObject *object, int drawPriority) {
    Sprite_SetDrawPriority(PokeathlonCourseMonGfx_GetSprite(object->gfx), drawPriority);
    if (object->shadowSprite) {
        Sprite_SetDrawPriority(object->shadowSprite, drawPriority + 2 * object->drawPriorityStep);
    }
    if (object->markerSprite) {
        Sprite_SetDrawPriority(object->markerSprite, drawPriority + object->drawPriorityStep);
    }
}

#pragma force_active reset

void PokeathlonCourseMonObject_SetDrawPriorityStep(PokeathlonCourseMonObject *object, s32 step) {
    object->drawPriorityStep = step;
}

void PokeathlonCourseMonObject_SetAffineOverwriteMode(PokeathlonCourseMonObject *object, u8 mode) {
    Sprite_SetAffineOverwriteMode(PokeathlonCourseMonGfx_GetSprite(object->gfx), mode);
}

void PokeathlonCourseMonObject_SetAffineMatrix(PokeathlonCourseMonObject *object, VecFx32 *matrix) {
    Sprite_SetAffineMatrix(PokeathlonCourseMonGfx_GetSprite(object->gfx), matrix);
}

void PokeathlonCourseMonObject_SetAnimStateRaw(PokeathlonCourseMonObject *object, u32 animState) {
    object->animState = animState;
}

#pragma force_active on

// Animation state 1..6: facing / pose; selects the walk animation sequence (6 -> sequence 8).
void PokeathlonCourseMonObject_SetAnimState(PokeathlonCourseMonObject *object, u32 animState) {
    object->animState = animState;
    if (animState) {
        Sprite *sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);
        u8 seq;
        switch (animState) {
        case 1:
            seq = 0;
            break;
        case 2:
            seq = 1;
            break;
        case 3:
            seq = 2;
            break;
        case 4:
            seq = 3;
            break;
        case 5:
            seq = 4;
            break;
        case 6:
            seq = 8;
            break;
        default:
            GF_AssertFail();
            return;
        }
        Sprite_TryChangeAnimSeq(sprite, seq);
    }
}

void PokeathlonCourseMonObject_SetAnim(PokeathlonCourseMonObject *object, int anim) {
    Sprite *sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);
    switch (anim) {
    case 0:
    case 4:
    case 8:
    case 12:
    case 16:
    case 20:
    case 21:
    case 22:
    case 26:
        object->anim = anim;
        break;
    default:
        return;
    }
    u8 seq;
    if (anim != 20 && anim != 21) {
        switch (object->animState) {
        case 1:
            seq = (u8)anim;
            break;
        case 2:
            seq = (u8)(anim + 1);
            break;
        case 3:
            seq = (u8)(anim + 2);
            break;
        case 4:
            seq = (u8)(anim + 3);
            break;
        default:
            GF_AssertFail();
            return;
        }
    } else {
        seq = (u8)anim;
    }
    Sprite_TryChangeAnimSeq(sprite, seq);
}

void ov96_021EAD08(PokeathlonCourseMonObject *object, int anim) {
    Sprite *sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);
    switch (anim) {
    case 0:
        object->anim = 0;
        break;
    case 20:
        object->anim = 4;
        break;
    case 21:
        object->anim = 5;
        break;
    default:
        return;
    }
    u8 seq;
    if (anim != 20 && anim != 21) {
        switch (object->animState) {
        case 1:
            seq = 0;
            break;
        case 2:
            seq = 1;
            break;
        case 3:
            seq = 2;
            break;
        case 4:
            seq = 3;
            break;
        default:
            GF_AssertFail();
            return;
        }
    } else {
        seq = (u8)object->anim;
    }
    Sprite_TryChangeAnimSeq(sprite, seq);
}

#pragma force_active reset

BOOL PokeathlonCourseMonObject_IsAnimated(PokeathlonCourseMonObject *object) {
    return Sprite_IsAnimated(PokeathlonCourseMonGfx_GetSprite(object->gfx));
}

// Places the Pokemon's top-left corner at (x, y), its marker below it and, if moveShadow, its shadow.
void PokeathlonCourseMonObject_SetPosition(PokeathlonCourseMonObject *object, s32 x, s32 y, BOOL moveShadow) {
    VecFx32 matrix;
    VecFx32 position = { 0 };
    Sprite *sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);

    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    object->x = x + object->halfWidth;
    object->y = y + object->halfHeight;
    if (object->markerSprite != NULL) {
        matrix = position;
        matrix.x += object->halfWidth << FX32_SHIFT;
        matrix.y += (2 * object->halfHeight - object->markerOffsetY) << FX32_SHIFT;
        Sprite_SetMatrix(object->markerSprite, &matrix);
    }
    if (moveShadow && object->shadowSprite != NULL) {
        matrix = position;
        matrix.x += object->halfWidth << FX32_SHIFT;
        matrix.y += (2 * object->halfHeight - object->shadowOffsetY) << FX32_SHIFT;
        Sprite_SetMatrix(object->shadowSprite, &matrix);
    }
    if (PokeathlonCourseMonGfx_IsShiftedUp(object->gfx)) {
        position.y += 8 * FX32_ONE;
    }
    Sprite_SetMatrix(sprite, &position);
}

void PokeathlonCourseMonObject_SetMonPosition(PokeathlonCourseMonObject *object, s32 x, s32 y) {
    VecFx32 position = { 0 };
    Sprite *sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    object->x = x + object->halfWidth;
    object->y = y + object->halfHeight;
    if (PokeathlonCourseMonGfx_IsShiftedUp(object->gfx)) {
        position.y += 8 * FX32_ONE;
    }
    Sprite_SetMatrix(sprite, &position);
}

void PokeathlonCourseMonObject_GetSpritePosition(PokeathlonCourseMonObject *object, s32 *x, s32 *y) {
    VecFx32 *matrix = Sprite_GetMatrixPtr(PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonObject_GetGfx(object)));
    *x = ((s32)(matrix->x + ((u32)(matrix->x >> 11) >> 20))) >> FX32_SHIFT;
    *y = ((s32)(matrix->y + ((u32)(matrix->y >> 11) >> 20))) >> FX32_SHIFT;
}

void PokeathlonCourseMonObject_GetPosition(PokeathlonCourseMonObject *object, s32 *x, s32 *y) {
    *x = object->x;
    *y = object->y;
}

#pragma force_active on

void PokeathlonCourseMonObject_Move(PokeathlonCourseMonObject *object, s32 dx, s32 dy, u32 animState, BOOL moveShadow) {
    VecFx32 delta;
    VecFx32 result;
    Sprite *sprite;
    object->x += dx;
    object->y += dy;
    delta.x = dx << FX32_SHIFT;
    delta.y = dy << FX32_SHIFT;
    delta.z = 0;
    sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);
    PokeathlonCourseMonObject_SetAnimState(object, animState);
    VEC_Add(&delta, Sprite_GetMatrixPtr(sprite), &result);
    Sprite_SetMatrix(sprite, &result);
    if (object->markerSprite) {
        VEC_Add(&delta, Sprite_GetMatrixPtr(object->markerSprite), &result);
        Sprite_SetMatrix(object->markerSprite, &result);
    }
    if (moveShadow && object->shadowSprite) {
        VEC_Add(&delta, Sprite_GetMatrixPtr(object->shadowSprite), &result);
        Sprite_SetMatrix(object->shadowSprite, &result);
    }
}

#pragma force_active reset
