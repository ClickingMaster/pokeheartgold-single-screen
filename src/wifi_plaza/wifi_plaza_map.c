#include "wifi_plaza/wifi_plaza_map.h"

#include <string.h>

#include "global.h"

#include "gf_3d_loader.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "model_box_test.h"
#include "overlay_42.h"

// Tile attribute sets tested by the player movement handlers (rodata of this overlay)
static const u8 sWiFiPlazaWalkUpActionTiles[] = { 39, 40, 41, 0 };

static const u8 sWiFiPlazaPropTiles[] = { 2, 3, 4, 5 };

static const u8 sWiFiPlazaCheckActionTiles[] = { 36, 37, 38, 39, 40, 41, 43, 0 };

static const u8 sWiFiPlazaStandActionTiles[] = { 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 44 };

void WiFiPlazaMap_PosToVecFx32(const s16 *pos, s32 *vec) {
    vec[0] = pos[0] << 12;
    vec[2] = pos[1] << 12;
    vec[1] = 0;
}

void WiFiPlazaMap_VecFx32ToPos(const s32 *vec, s16 *pos) {
    pos[0] = vec[0] / 4096;
    pos[1] = vec[2] / 4096;
}

// Loads a model file, uploads its textures and keeps a copy of the file without the texture data.
void WiFiPlazaMap_LoadModelFile(NNSG3dResFileHeader **dest, NARC *narc, int fileId, enum HeapID heapID) {
    NNSG3dResFileHeader *source;
    NNSG3dResTex *texture;
    NNSG3dResTex *newTexture;
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;
    NNSG3dPlttKey paletteKey;
    u32 size;

    source = GfGfxLoader_LoadFromOpenNarc(narc, fileId, FALSE, heapID, TRUE);
    texture = NNS_G3dGetTex(source);
    GF3dRender_AllocAndLoadTexResources(texture);
    NNS_G3dTexReleaseTexKey(texture, &texKey, &tex4x4Key);
    paletteKey = NNS_G3dPlttReleasePlttKey(texture);
    size = G3dResFileHeader_GetSizeWithoutTex(source);
    *dest = Heap_Alloc(heapID, size);
    memcpy(*dest, source, size);
    newTexture = NNS_G3dGetTex(*dest);
    NNS_G3dTexSetTexKey(newTexture, texKey, tex4x4Key);
    NNS_G3dPlttSetPlttKey(newTexture, paletteKey);
    Heap_Free(source);
}

BOOL WiFiPlazaMap_IsModelInstanceInView(ModelResource *model, ModelInstance *instance) {
    VecFx32 position;
    VecFx32 scale;
    MtxFx33 rotation;
    MtxFx33 part;
    u16 x;
    u16 y;
    u16 z;

    ModelInstance_GetPosition(instance, &position.x, &position.y, &position.z);
    ModelInstance_GetScale(instance, &scale.x, &scale.y, &scale.z);
    x = ModelInstance_GetRotation(instance, 0);
    y = ModelInstance_GetRotation(instance, 1);
    z = ModelInstance_GetRotation(instance, 2);
    MTX_Identity33_(&rotation);
    MTX_RotX33_(&part, FX_SinIdx(x), FX_CosIdx(x));
    MTX_Concat33(&part, &rotation, &rotation);
    MTX_RotZ33_(&part, FX_SinIdx(z), FX_CosIdx(z));
    MTX_Concat33(&part, &rotation, &rotation);
    MTX_RotY33_(&part, FX_SinIdx(y), FX_CosIdx(y));
    MTX_Concat33(&part, &rotation, &rotation);
    return GF3d_ModelBoxTest(model->model, &position, &rotation, &scale);
}

WiFiPlazaMap *WiFiPlazaMap_Create(enum HeapID heapID) {
    WiFiPlazaMap *map;
    void *file;

    map = Heap_Alloc(heapID, sizeof(*map));
    map->grid = Field2DMap_New(0x23, 0x2A, heapID);
    file = GfGfxLoader_LoadFromNarc(NARC_a_2_0_0, 0, FALSE, heapID, TRUE);
    Field2DMap_Load(map->grid, file);
    Heap_Free(file);
    return map;
}

void WiFiPlazaMap_Delete(WiFiPlazaMap *map) {
    Field2DMap_Free(map->grid);
    Heap_Free(map);
}

u32 WiFiPlazaMap_GetWidth(void) {
    return 0x23;
}

s32 WiFiPlazaMap_GetCollision(WiFiPlazaMap *map, u16 x, u16 y) {
    return Field2DMap_IsBlocked(map->grid, x, y);
}

u32 WiFiPlazaMap_GetTileTag(WiFiPlazaMap *map, u16 x, u16 y) {
    return Field2DMap_GetTag(map->grid, x, y) >> 15;
}

u32 WiFiPlazaMap_GetTileAttr(WiFiPlazaMap *map, u16 x, u16 y) {
    return Field2DMap_GetTag(map->grid, x, y) & 0x7FFF;
}

// Finds the cell holding the ordinal-th occurrence of tag, scanning row by row.
BOOL WiFiPlazaMap_FindTileTag(WiFiPlazaMap *map, int tag, u16 *outX, u16 *outY, u32 ordinal) {
    int x;
    int y;
    u32 count = 0;

    for (y = 0; y < 42; y++) {
        u16 narrowedY = y;

        for (x = 0; x < 35; x++) {
            int value = WiFiPlazaMap_GetTileTag(map, x, narrowedY);

            if (value == tag) {
                if (count >= ordinal) {
                    *outX = x;
                    *outY = y;
                    return TRUE;
                }
                count++;
            }
        }
    }
    return FALSE;
}

BOOL WiFiPlazaMap_IsStandActionTile(u32 attr) {
    u32 i;

    for (i = 0; i < 32; i++) {
        if (attr == sWiFiPlazaStandActionTiles[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL WiFiPlazaMap_IsWalkUpActionTile(u32 attr) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (attr == sWiFiPlazaWalkUpActionTiles[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL WiFiPlazaMap_IsCheckActionTile(u32 attr) {
    u32 i;

    for (i = 0; i < 7; i++) {
        if (attr == sWiFiPlazaCheckActionTiles[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL WiFiPlazaMap_IsPropTile(u32 attr) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (attr == sWiFiPlazaPropTiles[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

Field2DMap *WiFiPlazaMap_GetGrid(WiFiPlazaMap *map) {
    return map->grid;
}
