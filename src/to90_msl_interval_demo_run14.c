#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 *type;
    void *object, *extra;
    u8 pad[12];
    u8 *frame;
} Throw;

typedef struct {
    void *object;
    u8 *type;
    void *extra, *adjusted, *indirect;
} Catch;

void _Z14SetupCatchInfoP12ThrowContextll(Throw *, s32, s32);

void _Z14SetupCatchInfoP12ThrowContextll(Throw *ctx, s32 offset, s32 adjust) {
    Catch *c = (Catch *)(ctx->frame + offset);
    c->object = ctx->object;
    c->type = ctx->type;
    c->extra = ctx->extra;
    if (ctx->type[0] == '*') {
        c->adjusted = &c->indirect;
        c->indirect = (u8 *)*(void **)ctx->object + adjust;
    } else {
        c->adjusted = (u8 *)ctx->object + adjust;
    }
}
