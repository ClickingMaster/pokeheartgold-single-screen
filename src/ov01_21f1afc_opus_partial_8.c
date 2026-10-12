#include "ov01_21f1afc_opus_partial_internal.h"

extern const StepFunc ov01_02206994[5];

extern const StepFunc ov01_022069A8[5];

extern const StepFunc ov01_022069BC[5];

extern const StepFunc ov01_022069D0[8];

extern const StepFunc ov01_022069F0[9];

extern const u16 __attribute__((aligned(4))) ov01_02206A14[4][5];

extern void (*const sPlayerAvatarBitUpdateFuncs[15])(PlayerAvatar *);

int ov01_021F27C0(struct RockClimbWork *work, TaskManager *taskManager) {
    if (MapObject_AreBitsSetForMovementScriptInit(work->obj) == TRUE) {
        MapObject_SetHeldMovement(work->obj, GetMovementForDirection(work->direction, 0x34));
        work->state++;
    }
    return 0;
}

int ov01_021F27E8(struct RockClimbWork *work, TaskManager *taskManager) {
    if (MapObject_IsMovementPaused(work->obj) == TRUE) {
        work->state++;
        RockClimbEffect_SetFollow(work->unk18, 1);
    }
    return 0;
}

int ov01_021F2808(struct RockClimbWork *work, TaskManager *taskManager) {
    if (MapObject_AreBitsSetForMovementScriptInit(work->obj) == TRUE) {
        MapObject_SetHeldMovement(work->obj, GetMovementForDirection(work->direction, 0x10));
        work->state++;
    }
    return 0;
}
