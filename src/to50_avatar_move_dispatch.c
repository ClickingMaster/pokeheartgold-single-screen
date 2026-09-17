#include "to50_avatar_move_dispatch_internal.h"

void sub_0205D6E8(PlayerAvatar *avatar, MapObject *mapObject, int unused, int direction, int arg4, int arg5) {
    switch (sub_0205D75C(avatar, direction)) {
    case 0:
        sub_0205D818(avatar, mapObject, direction, arg4, arg5);
        break;
    case 1:
        sub_0205D83C(avatar, mapObject, direction, arg4, arg5);
        break;
    case 2:
        sub_0205D948(avatar, mapObject, direction, arg4, arg5);
        break;
    case 3:
        sub_0205D978(avatar, mapObject, direction, arg4, arg5);
        break;
    }
}
