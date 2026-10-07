#pragma force_active on
#include "global.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
BOOL ov12_02264334(BattleSystem *bsys, u8 *data);
// @fn ov12_02264334
BOOL ov12_02264334(BattleSystem *bsys, u8 *data) {
    BOOL ret = TRUE; int id = data[1]; int type = data[0]; int size = data[2] | (data[3] << 8); int i;

    if (type == 0) {
        for (i = 0; i < size; i++) {
            bsys->ctx->battleBuffer[id][i] = data[4 + i];
        }
    } else if (type == 1) {
        if (bsys->opponentData[id]->unk1A8 == 0) {
            bsys->opponentData[id]->unk1A8 = 1;
            for (i = 0; i < size; i++) {
                bsys->opponentData[id]->unk8C[8 + i] = data[4 + i];
            }
        } else {
            ret = FALSE;
        }
    } else if (type == 2) {
        int a = data[4];
        int b = data[5];
        if (ov12_0223B688(bsys)) {
            ov12_0224ED00(bsys->ctx, b, id, a);
        }
    }
    return ret;
}
