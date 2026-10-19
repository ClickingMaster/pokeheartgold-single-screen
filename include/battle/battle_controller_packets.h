#ifndef POKEHEARTGOLD_BATTLE_BATTLE_CONTROLLER_PACKETS_H
#define POKEHEARTGOLD_BATTLE_BATTLE_CONTROLLER_PACKETS_H

#include "constants/battle.h"
#include "constants/pokemon.h"

#include "battle/battle.h"

// Packets sent by the BattleController_Emit* functions. The first byte is the command that
// OpponentData_RunCommand dispatches on.

typedef struct BattleControllerPacketEncounterAnimation {
    u32 command;
    u32 randTemp;
} BattleControllerPacketEncounterAnimation;

typedef struct BattleControllerPacketPokemonEncounter {
    u8 command;
    u8 gender : 2;
    u8 shiny : 1;
    u8 form : 5;
    u16 species;
    u32 personality;
    u32 cryParam;
    u16 moves[MAX_MON_MOVES];
    u16 movePPCur[MAX_MON_MOVES];
    u16 movePP[MAX_MON_MOVES];
    u16 nickname[POKEMON_NAME_LENGTH + 1];
} BattleControllerPacketPokemonEncounter;

// Shared by the slide-in (3) and send-out (4) commands
typedef struct BattleControllerPacketPokemonSendOut {
    u8 command;
    u8 gender : 2;
    u8 shiny : 1;
    u8 form : 5;
    u16 species;
    u32 personality;
    u32 cryParam;
    u32 selectedMonIndex;
    u32 ball;
    u32 breakOut;
    u16 moves[MAX_MON_MOVES];
    u16 movePPCur[MAX_MON_MOVES];
    u16 movePP[MAX_MON_MOVES];
    u16 nickname[POKEMON_NAME_LENGTH + 1];
    u8 padding_46[2];
    u32 partnerSelectedMonIndex;
    u32 isSubstitute;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
} BattleControllerPacketPokemonSendOut;

typedef struct BattleControllerPacketRecallPokemon {
    u8 command;
    u8 height;
    u16 ball;
    u32 isSubstitute;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
    u32 selectedMonIndex;
} BattleControllerPacketRecallPokemon;

typedef struct BattleControllerPacketPokemonEnterBall {
    u8 command;
    u8 height;
    u16 ball;
} BattleControllerPacketPokemonEnterBall;

typedef struct BattleControllerPacketTrainerEncounter {
    u8 command;
    u8 trainerGender;
    u16 trainerClass;
} BattleControllerPacketTrainerEncounter;

typedef struct BattleControllerPacketThrowPokeball {
    u8 command;
    u8 throwType;
    u16 partnerSelectedMonIndex;
} BattleControllerPacketThrowPokeball;

typedef struct BattleControllerPacketTrainerSlideIn {
    u8 command;
    u8 trainerGender;
    u16 trainerClass;
    u32 index;
} BattleControllerPacketTrainerSlideIn;

typedef struct BattleControllerPacketHealthbarSlideIn {
    u8 command;
    u8 level;
    s16 hp;
    u16 maxHp;
    u8 selectedMonIndex;
    u8 status : 5;
    u8 gender : 2;
    u8 caught : 1;
    u32 exp;
    u32 maxExp;
    u32 safariBallCount;
    u8 delay;
    u8 padding_15[3];
} BattleControllerPacketHealthbarSlideIn;

typedef struct BattleControllerPacketSelectCommand {
    u8 command;
    u8 partySlot;
    u8 expPercents[PARTY_SIZE];
    u8 ballStatus[2][PARTY_SIZE];
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 movePP[MAX_MON_MOVES];
    s16 hp;
    u16 maxHp;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
    u16 unk_2A;
} BattleControllerPacketSelectCommand;

typedef struct BattleControllerPacketSelectMove {
    u8 command;
    u8 selectedMonIndex;
    u16 unusableMovesMask;
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 movePP[MAX_MON_MOVES];
} BattleControllerPacketSelectMove;

typedef struct BattleControllerPacketSelectTarget {
    u8 command;
    u8 isLastSelection;
    u16 moveRange;
    TargetPokemon targetMons[BATTLER_MAX];
} BattleControllerPacketSelectTarget;

typedef struct BattleControllerPacketSelectItem {
    u8 command;
    u8 twoTargets;
    u8 targetSemiInvulnerable;
    u8 targetSubstitute;
    u8 selectedMonIndex[BATTLER_MAX];
    u8 partyOrder[BATTLER_MAX][PARTY_SIZE];
    u8 embargoFlag[BATTLER_MAX];
} BattleControllerPacketSelectItem;

typedef struct BattleControllerPacketShowMonList {
    u8 command;
    u8 battlerId;
    u8 mode;
    u8 reservedMon;
    u8 selectedMonIndex[BATTLER_MAX];
    u8 partyOrder[BATTLER_MAX][PARTY_SIZE];
    u32 trapped;
    u8 switchInFlag;
    u8 padding_25[3];
} BattleControllerPacketShowMonList;

typedef struct BattleControllerPacketYesNoBox {
    u8 command;
    u8 mode;
    u16 msgId;
    u32 value;
    u32 param;
} BattleControllerPacketYesNoBox;

typedef struct BattleControllerPacketAttackMessage {
    u8 command;
    u8 selectedMonIndex;
    u16 move;
} BattleControllerPacketAttackMessage;

typedef struct BattleControllerPacketHealthbarUpdate {
    u8 command;
    u8 level;
    s16 hp;
    u16 maxHp;
    u8 unk_06;
    u8 gender;
    s32 gainedHp;
    u32 exp;
    u32 maxExp;
} BattleControllerPacketHealthbarUpdate;

typedef struct BattleControllerPacketExpBarUpdate {
    u8 command;
    u8 padding_01[3];
    s32 prevExp;
    s32 exp;
    s32 maxExp;
} BattleControllerPacketExpBarUpdate;

typedef struct BattleControllerPacketFaintAnimation {
    u8 command;
    u8 gender;
    u16 species;
    u32 personality;
    u8 form;
    u8 isSubstitute;
    u8 isTransformed;
    u8 unk_0B;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
} BattleControllerPacketFaintAnimation;

typedef struct BattleControllerPacketPlaySE {
    u8 command;
    u8 unk_01;
    u16 sndSeqNo;
} BattleControllerPacketPlaySE;

typedef struct BattleControllerPacketToggleVanish {
    u8 command;
    u8 vanish;
    u8 isSubstitute;
    u8 unk_03;
    u16 allSpecies[BATTLER_MAX];
    u8 allGender[BATTLER_MAX];
    u8 allShiny[BATTLER_MAX];
    u8 allForm[BATTLER_MAX];
    u32 allPersonality[BATTLER_MAX];
} BattleControllerPacketToggleVanish;

typedef struct BattleControllerPacketHealthbarStatus {
    u8 command;
    u8 status;
    u16 unk_02;
} BattleControllerPacketHealthbarStatus;

typedef struct BattleControllerPacketTrainerMessage {
    u8 command;
    u8 msg;
    u16 unk_02;
} BattleControllerPacketTrainerMessage;

// Recall (32) and send-out (33) messages; the value grades the enemy's remaining HP
typedef struct BattleControllerPacketSwitchMessage {
    u8 command;
    u8 selectedMonIndex;
    u16 value;
} BattleControllerPacketSwitchMessage;

typedef struct BattleControllerPacketFirstSendOutMessage {
    u8 command;
    u8 padding_01[3];
    u8 selectedMonIndex[BATTLER_MAX];
} BattleControllerPacketFirstSendOutMessage;

typedef struct BattleControllerPacketSelectionMessage {
    u8 command;
    u8 padding_01[3];
    BattleMessage msg;
} BattleControllerPacketSelectionMessage;

typedef struct BattleControllerPacketRefreshHealthbar {
    u8 command;
    u8 level;
    s16 hp;
    u16 maxHp;
    u8 selectedMonIndex;
    u8 status : 5;
    u8 gender : 2;
    u8 caught : 1;
    u32 exp;
    u32 maxExp;
    u32 safariBallCount;
} BattleControllerPacketRefreshHealthbar;

typedef struct BattleControllerPacketBattleMonToPartyMonCopy {
    u8 command;
    u8 selectedMonIndex : 4;
    u8 mimicedMoveIndex : 4;
    s16 hp;
    u32 status;
    u32 battlerBitKnockedOffItem;
    u16 item;
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 padding_1A[2];
    u32 status2;
    u16 form;
    u16 unk_22;
    u32 ability;
    u16 recalcMonStats;
    u16 formChange;
} BattleControllerPacketBattleMonToPartyMonCopy;

typedef struct BattleControllerPacketPartyStatusHeal {
    u8 command;
    u8 ability;
    u16 moveNo;
} BattleControllerPacketPartyStatusHeal;

typedef struct BattleControllerPacketSelectMoveToForget {
    u8 command;
    u8 slot;
    u16 move;
} BattleControllerPacketSelectMoveToForget;

typedef struct BattleControllerPacketMosaicAnimation {
    u8 command;
    u8 mosaic;
    u8 delay;
    u8 unk_03;
} BattleControllerPacketMosaicAnimation;

typedef struct BattleControllerPacketChangeForm {
    u8 command;
    u8 form;
    u16 species;
    u8 gender;
    u8 shiny;
    u16 unk_06;
    u32 personality;
} BattleControllerPacketChangeForm;

typedef struct BattleControllerPacketBallGauge {
    u8 command;
    u8 unk_01;
    u8 status[PARTY_SIZE];
} BattleControllerPacketBallGauge;

typedef struct BattleControllerPacketIncrementGameStat {
    u8 command;
    u8 flag;
    u16 id;
} BattleControllerPacketIncrementGameStat;

// Commands that also carry the record data of a recorded link battle (see BattleSystem_PackRecordUpdate)
typedef struct BattleControllerPacketRecordData {
    u8 command;
    u8 escapedMask;
    u16 recordSize;
    u8 record[28];
} BattleControllerPacketRecordData;

typedef struct BattleControllerPacketMoveSE {
    u8 command;
    u8 effectiveness;
    u8 padding_02[2];
} BattleControllerPacketMoveSE;

typedef struct BattleControllerPacketPlaySong {
    u8 command;
    u8 unk_01;
    u16 song;
} BattleControllerPacketPlaySong;

typedef struct BattleControllerPacketBattleResults {
    u8 command;
    u8 unk_01;
    vu16 recordSize;
    u32 outcomeFlags;
    u8 record[28];
} BattleControllerPacketBattleResults;

typedef struct BattleControllerPacketCommandComplete {
    u8 command;
    u8 netId;
    u16 unk_02;
} BattleControllerPacketCommandComplete;

typedef struct BattleControllerPacketScreenFade {
    u8 command;
    u8 mode;
    u16 unk_02;
} BattleControllerPacketScreenFade;

#endif // POKEHEARTGOLD_BATTLE_BATTLE_CONTROLLER_PACKETS_H
