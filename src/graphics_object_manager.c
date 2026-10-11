#include "graphics_object_manager.h"

#include <nnsys/g2d/g2d_Softsprite.h>

static inline void SetMtxScale(fx32 x, fx32 y, fx32 z) {
    reg_G3_MTX_SCALE = x;
    reg_G3_MTX_SCALE = y;
    reg_G3_MTX_SCALE = z;
}

GraphicsManager *GraphicsManager_New(GraphicsManagerTemplate *template) {
    GraphicsManager *manager = Heap_Alloc(template->heapId, sizeof(GraphicsManager));
    if (manager == NULL) {
        GF_ASSERT(FALSE);
    }
    manager->objects = GraphicsObjects_New(template->objectCount, template->heapId);
    manager->objectCount = template->objectCount;
    manager->images = GraphicsImages_New(template->imageCount, template->heapId);
    manager->imageCount = template->imageCount;
    manager->palettes = GraphicsPalettes_New(template->paletteCount, template->heapId);
    manager->paletteCount = template->paletteCount;
    return manager;
}

void GraphicsManager_Delete(GraphicsManager *manager) {
    GF_ASSERT(manager != NULL);
    GF_ASSERT(manager->objects != NULL);
    GF_ASSERT(manager->images != NULL);
    GF_ASSERT(manager->palettes != NULL);
    Heap_Free(manager->objects);
    Heap_Free(manager->images);
    Heap_Free(manager->palettes);
    Heap_Free(manager);
}

void GraphicsManager_DrawObjects(GraphicsManager *manager) {
    int i;
    G3_PushMtx();
    for (i = 0; i < manager->objectCount; i++) {
        if (manager->objects[i].visible != FALSE) {
            GraphicsObject_Draw(&manager->objects[i]);
        }
    }
    G3_PopMtx(1);
}

GraphicsImage *GraphicsManager_LoadImage(GraphicsImageTemplate *template) {
    GraphicsImage *image = GraphicsManager_FindFreeImage(template->manager);
    if (image == NULL) {
        GF_ASSERT(FALSE);
    }
    if (template->character->mapingType != GX_OBJVRAMMODE_CHAR_2D) {
        GF_ASSERT(FALSE);
    }
    image->key = GraphicsImage_AllocVram(template->character);
    if (image->key == 0) {
        GF_ASSERT(FALSE);
    }
    GraphicsImage_LoadToVram(template->character, image->key, &image->proxy);
    return image;
}

void GraphicsImage_Free(GraphicsImage *image) {
    NNS_GfdFreeTexVram(image->key);
    GraphicsImage_Init(image);
}

void GraphicsManager_FreeImages(GraphicsManager *manager) {
    int i;
    for (i = 0; i < manager->imageCount; i++) {
        if (manager->images[i].key != 0) {
            GraphicsImage_Free(&manager->images[i]);
        }
    }
}

GraphicsPalette *GraphicsManager_LoadPalette(GraphicsPaletteTemplate *template) {
    GraphicsPalette *palette = GraphicsManager_FindFreePalette(template->manager);
    if (palette == NULL) {
        GF_ASSERT(FALSE);
    }
    palette->key = GraphicsPalette_AllocVram(template->paletteCount);
    if (palette->key == 0) {
        GF_ASSERT(FALSE);
    }
    GraphicsPalette_LoadToVram(template->palette, palette->key, &palette->proxy);
    return palette;
}

void GraphicsPalette_Free(GraphicsPalette *palette) {
    NNS_GfdFreePlttVram(palette->key);
    GraphicsPalette_Init(palette);
}

void GraphicsManager_FreePalettes(GraphicsManager *manager) {
    int i;
    for (i = 0; i < manager->paletteCount; i++) {
        if (manager->palettes[i].key != 0) {
            GraphicsPalette_Free(&manager->palettes[i]);
        }
    }
}

GraphicsObject *GraphicsManager_AddObject(GraphicsObjectTemplate *template) {
    GraphicsObject *object = GraphicsManager_FindFreeObject(template->manager);
    if (object == NULL) {
        GF_ASSERT(FALSE);
    }
    GraphicsObject_InitFromTemplate(object, template);
    object->active = TRUE;
    object->visible = TRUE;
    return object;
}

void GraphicsObject_SetVisible(GraphicsObject *object, BOOL visible) {
    GF_ASSERT(object != NULL);
    object->visible = visible;
}

void GraphicsObject_SetPosition(GraphicsObject *object, s16 x, s16 y) {
    object->x = x;
    object->y = y;
}

u32 GraphicsObject_GetPosition(GraphicsObject *object) {
    return ((u16)object->y << 16) | (u16)object->x;
}

void GraphicsObject_SetCenter(GraphicsObject *object, s16 x, s16 y) {
    object->centerX = x;
    object->centerY = y;
}

void GraphicsObject_SetSize(GraphicsObject *object, int width, int height) {
    object->width = width;
    object->height = height;
}

u32 GraphicsObject_GetSize(GraphicsObject *object) {
    return ((u16)object->height << 16) | (u16)object->width;
}

void GraphicsObject_SetDepth(GraphicsObject *object, u32 depth) {
    object->depth = depth;
}

u32 GraphicsObject_GetDepth(GraphicsObject *object) {
    return object->depth;
}

void GraphicsObject_SetAlpha(GraphicsObject *object, u8 alpha) {
    object->alpha = alpha;
}

void GraphicsObject_SetImageAttr(GraphicsObject *object, NNSG2dImageAttr *attr) {
    object->imageAttr = attr;
}

void GraphicsObject_SetImageAddr(GraphicsObject *object, u32 addr) {
    object->imageAddr = addr;
}

void GraphicsObject_SetPaletteAddr(GraphicsObject *object, u32 addr) {
    object->paletteAddr = addr;
}

void GraphicsObject_SetPaletteSlot(GraphicsObject *object, u32 slot) {
    object->paletteSlot = slot;
}

// axis: 0 = horizontal, otherwise vertical
void GraphicsObject_SetFlip(GraphicsObject *object, int axis, BOOL flip) {
    if (axis == 0) {
        object->flipX = flip;
    } else {
        object->flipY = flip;
    }
}

void GraphicsObject_SetRotation(GraphicsObject *object, u32 rotation) {
    object->rotation = rotation;
}

void GraphicsObject_Init(GraphicsObject *object) {
    memset(object, 0, sizeof(GraphicsObject));
    object->alpha = 31;
    object->color = GX_RGB(31, 31, 31);
}

void GraphicsImage_Init(GraphicsImage *image) {
    image->key = 0;
    NNS_G2dInitImageProxy(&image->proxy);
}

void GraphicsPalette_Init(GraphicsPalette *palette) {
    palette->key = 0;
    NNS_G2dInitImagePaletteProxy(&palette->proxy);
}

GraphicsObject *GraphicsObjects_New(int count, enum HeapID heapId) {
    GraphicsObject *objects = Heap_Alloc(heapId, count * sizeof(GraphicsObject));
    int i;
    if (objects == NULL) {
        GF_ASSERT(FALSE);
    }
    for (i = 0; i < count; i++) {
        GraphicsObject_Init(&objects[i]);
    }
    return objects;
}

GraphicsImage *GraphicsImages_New(int count, enum HeapID heapId) {
    GraphicsImage *images = Heap_Alloc(heapId, count * sizeof(GraphicsImage));
    int i;
    if (images == NULL) {
        GF_ASSERT(FALSE);
    }
    for (i = 0; i < count; i++) {
        GraphicsImage_Init(&images[i]);
    }
    return images;
}

GraphicsPalette *GraphicsPalettes_New(int count, enum HeapID heapId) {
    GraphicsPalette *palettes = Heap_Alloc(heapId, count * sizeof(GraphicsPalette));
    int i;
    if (palettes == NULL) {
        GF_ASSERT(FALSE);
    }
    for (i = 0; i < count; i++) {
        GraphicsPalette_Init(&palettes[i]);
    }
    return palettes;
}

GraphicsObject *GraphicsManager_FindFreeObject(GraphicsManager *manager) {
    int i;
    for (i = 0; i < manager->objectCount; i++) {
        if (manager->objects[i].active == FALSE) {
            return &manager->objects[i];
        }
    }
    return NULL;
}

// Bug: bounded by objectCount, not imageCount
GraphicsImage *GraphicsManager_FindFreeImage(GraphicsManager *manager) {
    int i;
    for (i = 0; i < manager->objectCount; i++) {
        if (manager->images[i].key == 0) {
            return &manager->images[i];
        }
    }
    return NULL;
}

// Bug: bounded by objectCount, not paletteCount
GraphicsPalette *GraphicsManager_FindFreePalette(GraphicsManager *manager) {
    int i;
    for (i = 0; i < manager->objectCount; i++) {
        if (manager->palettes[i].key == 0) {
            return &manager->palettes[i];
        }
    }
    return NULL;
}

NNSGfdTexKey GraphicsImage_AllocVram(NNSG2dCharacterData *character) {
    return NNS_GfdAllocTexVram(character->szByte, FALSE, 0);
}

NNSGfdPlttKey GraphicsPalette_AllocVram(u32 paletteCount) {
    return NNS_GfdAllocPlttVram(paletteCount * 32, FALSE, 0);
}

void GraphicsImage_LoadToVram(NNSG2dCharacterData *character, NNSGfdTexKey key, NNSG2dImageProxy *proxy) {
    NNS_G2dLoadImage2DMapping(character, NNS_GfdGetTexKeyAddr(key), NNS_G2D_VRAM_TYPE_3DMAIN, proxy);
}

// Loads only as many bytes as were allocated for the key
void GraphicsPalette_LoadToVram(NNSG2dPaletteData *palette, NNSGfdPlttKey key, NNSG2dImagePaletteProxy *proxy) {
    u32 size = (key & 0xFFFF0000) >> 16; // in 8-byte units
    u32 savedSize = palette->szByte;
    palette->szByte = size << 3;
    NNS_G2dLoadPalette(palette, NNS_GfdGetPlttKeyAddr(key), NNS_G2D_VRAM_TYPE_3DMAIN, proxy);
    palette->szByte = savedSize;
}

#pragma force_active on

void GraphicsObject_Draw(GraphicsObject *object) {
    int u0, u1, v0, v1;

    G3_PushMtx();
    G3_MaterialColorDiffAmb(object->color, GX_RGB(31, 31, 31), TRUE);
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), GX_RGB(0, 0, 0), FALSE);
    G3_TexImageParam(object->imageAttr->fmt, GX_TEXGEN_TEXCOORD, object->imageAttr->sizeS, object->imageAttr->sizeT, GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, object->imageAttr->plttUse, object->imageAddr);
    G3_TexPlttBase(object->paletteAddr + object->paletteSlot * 32, object->imageAttr->fmt);
    G3_PolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, object->alpha, 0);
    if (object->flipX) {
        u0 = object->width;
        u1 = 0;
    } else {
        u1 = object->width;
        u0 = 0;
    }
    if (object->flipY) {
        v0 = object->height;
        v1 = 0;
    } else {
        v1 = object->height;
        v0 = 0;
    }
    G3_Translate((object->x + object->centerX) << FX32_SHIFT, (object->y + object->centerY) << FX32_SHIFT, object->depth << FX32_SHIFT);
    G3_RotZ(FX_SinIdx(object->rotation), FX_CosIdx(object->rotation));
    SetMtxScale(object->scaleX, object->scaleY, FX32_ONE);
    G3_Translate(-object->centerX << FX32_SHIFT, -object->centerY << FX32_SHIFT, 0);
    NNS_G2dDrawSpriteFast(0, 0, 0, object->width, object->height, u0, v0, u1, v1);
    G3_PopMtx(1);
}

#pragma opt_treetransformation off

void GraphicsObject_InitFromTemplate(GraphicsObject *object, GraphicsObjectTemplate *template) {
    u32 width, height;
    int i;
    GraphicsImage *image = template->image;
    // template->image is volatile: the loop bounds are re-read on every pass
    int bound = image->proxy.attr.sizeS;

    width = 8;
    for (i = 0; i < bound; i++) {
        bound = template->image->proxy.attr.sizeS;
        width <<= 1;
    }
    bound = image->proxy.attr.sizeT;
    height = 8;
    for (i = 0; i < bound; i++) {
        bound = template->image->proxy.attr.sizeT;
        height <<= 1;
    }
    GraphicsObject_SetPosition(object, template->x, template->y);
    GraphicsObject_SetCenter(object, (s32)width / 2, (s32)height / 2);
    object->scaleX = FX32_ONE;
    object->scaleY = FX32_ONE;
    GraphicsObject_SetSize(object, width, height);
    GraphicsObject_SetDepth(object, template->depth);
    GraphicsObject_SetAlpha(object, template->alpha);
    GraphicsObject_SetImageAttr(object, &template->image->proxy.attr);
    GraphicsObject_SetImageAddr(object, NNS_G2dGetImageLocation(&template->image->proxy, NNS_G2D_VRAM_TYPE_3DMAIN));
    GraphicsObject_SetPaletteAddr(object, NNS_G2dGetImagePaletteLocation(&template->palette->proxy, NNS_G2D_VRAM_TYPE_3DMAIN));
    GraphicsObject_SetPaletteSlot(object, template->paletteSlot);
    GraphicsObject_SetFlip(object, 0, FALSE);
    GraphicsObject_SetFlip(object, 1, FALSE);
    GraphicsObject_SetRotation(object, template->rotation);
}
