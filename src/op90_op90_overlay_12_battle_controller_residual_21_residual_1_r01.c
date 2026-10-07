#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "constants/abilities.h"
#include "constants/battle.h"

typedef struct MoveAnimData {
    u8 command;
    u8 unk01;
    u16 move;
    int damage;
    u16 power;
    u16 unk0A;
    u16 friendship;
    u16 substitute : 1;
    u16 transformed : 1;
    u16 attackerActive : 1;
    u16 defenderActive : 1;
    u16 unk0E_4 : 12;
    u32 fieldCondition;
    u16 attacker;
    u16 defender;
    u16 species[4];
    u8 gender[4];
    u8 shiny[4];
    u8 form[4];
    u32 personality[4];
    u32 moveEffectFlags[4];
    int unk4C;
    int unk50;
    int terrain;
} MoveAnimData;

void ov12_022643C8(BattleSystem *bsys, BattleContext *ctx, MoveAnimData *anim, int a3, int a4, int attacker, int defender, u16 move);
// @fn ov12_022643C8
void ov12_022643C8(BattleSystem *bsys, BattleContext *ctx, MoveAnimData *anim, int a3, int a4, int attacker, int defender, u16 move) {
    int i;
    u8 slot;

    anim->command = 22;
    anim->move = move;
    anim->attacker = attacker;
    anim->defender = defender;
    anim->unk4C = a3;
    anim->unk50 = a4;
    anim->terrain = BattleSystem_GetTerrainId(bsys);
    anim->attackerActive = FALSE;
    anim->defenderActive = FALSE;
    if (ctx == NULL) {
        return;
    }
    anim->damage = ctx->damage;
    if (ctx->movePower) {
        anim->power = ctx->movePower;
    } else {
        anim->power = ctx->trainerAIData.moveData[move].power;
    }
    anim->friendship = ctx->battleMons[attacker].friendship;
    if (!CheckAbilityActive(bsys, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_CLOUD_NINE) && !CheckAbilityActive(bsys, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_AIR_LOCK)) {
        anim->fieldCondition = ctx->fieldCondition;
    } else {
        anim->fieldCondition = 0;
    }
    anim->unk0A = ctx->unk_2164;
    anim->substitute = (ctx->battleMons[(u32)attacker].status2 & STATUS2_SUBSTITUTE) ? 1 : 0;
    anim->transformed = (ctx->battleMons[(u32)attacker].status2 & STATUS2_TRANSFORM) != 0;
    for (i = 0; i < 4; i++) {
        anim->species[i] = ctx->battleMons[i].species;
        anim->shiny[i] = ctx->battleMons[i].shiny;
        anim->form[i] = ctx->battleMons[i].form;
        anim->moveEffectFlags[i] = ctx->battleMons[i].moveEffectFlags;
        if (ctx->battleMons[i].status2 & STATUS2_TRANSFORM) {
            anim->gender[i] = ctx->battleMons[i].unk88.transformGender;
            anim->personality[i] = ctx->battleMons[i].unk88.transformPersonality;
        } else {
            anim->gender[i] = ctx->battleMons[i].gender;
            anim->personality[i] = ctx->battleMons[i].personality;
        }
    }
    if (attacker != 0xFF) {
        slot = ov12_0223C140(bsys, attacker);
        if (slot != 0xFF && slot == ctx->selectedMonIndex[attacker]) {
            anim->attackerActive = TRUE;
        }
    }
    if (defender != 0xFF) {
        slot = ov12_0223C140(bsys, defender);
        if (slot != 0xFF && slot == ctx->selectedMonIndex[defender]) {
            anim->defenderActive = TRUE;
        }
    }
}
