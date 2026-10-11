#include "global.h"

#include "to57_new31_overlay07_private.h"

void ov07_0221C9C0(Ov07Ctx *ctx) {
    int i, sum = 0;
    if (ctx->phase17E < 1) {
        ctx->state8D = 1;
        ctx->phase17E++;
        return;
    }
    for (i = 0; i < 16; i++) {
        if (ctx->inner->actor[i] != NULL) {
            sum += ParticleSystem_GetActiveEmitterCount(ctx->inner->actor[i]);
        }
    }
    if (sum != 0 || ctx->half8E != 0 || ctx->half90 != 0) {
        ctx->state8D = 1;
        ctx->timer17D = 0;
        return;
    }
    if (GF_IsAnySEPlaying()) {
        ctx->timer17D++;
        if (ctx->timer17D > 90) {
            ctx->timer17D = 0;
            ctx->state8D = 0;
        } else {
            ctx->state8D = 1;
            return;
        }
    }
    ctx->timer17D = 0;
    ctx->phase17E = 0;
    for (i = 0; i < 3; i++) {
        ctx->words1C[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        ctx->mids[i].a = 0;
        ctx->mids[i].b = 0;
        ctx->mids[i].c = 0;
        ctx->mids[i].d = 0;
    }
    for (i = 0; i < 4; i++) {
        if (ctx->managers[i] != NULL) {
            SpriteSystem_FreeResourcesAndManager(ctx->inner->spriteSystem, ctx->managers[i]);
        }
        ctx->managers[i] = NULL;
    }
    for (i = 0; i < 5; i++) {
        ov07_0221D55C(ctx, i);
    }
    for (i = 0; i < 16; i++) {
        if (ctx->inner->actor[i] != NULL) {
            ov07_0221FF2C(ctx->inner->actor[i]);
            ctx->inner->actor[i] = NULL;
        }
    }
    ov07_0221DD14((u8 *)ctx + 0x180, 5);
    BattleSystem_SetDefaultBlend();
    {
        int bg = ov07_0221FAEC(ctx, 1);
        int heap = ov07_0221BFD0(ctx);
        BG_ClearCharDataRange((u8)bg, 0x4000, 0, heap);
    }
    BgClearTilemapBufferAndCommit(ov07_0221C4A0(ctx), (u8)ov07_0221FAEC(ctx, 1));
    ToggleBgLayer(2, 1);
    SetBgPriority(0, ctx->prio0);
    SetBgPriority(1, ctx->prio1);
    SetBgPriority(2, ctx->prio2);
    SetBgPriority(3, ctx->prio3);
    BgSetPosTextAndCommit(ctx->bg, 2, 0, 0);
    BgSetPosTextAndCommit(ctx->bg, 2, 3, 0);
    BgSetPosTextAndCommit(ctx->bg, 3, 0, 0);
    BgSetPosTextAndCommit(ctx->bg, 3, 3, 0);
    ctx->word10 = 0;
}

void ov07_0221CBC0(Ov07Ctx *ctx) {
    int idx;
    u32 a, b;
    V3 pos;
    ctx->script += 1;
    idx = *ctx->script;
    ctx->script += 1;
    a = *ctx->script;
    ctx->script += 1;
    b = *ctx->script;
    ctx->script += 1;
    ctx->inner->current = idx;
    if (ctx->flag[idx]) {
        ParticleSystem_GetCameraUp(ctx->inner->actor[idx], &pos);
        pos.y *= -1;
        ParticleSystem_SetCameraUp(ctx->inner->actor[idx], &pos);
    }
    ParticleSystem_SetPerspectiveType(ctx->inner->actor[idx], ctx->kind[idx]);
    ctx->inner->out[0] = ov07_0221FF18(ctx->inner->actor[idx], a, b, ctx);
}

void ov07_0221CC54(Ov07Ctx *ctx) {
    int idx, slot;
    u32 a, b;
    V3 pos;
    ctx->script += 1;
    idx = *ctx->script;
    ctx->script += 1;
    slot = *ctx->script;
    ctx->script += 1;
    a = *ctx->script;
    ctx->script += 1;
    b = *ctx->script;
    ctx->script += 1;
    ctx->inner->current = idx;
    if (ctx->flag[idx]) {
        ParticleSystem_GetCameraUp(ctx->inner->actor[idx], &pos);
        pos.y *= -1;
        ParticleSystem_SetCameraUp(ctx->inner->actor[idx], &pos);
    }
    ParticleSystem_SetPerspectiveType(ctx->inner->actor[idx], ctx->kind[idx]);
    ctx->inner->out[slot] = ov07_0221FF18(ctx->inner->actor[idx], a, b, ctx);
}

int ov07_0221CCF4(Ov07Ctx *ctx) {
    Tbl36 table = ov07_02234CC8;
    int x = ov07_0221C468(ctx);
    int y = ov07_0221C470(ctx);
    int a = ov07_02231924(ctx, x);
    int b = ov07_02231924(ctx, y);
    int ret = table.v[a][b];
    if (ret == 0xff) {
        GF_AssertFail();
    }
    return ret - 1;
}
int ov07_0221CD4C(Ov07Ctx *ctx) {
    Tbl36 table = ov07_02234D58;
    int x = ov07_0221C468(ctx);
    int y = ov07_0221C470(ctx);
    int a = ov07_02231924(ctx, x);
    int b = ov07_02231924(ctx, y);
    int ret = table.v[a][b];
    if (ret == 0xff) {
        GF_AssertFail();
    }
    return ret;
}

void ov07_0221CDA4(Ov07Ctx *ctx) {
    int idx, select, i;
    u32 args[6], b;
    V3 pos;
    ctx->script += 1;
    idx = *ctx->script;
    ctx->script += 1;
    for (i = 0; i < 6; i++) {
        args[i] = *ctx->script;
        ctx->script += 1;
    }
    b = *ctx->script;
    ctx->script += 1;
    ctx->inner->current = idx;
    if (ctx->flag[idx]) {
        ParticleSystem_GetCameraUp(ctx->inner->actor[idx], &pos);
        pos.y *= -1;
        ParticleSystem_SetCameraUp(ctx->inner->actor[idx], &pos);
    }
    select = ov07_0221CCF4(ctx);
    ParticleSystem_SetPerspectiveType(ctx->inner->actor[idx], ctx->kind[idx]);
    ctx->inner->out[0] = ov07_0221FF18(ctx->inner->actor[idx], args[select], b, ctx);
}
void ov07_0221CE5C(Ov07Ctx *ctx) {
    int idx, select, i;
    u32 args[4], b;
    V3 pos;
    ctx->script += 1;
    idx = *ctx->script;
    ctx->script += 1;
    for (i = 0; i < 4; i++) {
        args[i] = *ctx->script;
        ctx->script += 1;
    }
    b = *ctx->script;
    ctx->script += 1;
    ctx->inner->current = idx;
    if (ctx->flag[idx]) {
        ParticleSystem_GetCameraUp(ctx->inner->actor[idx], &pos);
        pos.y *= -1;
        ParticleSystem_SetCameraUp(ctx->inner->actor[idx], &pos);
    }
    select = ov07_0221CD4C(ctx);
    ParticleSystem_SetPerspectiveType(ctx->inner->actor[idx], ctx->kind[idx]);
    ctx->inner->out[0] = ov07_0221FF18(ctx->inner->actor[idx], args[select], b, ctx);
}
