#pragma force_active on
#include "global.h"

typedef struct UnkSub48 {
    u8 unk0[4];
    u8 unk4;
} UnkSub48;

typedef struct UnkCallbackArg {
    u16 unk0;
    u16 errcode;
    u8 unk4[4];
    u16 state;
    u8 unkA[0x36 - 0xA];
    u16 length;
    u8 unk38[4];
    u32 aid;
    u8 unk40[3];
    u8 flags;
    u8 unk44[4];
    UnkSub48 sub48;
} UnkCallbackArg;

typedef struct UnkState {
    u8 unk0[8];
    u32 aid;
    u8 unkC[0x1220 - 0xC];
    u8 buffer[0x1300 - 0x1220];
    void (*onSend)(void *);
    u8 unk1304[0x130E - 0x1304];
    u16 pending;
    u32 mode;
    u8 unk1314[0x1320 - 0x1314];
    void (*onRecv)(u32, u8);
} UnkState;

typedef struct UnkGlobal {
    u32 unk0;
    UnkState *state;
} UnkGlobal;

extern UnkGlobal _021D4124;
extern void sub_02032858(int);
extern void sub_02032844(int);
extern int sub_02032E48(void);
extern int sub_02032C84(void);
void sub_02032D4C(void *input);

// @fn sub_02032D4C
#pragma opt_propagation off
void sub_02032D4C(void *input) {
    UnkCallbackArg *arg = input;
    UnkState *state;
    u16 errcode = arg->errcode;

    if (errcode != 0) {
        sub_02032858(errcode);
        sub_02032844(9);
        return;
    }
    state = _021D4124.state;
    if (state->mode != 2) {
        state->pending = 0;
        if (!sub_02032E48()) {
            sub_02032844(9);
        }
        return;
    }
    switch (arg->state) {
    case 3:
        return;
    case 5:
        DC_InvalidateRange(state->buffer, 0xC0);
        state = _021D4124.state;
        if (state->onRecv != NULL && arg->length >= 8) {
            UnkSub48 *sub = &arg->sub48;
            state->onRecv(arg->aid, sub->unk4);
        }
        if (arg->length >= 8) {
            state = _021D4124.state;
            if (arg->aid == state->aid && (arg->flags & 3) == 1) {
                if (state->onSend != NULL) {
                    state->onSend(state->buffer);
                }
                state = _021D4124.state;
                if (state->pending != 0) {
                    if (!sub_02032E48()) {
                        sub_02032844(9);
                    }
                    return;
                }
            }
        }
        break;
    case 4:
    default:
        break;
    }
    if (!sub_02032C84()) {
        sub_02032844(9);
    }
}
#pragma opt_propagation reset
