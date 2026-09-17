#include "player_avatar_movement_control_internal.h"

void sub_0205D6E8(PlayerAvatar *avatar, MapObject *mapObject, int arg2, int arg3, u16 arg4, u16 arg5);
void sub_0205D640(PlayerAvatar *avatar, MapObject *mapObject, int arg2, int arg3, u16 arg4, u16 arg5);

void sub_0205D640(PlayerAvatar *avatar, MapObject *mapObject, int arg2, int arg3, u16 arg4, u16 arg5) {
    sub_0205D6E8(avatar, mapObject, arg2, arg3, arg4, arg5);
}
