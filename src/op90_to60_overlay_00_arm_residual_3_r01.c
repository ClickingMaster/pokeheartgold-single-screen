#pragma force_active on
#include <nitro/types.h>

typedef struct WProfile {
    u8 unk00[0xc0];
    u8 ip[4];
    u8 gateway[4];
    u8 dns1[4];
    u8 dns2[4];
    u8 prefix;
    u8 unkD1[0x16];
    u8 wepMode;
    u8 unkE8[0x18];
} WProfile;

typedef struct WCtx {
    WProfile profiles[3];
    u8 unk300[0xd0d - 0x300];
    u8 profileIdx;
    u8 unkD0E[0xa];
} WCtx;

int ov00_021FAFC8(int idx);
u32 ov00_021FCFEC(u8 *addr);
void SOC_SetResolver(u32 *dns1, u32 *dns2);
void ov00_021FD094(WCtx *ctx);

#include <nitro/code32.h>

// @fn ov00_021FD094
void ov00_021FD094(WCtx *ctx) {
    u32 dns1;
    u32 dns2;
    WProfile *p;
    int sum;
    u16 first;
    if (ctx->profileIdx >= 6) {
        return;
    }
    p = &ctx->profiles[ov00_021FAFC8(ctx->profileIdx)];
    first = (u32)p->dns1[0];
    sum = (u8)first + p->dns1[1] + p->dns1[2] + p->dns1[3];
    if (p->ip[0] != 0) {
        return;
    }
    if (sum == 0) {
        return;
    }
    dns1 = ov00_021FCFEC(p->dns1);
    dns2 = ov00_021FCFEC(p->dns2);
    SOC_SetResolver(&dns1, &dns2);
}
