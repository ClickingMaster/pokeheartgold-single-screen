#include "global.h"
typedef struct Ov93FE08State { u8 pad[0x10]; s32 total, quotient, values[4]; } Ov93FE08State;
typedef struct Ov93FE08System { u8 pad[0x3848]; s32 total; } Ov93FE08System;
extern s32 ov93_0225E3C4(void *, s32);
extern u32 Comm_GetLocalNetId(void);
extern void ov93_02262098(void *, s32);
s32 ov93_0225FE08(Ov93FE08System *sys, Ov93FE08State *state, s32 change, u32 id);
s32 ov93_0225FE08(Ov93FE08System *sys, Ov93FE08State *state, s32 change, u32 id) {
    s32 slot;
    state->total += change;
    state->quotient = state->total / 6;
    slot = ov93_0225E3C4(sys, id);
    state->values[slot] += change;
    if (id == Comm_GetLocalNetId()) {
        sys->total += change;
        ov93_02262098((u8 *)sys + 0x15a8, sys->total);
    }
    return state->total;
}
