#include "field_terrain_queries_internal.h"

#define MAP_TILES_COUNT_X    32
#define MAP_TILES_COUNT_Z    32
#define MAP_OBJECT_TILE_SIZE FX32_CONST(16)
#define QUADRANT_COUNT       4

enum CalculatedHeightSource {
    CALCULATED_HEIGHT_SOURCE_NONE = 0,
    CALCULATED_HEIGHT_SOURCE_BDHC,
    CALCULATED_HEIGHT_SOURCE_DYNAMIC
};

int sub_02054654(FieldSystem *fieldSystem, int objectHeightMode, int objectHeight, fx32 objectX, fx32 objectZ, u8 *newObjectHeightSourceOut) {
    BOOL dynHeightPlateFound;
    u32 tileX, tileZ;
    u32 mapMatrixX, mapMatrixZ;
    VecFx32 objectPosition;

    int mapMatrixWidth;
    int mapMatrixWidthTiles;
    MapLoadManager *landDataMan;
    BOOL bdhcHeightValid;
    fx32 newObjectHeight = 0;

    objectPosition.x = objectX;
    objectPosition.z = objectZ;
    objectPosition.y = 0;

    landDataMan = fieldSystem->mapLoadManager;
    mapMatrixWidth = MapMatrix_GetWidth(fieldSystem->mapMatrix);
    mapMatrixWidthTiles = mapMatrixWidth * MAP_TILES_COUNT_X;

    tileX = objectX / 0x10000;
    tileZ = objectZ / 0x10000;

    u8 dynHeightPlateIndex;
    dynHeightPlateFound = DynamicTerrainHeightManager_FindPlate(tileX, tileZ, fieldSystem->dynamicTerrainHeightManager, &dynHeightPlateIndex);

    mapMatrixX = tileX / MAP_TILES_COUNT_X;
    mapMatrixZ = tileZ / MAP_TILES_COUNT_Z;
    u32 mapMatrixIndex = mapMatrixX + mapMatrixZ * mapMatrixWidth;

    fx32 currentMapOriginX = (mapMatrixX * MAP_TILES_COUNT_X + MAP_TILES_COUNT_X / 2) * MAP_OBJECT_TILE_SIZE;
    fx32 currentMapOriginZ = (mapMatrixZ * MAP_TILES_COUNT_Z + MAP_TILES_COUNT_Z / 2) * MAP_OBJECT_TILE_SIZE;

    objectPosition.x = objectX - currentMapOriginX;
    objectPosition.z = objectZ - currentMapOriginZ;

    u32 tileIndex = tileX + tileZ * mapMatrixWidthTiles;
    int tileMapQuadrant = ov01_021F6328(tileIndex, mapMatrixWidthTiles);
    u32 loadedMapIndex = ov01_021F635C(mapMatrixIndex, tileMapQuadrant, landDataMan);

    if (loadedMapIndex > QUADRANT_COUNT - 1) {
        bdhcHeightValid = FALSE;
    } else {
        void *bdhc = ov01_021F65D0(landDataMan, loadedMapIndex);
        bdhcHeightValid = ov01_021FAE50(objectHeightMode, objectHeight, objectPosition.x, objectPosition.z, bdhc, &objectPosition.y);
    }

    u8 newObjectHeightSource;

    if (dynHeightPlateFound) {
        fx32 dynamicHeight = DynamicTerrainHeightManager_GetPlateHeight(dynHeightPlateIndex, fieldSystem->dynamicTerrainHeightManager);

        if (bdhcHeightValid) {
            if (dynamicHeight <= objectPosition.y) {
                newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_BDHC;
                newObjectHeight = objectPosition.y;
            } else {
                fx32 bdhcHeightDiff = sub_02054648(objectPosition.y, objectHeight);
                fx32 dynamicHeightDiff = sub_02054648(dynamicHeight, objectHeight);

                if (bdhcHeightDiff <= dynamicHeightDiff) {
                    newObjectHeight = objectPosition.y;
                    newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_BDHC;
                } else {
                    newObjectHeight = dynamicHeight;
                    newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_DYNAMIC;
                }
            }
        } else {
            newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_DYNAMIC;
            newObjectHeight = dynamicHeight;
        }
    } else {
        if (bdhcHeightValid) {
            newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_BDHC;
            newObjectHeight = objectPosition.y;
        } else {
            newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_NONE;
        }
    }

    if (newObjectHeightSourceOut != NULL) {
        *newObjectHeightSourceOut = newObjectHeightSource;
    }

    return newObjectHeight;
}
