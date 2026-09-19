#include "to52_battle_controller_emitters_r19_private.h"
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void BattleController_EmitPlayMoveSE(BattleSystem *bsys, BattleContext *ctx, int battler) {
    MoveSoundMessage msg;
    msg.command = 0x3f;
    if (U32(ctx, 0x216c) & 2) {
        msg.effectiveness = 2;
    } else if (U32(ctx, 0x216c) & 4) {
        msg.effectiveness = 1;
    } else {
        msg.effectiveness = 0;
    }
    ov12_02262240(bsys, 1, battler, &msg, sizeof(msg));
}
