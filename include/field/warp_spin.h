#ifndef POKEHEARTGOLD_FIELD_WARP_SPIN_H
#define POKEHEARTGOLD_FIELD_WARP_SPIN_H

#include "field_system.h"
#include "sys_task.h"

// The player spinning up out of the map (departing: rise and fade out) or down into it (arriving: fade in and
// descend), used by the warp tasks.
typedef struct WarpSpinTask {
    FieldSystem *fieldSystem;
    int *done; // set to 1 when the spin has ended
    int state;
    int counter;
    int facing;
} WarpSpinTask;

void WarpSpin_RotatePlayer(WarpSpinTask *task);
void Task_WarpSpinDeparture(SysTask *sysTask, WarpSpinTask *task);
void Task_WarpSpinArrival(SysTask *sysTask, WarpSpinTask *task);
void FieldSystem_StartWarpSpin(FieldSystem *fieldSystem, BOOL departing, int *done);

#endif // POKEHEARTGOLD_FIELD_WARP_SPIN_H
