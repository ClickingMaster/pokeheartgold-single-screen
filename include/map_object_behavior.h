#ifndef POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_H
#define POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_H

#include "script.h"

BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible);
int TrainerApproach_Start(FieldSystem *fieldSystem, LocalMapObject *localMapObject, PlayerAvatar *playerAvatar, int a3, int a4, int a5, int a6, u16 trainerNum);
BOOL TrainerApproach_IsDone(void);
void TrainerApproach_Delete(int a0);

#endif // POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_H
