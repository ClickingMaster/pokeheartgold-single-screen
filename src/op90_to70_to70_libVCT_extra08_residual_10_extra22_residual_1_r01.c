#pragma force_active on
#include "global.h"
#include "vct/vct_session.h"
#include <nitro/code32.h>
typedef struct Packet { u8 pad[4]; u8 kind; u8 pad5; u8 index; } Packet;
extern u8 sRecvState[], sTransRecvState[];
extern int vct_response_impl(Session *, int);
int vct_check_recv_state(Session *s, Packet *p);
// @fn vct_check_recv_state
int vct_check_recv_state(Session *s, Packet *p) {
    int state;
    int index = p->index;
    if (!p->kind) index += 5;
    if (index < 0 || index >= 11) index = -1;
    if (index < 0) return -1;
    if (__vct.mode == 1) state = ((signed char(*)[11])sRecvState)[s->mode][index];
    else state = ((signed char(*)[11])sTransRecvState)[s->mode][index];
    switch (state) {
    case -1:
        return state;
    case -2:
        vct_response_impl(s, 1);
        s->mode = 0;
        s->state = 0;
        return state;
    case -3:
        vct_response_impl(s, 3);
        s->mode = 0;
        s->state = 0;
        return state;
    }
    return state;
}
