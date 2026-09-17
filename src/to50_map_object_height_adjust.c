#include "to50_map_object_height_adjust_internal.h"

int sub_02061070(LocalMapObject *object) {
    VecFx32 position;
    VecFx32 adjusted;
    int result;

    MapObject_CopyPositionVector(object, &position);
    adjusted = position;
    if (MapObject_CheckIgnoreHeights(object) == TRUE) {
        MapObject_ClearFlagsBits(object, 1 << 12);
        return FALSE;
    }
    result = MapObject_CheckFlag29(object);
    result = sub_02061248(MapObject_GetFieldSystem(object), (MapPosition *)&adjusted, result);
    if (result == TRUE) {
        position.y = adjusted.y;
        MapObject_SetPositionVector(object, &position);
        MapObject_SetPreviousY(object, MapObject_GetYCoord(object));
        MapObject_SetCurrentY(object, (position.y >> 3) / FX32_ONE);
        MapObject_ClearFlagsBits(object, 1 << 12);
    } else {
        MapObject_SetFlagsBits(object, 1 << 12);
    }
    return result;
}
