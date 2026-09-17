#include "global.h"

#include "to50_map_object_face_origin_internal.h"

extern const VecFx32 _020FE3DC;

void sub_020662C4(LocalMapObject *object, int value) {
    u8 *state = sub_0205F370(object, 8);
    state[1] = value;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
    MapObject_SetFlagsBits(object, (MapObjectFlagBits)(1 << 20));
    VecFx32 vector = _020FE3DC;
    MapObject_SetFacingVector(object, &vector);
}
