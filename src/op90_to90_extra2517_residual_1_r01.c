#pragma force_active on
#include "global.h"
#include "pokemon.h"
typedef struct {
    u8 pad0[8];
    void *bsys;
    void *unkC;
    u8 pad10;
    u8 idx;
    u8 pad12[0x22 - 0x12];
    u16 unk22;
    u8 pad24[4];
    int battler;
    u8 partySlots[7];
    u8 unk33;
} BattleCtx;
typedef struct {
    u16 move;
    u8 pp;
    u8 pad[5];
} MoveSlot;
typedef struct {
    Pokemon *mon;
    u8 pad4[0x30 - 0x4];
    MoveSlot moves[4];
} MonEntry;
typedef struct {
    BattleCtx *ctx;
    MonEntry mons[6];
    u8 pad1E4[0x2079 - 4 - 6 * 0x50];
    u8 unk2079;
    u8 pad207A[2];
    u8 state;
    u8 pad207D[3];
    u16 ppMax[4];
} Work;
extern Pokemon *BattleSystem_GetPartyMon(void *, int, int);
extern void ov08_02220224(Work *);
extern void ov08_02220064(Work *, u16, u16);
extern void ov08_0221DBCC(void *, u16, u8, void *);
extern void ov08_022201C0(Work *);
extern void PlaySE(int);
int ov08_0221CC38(Work *w);
// @fn ov08_0221CC38
int ov08_0221CC38(Work *w) {
    BattleCtx *ctx = w->ctx;
    u32 i;
    u32 cnt;
    switch (w->state) {
    case 0:
        w->mons[ctx->idx].mon = BattleSystem_GetPartyMon(ctx->bsys, ctx->battler, ctx->partySlots[ctx->idx]);
        for (i = 0; i < 4; i++) {
            if (w->mons[ctx->idx].moves[i].move != 0) {
                w->ppMax[i] = GetMonData(w->mons[ctx->idx].mon, MON_DATA_MOVE1_PP + i, NULL);
            }
        }
        ov08_02220224(w);
        PlaySE(1516);
        w->state = 1;
        break;
    case 1:
        cnt = 0;
        for (i = 0; i < 4; i++) {
            if (w->mons[ctx->idx].moves[i].move == 0) {
                cnt++;
            } else if (w->ppMax[i] != w->mons[ctx->idx].moves[i].pp) {
                w->mons[ctx->idx].moves[i].pp++;
                ov08_02220064(w, i + 1, i);
            } else {
                cnt++;
            }
        }
        if (cnt == 4) {
            w->state = 2;
        }
        break;
    case 2:
        ov08_0221DBCC(ctx->bsys, ctx->unk22, ctx->unk33, ctx->unkC);
        ov08_022201C0(w);
        w->unk2079 = 25;
        return 17;
    }
    return 24;
}
