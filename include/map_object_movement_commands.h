#ifndef POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_COMMANDS_H
#define POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_COMMANDS_H

#include "constants/movements.h"

#include "field_system.h"

typedef struct MovementScriptCommand {
    u16 command;
    u16 length;
} MovementScriptCommand;

typedef struct EventObjectMovementMan EventObjectMovementMan;

EventObjectMovementMan *EventObjectMovementMan_Create(LocalMapObject *object, const MovementScriptCommand *data);
BOOL EventObjectMovementMan_IsFinish(EventObjectMovementMan *movementMan);
void EventObjectMovementMan_Delete(EventObjectMovementMan *movementMan);
BOOL MapObject_AreBitsSetForMovementScriptInit(LocalMapObject *object);
BOOL MapObject_ClearHeldMovementIfActive(LocalMapObject *object);
void MapObject_ClearHeldMovement(LocalMapObject *object);
void MapObject_SetHeldMovement(LocalMapObject *object, u32 movement);
BOOL MapObject_IsMovementPaused(LocalMapObject *object);

#endif // POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_COMMANDS_H
