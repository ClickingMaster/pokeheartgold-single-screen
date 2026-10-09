// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {OSThread *thread;int pending;u8 state;} Socket;
typedef struct Thread Thread;struct Thread {u8 pad[104];Thread *next;u8 pad2[56];Socket *socket;};
typedef struct {u32 pad[3];int value;u32 pad2[3];int a,b;u32 pad3[2];int c;u32 pad4[4];void(*free)(void*);u32 pad5[3];void *active;u32 pad6[3];int d,e;} Globals;
extern Globals _021D4620;extern u8 _021D46A0[];extern u8 _021D4948[];
extern void CPSi_SslCleanup(void);
void reset_network_vars(int);
#pragma force_active on
#pragma push
#pragma opt_propagation off

// @fn reset_network_vars
void reset_network_vars(int value) {
    int active;
    Thread *t;
    int i;
    u8 *p;
    {
        Globals *g = &_021D4620;
        Globals *h;
        active = g->active != 0;
        h = &_021D4620;
        h->value = value;
        _021D4620.active = 0;
        _021D4620.a = 0;
        _021D4620.c = 0;
        _021D4620.d = 0;
        _021D4620.e = 0;
        _021D4620.b = 0;
    }
    if (!active) {
        return;
    }
    MI_CpuFill8(_021D46A0, 0, 96);
    for (t = (Thread *)OSi_ThreadInfo.list; t; t = t->next) {
        Socket *s = t->socket;
        if (s && s->thread) {
            if (s->state != 10 && s->state != 11) {
                s->state = 0;
            }
            if (s->pending) {
                s->pending = 0;
                OS_WakeupThreadDirect(s->thread);
            }
        }
    }
    i = 0;
    p = _021D4948;
    do {
        if (*(u16 *)(p + 4)) {
            _021D4620.free(*(void **)(p + 52));
            *(u16 *)(p + 4) = 0;
        }
        i++;
        p += 56;
    } while (i < 8);
    CPSi_SslCleanup();
}
#pragma pop
