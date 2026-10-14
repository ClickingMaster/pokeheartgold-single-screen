#ifndef POKEHEARTGOLD_OVERLAY_02_FIELD_UTILS_H
#define POKEHEARTGOLD_OVERLAY_02_FIELD_UTILS_H

#include "field_system.h"

BOOL PlayerStepEvent_RepelCounterDecrement(SaveData *saveData, FieldSystem *fieldSystem);
BOOL FieldSystem_CanPlaceSafariObject(FieldSystem *fieldSystem);
BOOL SafariZone_CanPlaceObjectOnLand(u8 behavior, BOOL blocked);
BOOL SafariZone_CanPlaceObjectOnWater(u8 behavior, BOOL blocked);
FollowMonInteraction *FollowMonInteraction_New(enum HeapID heapId);
void FollowMonInteraction_Delete(FollowMonInteraction *interaction);

#endif // POKEHEARTGOLD_OVERLAY_02_FIELD_UTILS_H
