#ifndef POKEHEARTGOLD_FIELD_INTERACTION_H
#define POKEHEARTGOLD_FIELD_INTERACTION_H

#include "script.h"

void FieldSystem_GetFacingObject(FieldSystem *fieldSystem, LocalMapObject **ret_p);
BOOL FieldSystem_GetInteractableFacingObject(FieldSystem *fieldSystem, LocalMapObject **localMapObject);
u16 GetInteractedBackgroundEventScript(FieldSystem *fieldSystem, BgEvent *bgEvents, int numEvents);
u32 GetFacingSignScript(FieldSystem *fieldSystem, BgEvent *bgEvents, int numEvents);
u16 GetTriggeredCoordEventScript(FieldSystem *fieldSystem, const CoordEvent *coordEvent, int numEvents);

// The bottom screen operations of a map without a bottom screen app.
void BottomScreen_None_Init(FieldSystem *fieldSystem);
void BottomScreen_None_End(FieldSystem *fieldSystem);
BOOL BottomScreen_None_IsDone(FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_FIELD_INTERACTION_H
