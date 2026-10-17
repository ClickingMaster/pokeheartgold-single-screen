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

CharSpriteManager *CharSpriteManager_New(SpriteList *spriteList, PaletteData *paletteData, s32 maxSprites, enum HeapID heapID) {
    int i;
    CharSpriteManager *charSpriteManager = Heap_Alloc(heapID, sizeof(CharSpriteManager));
    memset(charSpriteManager, 0, sizeof(CharSpriteManager));
    charSpriteManager->spriteList = spriteList;
    charSpriteManager->paletteData = paletteData;
    charSpriteManager->charNarc = NARC_New(NARC_a_1_7_9, heapID);
    charSpriteManager->avatarNarc = NARC_New(NARC_a_1_8_6, heapID);
    for (i = 0; i < 2; i++) {
        charSpriteManager->resManagers[i] = Create2DGfxResObjMan(154, (GfGfxResType)i, heapID);
    }
    for (i = 0; i < 2; i++) {
        charSpriteManager->resManagers[i + 2] = Create2DGfxResObjMan(23, (GfGfxResType)(i + 2), heapID);
    }
    for (i = 0; i < 3; i++) {
        CharSpriteManager_LoadCellAnims(charSpriteManager, i, heapID);
    }
    charSpriteManager->maxSprites = maxSprites;
    charSpriteManager->sprites = Heap_Alloc(heapID, maxSprites * sizeof(CharSprite));
    memset(charSpriteManager->sprites, 0, sizeof(CharSprite) * charSpriteManager->maxSprites);
    return charSpriteManager;
}

void CharSpriteManager_Free(CharSpriteManager *charSpriteManager) {
    for (int i = 0; i < charSpriteManager->maxSprites; i++) {
        if (CharSprite_IsFree(&charSpriteManager->sprites[i]) == 0) {
            CharSprite_Delete(&charSpriteManager->sprites[i]);
        }
    }
    for (int i = 0; i < 3; i++) {
        CharSpriteManager_UnloadCellAnims(charSpriteManager, i);
    }
    CharSpriteManager_UnloadAllChars(charSpriteManager);
    NARC_Delete(charSpriteManager->charNarc);
    NARC_Delete(charSpriteManager->avatarNarc);
    for (int i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(charSpriteManager->resManagers[i]);
    }
    Heap_Free(charSpriteManager->sprites);
    Heap_Free(charSpriteManager);
}

void CharSpriteManager_LoadChar(CharSpriteManager *charSpriteManager, s32 spriteId, u8 vramType, s32 animSet, enum HeapID heapID) {
    GF_ASSERT(CharSprite_IsAnimSetAllowed(spriteId, animSet));
    CharSpriteManager_LoadCharResources(charSpriteManager, CharSprite_GetCharIndex(spriteId), vramType, animSet, heapID, 0);
}

void CharSpriteManager_UnloadChar(CharSpriteManager *charSpriteManager, s32 spriteId) {
    CharSpriteManager_UnloadCharResources(charSpriteManager, CharSprite_GetCharIndex(spriteId));
}

s32 CharSpriteManager_IsCharLoaded(CharSpriteManager *charSpriteManager, s32 spriteId) {
    return CharSpriteManager_IsCharIndexLoaded(charSpriteManager, CharSprite_GetCharIndex(spriteId));
}

void CharSpriteManager_UnloadAllChars(CharSpriteManager *charSpriteManager) {
    if (charSpriteManager->avatarPalette != 0) {
        CharSpriteManager_UnloadAvatarChars(charSpriteManager);
    }
    for (int i = 0; i < 153; i++) {
        if (CharSpriteManager_IsCharIndexLoaded(charSpriteManager, i) == 1) {
            CharSpriteManager_UnloadCharResources(charSpriteManager, i);
        }
    }
}

void CharSpriteManager_LoadAvatarChars(CharSpriteManager *charSpriteManager, u8 vramType, s32 animSet, enum HeapID heapID) {
    GF_ASSERT(animSet);
    CharSpriteManager_LoadAvatarPalette(charSpriteManager, vramType, heapID);
    CharSpriteManager_LoadAvatarCellAnims(charSpriteManager, heapID);
    for (int i = 0; i < 16; i++) {
        CharSpriteManager_LoadCharResources(charSpriteManager, sAvatarCharIndices[i], vramType, animSet, heapID, 1);
    }
}

void CharSpriteManager_UnloadAvatarChars(CharSpriteManager *charSpriteManager) {
    CharSpriteManager_UnloadAvatarPalette(charSpriteManager);
    CharSpriteManager_UnloadAvatarCellAnims(charSpriteManager);
    for (int i = 0; i < 16; i++) {
        CharSpriteManager_UnloadCharResources(charSpriteManager, sAvatarCharIndices[i]);
    }
}

void CharSpriteManager_LoadShadow(CharSpriteManager *charSpriteManager, u8 vramType, s32 drawPriority, enum HeapID heapID) {
    CharSpriteManager_LoadShadowResources(charSpriteManager, vramType, heapID);
    charSpriteManager->shadowDrawPriority = drawPriority;
}

void CharSpriteManager_UnloadShadow(CharSpriteManager *charSpriteManager) {
    CharSpriteManager_UnloadShadowResources(charSpriteManager);
}

CharSprite *CharSpriteManager_CreateSprite(CharSpriteManager *charSpriteManager, s16 *spriteParams, s32 spriteId, enum HeapID heapID) {
    CharSprite *charSprite = CharSpriteManager_GetFreeSprite(charSpriteManager);
    u16 charIndex = CharSprite_GetCharIndex(spriteId);
    GF_ASSERT(CharSpriteManager_IsCharIndexLoaded(charSpriteManager, charIndex) == 1);

    SpriteTemplate spriteTemplate = { NULL };
    spriteTemplate.spriteList = charSpriteManager->spriteList;
    spriteTemplate.header = &charSpriteManager->chars[charIndex].header;
    spriteTemplate.position.x = spriteParams[0] << FX32_SHIFT;
    spriteTemplate.position.y = spriteParams[1] << FX32_SHIFT;

    spriteTemplate.scale.x = 4096;
    spriteTemplate.scale.y = 4096;
    spriteTemplate.scale.z = 4096;
    spriteTemplate.drawPriority = spriteParams[2];
    spriteTemplate.heapID = heapID;

    if ((charSpriteManager->chars[charIndex].vramType == 3) || (charSpriteManager->chars[charIndex].vramType == 1)) {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }
    Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);
    charSprite->sprite = sprite;
    Sprite_SetPriority(sprite, spriteParams[3]);

    if (charSpriteManager->chars[charIndex].animSet == 1) {
        Sprite_SetAnimCtrlSeq(charSprite->sprite, 6);
    } else {
        Sprite_SetAnimCtrlSeq(charSprite->sprite, 1);
    }
    charSprite->animKind = 9;
    charSprite->animFrames = 0;
    charSprite->direction = 1;
    charSprite->resource = &charSpriteManager->chars[charIndex].sharedPalette;
    if (CharSprite_HasShadowResources(charSpriteManager->shadowResources) != 0) {
        memset(&spriteTemplate, 0, sizeof(SpriteTemplate));
        spriteTemplate.spriteList = charSpriteManager->spriteList;
        spriteTemplate.header = &charSpriteManager->shadowHeader;
        spriteTemplate.position.x = spriteParams[0] << FX32_SHIFT;
        spriteTemplate.position.y = spriteParams[1] << FX32_SHIFT;

        spriteTemplate.scale.x = 4096;
        spriteTemplate.scale.y = 4096;
        spriteTemplate.scale.z = 4096;
        spriteTemplate.drawPriority = charSpriteManager->shadowDrawPriority;
        spriteTemplate.heapID = heapID;

        if ((charSpriteManager->chars[charIndex].vramType == 3) || (charSpriteManager->chars[charIndex].vramType == 1)) {
            spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        } else {
            spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
        }
        Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);
        charSprite->shadow = sprite;
        Sprite_SetPriority(sprite, spriteParams[3]);
    } else {
        charSprite->shadow = NULL;
    }
    return charSprite;
}

void CharSprite_Delete(CharSprite *charSprite) {
    if (charSprite->shadow != 0) {
        Sprite_Delete(charSprite->shadow);
    }
    Sprite_Delete(charSprite->sprite);
    memset(charSprite, 0, sizeof(CharSprite));
}

Sprite *CharSprite_GetSprite(CharSprite *charSprite) {
    return charSprite->sprite;
}

void CharSprite_SetPosition(CharSprite *charSprite, s16 x, s16 y) {
    VecFx32 position;
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    position.z = 0;
    Sprite_SetMatrix(charSprite->sprite, &position);
    if (charSprite->shadow != 0) {
        position.x += 32768;
        position.y += 57344;
        Sprite_SetMatrix(charSprite->shadow, &position);
    }
}

void CharSprite_SetDrawPriority(CharSprite *charSprite, u16 drawPriority) {
    Sprite_SetDrawPriority(charSprite->sprite, drawPriority);
}

u16 CharSprite_GetDrawPriority(CharSprite *charSprite) {
    return Sprite_GetDrawPriority(charSprite->sprite);
}

void CharSprite_SetVisible(CharSprite *charSprite, u16 visible) {
    Sprite_SetDrawFlag(charSprite->sprite, visible);
    if (charSprite->shadow != NULL) {
        Sprite_SetDrawFlag(charSprite->shadow, visible);
    }
}

BOOL CharSprite_IsVisible(CharSprite *charSprite) {
    return Sprite_GetDrawFlag(charSprite->sprite);
}

s32 CharSprite_GetAnimSet(CharSprite *charSprite) {
    return charSprite->resource[4];
}

u32 CharSprite_GetAnimKind(CharSprite *charSprite) {
    return charSprite->animKind;
}

void CharSprite_SetAnimFrame(CharSprite *charSprite, u16 frame) {
    Sprite_SetAnimationFrame(charSprite->sprite, frame);
}

u16 CharSprite_GetAnimFrame(CharSprite *charSprite) {
    return Sprite_GetAnimationFrame(charSprite->sprite);
}

void CharSprite_SetShadowPosition(CharSprite *charSprite, s16 x, s16 y) {
    if (charSprite->shadow != 0) {
        VecFx32 position;
        position.x = (x + 8) << FX32_SHIFT;
        position.y = (y + 14) << FX32_SHIFT;
        position.z = 0;
        Sprite_SetMatrix(charSprite->shadow, &position);
    }
}

void CharSprite_StartAnim(CharSprite *charSprite, s32 animKind, s32 direction) {
    GF_ASSERT(animKind < 10);
    GF_ASSERT(direction < 4);
    charSprite->animKind = animKind;
    charSprite->animFrames = 0;
    CharSpriteAnimStartFunc func = sCharSpriteAnimStartFuncs[animKind];
    if (func != NULL) {
        func(charSprite, direction);
    }
}

void CharSprite_UpdateAnim(CharSprite *charSprite) {
    BOOL doStep = FALSE;
    u8 frameLimit = sCharSpriteAnimFrameLimits[charSprite->animKind];
    if (frameLimit == 255) {
        doStep = TRUE;
    } else if (frameLimit >= charSprite->animFrames + 1) {
        doStep = TRUE;
    }
    if (doStep == TRUE) {
        CharSprite_StepAnim(charSprite);
        charSprite->animFrames++;
    }
}

void CharSpriteManager_LoadCellAnims(CharSpriteManager *charSpriteManager, s32 animSet, enum HeapID heapID) {
    s32 base = animSet * 4;
    GF_ASSERT(charSpriteManager->cellAnims[base] == NULL);
    for (int i = 0; i < 4; i++) {
        s32 index = i % 2 + 2;
        s32 offset = 256 + animSet * 2;
        charSpriteManager->cellAnims[base + i] = AddCellOrAnimResObjFromOpenNarc(charSpriteManager->resManagers[index], charSpriteManager->charNarc, sCellAnimFileIds[base + i], FALSE, sCharCellAnimVariants[i] + offset, (GfGfxResType)index, heapID);
    }
}

void CharSpriteManager_UnloadCellAnims(CharSpriteManager *charSpriteManager, s32 animSet) {
    s32 base = animSet * 4;
    GF_ASSERT(charSpriteManager->cellAnims[base]);
    for (int i = 0; i < 4; i++) {
        u32 index = (i % 2) + 2;
        DestroySingle2DGfxResObj(charSpriteManager->resManagers[index], charSpriteManager->cellAnims[base + i]);
        charSpriteManager->cellAnims[base + i] = NULL;
    }
}

u16 CharSprite_GetCharIndex(s32 spriteId) {
    int j = 0;
    for (s32 i = 0; i < 153; i++) {
        if (spriteId == sSpriteIdToCharIndex[j]) {
            return sSpriteIdToCharIndex[i * 2 + 1];
        }
        j += 2;
    }
    GF_ASSERT(FALSE);
    return 0;
}

s32 CharSprite_IsAnimSetAllowed(u32 spriteId, s32 animSet) {
    if (animSet == 0) {
        if ((spriteId == 0) || (spriteId == 97) || (spriteId == 238) || (spriteId == 239) || (spriteId - 295 <= 1)) {
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 CharSprite_GetCellAnimResId(s32 animSet, s32 resType, s32 arg2) {
    GF_ASSERT(resType == 2 || resType == 3);
    s32 variantIndex = resType - 2;
    if (arg2 == 0) {
        variantIndex += 2;
    }
    return 256 + (animSet * 2) + sCharCellAnimVariants[variantIndex];
}

void CharSpriteManager_LoadCharResources(CharSpriteManager *charSpriteManager, s32 charIndex, u8 vramType, s32 animSet, enum HeapID heapID, s32 sharedPalette) {
    GF_ASSERT(!charSpriteManager->chars[charIndex].charResource);
    if (sharedPalette == 0) {
        CharSpriteManager_LoadCharWithPalette(charSpriteManager, charIndex, vramType, animSet, heapID);
        return;
    }
    CharSpriteManager_LoadSharedPaletteChar(charSpriteManager, charIndex, vramType, animSet, heapID);
}

void CharSpriteManager_LoadCharWithPalette(CharSpriteManager *charSpriteManager, s32 charIndex, s32 vramType, s32 animSet, enum HeapID heapID) {
    charSpriteManager->chars[charIndex].charResource = AddCharResObjFromOpenNarc(charSpriteManager->resManagers[0], charSpriteManager->charNarc, charIndex * 2 + 14, 0, charIndex + 512, vramType, (enum HeapID)heapID);
    charSpriteManager->chars[charIndex].plttResource = AddPlttResObjFromOpenNarc(charSpriteManager->resManagers[1], charSpriteManager->charNarc, charIndex * 2 + 13, 0, charIndex + 512, vramType, 1, (enum HeapID)heapID);
    NNSG2dCharacterData *charData = GF2DGfxResObj_GetCharDataPtr(charSpriteManager->chars[charIndex].charResource);

    s32 sp34;
    if (charData->szByte != 6144) {
        sp34 = 1;
    } else {
        sp34 = 0;
    }
    if (animSet == 2) {
        charData->szByte = 2048;
    } else if (animSet == 1) {
        if (sp34 != 0) {
            charData->szByte = 5120;
        } else {
            charData->szByte = 6144;
        }
    }
    if (SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(charSpriteManager->chars[charIndex].charResource) == 0) {
        GF_ASSERT(FALSE);
    }
    if (SpriteTransfer_CreatePlttTransferTask(charSpriteManager->chars[charIndex].plttResource) == 0) {
        GF_ASSERT(FALSE);
    }
    if (charSpriteManager->paletteData != 0) {
        CharSpriteManager_LoadPaletteToBuffer(charSpriteManager, charSpriteManager->chars[charIndex].plttResource, 1);
    }
    GF2DGfxResObj_FreeData(charSpriteManager->chars[charIndex].charResource);
    GF2DGfxResObj_FreeData(charSpriteManager->chars[charIndex].plttResource);
    charSpriteManager->chars[charIndex].vramType = vramType;
    charSpriteManager->chars[charIndex].animSet = animSet;
    charSpriteManager->chars[charIndex].sharedPalette = 0;
    s32 cellResId = CharSprite_GetCellAnimResId(animSet, 2, sp34);
    CreateSpriteResourcesHeader(&charSpriteManager->chars[charIndex].header, charIndex + 512, charIndex + 512, cellResId, CharSprite_GetCellAnimResId(animSet, 3, sp34), -1, -1, 0, 0, charSpriteManager->resManagers[0], charSpriteManager->resManagers[1], charSpriteManager->resManagers[2], charSpriteManager->resManagers[3], 0, 0);
}

void CharSpriteManager_LoadSharedPaletteChar(CharSpriteManager *charSpriteManager, s32 charIndex, s32 vramType, s32 animSet, enum HeapID heapID) {
    s32 avatarIndex = charIndex - 2;
    charSpriteManager->chars[charIndex].charResource = AddCharResObjFromOpenNarc(charSpriteManager->resManagers[0], charSpriteManager->avatarNarc, charIndex - 1, 0, charIndex + 512, vramType, heapID);
    if (animSet == 2) {
        GF2DGfxResObj_GetCharDataPtr(charSpriteManager->chars[charIndex].charResource)->szByte = 2048;
    } else if (animSet == 1) {
        GF2DGfxResObj_GetCharDataPtr(charSpriteManager->chars[charIndex].charResource)->szByte = 5120;
    }
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(charSpriteManager->chars[charIndex].charResource));
    GF2DGfxResObj_FreeData(charSpriteManager->chars[charIndex].charResource);
    charSpriteManager->chars[charIndex].plttResource = NULL;
    charSpriteManager->chars[charIndex].vramType = vramType;
    charSpriteManager->chars[charIndex].animSet = animSet;
    charSpriteManager->chars[charIndex].sharedPalette = 1;
    CreateSpriteResourcesHeader(&charSpriteManager->chars[charIndex].header, charIndex + 512, 1026, avatarIndex + 384, avatarIndex + 384, -1, -1, 0, 0, charSpriteManager->resManagers[0], charSpriteManager->resManagers[1], charSpriteManager->resManagers[2], charSpriteManager->resManagers[3], 0, 0);
}

void CharSpriteManager_UnloadCharResources(CharSpriteManager *charSpriteManager, s32 charIndex) {
    GF_ASSERT(charSpriteManager->chars[charIndex].charResource);
    if (charSpriteManager->chars[charIndex].sharedPalette == 0) {
        CharSpriteManager_UnloadCharWithPalette(charSpriteManager, charIndex);
        return;
    }
    CharSpriteManager_UnloadSharedPaletteChar(charSpriteManager, charIndex);
}

void CharSpriteManager_UnloadCharWithPalette(CharSpriteManager *charSpriteManager, s32 charIndex) {
    SpriteTransfer_DeleteCharTransferTask(charSpriteManager->chars[charIndex].charResource);
    SpriteTransfer_DeletePlttTransferTask(charSpriteManager->chars[charIndex].plttResource);
    DestroySingle2DGfxResObj(charSpriteManager->resManagers[0], charSpriteManager->chars[charIndex].charResource);
    DestroySingle2DGfxResObj(charSpriteManager->resManagers[1], charSpriteManager->chars[charIndex].plttResource);
    charSpriteManager->chars[charIndex].charResource = NULL;
    charSpriteManager->chars[charIndex].plttResource = NULL;
}

void CharSpriteManager_UnloadSharedPaletteChar(CharSpriteManager *charSpriteManager, s32 charIndex) {
    SpriteTransfer_DeleteCharTransferTask(charSpriteManager->chars[charIndex].charResource);
    DestroySingle2DGfxResObj(charSpriteManager->resManagers[0], charSpriteManager->chars[charIndex].charResource);
    charSpriteManager->chars[charIndex].charResource = NULL;
}

BOOL CharSpriteManager_IsCharIndexLoaded(CharSpriteManager *charSpriteManager, s32 charIndex) {
    if (charSpriteManager->chars[charIndex].charResource != NULL) {
        return TRUE;
    }
    return FALSE;
}

void CharSpriteManager_LoadShadowResources(CharSpriteManager *charSpriteManager, s32 vramType, enum HeapID heapID) {
    GF_ASSERT(charSpriteManager->shadowResources[0] == NULL);
    int i = 0;
    s32 found = 0;
    s32 playerCharIndex;
    for (i = 0; i < 6; i++) {
        if (CharSpriteManager_IsCharIndexLoaded(charSpriteManager, sPlayerCharIndices[i]) == 1) {
            playerCharIndex = sPlayerCharIndices[i];
            found = 1;
            break;
        }
    }
    GF_ASSERT(found == 1);
    for (i = 0; i < 2; i++) {
        charSpriteManager->shadowResources[i + 2] = AddCellOrAnimResObjFromOpenNarc(charSpriteManager->resManagers[i + 2], charSpriteManager->charNarc, i, 0, 288, (GfGfxResType)(i + 2), heapID);
    }
    charSpriteManager->shadowResources[0] = AddCharResObjFromOpenNarc(charSpriteManager->resManagers[0], charSpriteManager->charNarc, 2, 0, 288, vramType, heapID);
    if (SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(charSpriteManager->shadowResources[0]) == 0) {
        GF_ASSERT(FALSE);
    }
    GF2DGfxResObj_FreeData(charSpriteManager->shadowResources[0]);
    CreateSpriteResourcesHeader(&charSpriteManager->shadowHeader, 288, playerCharIndex + 512, 288, 288, -1, -1, 0, 0, charSpriteManager->resManagers[0], charSpriteManager->resManagers[1], charSpriteManager->resManagers[2], charSpriteManager->resManagers[3], 0, 0);
}

void CharSpriteManager_UnloadShadowResources(CharSpriteManager *charSpriteManager) {
    if (CharSprite_HasShadowResources(charSpriteManager->shadowResources) == 1) {
        SpriteTransfer_DeleteCharTransferTask(charSpriteManager->shadowResources[0]);
        DestroySingle2DGfxResObj(charSpriteManager->resManagers[0], charSpriteManager->shadowResources[0]);
        charSpriteManager->shadowResources[0] = NULL;
        for (int i = 0; i < 2; i++) {
            DestroySingle2DGfxResObj(charSpriteManager->resManagers[i + 2], charSpriteManager->shadowResources[i + 2]);
            charSpriteManager->shadowResources[i + 2] = NULL;
        }
    }
}

BOOL CharSprite_HasShadowResources(SpriteResource **spriteResources) {
    return spriteResources[GF_GFX_RES_TYPE_CHAR] != NULL;
}

void CharSpriteManager_LoadAvatarPalette(CharSpriteManager *charSpriteManager, s32 vramType, enum HeapID heapID) {
    GF_ASSERT(!charSpriteManager->avatarPalette);
    charSpriteManager->avatarPalette = AddPlttResObjFromOpenNarc(charSpriteManager->resManagers[1], charSpriteManager->avatarNarc, 0, 0, 1026, vramType, 8, heapID);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(charSpriteManager->avatarPalette));
    if (charSpriteManager->paletteData != 0) {
        CharSpriteManager_LoadPaletteToBuffer(charSpriteManager, charSpriteManager->avatarPalette, 8);
    }
    GF2DGfxResObj_FreeData(charSpriteManager->avatarPalette);
}

void CharSpriteManager_UnloadAvatarPalette(CharSpriteManager *charSpriteManager) {
    GF_ASSERT(charSpriteManager->avatarPalette);
    SpriteTransfer_DeletePlttTransferTask(charSpriteManager->avatarPalette);
    DestroySingle2DGfxResObj(charSpriteManager->resManagers[1], charSpriteManager->avatarPalette);
    charSpriteManager->avatarPalette = NULL;
}

void CharSpriteManager_LoadAvatarCellAnims(CharSpriteManager *charSpriteManager, enum HeapID heapID) {
    GF_ASSERT(!charSpriteManager->avatarCellAnims[0].cell);
    for (int i = 0; i < 16; i++) {
        charSpriteManager->avatarCellAnims[i].cell = AddCellOrAnimResObjFromOpenNarc(charSpriteManager->resManagers[2], charSpriteManager->avatarNarc, 17 + (i * 2), 0, i + 384, GF_GFX_RES_TYPE_CELL, heapID);
        charSpriteManager->avatarCellAnims[i].anim = AddCellOrAnimResObjFromOpenNarc(charSpriteManager->resManagers[3], charSpriteManager->avatarNarc, 18 + (i * 2), 0, i + 384, GF_GFX_RES_TYPE_ANIM, heapID);
    }
}

void CharSpriteManager_UnloadAvatarCellAnims(CharSpriteManager *charSpriteManager) {
    GF_ASSERT(charSpriteManager->avatarCellAnims[0].cell);
    for (int i = 0; i < 16; i++) {
        DestroySingle2DGfxResObj(charSpriteManager->resManagers[2], charSpriteManager->avatarCellAnims[i].cell);
        DestroySingle2DGfxResObj(charSpriteManager->resManagers[3], charSpriteManager->avatarCellAnims[i].anim);
        charSpriteManager->avatarCellAnims[i].cell = NULL;
        charSpriteManager->avatarCellAnims[i].anim = NULL;
    }
}

CharSprite *CharSpriteManager_GetFreeSprite(CharSpriteManager *charSpriteManager) {
    for (int i = 0; i < charSpriteManager->maxSprites; i++) {
        if (CharSprite_IsFree(&charSpriteManager->sprites[i]) == 1) {
            return &charSpriteManager->sprites[i];
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}

void CharSprite_StepAnim(CharSprite *charSprite) {
    CharSpriteAnimUpdateFunc spriteFunc = sCharSpriteAnimUpdateFuncs[charSprite->animKind];
    if (spriteFunc != NULL) {
        spriteFunc(charSprite);
    }
}
