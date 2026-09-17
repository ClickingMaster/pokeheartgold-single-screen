#ifndef TO50_FIELD_MOVEMENT_TERRAIN_CHECK_INTERNAL_H
#define TO50_FIELD_MOVEMENT_TERRAIN_CHECK_INTERNAL_H
#include "player_avatar_movement_control_internal.h"
BOOL sub_0205DCFC(PlayerAvatar *avatar, MapObject *mapObject, int direction);
BOOL sub_02060EBC(MapObject *mapObject, u32 behavior);
BOOL MetatileBehavior_IsVeryTallGrass(int behavior);
BOOL MetatileBehavior_IsMud(int behavior);
#endif
