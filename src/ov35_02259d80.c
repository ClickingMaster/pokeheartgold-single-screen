#include "global.h"

// Field hook object reached through FieldSystem->unk4->unk14 (field_system.c, field/bottom_screen_app.c).
// Its creator is not known, so the names stay as they are.

typedef void (*Ov35Callback)(void *arg);

typedef struct Ov35State {
    u8 unk_00[2];
    u8 unk_02; // the callback only runs when this is 2
    u8 unk_03; // returned by ov35_02259DB8; field_system.c uses it as "movement allowed"
    u8 unk_04; // set by event 1
    u8 unk_05[0x3F];
    Ov35Callback callback;
    void *callbackArg;
} Ov35State;

void ov35_02259D80(Ov35State *state, u32 event);
u8 ov35_02259DB8(Ov35State *state);

void ov35_02259D80(Ov35State *state, u32 event) {
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

u8 ov35_02259DB8(Ov35State *state) {
    return state->unk_03;
}
