#include "field/field_texture_manager.h"

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "vram_transfer_manager.h"

// Defined in texture_animation.c, which has no header yet.
void *Texture_GetImageData(NNSG3dResTex *texture, u32 index);
u32 Texture_GetImageVramOffsetByName(NNSG3dResTex *texture, const char *name);
u32 Texture_GetImageSizeByName(NNSG3dResTex *texture, const char *name);
int sprintf(char *buf, const char *fmt, ...);

int FieldTextureManager_AddAnimatedTexture(FieldTextureManager *textureManager, NNSG3dResTex *textureResource, int index, NARC *narc);
void FieldTextureManager_FreeSlot(FieldTextureManager *textureManager, u32 index);

char gAnimatedTextureFileFormat[] = "data/fld_anime%d.bin";

FieldTextureManager *FieldTextureManager_Init(void) {
    FieldTextureManager *textureManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(FieldTextureManager));
    s32 i;
    if (textureManager == NULL) {
        GF_AssertFail();
    }
    for (i = 0; i < MAX_TEXTURE_KEYS; i++) {
        textureManager->textureSlots[i].currentFrame = 0;
        textureManager->textureSlots[i].frameCounter = 0;
        textureManager->textureSlots[i].vramOffset = 0;
        textureManager->textureSlots[i].textureResource = NULL;
        textureManager->textureSlots[i].animationData = NULL;
        textureManager->textureSlots[i].fileData = NULL;
    }
    textureManager->unk188 = 0;
    return textureManager;
}

u32 FieldTextureManager_LoadTexture(FieldTextureManager *textureManager, NNSG3dResTex *textureResource) {
    NARC *narc;
    u32 loaded = 0;
    u32 i;
    narc = NARC_New(NARC_a_1_3_9, HEAP_ID_FIELD1);
    textureManager->dataBuffer = NARC_AllocAndReadWholeMember(narc, 0, HEAP_ID_FIELD1);
    textureManager->dataHeader = textureManager->dataBuffer;
    i = loaded;
    while (i < textureManager->dataHeader->textureCount) {
        if (FieldTextureManager_AddAnimatedTexture(textureManager, textureResource, i, narc) >= 0) {
            loaded++;
        }
        i++;
    }
    NARC_Delete(narc);
    return loaded;
}

int FieldTextureManager_AddAnimatedTexture(FieldTextureManager *textureManager, NNSG3dResTex *textureResource, int index, NARC *narc) {
    char fileName[40];
    int slot;
    if (textureManager == NULL) {
        return -1;
    }
    for (slot = 0; slot < MAX_TEXTURE_KEYS; slot++) {
        if (textureManager->textureSlots[slot].fileData == NULL) {
            break;
        }
    }
    if (slot == MAX_TEXTURE_KEYS || textureResource == NULL) {
        return -1;
    }
    textureManager->textureSlots[slot].vramOffset = Texture_GetImageVramOffsetByName(textureResource, textureManager->dataHeader->animationData[index].name);
    if (textureManager->textureSlots[slot].vramOffset == 0) {
        return -1;
    }
    textureManager->textureSlots[slot].vramSize = Texture_GetImageSizeByName(textureResource, textureManager->dataHeader->animationData[index].name);
    sprintf(fileName, gAnimatedTextureFileFormat, index);
    textureManager->textureSlots[slot].animationData = &textureManager->dataHeader->animationData[index];
    textureManager->textureSlots[slot].fileData = NARC_AllocAndReadWholeMember(narc, index + 1, HEAP_ID_FIELD1);
    textureManager->textureSlots[slot].textureResource = NNS_G3dGetTex(textureManager->textureSlots[slot].fileData);
    return slot;
}

void FieldTextureManager_Free(FieldTextureManager *textureManager) {
    s32 i;
    TextureSlot *slot;
    if (textureManager == NULL || textureManager->unk188 != 0) {
        return;
    }
    i = 0;
    slot = textureManager->textureSlots;
    while (i < MAX_TEXTURE_KEYS) {
        if (slot->fileData != NULL && slot->animationData != NULL) {
            if (slot->animationData->frames[slot->currentFrame][1] <= slot->frameCounter) {
                slot->frameCounter = 1;
                slot->currentFrame++;
                if (slot->animationData->frames[slot->currentFrame][0] == 0xFF) {
                    slot->currentFrame = 0;
                }
                GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, slot->vramOffset, Texture_GetImageData(slot->textureResource, slot->animationData->frames[slot->currentFrame][0]), slot->vramSize);
            } else {
                slot->frameCounter++;
            }
        }
        i++;
        slot++;
    }
}

void FieldTextureManager_FreeSlot(FieldTextureManager *textureManager, u32 index) {
    if (textureManager != NULL) {
        if (textureManager->textureSlots[index].fileData != NULL) {
            Heap_Free(textureManager->textureSlots[index].fileData);
        }
        if (textureManager->textureSlots[index].animationData != NULL) {
            textureManager->textureSlots[index].animationData = NULL;
        }
        textureManager->textureSlots[index].currentFrame = 0;
        textureManager->textureSlots[index].frameCounter = 0;
    }
}

void FieldTextureManager_FreeAllSlots(FieldTextureManager *textureManager) {
    s32 i;
    if (textureManager == NULL) {
        return;
    }
    for (i = 0; i < MAX_TEXTURE_KEYS; i++) {
        FieldTextureManager_FreeSlot(textureManager, i);
    }
    Heap_Free(textureManager->dataBuffer);
}

void FieldTextureManager_Destroy(FieldTextureManager *textureManager) {
    if (textureManager != NULL) {
        Heap_Free(textureManager);
    }
}
