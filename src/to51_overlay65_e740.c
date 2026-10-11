#include "to51_overlay65_e740_private.h"

typedef struct E740Work {
    u8 pad0[0x35f8];
    u32 states[32];
    u8 pad3678[0x3678 - 0x35f8 - 32 * 4];
    s32 selected;
    u32 unk367C;
    void *fmt;
    void *msg;
} E740Work;

int ov65_0221E740(void *p) {
    E740Work *w = p;
    int i;
    int j;
    void *state;

    w->selected = -1;
    for (i = 0; i < Comm_CountConnected(); i++) {
        if (w->states[i] == 2) {
            w->selected = i;
            w->states[i] = 0;
            break;
        }
    }
    if (w->selected == -1) {
        DestroyMsgData(w->msg);
        MessageFormat_Delete(w->fmt);
        sub_02037AC0(0x13);
        ov65_0221FB90((u8 *)p + 0x5b4, 0x1c, 1, U32(p, 0x190), PTR(p, 0x184));
        PTR(p, 0x2220) = ov65_0221E72C;
        return 0;
    }
    if (Bag_HasItem(Save_Bag_Get(PTR(p, 4)), 0x1b5, 1, 0x1a) == 1) {
        BufferPlayersName(w->fmt, 0, sub_02034818(U32(p, 0x3678)));
        ov65_0221E330(p, 0x3e);
        PTR(p, 0x2220) = ov65_0221E684;
        return 0;
    }
    state = sub_0202C6F4(PTR(p, 4));
    for (j = 0; j < 0x20; j++) {
        if (!sub_0202C2DC(state, j)) {
            sub_0203A280(PTR(p, 4), U32(p, 0x3678), j, 0x1a, 0);
            break;
        }
    }
    PTR(p, 0x2220) = ov65_0221E740;
    return 0;
}
