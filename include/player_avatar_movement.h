#ifndef POKEHEARTGOLD_PLAYER_AVATAR_MOVEMENT_H
#define POKEHEARTGOLD_PLAYER_AVATAR_MOVEMENT_H

#include "player_avatar.h"

void PlayerAvatar_GetCoordsInFront(PlayerAvatar *playerAvatar, int *x, int *z);
void PlayerAvatar_UpdateMovement(PlayerAvatar *playerAvatar);
void PlayerAvatar_MoveControl(PlayerAvatar *playerAvatar, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys, int allowTurn);
int PlayerAvatar_CheckReadyToMove(PlayerAvatar *, int, int, int);
void PlayerAvatar_ResetMovement(PlayerAvatar *playerAvatar);
BOOL PlayerAvatar_IsBetweenSteps(PlayerAvatar *playerAvatar);
void PlayerAvatar_StopAndFace(PlayerAvatar *playerAvatar, int direction);
u32 PlayerAvatar_CheckCollision(PlayerAvatar *, LocalMapObject *, int);
int PlayerAvatar_GetInputDirection(PlayerAvatar *avatar, u16 newKeys, u16 heldKeys);
BOOL PlayerAvatar_IsReadyOrBumpingWall(PlayerAvatar *);
BOOL PlayerAvatar_IsRunning(PlayerAvatar *playerAvatar);
int PlayerAvatar_GetMoveStateForDirection(PlayerAvatar *, int);
u32 PlayerAvatar_GetMovementFromInput(PlayerAvatar *, int, int, u32, int, int);
BOOL PlayerAvatar_IsReadyForMovement(PlayerAvatar *);
void PlayerAvatar_SetMovement(PlayerAvatar *, int);
void PlayerAvatar_GetMetatileBehaviorInDirection(PlayerAvatar *, int);

#endif // POKEHEARTGOLD_PLAYER_AVATAR_MOVEMENT_H
