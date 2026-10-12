#ifndef POKEHEARTGOLD_FIELD_SPIN_SLIDE_H
#define POKEHEARTGOLD_FIELD_SPIN_SLIDE_H

#include "field_system.h"
#include "player_avatar.h"
#include "task.h"

// Arrow tiles (Viridian Gym) that send the player sliding and spinning across the floor.
typedef struct SpinSlideTask {
    int direction;
    int countdown;
    int state;
    FieldSystem *fieldSystem;
    PlayerAvatar *avatar;
    u32 unused14;
} SpinSlideTask;

BOOL PlayerAvatar_TryStartSpinSlide(FieldSystem *fieldSystem, PlayerAvatar *avatar, int facingDirection);
void SpinSlide_Start(FieldSystem *fieldSystem, PlayerAvatar *avatar, int direction);
int SpinSlide_RotateDirection(int direction);
BOOL Task_SpinSlide(TaskManager *taskManager);
void *SpinSlide_AllocZeroed(u32 size);
void SpinSlide_Free(void *ptr);

#endif // POKEHEARTGOLD_FIELD_SPIN_SLIDE_H
