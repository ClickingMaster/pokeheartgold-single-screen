#ifndef POKEHEARTGOLD_TO50_AVATAR_MOVE_DISPATCH_INTERNAL_H
#define POKEHEARTGOLD_TO50_AVATAR_MOVE_DISPATCH_INTERNAL_H
#include "global.h"
typedef struct PlayerAvatar PlayerAvatar;
typedef struct LocalMapObject MapObject;
void sub_0205D6E8(PlayerAvatar *avatar, MapObject *mapObject, int unused, int direction, int arg4, int arg5);
int sub_0205D75C(PlayerAvatar *avatar, int direction);
void sub_0205D818(PlayerAvatar *avatar, MapObject *mapObject, int direction, int arg4, int arg5);
void sub_0205D83C(PlayerAvatar *avatar, MapObject *mapObject, int direction, int arg4, int arg5);
void sub_0205D948(PlayerAvatar *avatar, MapObject *mapObject, int direction, int arg4, int arg5);
void sub_0205D978(PlayerAvatar *avatar, MapObject *mapObject, int direction, int arg4, int arg5);
#endif
