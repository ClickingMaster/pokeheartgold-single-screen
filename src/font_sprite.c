#include "font_sprite.h"

#include "gf_gfx_loader.h"
#include "obj_char_transfer.h"

// A FontSystem shows text that was drawn into a BG window as sprites: the window is split into OBJ cells of the 12
// hardware sizes (largest first, row by row), each cell's tiles are loaded to OBJ VRAM and get one sprite.

struct FontSystem {
    void *cellFiles[12];
    NNSG2dCellDataBank *cellBanks[12]; // one per entry of gFontOAMCellSizes
    TextOBJ *objects;
    int capacity;
};

struct FontOAMSprite {
    Sprite *sprite;
    int x; // cell offset in pixels
    int y;
};

typedef struct FontOAMCell {
    int x; // in tiles
    int y;
    int index; // into gFontOAMCellSizes
    struct FontOAMCell *prev;
    struct FontOAMCell *next;
} FontOAMCell;

struct TextOBJLayout {
    FontOAMCell list;
    int count;
};

typedef NNSG2dImageProxy FontSpriteData;

typedef struct FontRect {
    int y;
    int x;
    int width;
    int height;
} FontRect;

typedef struct FontSplit {
    FontRect current;
    FontRect remainder; // right of the last cell, laid out once the rows of current are done
    u8 hasRemainder;
} FontSplit;

extern const u8 gFontOAMCellSizes[12][2];

void TextOBJ_UpdatePosition(TextOBJ *object);
void TextOBJ_SetPriority(TextOBJ *object, u8 priority);
void TextOBJ_SetPalIndexRespectVramOffset(TextOBJ *object, int palette);
void TextOBJ_SetPalOffset(TextOBJ *object, u8 palette);
void TextOBJ_Clear(TextOBJ *object);
TextOBJ *FontSystem_GetFreeTextOBJ(FontSystem *system);
int FontOAM_GetCellSizeIndex(int width, int height);
BOOL FontOAM_SplitRect(FontSplit *split, FontOAMCell *list, enum HeapID heapId);
int FontOAM_BuildLayout(int width, int height, enum HeapID heapId, FontOAMCell *list);
void FontOAM_LoadChars(Window *window, FontOAMCell *list, FontSpriteData *sprites, u32 offset, int vram, enum HeapID heapId);
u32 FontOAM_LoadCellChars(Window *window, FontOAMCell *node, FontSpriteData *proxy, int blockSize, GXOBJVRamModeChar mode, volatile u32 offset, int vram, enum HeapID heapId);
void FontOAM_CopyChars(Window *window, void *buffer, FontOAMCell *list, NNS_G2D_VRAM_TYPE vram, enum HeapID heapId);
u32 FontOAM_CopyCellChars(Window *window, FontOAMCell *node, void *buffer, u32 offset, int blockSize, GXOBJVRamModeChar mode, enum HeapID heapId);
int FontOAM_GetLayoutCharSize(FontOAMCell *list, NNS_G2D_VRAM_TYPE vram);
void TextOBJ_CreateSprites(const TextOBJTemplate *template, FontOAMCell *list, FontSpriteData *sprites, TextOBJ *object);
void TextOBJ_DeleteSprites(TextOBJ *object);
Sprite *FontOAM_CreateCellSprite(const TextOBJTemplate *template, FontOAMCell *node, FontSpriteData *proxy);
FontOAMCell *FontOAMCell_New(enum HeapID heapId);
void FontOAMCell_Delete(FontOAMCell *node);
void FontOAMCell_FreeList(FontOAMCell *list);
void FontOAMCell_InsertBefore(FontOAMCell *node, FontOAMCell *next);
void TextOBJ_SetParentSprite(TextOBJ *object, const Sprite *sprite);

FontSystem *FontSystem_NewInit(int count, enum HeapID heapId) {
    FontSystem *system = Heap_Alloc(heapId, sizeof(FontSystem));
    GF_ASSERT(system != NULL);
    for (int i = 0; i < 12; i++) {
        system->cellFiles[i] = GfGfxLoader_GetCellBank((NarcId)35, i, FALSE, &system->cellBanks[i], heapId);
        GF_ASSERT(system->cellFiles[i] != NULL);
    }
    system->objects = Heap_Alloc(heapId, sizeof(TextOBJ) * count);
    GF_ASSERT(system->objects != NULL);
    system->capacity = count;
    memset(system->objects, 0, sizeof(TextOBJ) * count);
    return system;
}

void FontSystem_Delete(FontSystem *system) {
    GF_ASSERT(system != NULL);
    for (int i = 0; i < 12; i++) {
        Heap_Free(system->cellFiles[i]);
    }
    Heap_Free(system->objects);
    Heap_Free(system);
}

TextOBJ *FontOAM_Create(TextOBJTemplate *template) {
    TextOBJ *object;
    FontOAMCell list;
    int count;
    FontSpriteData *sprites;
    GF_ASSERT(template != NULL);
    object = FontSystem_GetFreeTextOBJ(template->fontSystem);
    GF_ASSERT(object != NULL);
    object->parent = template->sprite;
    object->x = template->x;
    object->y = template->y;
    list.prev = &list;
    list.next = &list;
    count = FontOAM_BuildLayout(template->window->width, template->window->height, template->heapID, &list);
    sprites = Heap_AllocAtEnd(template->heapID, sizeof(FontSpriteData) * count);
    object->sprites = Heap_Alloc(template->heapID, 12 * count);
    object->spriteCount = count;
    FontOAM_LoadChars(template->window, &list, sprites, template->offset, template->vram, template->heapID);
    TextOBJ_CreateSprites(template, &list, sprites, object);
    Heap_Free(sprites);
    FontOAMCell_FreeList(&list);
    return object;
}

void FontOAM_Delete(TextOBJ *object) {
    GF_ASSERT(object != NULL);
    GF_ASSERT(object->sprites != NULL);
    TextOBJ_DeleteSprites(object);
    Heap_Free(object->sprites);
    TextOBJ_Clear(object);
}

int FontOAM_GetCharSize(Window *window, NNS_G2D_VRAM_TYPE vram, int heapId) {
    FontOAMCell list;
    int size;
    list.prev = &list;
    list.next = &list;
    FontOAM_BuildLayout(window->width, window->height, (enum HeapID)heapId, &list);
    size = FontOAM_GetLayoutCharSize(&list, vram);
    FontOAMCell_FreeList(&list);
    return size;
}

void TextOBJ_SetPosition(TextOBJ *object, int x, int y) {
    VecFx32 position;
    GF_ASSERT(object != NULL);
    object->x = x;
    object->y = y;
    x = x << FX32_SHIFT;
    y = y << FX32_SHIFT;
    if (object->parent != NULL) {
        const VecFx32 *parent = Sprite_GetMatrixPtr((Sprite *)object->parent);
        x += parent->x;
        y += parent->y;
    }
    position.z = 0;
    for (int i = 0; i < object->spriteCount; i++) {
        position.x = x + (object->sprites[i].x << FX32_SHIFT);
        position.y = y + (object->sprites[i].y << FX32_SHIFT);
        Sprite_SetMatrix(object->sprites[i].sprite, &position);
    }
}

void TextOBJ_UpdatePosition(TextOBJ *object) {
    fx32 baseX;
    fx32 baseY;
    VecFx32 position;
    GF_ASSERT(object != NULL);
    if (object->parent != NULL) {
        baseX = object->x << FX32_SHIFT;
        baseY = object->y << FX32_SHIFT;
        const VecFx32 *parent = Sprite_GetMatrixPtr((Sprite *)object->parent);
        baseX += parent->x;
        baseY += parent->y;
        position.z = 0;
        for (int i = 0; i < object->spriteCount; i++) {
            position.x = baseX + (object->sprites[i].x << FX32_SHIFT);
            position.y = baseY + (object->sprites[i].y << FX32_SHIFT);
            Sprite_SetMatrix(object->sprites[i].sprite, &position);
        }
    }
}

void TextOBJ_GetPosition(void *data, int *x, int *y) {
    TextOBJ *object = data;
    GF_ASSERT(object != NULL);
    GF_ASSERT(x != NULL);
    GF_ASSERT(y != NULL);
    *x = object->x;
    *y = object->y;
}

void TextOBJ_SetSpritesDrawFlag(TextOBJ *object, BOOL value) {
    GF_ASSERT(object != NULL);
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_SetDrawFlag(object->sprites[i].sprite, value);
    }
}

void TextOBJ_SetPriority(TextOBJ *object, u8 value) {
    GF_ASSERT(object != NULL);
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_SetPriority(object->sprites[i].sprite, value);
    }
}

void TextOBJ_SetDrawPriority(TextOBJ *object, int value) {
    GF_ASSERT(object != NULL);
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_SetDrawPriority(object->sprites[i].sprite, value);
    }
}

void TextOBJ_SetPaletteNum(TextOBJ *object, int value) {
    GF_ASSERT(object != NULL);
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_SetPaletteOverride(object->sprites[i].sprite, value);
    }
}

void TextOBJ_SetPalIndexRespectVramOffset(TextOBJ *object, int value) {
    GF_ASSERT(object != NULL);
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_SetPalIndexRespectVramOffset(object->sprites[i].sprite, value);
    }
}

void TextOBJ_SetPalOffset(TextOBJ *object, u8 value) {
    GF_ASSERT(object != NULL);
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_SetPalOffset(object->sprites[i].sprite, value);
    }
}

void TextOBJ_SetPalOffsetRespectVramOffset(TextOBJ *object, int value) {
    GF_ASSERT(object != NULL);
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_SetPalOffsetRespectVramOffset(object->sprites[i].sprite, value);
    }
}

TextOBJLayout *TextOBJLayout_New(Window *window, enum HeapID heapId) {
    TextOBJLayout *layout = Heap_Alloc(heapId, sizeof(TextOBJLayout));
    layout->list.prev = &layout->list;
    layout->list.next = &layout->list;
    layout->count = FontOAM_BuildLayout(window->width, window->height, heapId, &layout->list);
    return layout;
}

void TextOBJLayout_Delete(TextOBJLayout *layout) {
    FontOAMCell_FreeList(&layout->list);
    Heap_Free(layout);
}

u32 TextOBJLayout_GetCharSize(TextOBJLayout *layout, NNS_G2D_VRAM_TYPE vram) {
    return FontOAM_GetLayoutCharSize(&layout->list, vram);
}

TextOBJ *TextOBJ_Create(const TextOBJTemplate *template, TextOBJLayout *layout) {
    TextOBJ *object;
    FontSpriteData *sprites;
    GF_ASSERT(template != NULL);
    object = FontSystem_GetFreeTextOBJ(template->fontSystem);
    GF_ASSERT(object != NULL);
    object->parent = template->sprite;
    object->x = template->x;
    object->y = template->y;
    sprites = Heap_AllocAtEnd(template->heapID, sizeof(FontSpriteData) * layout->count);
    object->sprites = Heap_Alloc(template->heapID, sizeof(FontOAMSprite) * layout->count);
    object->spriteCount = layout->count;
    FontOAM_LoadChars(template->window, &layout->list, sprites, template->offset, template->vram, template->heapID);
    TextOBJ_CreateSprites(template, &layout->list, sprites, object);
    Heap_Free(sprites);
    return object;
}

void TextOBJ_Destroy(TextOBJ *object) {
    FontOAM_Delete(object);
}

void TextOBJ_CopyFromBGWindow(TextOBJ *object, TextOBJLayout *layout, Window *window, enum HeapID heapId) {
    Sprite *sprite = object->sprites[0].sprite;
    NNS_G2D_VRAM_TYPE vram = Sprite_GetVramType(sprite);
    u32 size = TextOBJLayout_GetCharSize(layout, vram);
    void *buffer = Heap_AllocAtEnd(heapId, size);
    memset(buffer, 0, size);
    FontOAM_CopyChars(window, buffer, &layout->list, vram, heapId);
    DC_FlushRange(buffer, size);
    NNSG2dImageProxy *proxy = Sprite_GetImageProxy(sprite);
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJ(buffer, NNS_G2dGetImageLocation(proxy, NNS_G2D_VRAM_TYPE_2DMAIN), size);
    } else {
        GXS_LoadOBJ(buffer, NNS_G2dGetImageLocation(proxy, NNS_G2D_VRAM_TYPE_2DSUB), size);
    }
    Heap_Free(buffer);
}

void FontOAM_CopyWindowChars(Window *window, int width, int height, int x, int y, void *charBuf) {
    // Retain the original per-row width read and address calculation order.
    int sourceOffset;
    GF_ASSERT(window->width >= width + x);
    GF_ASSERT(window->height >= height + y);
    for (int i = 0; i < height; i++) {
        sourceOffset = ((volatile Window *)window)->width * (i + y);
        sourceOffset += x;
        memcpy((u8 *)charBuf + i * width * 32, (u8 *)window->pixelBuffer + sourceOffset * 32, width * 32);
    }
}

void TextOBJ_Clear(TextOBJ *object) {
    memset(object, 0, sizeof(TextOBJ));
}

TextOBJ *FontSystem_GetFreeTextOBJ(FontSystem *input) {
    // Keep the original pool metadata reads at each iteration and on return.
    volatile FontSystem *system = input;
    int i = 0;
    if (i < system->capacity) {
        TextOBJ *object = system->objects;
        do {
            if (object->sprites == NULL) {
                return system->objects + i;
            }
            i++;
            object++;
        } while (i < system->capacity);
    }
    return NULL;
}

int FontOAM_GetCellSizeIndex(int width, int height) {
    for (int i = 0; i < 12; i++) {
        if (gFontOAMCellSizes[i][0] <= width && gFontOAMCellSizes[i][1] <= height) {
            return i;
        }
    }
    return 12;
}

BOOL FontOAM_SplitRect(FontSplit *split, FontOAMCell *list, enum HeapID heapId) {
    int remainingWidth;
    FontOAMCell *node = FontOAMCell_New(heapId);
    int remainingHeight;
    FontOAMCell_InsertBefore(node, list->next);
    node->index = FontOAM_GetCellSizeIndex(split->current.width, split->current.height);
    node->x = split->current.x;
    node->y = split->current.y;
    remainingWidth = split->current.width;
    remainingWidth -= gFontOAMCellSizes[node->index][0];
    remainingHeight = split->current.height;
    remainingHeight -= gFontOAMCellSizes[node->index][1];
    if (remainingWidth > 0) {
        split->remainder.height = split->current.height;
        split->remainder.width = remainingWidth;
        split->remainder.y = split->current.y;
        split->remainder.x = split->current.x + gFontOAMCellSizes[node->index][0];
        GF_ASSERT(split->hasRemainder != 1);
        split->hasRemainder = 1;
    }
    if (remainingHeight > 0) {
        split->current.y += gFontOAMCellSizes[node->index][1];
        split->current.height = remainingHeight;
    } else if (split->hasRemainder == 1) {
        split->current = split->remainder;
        split->hasRemainder = 0;
    } else {
        return TRUE;
    }
    return FALSE;
}

int FontOAM_BuildLayout(int width, int height, enum HeapID heapId, FontOAMCell *list) {
    FontRect next;
    FontSplit split;
    int count;
    GF_ASSERT(width != 0);
    GF_ASSERT(height != 0);
    count = 0;
    split.current.height = height;
    split.current.y = 0;
    split.current.x = 0;
    split.current.width = width;
    split.hasRemainder = 0;
    next.x = 0;
    next.width = width;
    while (height != 0) {
        int index = FontOAM_GetCellSizeIndex(split.current.width, height);
        int rowHeight = gFontOAMCellSizes[index][1];
        next.y = split.current.y + rowHeight;
        next.height = split.current.height - rowHeight;
        split.current.height = rowHeight;
        do {
            count++;
        } while (!FontOAM_SplitRect(&split, list, heapId));
        split.current = next;
        height = split.current.height;
    }
    return count;
}

void FontOAM_LoadChars(Window *window, FontOAMCell *list, FontSpriteData *sprites, u32 offset, int vram, enum HeapID heapId) {
    GXOBJVRamModeChar mode = vram == NNS_G2D_VRAM_TYPE_2DMAIN ? GX_GetOBJVRamModeChar() : GXS_GetOBJVRamModeChar();
    int blockSize = ObjCharTransfer_GetBlockSizeFromMode(mode);
    for (FontOAMCell *node = list->prev; node != list; node = node->prev) {
        NNS_G2dInitImageProxy(sprites);
        offset = FontOAM_LoadCellChars(window, node, sprites, blockSize, mode, offset, vram, heapId);
        sprites++;
    }
}

// Volatile preserves the original stack reloads of the VRAM offset.
u32 FontOAM_LoadCellChars(Window *window, FontOAMCell *node, FontSpriteData *proxy, int blockSize, GXOBJVRamModeChar mode, volatile u32 offset, int vram, enum HeapID heapId) {
    int width = gFontOAMCellSizes[node->index][0];
    int height = gFontOAMCellSizes[node->index][1];
    u32 size = width;
    void *buffer;
    size *= height;
    if ((int)size < blockSize) {
        size = blockSize;
    }
    size *= 32;
    buffer = Heap_AllocAtEnd(heapId, size);
    FontOAM_CopyWindowChars(window, width, height, node->x, node->y, buffer);
    DC_FlushRange(buffer, size);
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        u32 location = offset;
        GX_LoadOBJ(buffer, location, size);
        proxy->vramLocation.baseAddrOfVram[1] = location;
        proxy->attr.mappingType = GX_GetOBJVRamModeChar();
    } else {
        u32 location = offset;
        GXS_LoadOBJ(buffer, location, size);
        proxy->vramLocation.baseAddrOfVram[2] = location;
        proxy->attr.mappingType = GXS_GetOBJVRamModeChar();
    }
    proxy->attr.sizeS = (GXTexSizeS)65535;
    proxy->attr.sizeT = (GXTexSizeT)65535;
    proxy->attr.fmt = (GXTexFmt)3;
    proxy->attr.bExtendedPlt = FALSE;
    proxy->attr.plttUse = (GXTexPlttColor0)1;
    proxy->attr.mappingType = mode;
    Heap_Free(buffer);
    return offset + size;
}

void FontOAM_CopyChars(Window *window, void *buffer, FontOAMCell *list, NNS_G2D_VRAM_TYPE vram, enum HeapID heapId) {
    GXOBJVRamModeChar mode = vram == NNS_G2D_VRAM_TYPE_2DMAIN ? GX_GetOBJVRamModeChar() : GXS_GetOBJVRamModeChar();
    int blockSize = ObjCharTransfer_GetBlockSizeFromMode(mode);
    u32 offset = 0;
    for (FontOAMCell *node = list->prev; node != list; node = node->prev) {
        offset = FontOAM_CopyCellChars(window, node, buffer, offset, blockSize, mode, heapId);
    }
}

u32 FontOAM_CopyCellChars(Window *window, FontOAMCell *node, void *buffer, u32 offset, int blockSize, GXOBJVRamModeChar mode, enum HeapID heapId) {
    int width = gFontOAMCellSizes[node->index][0];
    int height = gFontOAMCellSizes[node->index][1];
    u32 size = width * height;
    if ((int)size < blockSize) {
        size = blockSize;
    }
    FontOAM_CopyWindowChars(window, width, height, node->x, node->y, (u8 *)buffer + offset);
    return offset + size * 32;
}

int FontOAM_GetLayoutCharSize(FontOAMCell *list, NNS_G2D_VRAM_TYPE vram) {
    GXOBJVRamModeChar mode = vram == NNS_G2D_VRAM_TYPE_2DMAIN ? GX_GetOBJVRamModeChar() : GXS_GetOBJVRamModeChar();
    int blockSize = ObjCharTransfer_GetBlockSizeFromMode(mode);
    FontOAMCell *node = list->prev;
    u32 size = 0;
    for (; node != list; node = node->prev) {
        int tiles = gFontOAMCellSizes[node->index][0] * gFontOAMCellSizes[node->index][1];
        if (tiles < blockSize) {
            tiles = blockSize;
        }
        size += tiles * 32;
    }
    return size;
}

void TextOBJ_CreateSprites(const TextOBJTemplate *template, FontOAMCell *list, FontSpriteData *sprites, TextOBJ *object) {
    int i = 0;
    for (FontOAMCell *node = list->prev; node != list; node = node->prev) {
        object->sprites[i].sprite = FontOAM_CreateCellSprite(template, node, sprites);
        GF_ASSERT(object->sprites[i].sprite != NULL);
        object->sprites[i].x = node->x * 8;
        object->sprites[i].y = node->y * 8;
        sprites++;
        i++;
    }
}

void TextOBJ_DeleteSprites(TextOBJ *object) {
    for (int i = 0; i < object->spriteCount; i++) {
        Sprite_Delete(object->sprites[i].sprite);
    }
}

Sprite *FontOAM_CreateCellSprite(const TextOBJTemplate *template, FontOAMCell *node, FontSpriteData *proxy) {
    SimpleSpriteTemplate sprite;
    SpriteResourcesHeader header;
    header.imageProxy = proxy;
    header.charData = NULL;
    header.plttProxy = template->plttResourceProxy;
    header.cellData = template->fontSystem->cellBanks[node->index];
    header.cellAnim = NULL;
    header.multiCellData = NULL;
    header.multiCellAnim = NULL;
    header.flag = 0;
    header.priority = template->unk_20;
    sprite.spriteList = template->spriteList;
    sprite.header = &header;
    sprite.priority = template->unk_24;
    sprite.whichScreen = (NNS_G2D_VRAM_TYPE) template->vram;
    sprite.heapID = template->heapID;
    sprite.position.x = 0;
    sprite.position.y = 0;
    sprite.position.z = 0;
    if (template->sprite != NULL) {
        sprite.position = *Sprite_GetMatrixPtr(template->sprite);
    }
    sprite.position.x += (template->x + node->x * 8) << FX32_SHIFT;
    sprite.position.y += (template->y + node->y * 8) << FX32_SHIFT;
    return Sprite_Create(&sprite);
}

FontOAMCell *FontOAMCell_New(enum HeapID heapId) {
    FontOAMCell *node = Heap_AllocAtEnd(heapId, sizeof(FontOAMCell));
    GF_ASSERT(node != NULL);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void FontOAMCell_Delete(FontOAMCell *node) {
    GF_ASSERT(node != NULL);
    Heap_Free(node);
}

void FontOAMCell_FreeList(FontOAMCell *list) {
    FontOAMCell *node = list->prev;
    while (node != list) {
        FontOAMCell *next = node->prev;
        FontOAMCell_Delete(node);
        node = next;
    }
}

void FontOAMCell_InsertBefore(FontOAMCell *node, FontOAMCell *next) {
    node->prev = next->prev;
    node->next = next;
    next->prev->next = node;
    next->prev = node;
}

void TextOBJ_SetParentSprite(TextOBJ *object, const Sprite *sprite) {
    object->parent = sprite;
    TextOBJ_UpdatePosition(object);
}
