#ifndef POKEHEARTGOLD_TEXTURE_ANIMATION_H
#define POKEHEARTGOLD_TEXTURE_ANIMATION_H

#include "billboard_gfx_sequence.h"
#include "heap.h"

typedef struct TextureAnimation TextureAnimation;
typedef struct TextureAnimationPool TextureAnimationPool;

TextureAnimationPool *TextureAnimationPool_New(int, enum HeapID);
void TextureAnimationPool_Free(TextureAnimationPool *);
TextureAnimation *TextureAnimationPool_Add(TextureAnimationPool *, const BillboardGfxSequence *, NNSG3dResTex *, u32, u32, fx32);
void TextureAnimation_Remove(TextureAnimation *);
void TextureAnimation_SetFrame(TextureAnimation *, fx32);
void *Texture_GetImageData(NNSG3dResTex *, u32);
void *Texture_GetPaletteData(NNSG3dResTex *, u32);
u32 Texture_GetImageVramOffsetByName(NNSG3dResTex *, const char *);
u32 Texture_GetImageVramOffsetByIndex(NNSG3dResTex *, u32);
u32 Texture_GetImageSizeByIndex(NNSG3dResTex *, u32);
u32 Texture_GetImageSizeByName(NNSG3dResTex *, const char *);

#endif // POKEHEARTGOLD_TEXTURE_ANIMATION_H
