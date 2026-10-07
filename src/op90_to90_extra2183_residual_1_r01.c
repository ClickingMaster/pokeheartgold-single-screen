#pragma force_active on
#include "global.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
BOOL BattleController_RecvData(BattleSystem *bsys, const u8 *data);
// @fn BattleController_RecvData
BOOL BattleController_RecvData(BattleSystem *bsys, const u8 *data) {
 int i = 0; int type = data[0]; int id = data[1]; int size = data[2] | (data[3] << 8);
    if (type == 0) {
        if (bsys->ctx->battleBuffer[id][0] == 0) {
            for (i = 0; i < size; i++) {
                bsys->ctx->battleBuffer[id][i] = data[4 + i];
            }
            i = TRUE;
        }
    } else if (type == 1) {
        if (bsys->opponentData[id]->unk8C[8] == 0) {
            for (i = 0; i < size; i++) {
                bsys->opponentData[id]->unk8C[8 + i] = data[4 + i];
            }
            i = TRUE;
        }
    } else if (type == 2) {
        int a = data[4];
        int b = data[5];
        if (ov12_0223B688(bsys)) {
            ov12_0224ED00(bsys->ctx, b, id, a);
        }
        i = TRUE;
    }
    return i;
}
