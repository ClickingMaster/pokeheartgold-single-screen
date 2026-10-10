#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad0[16];
    int status;
    u8 type, error, phase;
    u8 pad17[11];
    u8 reason;
} State;

static inline int c800(void) {
    return 51200;
}

static inline int defaultError(int reason) {
    return -c800() - reason;
}

int ov00_021FBE14(State *);
#pragma opt_rebuildconditionals off

int ov00_021FBE14(State *p) {
    int reason = p->reason;
    unsigned int phase = p->phase;
    if (reason == 0) {
        reason = p->error;
    }
    if (phase < 10) {
        if (p->type == 3) {
            return -51100 - reason;
        }
        if (p->type != 4) {
            return -51300 - reason;
        }
        return defaultError(reason);
    }
    if (phase < 13) {
        return -52000 - reason;
    }
    if (p->status == 0) {
        return -50000 - reason;
    }
    if (p->status == -1) {
        return -52100 - reason;
    }
    if (p->status == -2) {
        return -52200 - reason;
    }
    if (p->status == -3) {
        return -52300 - reason;
    }
    if (p->status == -4) {
        return -53000 - reason;
    }
    if (p->status == -5) {
        return -53100 - reason;
    }
    if (p->status == -6) {
        return -53200 - reason;
    }
    return p->status;
}
