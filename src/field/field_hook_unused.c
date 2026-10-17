#include "global.h"

// The field hook kept in the field map systems: it receives the bottom screen app events and tells the field
// whether the player may move.

typedef void (*FieldHookCallback)(void *arg);

typedef struct FieldHookState {
    u8 unk_00[2];
    u8 unk_02; // the callback only runs when this is 2
    u8 movementAllowed;
    u8 unk_04; // set by event 1
    u8 unk_05[0x3F];
    FieldHookCallback callback;
    void *callbackArg;
} FieldHookState;

void FieldHookUnused_OnEvent(FieldHookState *state, u32 event);
u8 FieldHookUnused_IsMovementAllowed(FieldHookState *state);

void FieldHookUnused_OnEvent(FieldHookState *state, u32 event) {
    switch (event) {
    case 0:
        break;
    case 1:
        state->unk_04 = 1;
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        if (state->unk_02 == 2 && state->callback != NULL) {
            state->callback(state->callbackArg);
        }
        break;
    case 5:
        break;
    }
}

u8 FieldHookUnused_IsMovementAllowed(FieldHookState *state) {
    return state->movementAllowed;
}
