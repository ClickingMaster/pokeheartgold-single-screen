#include "global.h"

#define PTR_AT(p, n) (*(void **)((u8 *)(p) + (n)))

extern void ov80_0222F5D0(void *, u8);
extern void ov80_0222F5EC(void *, u8);

int FrtCmd_206(void *);
int FrtCmd_207(void *);

int FrtCmd_206(void *ctx)
{
    void *system = PTR_AT(ctx, 0);
    u8 *script = PTR_AT(ctx, 0x1c);
    PTR_AT(ctx, 0x1c) = script + 1;
    ov80_0222F5D0(PTR_AT(system, 0x60), *script);
    return 1;
}

int FrtCmd_207(void *ctx)
{
    void *system = PTR_AT(ctx, 0);
    u8 *script = PTR_AT(ctx, 0x1c);
    PTR_AT(ctx, 0x1c) = script + 1;
    ov80_0222F5EC(PTR_AT(system, 0x60), *script);
    return 1;
}
