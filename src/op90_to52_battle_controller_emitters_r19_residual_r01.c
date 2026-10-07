#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "constants/battle.h"

typedef struct SubstituteSpriteMessage {
    u8 command;
    u8 unk01[0x17];
    u16 species[4];
    u8 gender[4];
    u8 shiny[4];
    u8 form[4];
    u32 personality[4];
    u8 unk3C[0x1C];
} SubstituteSpriteMessage;

void ov12_02262240(BattleSystem *bsys, int direction, int battlerId, const void *data, u8 size);
// @fn BattleController_EmitSwapToSubstituteSprite
void BattleController_EmitSwapToSubstituteSprite(BattleSystem *bsys, BattleContext *ctx, int battlerId) {
    SubstituteSpriteMessage msg;
    int i;

    msg.command = 62;
    for (i = 0; i < 4; i++) {
        msg.species[i] = ctx->battleMons[i].species;
        msg.shiny[i] = ctx->battleMons[i].shiny;
        msg.form[i] = ctx->battleMons[i].form;
        if (ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            msg.gender[i] = ctx->battleMons[i].unk88.transformGender;
            msg.personality[i] = ctx->battleMons[i].unk88.transformPersonality;
        } else {
            msg.gender[i] = ctx->battleMons[i].gender;
            msg.personality[i] = ctx->battleMons[i].personality;
        }
    }
    ov12_02262240(bsys, 1, battlerId, &msg, sizeof(SubstituteSpriteMessage));
}
