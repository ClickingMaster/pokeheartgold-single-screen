#ifndef POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_H
#define POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_H

#include "script.h"

BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible);
int TrainerApproach_Start(FieldSystem *fieldSystem, LocalMapObject *localMapObject, PlayerAvatar *playerAvatar, int movement, int remaining, int unk10, int unk14, u16 trainerNum);
BOOL TrainerApproach_IsDone(void);
void TrainerApproach_Delete(int task);

#endif // POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_H
