#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma opt_strength_reduction off
#include "vct/vct_session.h"
typedef char SspExtent[(sizeof(Ssp) == 36) ? 1 : -1];
typedef char SspBankExtent[(sizeof(SspBank) == 84) ? 1 : -1];
typedef char ControlExtent[(sizeof(Control) == 36) ? 1 : -1];
typedef char InitExtent[(sizeof(InitArgs) == 32) ? 1 : -1];

static inline void LinkSessions(InitArgs *args) {
    u32 offset, index;
    index = 0;
    if (args->count - 1) {
        offset = 0;
        do {
            Session *base = gVctSsp.core.free;
            u8(*arena)[args->count * sizeof(Session)] = (void *)base;
            Session *current = (Session *)&(*arena)[offset];
            index++;
            current->next = base + index;
            offset += sizeof(Session);
        } while (index < args->count - 1);
    }
    gVctSsp.core.free[args->count - 1].next = NULL;
}

s32 vct_init_ssp(InitArgs *);

s32 vct_init_ssp(InitArgs *args) {
    Session *arena = args->data;
    if (!arena || !args->count || args->count > 8) {
        return 0;
    }
    gVctSsp.core.free = arena;
    MI_CpuFill8(arena, 0, args->count * sizeof(Session));
    MI_CpuFill8(&gVctServerSession, 0, sizeof(Session));
    MI_CpuFill8(&gVctRejectSession, 0, sizeof(Session));
    LinkSessions(args);
    gVctSsp.core.used = NULL;
    gVctSsp.core.mask = 0;
    gVctSsp.core.count = 0;
    __vct.flag = 0;
    gVctSsp.core.clock = OS_GetTick;
    gVctSsp.core.seed = 0x5d588b65;
    gVctSsp.core.add = 0x269ec3;
    return 1;
}
