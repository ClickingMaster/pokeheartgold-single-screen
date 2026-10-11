#ifndef POKEHEARTGOLD_GRAPHICS_OBJECT_MANAGER_H
#define POKEHEARTGOLD_GRAPHICS_OBJECT_MANAGER_H

#include "global.h"

#include "heap.h"

// Textured quads drawn with the 3D engine: a manager owns fixed-size pools of objects, texture images (in texture
// VRAM) and palettes (in palette VRAM). An object is drawn as one rotated, scaled and flipped sprite.

typedef struct GraphicsObject {
    s16 x;
    s16 y;
    s16 centerX; // rotation and scale centre, relative to (x, y)
    s16 centerY;
    s16 width;
    s16 height;
    fx32 scaleX;
    fx32 scaleY;
    u32 depth;
    u32 rotation;
    BOOL active;
    BOOL visible;
    NNSG2dImageAttr *imageAttr;
    u32 imageAddr;
    u32 paletteAddr;
    u32 paletteSlot; // 16-colour palette within the loaded palette block
    BOOL flipX;
    BOOL flipY;
    u16 color;
    u8 alpha;
    u8 padding_3F;
} GraphicsObject; // size: 0x40

typedef struct GraphicsImage {
    NNSGfdTexKey key; // 0 = free
    NNSG2dImageProxy proxy;
} GraphicsImage; // size: 0x28

typedef struct GraphicsPalette {
    NNSGfdPlttKey key; // 0 = free
    NNSG2dImagePaletteProxy proxy;
} GraphicsPalette; // size: 0x18

typedef struct GraphicsManager {
    GraphicsObject *objects;
    int objectCount;
    GraphicsImage *images;
    int imageCount;
    GraphicsPalette *palettes;
    int paletteCount;
} GraphicsManager;

typedef struct GraphicsManagerTemplate {
    int objectCount;
    int imageCount;
    int paletteCount;
    enum HeapID heapId;
} GraphicsManagerTemplate;

typedef struct GraphicsImageTemplate {
    GraphicsManager *manager;
    NNSG2dCharacterData *character;
} GraphicsImageTemplate;

typedef struct GraphicsPaletteTemplate {
    GraphicsManager *manager;
    NNSG2dPaletteData *palette;
    u32 paletteCount; // in 16-colour palettes
} GraphicsPaletteTemplate;

typedef struct GraphicsObjectTemplate {
    GraphicsManager *manager;
    GraphicsImage *volatile image;
    GraphicsPalette *palette;
    s16 x;
    s16 y;
    u16 rotation;
    u16 padding_12;
    u32 alpha;
    u32 depth;
    u16 paletteSlot;
    u16 padding_1E;
} GraphicsObjectTemplate;

GraphicsManager *GraphicsManager_New(GraphicsManagerTemplate *template);
void GraphicsManager_Delete(GraphicsManager *manager);
void GraphicsManager_DrawObjects(GraphicsManager *manager);
GraphicsImage *GraphicsManager_LoadImage(GraphicsImageTemplate *template);
void GraphicsImage_Free(GraphicsImage *image);
void GraphicsManager_FreeImages(GraphicsManager *manager);
GraphicsPalette *GraphicsManager_LoadPalette(GraphicsPaletteTemplate *template);
void GraphicsPalette_Free(GraphicsPalette *palette);
void GraphicsManager_FreePalettes(GraphicsManager *manager);
GraphicsObject *GraphicsManager_AddObject(GraphicsObjectTemplate *template);
void GraphicsObject_SetVisible(GraphicsObject *object, BOOL visible);
void GraphicsObject_SetPosition(GraphicsObject *object, s16 x, s16 y);
u32 GraphicsObject_GetPosition(GraphicsObject *object);
void GraphicsObject_SetCenter(GraphicsObject *object, s16 x, s16 y);
void GraphicsObject_SetSize(GraphicsObject *object, int width, int height);
u32 GraphicsObject_GetSize(GraphicsObject *object);
void GraphicsObject_SetDepth(GraphicsObject *object, u32 depth);
u32 GraphicsObject_GetDepth(GraphicsObject *object);
void GraphicsObject_SetAlpha(GraphicsObject *object, u8 alpha);
void GraphicsObject_SetImageAttr(GraphicsObject *object, NNSG2dImageAttr *attr);
void GraphicsObject_SetImageAddr(GraphicsObject *object, u32 addr);
void GraphicsObject_SetPaletteAddr(GraphicsObject *object, u32 addr);
void GraphicsObject_SetPaletteSlot(GraphicsObject *object, u32 slot);
void GraphicsObject_SetFlip(GraphicsObject *object, int axis, BOOL flip);
void GraphicsObject_SetRotation(GraphicsObject *object, u32 rotation);
void GraphicsObject_Init(GraphicsObject *object);
void GraphicsImage_Init(GraphicsImage *image);
void GraphicsPalette_Init(GraphicsPalette *palette);
GraphicsObject *GraphicsObjects_New(int count, enum HeapID heapId);
GraphicsImage *GraphicsImages_New(int count, enum HeapID heapId);
GraphicsPalette *GraphicsPalettes_New(int count, enum HeapID heapId);
GraphicsObject *GraphicsManager_FindFreeObject(GraphicsManager *manager);
GraphicsImage *GraphicsManager_FindFreeImage(GraphicsManager *manager);
GraphicsPalette *GraphicsManager_FindFreePalette(GraphicsManager *manager);
NNSGfdTexKey GraphicsImage_AllocVram(NNSG2dCharacterData *character);
NNSGfdPlttKey GraphicsPalette_AllocVram(u32 paletteCount);
void GraphicsImage_LoadToVram(NNSG2dCharacterData *character, NNSGfdTexKey key, NNSG2dImageProxy *proxy);
void GraphicsPalette_LoadToVram(NNSG2dPaletteData *palette, NNSGfdPlttKey key, NNSG2dImagePaletteProxy *proxy);
void GraphicsObject_Draw(GraphicsObject *object);
void GraphicsObject_InitFromTemplate(GraphicsObject *object, GraphicsObjectTemplate *template);

#endif // POKEHEARTGOLD_GRAPHICS_OBJECT_MANAGER_H
