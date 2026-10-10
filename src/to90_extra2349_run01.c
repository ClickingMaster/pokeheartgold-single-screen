#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Session {
    char pad[12];
    int state;
} Session;

typedef struct Packet {
    u8 pad[4], kind, pad5, index;
} Packet;

extern struct {
    char pad[16];
    int mode;
} __vct;

extern signed char sSendState[];
extern signed char sTransSendState[];
int vct_check_send_state(Session *, Packet *);

#pragma force_active on

int vct_check_send_state(Session *s, Packet *p) {
    int state;
    int index = p->index;
    if (!p->kind) {
        index += 5;
    }
    if (index < 0 || index >= 11) {
        index = -1;
    }
    if (index < 0) {
        return -1;
    }
    state = s->state;
    if (__vct.mode == 1) {
        state = ((signed char(*)[11])sSendState)[state][index];
    } else {
        state = ((signed char(*)[11])sTransSendState)[state][index];
    }
    if (state == -1) {
        return 1;
    }
    if (state == -2) {
        return 0;
    }
    s->state = state;
    return 1;
}
