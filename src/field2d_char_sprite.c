#include "overlay_42.h"
#include "sprite_transfer.h"

const u8 sCharCellAnimVariants[4] = { 0, 0, 1, 1 };
const u8 sPlayerCharIndices[8] = { 0, 1, 149, 150, 151, 152, 0, 0 };
const u8 sCharSpriteAnimFrameLimits[12] = { 1, 255, 8, 2, 4, 16, 16, 2, 4, 0, 0, 0 };
const u8 sAvatarCharIndices[16] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };

const CharSpriteAnimUpdateFunc sCharSpriteAnimUpdateFuncs[10] = { NULL, CharSprite_UpdateAnimLoop, CharSprite_UpdateAnimWalk, CharSprite_UpdateAnimTurn, CharSprite_UpdateAnimRun, CharSprite_UpdateAnimBump, CharSprite_UpdateAnimWalkSlow, CharSprite_UpdateAnimWalkVeryFast, CharSprite_UpdateAnimWalkFast, NULL };

const CharSpriteAnimStartFunc sCharSpriteAnimStartFuncs[10] = { CharSprite_StartAnimStand, CharSprite_StartAnimLoop, CharSprite_StartAnimWalk, CharSprite_StartAnimTurn, CharSprite_StartAnimRun, CharSprite_StartAnimBump, CharSprite_StartAnimWalkSlow, CharSprite_StartAnimWalkVeryFast, CharSprite_StartAnimWalkFast, NULL };

const u32 sCellAnimFileIds[12] = { 3, 4, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
const u16 sSpriteIdToCharIndex[305] = { 0, 0, 3, 2, 5, 3, 11, 4, 31, 5, 50, 6, 51, 7, 62, 8, 70, 9, 97, 1, 6, 10, 7, 11, 13, 12, 14, 13, 35, 14, 37, 15, 42, 16, 63, 17, 4, 18, 8, 19, 52, 20, 53, 21, 2, 22, 15, 23, 16, 24, 59, 25, 60, 26, 9, 27, 12, 28, 23, 29, 22, 30, 41, 31, 38, 32, 39, 33, 17, 34, 68, 35, 69, 36, 1, 37, 45, 38, 54, 39, 56, 40, 20, 41, 10, 42, 19, 43, 29, 44, 36, 45, 40, 46, 43, 47, 34, 48, 55, 49, 71, 50, 141, 51, 142, 52, 143, 53, 144, 54, 145, 55, 169, 56, 44, 58, 30, 59, 175, 94, 18, 103, 148, 114, 84, 115, 24, 116, 25, 117, 168, 132, 86, 135, 277, 139, 278, 140, 279, 141, 280, 142, 281, 143, 282, 144, 283, 145, 284, 146, 285, 147, 286, 148, 238, 149, 239, 150, 295, 151, 296, 152 };

CharSpriteManager *CharSpriteManager_New(SpriteList *arg0, PaletteData *arg1, s32 arg2, enum HeapID heapID) {
    int i;
    CharSpriteManager *temp_r0 = Heap_Alloc(heapID, sizeof(CharSpriteManager));
    memset(temp_r0, 0, sizeof(CharSpriteManager));
    temp_r0->spriteList = arg0;
    temp_r0->paletteData = arg1;
    temp_r0->charNarc = NARC_New(NARC_a_1_7_9, heapID);
    temp_r0->avatarNarc = NARC_New(NARC_a_1_8_6, heapID);
    for (i = 0; i < 2; i++) {
        temp_r0->resManagers[i] = Create2DGfxResObjMan(154, (GfGfxResType)i, heapID);
    }
    for (i = 0; i < 2; i++) {
        temp_r0->resManagers[i + 2] = Create2DGfxResObjMan(23, (GfGfxResType)(i + 2), heapID);
    }
    for (i = 0; i < 3; i++) {
        CharSpriteManager_LoadCellAnims(temp_r0, i, heapID);
    }
    temp_r0->maxSprites = arg2;
    temp_r0->sprites = Heap_Alloc(heapID, arg2 * sizeof(CharSprite));
    memset(temp_r0->sprites, 0, sizeof(CharSprite) * temp_r0->maxSprites);
    return temp_r0;
}

void CharSpriteManager_Free(CharSpriteManager *arg0) {
    for (int i = 0; i < arg0->maxSprites; i++) {
        if (CharSprite_IsFree(&arg0->sprites[i]) == 0) {
            CharSprite_Delete(&arg0->sprites[i]);
        }
    }
    for (int i = 0; i < 3; i++) {
        CharSpriteManager_UnloadCellAnims(arg0, i);
    }
    CharSpriteManager_UnloadAllChars(arg0);
    NARC_Delete(arg0->charNarc);
    NARC_Delete(arg0->avatarNarc);
    for (int i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(arg0->resManagers[i]);
    }
    Heap_Free(arg0->sprites);
    Heap_Free(arg0);
}

void CharSpriteManager_LoadChar(CharSpriteManager *arg0, s32 arg1, u8 arg2, s32 arg3, enum HeapID heapID) {
    GF_ASSERT(CharSprite_IsAnimSetAllowed(arg1, arg3));
    CharSpriteManager_LoadCharResources(arg0, CharSprite_GetCharIndex(arg1), arg2, arg3, heapID, 0);
}

void CharSpriteManager_UnloadChar(CharSpriteManager *arg0, s32 arg1) {
    CharSpriteManager_UnloadCharResources(arg0, CharSprite_GetCharIndex(arg1));
}

s32 CharSpriteManager_IsCharLoaded(CharSpriteManager *arg0, s32 arg1) {
    return CharSpriteManager_IsCharIndexLoaded(arg0, CharSprite_GetCharIndex(arg1));
}

void CharSpriteManager_UnloadAllChars(CharSpriteManager *arg0) {
    if (arg0->avatarPalette != 0) {
        CharSpriteManager_UnloadAvatarChars(arg0);
    }
    for (int i = 0; i < 153; i++) {
        if (CharSpriteManager_IsCharIndexLoaded(arg0, i) == 1) {
            CharSpriteManager_UnloadCharResources(arg0, i);
        }
    }
}

void CharSpriteManager_LoadAvatarChars(CharSpriteManager *arg0, u8 arg1, s32 arg2, enum HeapID heapID) {
    GF_ASSERT(arg2);
    CharSpriteManager_LoadAvatarPalette(arg0, arg1, heapID);
    CharSpriteManager_LoadAvatarCellAnims(arg0, heapID);
    for (int i = 0; i < 16; i++) {
        CharSpriteManager_LoadCharResources(arg0, sAvatarCharIndices[i], arg1, arg2, heapID, 1);
    }
}

void CharSpriteManager_UnloadAvatarChars(CharSpriteManager *arg0) {
    CharSpriteManager_UnloadAvatarPalette(arg0);
    CharSpriteManager_UnloadAvatarCellAnims(arg0);
    for (int i = 0; i < 16; i++) {
        CharSpriteManager_UnloadCharResources(arg0, sAvatarCharIndices[i]);
    }
}

void CharSpriteManager_LoadShadow(CharSpriteManager *arg0, u8 arg1, s32 arg2, enum HeapID heapID) {
    CharSpriteManager_LoadShadowResources(arg0, arg1, heapID);
    arg0->shadowDrawPriority = arg2;
}

void CharSpriteManager_UnloadShadow(CharSpriteManager *arg0) {
    CharSpriteManager_UnloadShadowResources(arg0);
}

CharSprite *CharSpriteManager_CreateSprite(CharSpriteManager *arg0, s16 *arg1, s32 arg2, enum HeapID heapID) {
    CharSprite *temp_r4 = CharSpriteManager_GetFreeSprite(arg0);
    u16 r0 = CharSprite_GetCharIndex(arg2);
    GF_ASSERT(CharSpriteManager_IsCharIndexLoaded(arg0, r0) == 1);

    SpriteTemplate spriteTemplate = { NULL };
    spriteTemplate.spriteList = arg0->spriteList;
    spriteTemplate.header = &arg0->chars[r0].header;
    spriteTemplate.position.x = arg1[0] << FX32_SHIFT;
    spriteTemplate.position.y = arg1[1] << FX32_SHIFT;

    spriteTemplate.scale.x = 4096;
    spriteTemplate.scale.y = 4096;
    spriteTemplate.scale.z = 4096;
    spriteTemplate.drawPriority = arg1[2];
    spriteTemplate.heapID = heapID;

    if ((arg0->chars[r0].vramType == 3) || (arg0->chars[r0].vramType == 1)) {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }
    Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);
    temp_r4->sprite = sprite;
    Sprite_SetPriority(sprite, arg1[3]);

    if (arg0->chars[r0].animSet == 1) {
        Sprite_SetAnimCtrlSeq(temp_r4->sprite, 6);
    } else {
        Sprite_SetAnimCtrlSeq(temp_r4->sprite, 1);
    }
    temp_r4->animKind = 9;
    temp_r4->animFrames = 0;
    temp_r4->direction = 1;
    temp_r4->resource = &arg0->chars[r0].sharedPalette;
    if (CharSprite_HasShadowResources(arg0->shadowResources) != 0) {
        memset(&spriteTemplate, 0, sizeof(SpriteTemplate));
        spriteTemplate.spriteList = arg0->spriteList;
        spriteTemplate.header = &arg0->shadowHeader;
        spriteTemplate.position.x = arg1[0] << FX32_SHIFT;
        spriteTemplate.position.y = arg1[1] << FX32_SHIFT;

        spriteTemplate.scale.x = 4096;
        spriteTemplate.scale.y = 4096;
        spriteTemplate.scale.z = 4096;
        spriteTemplate.drawPriority = arg0->shadowDrawPriority;
        spriteTemplate.heapID = heapID;

        if ((arg0->chars[r0].vramType == 3) || (arg0->chars[r0].vramType == 1)) {
            spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        } else {
            spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
        }
        Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);
        temp_r4->shadow = sprite;
        Sprite_SetPriority(sprite, arg1[3]);
    } else {
        temp_r4->shadow = NULL;
    }
    return temp_r4;
}

void CharSprite_Delete(CharSprite *arg0) {
    if (arg0->shadow != 0) {
        Sprite_Delete(arg0->shadow);
    }
    Sprite_Delete(arg0->sprite);
    memset(arg0, 0, sizeof(CharSprite));
}

Sprite *CharSprite_GetSprite(CharSprite *arg0) {
    return arg0->sprite;
}

void CharSprite_SetPosition(CharSprite *arg0, s16 arg1, s16 arg2) {
    VecFx32 sp0;
    sp0.x = arg1 << FX32_SHIFT;
    sp0.y = arg2 << FX32_SHIFT;
    sp0.z = 0;
    Sprite_SetMatrix(arg0->sprite, &sp0);
    if (arg0->shadow != 0) {
        sp0.x += 32768;
        sp0.y += 57344;
        Sprite_SetMatrix(arg0->shadow, &sp0);
    }
}

void CharSprite_SetDrawPriority(CharSprite *arg0, u16 arg1) {
    Sprite_SetDrawPriority(arg0->sprite, arg1);
}

u16 CharSprite_GetDrawPriority(CharSprite *arg0) {
    return Sprite_GetDrawPriority(arg0->sprite);
}

void CharSprite_SetVisible(CharSprite *arg0, u16 arg1) {
    Sprite_SetDrawFlag(arg0->sprite, arg1);
    if (arg0->shadow != NULL) {
        Sprite_SetDrawFlag(arg0->shadow, arg1);
    }
}

BOOL CharSprite_IsVisible(CharSprite *arg0) {
    return Sprite_GetDrawFlag(arg0->sprite);
}

s32 CharSprite_GetAnimSet(CharSprite *arg0) {
    return arg0->resource[4];
}

u32 CharSprite_GetAnimKind(CharSprite *arg0) {
    return arg0->animKind;
}

void CharSprite_SetAnimFrame(CharSprite *arg0, u16 arg1) {
    Sprite_SetAnimationFrame(arg0->sprite, arg1);
}

u16 CharSprite_GetAnimFrame(CharSprite *arg0) {
    return Sprite_GetAnimationFrame(arg0->sprite);
}

void CharSprite_SetShadowPosition(CharSprite *arg0, s16 arg1, s16 arg2) {
    if (arg0->shadow != 0) {
        VecFx32 sp0;
        sp0.x = (arg1 + 8) << FX32_SHIFT;
        sp0.y = (arg2 + 14) << FX32_SHIFT;
        sp0.z = 0;
        Sprite_SetMatrix(arg0->shadow, &sp0);
    }
}

void CharSprite_StartAnim(CharSprite *arg0, s32 arg1, s32 arg2) {
    GF_ASSERT(arg1 < 10);
    GF_ASSERT(arg2 < 4);
    arg0->animKind = arg1;
    arg0->animFrames = 0;
    CharSpriteAnimStartFunc func = sCharSpriteAnimStartFuncs[arg1];
    if (func != NULL) {
        func(arg0, arg2);
    }
}

void CharSprite_UpdateAnim(CharSprite *arg0) {
    BOOL var_r0 = FALSE;
    u8 temp_r2 = sCharSpriteAnimFrameLimits[arg0->animKind];
    if (temp_r2 == 255) {
        var_r0 = TRUE;
    } else if (temp_r2 >= arg0->animFrames + 1) {
        var_r0 = TRUE;
    }
    if (var_r0 == TRUE) {
        CharSprite_StepAnim(arg0);
        arg0->animFrames++;
    }
}

void CharSpriteManager_LoadCellAnims(CharSpriteManager *arg0, s32 arg1, enum HeapID heapID) {
    s32 r5 = arg1 * 4;
    GF_ASSERT(arg0->cellAnims[r5] == NULL);
    for (int i = 0; i < 4; i++) {
        s32 index = i % 2 + 2;
        s32 offset = 256 + arg1 * 2;
        arg0->cellAnims[r5 + i] = AddCellOrAnimResObjFromOpenNarc(arg0->resManagers[index], arg0->charNarc, sCellAnimFileIds[r5 + i], FALSE, sCharCellAnimVariants[i] + offset, (GfGfxResType)index, heapID);
    }
}

void CharSpriteManager_UnloadCellAnims(CharSpriteManager *arg0, s32 arg1) {
    s32 temp_r5 = arg1 * 4;
    GF_ASSERT(arg0->cellAnims[temp_r5]);
    for (int i = 0; i < 4; i++) {
        u32 index = (i % 2) + 2;
        DestroySingle2DGfxResObj(arg0->resManagers[index], arg0->cellAnims[temp_r5 + i]);
        arg0->cellAnims[temp_r5 + i] = NULL;
    }
}

u16 CharSprite_GetCharIndex(s32 arg0) {
    int j = 0;
    for (s32 i = 0; i < 153; i++) {
        if (arg0 == sSpriteIdToCharIndex[j]) {
            return sSpriteIdToCharIndex[i * 2 + 1];
        }
        j += 2;
    }
    GF_ASSERT(FALSE);
    return 0;
}

s32 CharSprite_IsAnimSetAllowed(u32 arg0, s32 arg1) {
    if (arg1 == 0) {
        if ((arg0 == 0) || (arg0 == 97) || (arg0 == 238) || (arg0 == 239) || (arg0 - 295 <= 1)) {
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 CharSprite_GetCellAnimResId(s32 arg0, s32 arg1, s32 arg2) {
    GF_ASSERT(arg1 == 2 || arg1 == 3);
    s32 var_r1 = arg1 - 2;
    if (arg2 == 0) {
        var_r1 += 2;
    }
    return 256 + (arg0 * 2) + sCharCellAnimVariants[var_r1];
}

void CharSpriteManager_LoadCharResources(CharSpriteManager *arg0, s32 arg1, u8 arg2, s32 arg3, enum HeapID heapID, s32 arg5) {
    GF_ASSERT(!arg0->chars[arg1].charResource);
    if (arg5 == 0) {
        CharSpriteManager_LoadCharWithPalette(arg0, arg1, arg2, arg3, heapID);
        return;
    }
    CharSpriteManager_LoadSharedPaletteChar(arg0, arg1, arg2, arg3, heapID);
}

void CharSpriteManager_LoadCharWithPalette(CharSpriteManager *arg0, s32 arg1, s32 arg2, s32 arg3, enum HeapID heapID) {
    arg0->chars[arg1].charResource = AddCharResObjFromOpenNarc(arg0->resManagers[0], arg0->charNarc, arg1 * 2 + 14, 0, arg1 + 512, arg2, (enum HeapID)heapID);
    arg0->chars[arg1].plttResource = AddPlttResObjFromOpenNarc(arg0->resManagers[1], arg0->charNarc, arg1 * 2 + 13, 0, arg1 + 512, arg2, 1, (enum HeapID)heapID);
    NNSG2dCharacterData *charData = GF2DGfxResObj_GetCharDataPtr(arg0->chars[arg1].charResource);

    s32 sp34;
    if (charData->szByte != 6144) {
        sp34 = 1;
    } else {
        sp34 = 0;
    }
    if (arg3 == 2) {
        charData->szByte = 2048;
    } else if (arg3 == 1) {
        if (sp34 != 0) {
            charData->szByte = 5120;
        } else {
            charData->szByte = 6144;
        }
    }
    if (SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(arg0->chars[arg1].charResource) == 0) {
        GF_ASSERT(FALSE);
    }
    if (SpriteTransfer_CreatePlttTransferTask(arg0->chars[arg1].plttResource) == 0) {
        GF_ASSERT(FALSE);
    }
    if (arg0->paletteData != 0) {
        CharSpriteManager_LoadPaletteToBuffer(arg0, arg0->chars[arg1].plttResource, 1);
    }
    GF2DGfxResObj_FreeData(arg0->chars[arg1].charResource);
    GF2DGfxResObj_FreeData(arg0->chars[arg1].plttResource);
    arg0->chars[arg1].vramType = arg2;
    arg0->chars[arg1].animSet = arg3;
    arg0->chars[arg1].sharedPalette = 0;
    s32 temp_r6_2 = CharSprite_GetCellAnimResId(arg3, 2, sp34);
    CreateSpriteResourcesHeader(&arg0->chars[arg1].header, arg1 + 512, arg1 + 512, temp_r6_2, CharSprite_GetCellAnimResId(arg3, 3, sp34), -1, -1, 0, 0, arg0->resManagers[0], arg0->resManagers[1], arg0->resManagers[2], arg0->resManagers[3], 0, 0);
}

void CharSpriteManager_LoadSharedPaletteChar(CharSpriteManager *arg0, s32 arg1, s32 arg2, s32 arg3, enum HeapID heapID) {
    s32 r7 = arg1 - 2;
    arg0->chars[arg1].charResource = AddCharResObjFromOpenNarc(arg0->resManagers[0], arg0->avatarNarc, arg1 - 1, 0, arg1 + 512, arg2, heapID);
    if (arg3 == 2) {
        GF2DGfxResObj_GetCharDataPtr(arg0->chars[arg1].charResource)->szByte = 2048;
    } else if (arg3 == 1) {
        GF2DGfxResObj_GetCharDataPtr(arg0->chars[arg1].charResource)->szByte = 5120;
    }
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(arg0->chars[arg1].charResource));
    GF2DGfxResObj_FreeData(arg0->chars[arg1].charResource);
    arg0->chars[arg1].plttResource = NULL;
    arg0->chars[arg1].vramType = arg2;
    arg0->chars[arg1].animSet = arg3;
    arg0->chars[arg1].sharedPalette = 1;
    CreateSpriteResourcesHeader(&arg0->chars[arg1].header, arg1 + 512, 1026, r7 + 384, r7 + 384, -1, -1, 0, 0, arg0->resManagers[0], arg0->resManagers[1], arg0->resManagers[2], arg0->resManagers[3], 0, 0);
}

void CharSpriteManager_UnloadCharResources(CharSpriteManager *arg0, s32 arg1) {
    GF_ASSERT(arg0->chars[arg1].charResource);
    if (arg0->chars[arg1].sharedPalette == 0) {
        CharSpriteManager_UnloadCharWithPalette(arg0, arg1);
        return;
    }
    CharSpriteManager_UnloadSharedPaletteChar(arg0, arg1);
}

void CharSpriteManager_UnloadCharWithPalette(CharSpriteManager *arg0, s32 arg1) {
    SpriteTransfer_DeleteCharTransferTask(arg0->chars[arg1].charResource);
    SpriteTransfer_DeletePlttTransferTask(arg0->chars[arg1].plttResource);
    DestroySingle2DGfxResObj(arg0->resManagers[0], arg0->chars[arg1].charResource);
    DestroySingle2DGfxResObj(arg0->resManagers[1], arg0->chars[arg1].plttResource);
    arg0->chars[arg1].charResource = NULL;
    arg0->chars[arg1].plttResource = NULL;
}

void CharSpriteManager_UnloadSharedPaletteChar(CharSpriteManager *arg0, s32 arg1) {
    SpriteTransfer_DeleteCharTransferTask(arg0->chars[arg1].charResource);
    DestroySingle2DGfxResObj(arg0->resManagers[0], arg0->chars[arg1].charResource);
    arg0->chars[arg1].charResource = NULL;
}

BOOL CharSpriteManager_IsCharIndexLoaded(CharSpriteManager *arg0, s32 arg1) {
    if (arg0->chars[arg1].charResource != NULL) {
        return TRUE;
    }
    return FALSE;
}

void CharSpriteManager_LoadShadowResources(CharSpriteManager *arg0, s32 arg1, enum HeapID heapID) {
    GF_ASSERT(arg0->shadowResources[0] == NULL);
    int i = 0;
    s32 sp34 = 0;
    s32 sp30;
    for (i = 0; i < 6; i++) {
        if (CharSpriteManager_IsCharIndexLoaded(arg0, sPlayerCharIndices[i]) == 1) {
            sp30 = sPlayerCharIndices[i];
            sp34 = 1;
            break;
        }
    }
    GF_ASSERT(sp34 == 1);
    for (i = 0; i < 2; i++) {
        arg0->shadowResources[i + 2] = AddCellOrAnimResObjFromOpenNarc(arg0->resManagers[i + 2], arg0->charNarc, i, 0, 288, (GfGfxResType)(i + 2), heapID);
    }
    arg0->shadowResources[0] = AddCharResObjFromOpenNarc(arg0->resManagers[0], arg0->charNarc, 2, 0, 288, arg1, heapID);
    if (SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(arg0->shadowResources[0]) == 0) {
        GF_ASSERT(FALSE);
    }
    GF2DGfxResObj_FreeData(arg0->shadowResources[0]);
    CreateSpriteResourcesHeader(&arg0->shadowHeader, 288, sp30 + 512, 288, 288, -1, -1, 0, 0, arg0->resManagers[0], arg0->resManagers[1], arg0->resManagers[2], arg0->resManagers[3], 0, 0);
}

void CharSpriteManager_UnloadShadowResources(CharSpriteManager *arg0) {
    if (CharSprite_HasShadowResources(arg0->shadowResources) == 1) {
        SpriteTransfer_DeleteCharTransferTask(arg0->shadowResources[0]);
        DestroySingle2DGfxResObj(arg0->resManagers[0], arg0->shadowResources[0]);
        arg0->shadowResources[0] = NULL;
        for (int i = 0; i < 2; i++) {
            DestroySingle2DGfxResObj(arg0->resManagers[i + 2], arg0->shadowResources[i + 2]);
            arg0->shadowResources[i + 2] = NULL;
        }
    }
}

BOOL CharSprite_HasShadowResources(SpriteResource **spriteResources) {
    return spriteResources[GF_GFX_RES_TYPE_CHAR] != NULL;
}

void CharSpriteManager_LoadAvatarPalette(CharSpriteManager *arg0, s32 arg1, enum HeapID heapID) {
    GF_ASSERT(!arg0->avatarPalette);
    arg0->avatarPalette = AddPlttResObjFromOpenNarc(arg0->resManagers[1], arg0->avatarNarc, 0, 0, 1026, arg1, 8, heapID);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(arg0->avatarPalette));
    if (arg0->paletteData != 0) {
        CharSpriteManager_LoadPaletteToBuffer(arg0, arg0->avatarPalette, 8);
    }
    GF2DGfxResObj_FreeData(arg0->avatarPalette);
}

void CharSpriteManager_UnloadAvatarPalette(CharSpriteManager *arg0) {
    GF_ASSERT(arg0->avatarPalette);
    SpriteTransfer_DeletePlttTransferTask(arg0->avatarPalette);
    DestroySingle2DGfxResObj(arg0->resManagers[1], arg0->avatarPalette);
    arg0->avatarPalette = NULL;
}

void CharSpriteManager_LoadAvatarCellAnims(CharSpriteManager *arg0, enum HeapID heapID) {
    GF_ASSERT(!arg0->avatarCellAnims[0].cell);
    for (int i = 0; i < 16; i++) {
        arg0->avatarCellAnims[i].cell = AddCellOrAnimResObjFromOpenNarc(arg0->resManagers[2], arg0->avatarNarc, 17 + (i * 2), 0, i + 384, GF_GFX_RES_TYPE_CELL, heapID);
        arg0->avatarCellAnims[i].anim = AddCellOrAnimResObjFromOpenNarc(arg0->resManagers[3], arg0->avatarNarc, 18 + (i * 2), 0, i + 384, GF_GFX_RES_TYPE_ANIM, heapID);
    }
}

void CharSpriteManager_UnloadAvatarCellAnims(CharSpriteManager *arg0) {
    GF_ASSERT(arg0->avatarCellAnims[0].cell);
    for (int i = 0; i < 16; i++) {
        DestroySingle2DGfxResObj(arg0->resManagers[2], arg0->avatarCellAnims[i].cell);
        DestroySingle2DGfxResObj(arg0->resManagers[3], arg0->avatarCellAnims[i].anim);
        arg0->avatarCellAnims[i].cell = NULL;
        arg0->avatarCellAnims[i].anim = NULL;
    }
}

CharSprite *CharSpriteManager_GetFreeSprite(CharSpriteManager *arg0) {
    for (int i = 0; i < arg0->maxSprites; i++) {
        if (CharSprite_IsFree(&arg0->sprites[i]) == 1) {
            return &arg0->sprites[i];
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}

void CharSprite_StepAnim(CharSprite *arg0) {
    CharSpriteAnimUpdateFunc spriteFunc = sCharSpriteAnimUpdateFuncs[arg0->animKind];
    if (spriteFunc != NULL) {
        spriteFunc(arg0);
    }
}
