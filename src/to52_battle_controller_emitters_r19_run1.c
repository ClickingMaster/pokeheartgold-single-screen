#include "to52_battle_controller_emitters_r19_private.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void BattleController_EmitRunAwayMessage(BattleSystem *bsys, BattleContext *ctx) {
    EscapeMessage msg;
    int i;
    u8 *p = (u8 *)ctx;
    u32 battleType = BattleSystem_GetBattleType(bsys);
    msg.command = 0x3c;
    msg.escaperMask = 0;
    msg.recordedCount = 0;
    for (i = 0; i < BattleSystem_GetMaxBattlers(bsys); i++) {
        if (U32(p, 0x21a8) == 0x10) {
            msg.escaperMask |= MaskOfFlagNo(i);
        }
        p += 0x10;
    }
    if ((battleType & 4) && sub_0202FC48() == TRUE && (U32(bsys, 0x240c) & 0x10) == 0) {
        msg.recordedCount = ov12_0223BE68(bsys, msg.recorded);
        if (msg.recordedCount >= 28) {
            GF_AssertFail();
        }
    }
    ov12_02262240(bsys, 1, 0, &msg, sizeof(msg));
}

void BattleController_EmitForefitMessage(BattleSystem *bsys) {
    ForfeitMessage msg;
    u32 battleType = BattleSystem_GetBattleType(bsys);
    msg.command = 0x3d;
    msg.recordedCount = 0;
    if ((battleType & 4) && sub_0202FC48() == TRUE && (U32(bsys, 0x240c) & 0x10) == 0) {
        msg.recordedCount = ov12_0223BE68(bsys, msg.recorded);
        if (msg.recordedCount >= 28) {
            GF_AssertFail();
        }
    }
    ov12_02262240(bsys, 1, 0, &msg, sizeof(msg));
}
