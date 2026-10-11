#include "wifi_plaza_map.h"

#include <string.h>

#include "global.h"

#include "gf_3d_loader.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "model_box_test.h"
#include "overlay_42.h"

// Tile attribute sets tested by the player movement handlers (rodata of this overlay)
extern const u8 _02269624[];
extern const u8 ov49_02269628[];
extern const u8 ov49_0226962C[];
extern const u8 ov49_02269634[];

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
    map->grid = ov42_02227EE0(0x23, 0x2A, heapID);
    file = GfGfxLoader_LoadFromNarc((NarcId)0xCA, 0, FALSE, heapID, TRUE);
    ov42_02227F48(map->grid, file);
    Heap_Free(file);
    return map;
}

void WiFiPlazaMap_Delete(WiFiPlazaMap *map) {
    ov42_02227F28(map->grid);
    Heap_Free(map);
}

u32 WiFiPlazaMap_GetWidth(void) {
    return 0x23;
}

s32 WiFiPlazaMap_GetCollision(WiFiPlazaMap *map, u16 x, u16 y) {
    return ov42_02227FA4(map->grid, x, y);
}

#pragma force_active on

u32 WiFiPlazaMap_GetTileTag(WiFiPlazaMap *map, u16 x, u16 y) {
    return ov42_02227FDC(map->grid, x, y) >> 15;
}

#pragma force_active reset

u32 WiFiPlazaMap_GetTileAttr(WiFiPlazaMap *map, u16 x, u16 y) {
    return ov42_02227FDC(map->grid, x, y) & 0x7FFF;
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

BOOL ov49_02258A30(u8 attr) {
    u32 i;

    for (i = 0; i < 32; i++) {
        if (attr == ov49_02269634[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ov49_02258A50(u8 attr) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (attr == _02269624[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ov49_02258A70(u8 attr) {
    u32 i;

    for (i = 0; i < 7; i++) {
        if (attr == ov49_0226962C[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ov49_02258A90(u8 attr) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (attr == ov49_02269628[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

UnkStruct_ov42_02227F68 *WiFiPlazaMap_GetGrid(WiFiPlazaMap *map) {
    return map->grid;
}
