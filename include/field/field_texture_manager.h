#ifndef POKEHEARTGOLD_FIELD_FIELD_TEXTURE_MANAGER_H
#define POKEHEARTGOLD_FIELD_FIELD_TEXTURE_MANAGER_H

#include "field_types_def.h"

#define MAX_TEXTURE_KEYS 16

// One animated texture listed in member 0 of NARC a/1/3/9: the texture image name and up to 18 frames
// (image index, duration). An image index of 0xFF loops back to frame 0.
typedef struct {
    char name[16];
    u8 frames[18][2];
} TextureAnimationData;

typedef struct TextureSlot {
    u32 vramOffset;
    u32 vramSize;
    NNSG3dResTex *textureResource;
    void *fileData; // NARC member holding the frames' texture images
    TextureAnimationData *animationData;
    u16 currentFrame;
    u16 frameCounter;
} TextureSlot;

typedef struct {
    u32 textureCount;
    TextureAnimationData animationData[0];
} TextureDataHeader;

struct FieldTextureManager {
    void *dataBuffer;
    TextureDataHeader *dataHeader;
    TextureSlot textureSlots[MAX_TEXTURE_KEYS];
    u32 unk188;
};

FieldTextureManager *FieldTextureManager_Init();
u32 FieldTextureManager_LoadTexture(FieldTextureManager *textureManager, NNSG3dResTex *textureResource);
void FieldTextureManager_Free(FieldTextureManager *textureManager);
void FieldTextureManager_FreeAllSlots(FieldTextureManager *textureManager);
void FieldTextureManager_Destroy(FieldTextureManager *textureManager);

#endif // POKEHEARTGOLD_FIELD_FIELD_TEXTURE_MANAGER_H
