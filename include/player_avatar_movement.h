#ifndef POKEHEARTGOLD_PLAYER_AVATAR_MOVEMENT_H
#define POKEHEARTGOLD_PLAYER_AVATAR_MOVEMENT_H

#include "player_avatar.h"

void PlayerAvatar_GetCoordsInFront(PlayerAvatar *playerAvatar, int *x, int *z);
void PlayerAvatar_UpdateMovement(PlayerAvatar *playerAvatar);
void PlayerAvatar_MoveControl(PlayerAvatar *playerAvatar, MapLoadManager *mapLoadManager, int a2, u16 a3, u16 a4, int a5);
void PlayerAvatar_ResetMovement(PlayerAvatar *playerAvatar);
BOOL PlayerAvatar_IsBetweenSteps(PlayerAvatar *playerAvatar);
void PlayerAvatar_StopAndFace(PlayerAvatar *playerAvatar, int direction);
int PlayerAvatar_GetInputDirection(PlayerAvatar *avatar, u16 newKeys, u16 heldKeys);
BOOL PlayerAvatar_IsRunning(PlayerAvatar *playerAvatar);

#endif // POKEHEARTGOLD_PLAYER_AVATAR_MOVEMENT_H
