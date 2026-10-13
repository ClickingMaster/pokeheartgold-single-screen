#ifndef POKEHEARTGOLD_BATTLE_CONTROLLER_H
#define POKEHEARTGOLD_BATTLE_CONTROLLER_H

#include "battle/battle.h"

typedef void (*ControllerFunction)(BattleSystem *, BattleContext *ctx);

// Command 22 (move animation); the restore-sprite (56) and substitute-sprite (62) commands
// send the same layout with only the per-battler sprite fields filled in.
typedef struct MoveAnimData {
    u8 command;
    u8 unk_01;
    u16 move;
    int damage;
    u16 power;
    u16 moveAnimVariant;
    u16 friendship;
    u16 isSubstitute : 1;
    u16 isTransformed : 1;
    u16 attackerActive : 1;
    u16 defenderActive : 1;
    u16 unk_0E_4 : 12;
    u32 fieldCondition;
    u16 attacker;
    u16 defender;
    u16 species[BATTLER_MAX];
    u8 gender[BATTLER_MAX];
    u8 shiny[BATTLER_MAX];
    u8 form[BATTLER_MAX];
    u32 personality[BATTLER_MAX];
    u32 moveEffectFlags[BATTLER_MAX];
    int isEffectAnim;
    int effectAnimId;
    int terrain;
} MoveAnimData;

// Packet queues. direction 0: input replies to the battle context (ctx->battleBuffer),
// 1: commands to a battler's OpponentData, 2: command-complete notices.
void BattleController_SendData(BattleSystem *battleSystem, int direction, int battlerId, const u8 *data, u8 size);
BOOL BattleController_RecvData(BattleSystem *battleSystem, const u8 *data);
void BattleController_ProcessQueue(BattleSystem *battleSystem, int direction);
void BattleController_EmitPacket(BattleSystem *battleSystem, int direction, int battlerId, const void *data, u8 size);
BOOL BattleController_RecvLinkData(BattleSystem *battleSystem, u8 *data);
void BattleController_EmitCommandComplete(BattleSystem *battleSystem, int battlerId, int command);

void BattleController_ReplyCommandSelection(BattleSystem *battleSystem, int battlerId, u32 input);
void BattleController_ReplyMoveSelection(BattleSystem *battleSystem, int battlerId, u32 input);
void BattleController_ReplyTargetSelection(BattleSystem *battleSystem, int battlerId, u32 input);
void BattleController_ReplyItemSelection(BattleSystem *battleSystem, int battlerId, u32 input);
void BattleController_ReplyPartySelection(BattleSystem *battleSystem, int battlerId, u32 input);
void BattleController_ReplySelectionMessageDone(BattleSystem *battleSystem, int battlerId);
void BattleController_InitMoveAnimData(BattleSystem *battleSystem, BattleContext *ctx, MoveAnimData *animData, int a3, int a4, int attacker, int defender, u16 move);

void BattleController_EmitPlayEncounterAnimation(BattleSystem *battleSystem, BOOL a1);
void BattleController_EmitPokemonEncounter(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitPokemonSlideIn(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitPokemonSendOut(BattleSystem *battleSystem, int battlerId, int ball, int a3);
void BattleController_EmitRecallPokemon(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitDeletePokemon(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitTrainerEncounter(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitThrowPokeball(BattleSystem *battleSystem, int battlerId, u32 throwType);
void BattleController_EmitTrainerSlideOut(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitTrainerSlideIn(BattleSystem *battleSystem, int battlerId, u32 index);
void BattleController_EmitBackgroundSlideIn(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitHealthbarSlideIn(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, u32 delay);
void BattleController_EmitHealthbarSlideOut(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitShowMonList(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, u32 a3, u32 a4, u32 a5);
void BattleController_EmitDrawYesNoBox(BattleSystem *battleSystem, BattleContext *ctx, int a2, int a3, int a4, int a5, int a6);
void BattleController_EmitShowWaitMessage(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitPrintAttackMessage(BattleSystem *battleSystem, BattleContext *ctx);
void BattleController_EmitPrintMessage(BattleSystem *battleSystem, BattleContext *ctx, BattleMessage *msg);
void BattleController_SetMoveAnimation(BattleSystem *battleSystem, BattleContext *ctx, u16 move);
void BattleController_SetMoveAnimationForBattlers(BattleSystem *battleSystem, BattleContext *ctx, u16 move, int attacker, int target);
void BattleController_EmitMonFlicker(BattleSystem *battleSystem, int battlerId, int a2);
void BattleController_EmitHealthbarUpdate(BattleSystem *battleSystem, BattleContext *ctx, int side);
void BattleController_EmitPlayFaintAnimation(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitPlaySE(BattleSystem *battleSystem, BattleContext *ctx, u32 sndSeqNo, int battlerId);
void BattleController_EmitFadeOutBattle(BattleSystem *battleSystem, BattleContext *ctx);
void BattleController_EmitToggleVanish(BattleSystem *battleSystem, int battlerId, int vanish);
void BattleController_EmitHealthbarStatus(BattleSystem *battleSystem, int battlerId, int status);
void BattleController_EmitPrintTrainerMessage(BattleSystem *battleSystem, int battlerId, int msg);
void BattleController_EmitSetStatus2Effect(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int status);
void BattleController_EmitCopyStatus2Effect(BattleSystem *battleSystem, BattleContext *ctx, int battlerIdA, int battlerIdB, int status);
void BattleController_EmitPrintReturnMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int a3);
void BattleController_EmitPrintSendOutMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int a3);
void BattleController_EmitPrintEncounterMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitPrintFirstSendOutMessage(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitBattleMonToPartyMonCopy(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleControl_EmitPartyStatusHeal(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int moveNo);
void BattleController_EmitSetBattleResults(BattleSystem *battleSystem);
void BattleController_EmitPlayMosaicAnimation(BattleSystem *battleSystem, int battlerId, int a2, int delay);
void BattleController_EmitChangeForm(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitSetBattleBackground(BattleSystem *battleSystem, int a1);
void BattleController_EmitInitStartBallGauge(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitDeleteStartBallGauge(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitInitBallGauge(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitDeleteBallGauge(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitLoadBallGfx(BattleSystem *battleSystem);
void BattleController_EmitDeleteBallGfx(BattleSystem *battleSystem);
void BattleController_EmitIncrementGameStat(BattleSystem *battleSystem, int battlerId, int flag, int id);
void BattleController_EmitRestoreSprite(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitSpriteToOAM(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitOAMToSprite(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitPrintResultMessage(BattleSystem *battleSystem);
void BattleController_EmitRunAwayMessage(BattleSystem *battleSystem, BattleContext *ctx);
void BattleController_EmitForefitMessage(BattleSystem *battleSystem);
void BattleController_EmitSwapToSubstituteSprite(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitPlayMoveSE(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitPlaySong(BattleSystem *battleSystem, int battlerId, int song);
void BattleSystem_ReloadMonData(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int monIndex);

void ov12_022645C8(BattleSystem *battleSystem, BattleContext *ctx, u8 a2);
void BattleController_EmitSelectCommand(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int partySlot);
void BattleController_EmitPokemonEnterBall(BattleSystem *battleSystem, s32 battlerId, s32 ball);
void BattleController_EmitPrintSelectionMessage(BattleSystem *battleSystem, int battlerId, BattleMessage msg);
void BattleController_EmitCancelSelection(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitSelectMove(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitSelectTarget(BattleSystem *battleSystem, BattleContext *ctx, int a2, int battlerId);
void BattleController_EmitSelectItem(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitEndSelection(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitExpBarUpdate(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, u32 prevExp);
void BattleController_EmitExpBarFullFlash(BattleSystem *battleSystem, int battlerId);
void BattleController_EmitRefreshHealthbar(BattleSystem *battleSystem, BattleContext *ctx, int battlerId);
void BattleController_EmitSelectMoveToForget(BattleSystem *battleSystem, int battlerId, u32 move, int slot);
void BattleController_EmitBlankMessage(BattleSystem *battleSystem);

#endif
