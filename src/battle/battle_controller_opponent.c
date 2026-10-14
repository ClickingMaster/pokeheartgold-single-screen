#include "battle/battle_controller_opponent.h"

#include "global.h"

#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/message_tags.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "battle/battle.h"
#include "battle/battle_022378C0.h"
#include "battle/battle_pokepic_bob.h"
#include "battle/battle_anim_internal.h"
#include "battle/battle_controller.h"
#include "battle/battle_cursor.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_input.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "battle/party_gauge.h"
#include "battle/trainer_ai.h"

#include "brightness.h"
#include "filesystem.h"
#include "heap.h"
#include "move.h"
#include "overlay_07.h"
#include "palette.h"
#include "palette_blink.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "render_window.h"
#include "sound.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "char_tiles.h"

// The command the battle controller sent to this battler (BattleController_RecvData copies it to 0x94).
#define OPPONENT_COMMAND(opponentData) ((void *)&(opponentData)->unk8C[8])

#define MOVE_STATUS_IMMUNE (MOVE_STATUS_NO_EFFECT | MOVE_STATUS_WONDER_GUARD_IMMUNE | MOVE_STATUS_LEVITATE_IMMUNE | MOVE_STATUS_MAGNET_RISE_IMMUNE)

#define OPPONENT_COMMAND_ID(opponentData) ((opponentData)->unk8C[0x08])
// The OAM copy of the battler's pokepic (BattleAnimBattlerSprites_New) is kept at 0x8C.
#define OPPONENT_SPRITE_OAM(opponentData) (*(void **)&(opponentData)->unk8C[0x00])

// The battle party menu overlay's entry point, which is linked at the same address as TrainerAI_Init.
#define BATTLE_PARTY_MENU_START(args) ((void (*)(BattlePartyMenuArgs *))TrainerAI_Init)(args)

// An item selection reply. It is passed by value as a struct: battle_controller.h declares the u32 it amounts to.
typedef struct BattleItemInput {
    u16 item;
    u8 pocket;
    u8 target;
} BattleItemInput;

#define REPLY_ITEM_SELECTION(battleSystem, battlerId, input) ((void (*)(BattleSystem *, int, BattleItemInput))BattleController_ReplyItemSelection)(battleSystem, battlerId, input)

// Functions of other modules no header declares yet.
void sub_02005B58(u8 value);

// Overlay 7 (battle animations) functions no header declares yet.
typedef struct BattlePokeballOpenArgs {
    int battlerType;
    int ball;
    Pokemon *mon;
} BattlePokeballOpenArgs;

typedef struct BattleEmitterParams {
    u32 index;
    s32 unk_04;
} BattleEmitterParams;

void *BallSealEffect_New(enum HeapID heapID, BattlePokeballOpenArgs *args);
void BallSealEffect_LoadResources(void *ballOpenEffect);
int BallSealEffect_IsLoaded(void *ballOpenEffect);
void BallSealEffect_Start(void *ballOpenEffect);
BOOL BallSealEffect_IsActive(void *ballOpenEffect);
void BallSealEffect_Free(void *ballOpenEffect);
BOOL BallThrow_IsActive(BallThrow *ballData);
BOOL BallThrow_HasOpened(BallThrow *ballData);
void BallThrow_SetMode(BallThrow *ballData, u32 mode);
void BallThrow_SetDrawFlag(BallThrow *ballData, int flag);
void BallThrow_SetPosition(BallThrow *ballData, s16 x, s16 y);
void BallThrow_SetAnimateSprite(BallThrow *ballData, u32 animate);
void BallThrow_SetDrawPriority(BallThrow *ballData, u16 priority);
void BallThrow_SetPriority(BallThrow *ballData, int priority);
void BallThrow_SetDelay(BallThrow *ballData, s32 delay);
void *BattleAnimEmitterManager_New(enum HeapID heapID, s32 a1);
void *BattleAnimEmitterManager_AddEmitter(void *emitterManager, BattleEmitterParams *params);
void BattleAnimEmitterManager_Free(void *emitterManager);
void BattleAnimEmitterManager_StopAll(void *emitterManager);
void BattleAnimEmitter_Start(void *emitter);
u32 BattleAnimEmitter_IsActive(void *emitter);
s16 ov07_02234B5C(u32 battlerType, u32 index);

extern const s16 ov07_022377DC[][2];
// Pokemon/trainer positions (x, y, z) by battler type: [0] on the field, [1] off screen where a slide-in starts.
extern const s16 ov07_022377F4[2][6][3];

// Data in asm/battle_controller_opponent_data.s
typedef void (*OpponentCommandHandler)(BattleSystem *battleSystem, OpponentData *opponentData);

typedef struct OpponentBallKindTable {
    int kinds[6];
} OpponentBallKindTable;

extern const OpponentCommandHandler sOpponentCommandHandlers[];
extern const u8 ov12_0226D120[];
extern const OpponentBallKindTable ov12_0226D128;
// not const: Task_SafariSelectCommand reads them before BattleSystem_Random, a const table would be read after it
extern u8 ov12_0226D140[][2];
extern u8 ov12_0226D141[][2];
extern const u16 sBallColors[];
extern const u32 ov12_0226D18C[];
extern const s16 ov12_0226D1E8[][6][2];
extern const s16 ov12_0226D1EA[][6][2];
extern const u8 ov12_0226D350[];

// Commands received from the battle controller (the BattleController_Emit* packets).
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
} BattleControllerPacketPokemonEncounter;

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
    u8 filler_46[2];
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
    u8 filler_15[3];
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
    u8 unk_01;
    u16 moveRange;
    TargetPokemon targetMons[BATTLER_MAX];
} BattleControllerPacketSelectTarget;

typedef struct BattleControllerPacketSelectItem {
    u8 command;
    u8 unk_01;
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
    u8 filler_25[3];
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
    u8 filler_06[2];
    s32 gainedHp;
} BattleControllerPacketHealthbarUpdate;

typedef struct BattleControllerPacketExpBarUpdate {
    u8 command;
    u8 filler_01[3];
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

typedef struct BattleControllerPacketSwitchMessage {
    u8 command;
    u8 selectedMonIndex;
    u16 value;
} BattleControllerPacketSwitchMessage;

typedef struct BattleControllerPacketFirstSendOutMessage {
    u8 command;
    u8 filler_01[3];
    u8 selectedMonIndex[BATTLER_MAX];
} BattleControllerPacketFirstSendOutMessage;

typedef struct BattleControllerPacketSelectionMessage {
    u8 command;
    u8 filler_01[3];
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
    u8 filler_1A[2];
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

typedef struct BattleControllerPacketRecordData {
    u8 command;
    u8 escapedMask;
    u16 recordSize;
    u8 record[28];
} BattleControllerPacketRecordData;

typedef struct BattleControllerPacketMoveSE {
    u8 command;
    u8 effectiveness;
    u8 filler_02[2];
} BattleControllerPacketMoveSE;

typedef struct BattleControllerPacketPlaySong {
    u8 command;
    u8 unk_01;
    u16 song;
} BattleControllerPacketPlaySong;

typedef struct BattleControllerPacketBattleResults {
    u8 command;
    u8 unk_01;
    u16 recordSize;
    u32 outcomeFlags;
    u8 record[28];
} BattleControllerPacketBattleResults;

typedef struct BattleControllerPacketScreenFade {
    u8 command;
    u8 mode;
    u16 unk_02;
} BattleControllerPacketScreenFade;

// Arguments of the battle party menu (BATTLE_PARTY_MENU_START), also used to pick the move to forget.
typedef struct BattlePartyMenuArgs {
    Party *party;
    u32 unk_04;
    BattleSystem *battleSystem;
    enum HeapID heapID;
    u8 unk_10;
    u8 selectedSlot;
    u8 reservedMon;
    u8 unk_13;
    u8 selectedMonIndex;
    u8 partnerSelectedMonIndex;
    u8 filler_16[2];
    u32 embargoFlag;
    u32 partnerEmbargoFlag;
    u16 hp;
    u16 item;
    u16 move;
    u8 filler_26[2];
    u32 battlerId;
    u8 partyOrder[PARTY_SIZE];
    u8 key;
    u8 pocket;
    u8 moveSlot;
    u8 mode;
    u8 done;
    u8 filler_37;
} BattlePartyMenuArgs;

// Arguments of the battle bag (BattleBagMenu_Create).
typedef struct BattleBagMenuArgs {
    BattleSystem *battleSystem;
    PlayerProfile *playerProfile;
    Bag *bag;
    enum HeapID heapID;
    u32 battlerId;
    u32 unk_14;
    u32 embargoFlag;
    u16 item;
    u8 pocket;
    u8 unk_1F;
    u8 unk_20;
    u8 unk_21;
    u8 unk_22;
    u8 targetSemiInvulnerable;
    u8 targetSubstitute;
    u8 key;
    u8 done;
    u8 filler_27[13];
} BattleBagMenuArgs;

void BattleBagMenu_Create(BattleBagMenuArgs *args);

// Task data of the command workers.
typedef struct OpponentEncounterAnimWork {
    BattleSystem *battleSystem;
    void *emitterManager;
    void *firstEmitter;
    void *secondEmitter;
    u32 unk_10;
    u8 state;
    u8 timer;
    u8 fadeStep;
} OpponentEncounterAnimWork;

typedef struct OpponentPokemonEncounterWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    BattleTerrain *terrain;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 facing;
    s16 target;
    u16 species;
    u32 cryParam;
    u32 battlerType;
    int delay;
    u32 nature;
    u32 shiny;
    u8 form;
} OpponentPokemonEncounterWork;

typedef struct OpponentPokepicAnimParams {
    u8 shadowSize;
    s8 shadowXOffset;
    s8 shadowHeight;
    PokepicAnimScript scripts[10];
} OpponentPokepicAnimParams;

typedef struct OpponentPokemonSendOutWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    void *ballOpenEffect;
    BallThrow *ballData;
    BattleAnimBattlerSprites *battlerSprites;
    PokepicTemplate pokepicTemplate;
    BattleAnimSystem *animSystem;
    MoveAnimData animData;
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    u8 facing;
    u8 height;
    u16 species;
    u32 cryParam;
    u8 selectedMonIndex;
    u8 nature;
    u16 ball;
    s8 shadowHeight;
    s8 shadowXOffset;
    u8 shiny;
    u8 shadowSize;
    u16 breakOut;
    u8 delay;
    u8 form;
    u32 isSubstitute;
} OpponentPokemonSendOutWork;

typedef struct OpponentRecallPokemonWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    BallThrow *ballData;
    MoveAnimData animData;
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    u8 height;
    u8 filler_6D;
    u16 ball;
    u32 isSubstitute;
    u32 selectedMonIndex;
} OpponentRecallPokemonWork;

typedef struct OpponentPokepicShrinkWork {
    BattleSystem *battleSystem;
    Pokepic *pokepic;
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    u8 height;
} OpponentPokepicShrinkWork;

typedef struct OpponentTrainerEncounterWork {
    BattleSystem *battleSystem;
    Pokepic *pokepic;
    BattleTerrain *terrain;
    ManagedSprite *managedSprite;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 facing;
    s16 targetX;
    u16 ballFlashStarted : 1;
    u16 unk_16_1 : 15;
    int battlerType;
    int delay;
    int bgScrollCount;
} OpponentTrainerEncounterWork;

typedef struct OpponentThrowPokeballWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 facing;
    int backpic;
    int throwType;
    int delay;
    int ballTargetState;
} OpponentThrowPokeballWork;

typedef struct OpponentTrainerSlideOutWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 facing;
    u8 filler_0C[4];
} OpponentTrainerSlideOutWork;

typedef struct OpponentTrainerSlideInWork {
    BattleSystem *battleSystem;
    ManagedSprite *managedSprite;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 facing;
    s16 targetX;
} OpponentTrainerSlideInWork;

typedef struct OpponentSelectCommandWork {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    u8 command;
    u8 battlerId;
    u8 state;
    s8 unk_0B;
    u32 input;
    u8 ballStatus[2][PARTY_SIZE];
    u8 expPercents[PARTY_SIZE];
    u8 unk_22;
    u8 partySlot;
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 movePP[MAX_MON_MOVES];
    u8 battlerType;
    u8 printerId;
    s16 hp;
    u16 maxHp;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
} OpponentSelectCommandWork;

typedef struct OpponentSelectMoveWork {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    u32 input;
    u16 moves[MAX_MON_MOVES];
    u8 movePPCur[MAX_MON_MOVES];
    u8 movePP[MAX_MON_MOVES];
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 selectedMonIndex;
    u8 state;
    u8 filler_21;
    u16 unusableMovesMask;
} OpponentSelectMoveWork;

typedef struct OpponentSelectTargetWork {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    int input;
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    TargetPokemon targetMons[BATTLER_MAX];
    u16 moveRange;
    u8 unk_32;
} OpponentSelectTargetWork;

typedef struct OpponentSelectItemPartyMenu {
    u32 unk_00;
    BattlePartyMenuArgs *args;
    u8 unk_08[4];
    u8 selectedMonIndex[BATTLER_MAX];
    u8 unk_10[0x24];
} OpponentSelectItemPartyMenu;

typedef struct OpponentSelectItemWork {
    BattleSystem *battleSystem;
    BattleBagMenuArgs *bagMenu;
    OpponentSelectItemPartyMenu *partyMenu;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 battlerType;
    u8 key;
    u8 printerId;
    u16 nextState;
    u8 unk_14;
    u8 targetSemiInvulnerable;
    u8 targetSubstitute;
    u8 timer;
    u8 partyOrder[BATTLER_MAX][PARTY_SIZE];
    u8 embargoFlag[BATTLER_MAX];
} OpponentSelectItemWork;

typedef struct OpponentShowMonListWork {
    BattleSystem *battleSystem;
    BattlePartyMenuArgs *partyMenu;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 mode;
    u8 selectedMonIndex[BATTLER_MAX];
    u32 trapped;
    u16 item;
    u8 reservedMon;
    u8 key;
    u8 switchInFlag;
    u8 filler_19[3];
    u8 partyOrder[BATTLER_MAX][PARTY_SIZE];
} OpponentShowMonListWork;

typedef struct OpponentYesNoWork {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    int input;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 mode;
    int msgId;
    int param;
    u16 value;
    u16 printerId;
} OpponentYesNoWork;

typedef struct OpponentYesNoMenu {
    BattleInputTwoOptionMenu twoOption;
    BattleMessage msg;
} OpponentYesNoMenu;

typedef struct OpponentMessageWork {
    BattleSystem *battleSystem;
    u8 command;
    u8 battlerId;
    u8 printerId;
    u8 unused;
} OpponentMessageWork;

typedef struct OpponentSelectionMessageWork {
    BattleSystem *battleSystem;
    u8 command;
    u8 battlerId;
    u8 printerId;
    u8 state;
    u8 timer;
} OpponentSelectionMessageWork;

typedef struct OpponentMoveAnimationWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    BattleAnimSystem *animSystem;
    MoveAnimData animData;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 disableHpBar;
    u8 updateG3;
} OpponentMoveAnimationWork;

typedef struct OpponentMonFlickerWork {
    BattleSystem *battleSystem;
    Pokepic *pokepic;
    u8 battlerId;
    u8 count;
    u8 delay;
} OpponentMonFlickerWork;

typedef struct OpponentFaintWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    MoveAnimData animData;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 facing;
    u16 species;
    u8 gender;
    u8 form;
    u32 personality;
    u16 isSubstitute;
    u16 isTransformed;
} OpponentFaintWork;

typedef struct OpponentFadeOutWork {
    BattleSystem *battleSystem;
    u8 command;
    u8 battlerId;
    u8 state;
} OpponentFadeOutWork;

typedef struct OpponentToggleVanishWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    MoveAnimData animData;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 vanish;
    u32 isSubstitute;
} OpponentToggleVanishWork;

typedef struct OpponentExpBarFlashWork {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 done;
} OpponentExpBarFlashWork;

typedef struct OpponentSelectMoveToForgetWork {
    BattleSystem *battleSystem;
    BattlePartyMenuArgs *partyMenu;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 filler_0B;
    u16 move;
    u8 slot;
} OpponentSelectMoveToForgetWork;

typedef struct OpponentMosaicWork {
    BattleSystem *battleSystem;
    Pokepic *pokepic;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 mosaic;
    u8 counter;
    u8 delay;
} OpponentMosaicWork;

typedef struct OpponentPartyGaugeWork {
    BattleSystem *battleSystem;
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    u8 status[PARTY_SIZE];
    u8 midBattle;
} OpponentPartyGaugeWork;

typedef struct OpponentSpriteOamWork {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    u8 command;
    u8 battlerId;
    u8 state;
} OpponentSpriteOamWork;

typedef struct OpponentScreenFadeWork {
    BattleSystem *battleSystem;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 mode;
    u8 counter;
} OpponentScreenFadeWork;

// Functions of this module, in link order.
int TrainerAI_GetSwitchedPartySlot(BattleSystem *battleSystem, int battler);
void BattleControllerOpponent_Nop(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PlayEncounterAnimation(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PokemonEncounter(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PokemonSlideIn(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PokemonSendOut(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_RecallPokemon(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PokemonEnterBall(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_DeletePokemon(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_TrainerEncounter(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ThrowPokeball(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_TrainerSlideOut(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_TrainerSlideIn(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_HealthbarSlideIn(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_HealthbarSlideOut(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SelectCommand(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SelectMove(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SelectTarget(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SelectItem(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ShowMonList(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_DrawYesNoBox(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintAttackMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PlayMoveAnimation(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_MonFlicker(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_HealthbarUpdate(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ExpBarUpdate(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PlayFaintAnimation(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PlaySE(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_FadeOutBattle(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ToggleVanish(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_HealthbarStatus(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintTrainerMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintReturnMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintEncounterMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintFirstSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ExpBarFullFlash(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintSelectionMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_RefreshHealthbar(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_BattleMonToPartyMonCopy(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_BackgroundSlideIn(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_CancelSelection(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PartyStatusHeal(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SelectMoveToForget(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PlayMosaicAnimation(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ChangeForm(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SetBattleBackground(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_EndSelection(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_InitStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_DeleteStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_InitBallGauge(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_DeleteBallGauge(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_LoadBallGfx(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_DeleteBallGfx(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_IncrementGameStat(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ShowWaitMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_RestoreSprite(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SpriteToOAM(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_OAMToSprite(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PrintResultMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_RunAwayMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_ForfeitMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SwapToSubstituteSprite(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PlayMoveSE(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_PlaySong(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_SetBattleResults(BattleSystem *battleSystem, OpponentData *opponentData);
void BattleControllerOpponent_BlankMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_ClearCommand(OpponentData *opponentData);
void BattleControllerOpponent_ScreenFade(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_PlayEncounterAnimation(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_PokemonEncounter(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonEncounter *packet);
void OpponentData_PokemonSlideIn(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonSendOut *packet);
void OpponentData_PokemonSendOut(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonSendOut *packet);
void OpponentData_RecallPokemon(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRecallPokemon *packet);
void OpponentData_PokemonEnterBall(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonEnterBall *packet);
void OpponentData_TrainerEncounter(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketTrainerEncounter *packet);
void OpponentData_ThrowPokeball(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketThrowPokeball *packet);
void OpponentData_TrainerSlideOut(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_TrainerSlideIn(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketTrainerSlideIn *packet);
void OpponentData_HealthbarSlideIn(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketHealthbarSlideIn *packet);
void OpponentData_HealthbarSlideOut(BattleSystem *battleSystem, OpponentData *opponentData);
SysTask *OpponentData_SelectCommand(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectCommand *packet);
void OpponentData_SelectMove(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectMove *packet);
void OpponentData_SelectTarget(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectTarget *packet);
SysTask *OpponentData_SelectItem(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectItem *packet);
void OpponentData_ShowMonList(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketShowMonList *packet);
void OpponentData_DrawYesNoBox(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketYesNoBox *packet);
void OpponentData_PrintAttackMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketAttackMessage *packet);
void OpponentData_PrintMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg);
void OpponentData_PlayMoveAnimation(BattleSystem *battleSystem, OpponentData *opponentData, MoveAnimData *animData);
void OpponentData_MonFlicker(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_HealthbarUpdate(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketHealthbarUpdate *packet);
void OpponentData_ExpBarUpdate(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketExpBarUpdate *packet);
void OpponentData_PlayFaintAnimation(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketFaintAnimation *packet);
void OpponentData_PlaySE(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPlaySE *packet);
void OpponentData_FadeOutBattle(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_ToggleVanish(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketToggleVanish *packet);
void OpponentData_HealthbarStatus(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketHealthbarStatus *packet);
void OpponentData_PrintTrainerMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketTrainerMessage *packet);
void OpponentData_PrintReturnMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet);
void OpponentData_PrintSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet);
void OpponentData_PrintEncounterMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_PrintFirstSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketFirstSendOutMessage *packet);
void OpponentData_ExpBarFullFlash(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_PrintSelectionMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectionMessage *packet);
void OpponentData_RefreshHealthbar(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRefreshHealthbar *packet);
void OpponentData_SelectMoveToForget(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectMoveToForget *packet);
void OpponentData_PlayMosaicAnimation(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketMosaicAnimation *packet);
void OpponentData_InitStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet);
void OpponentData_DeleteStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet);
void OpponentData_InitBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet);
void OpponentData_DeleteBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet);
void OpponentData_ShowWaitMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_RestoreSprite(BattleSystem *battleSystem, OpponentData *opponentData, MoveAnimData *animData);
void OpponentData_SpriteToOAM(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_OAMToSprite(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_PrintResultMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_RunAwayMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRecordData *packet);
void OpponentData_ForfeitMessage(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_SwapToSubstituteSprite(BattleSystem *battleSystem, OpponentData *opponentData, MoveAnimData *animData);
void OpponentData_PlayMoveSE(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketMoveSE *packet);
void OpponentData_PlaySong(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPlaySong *packet);
void OpponentData_ScreenFade(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketScreenFade *packet);
void Task_PokemonEncounter(SysTask *task, void *data);
void Task_GiratinaEncounter(SysTask *task, void *data);
void Task_PokemonSlideIn(SysTask *task, void *data);
void Task_FollowMonSlideIn(SysTask *task, void *data);
void Task_PokemonSendOut(SysTask *task, void *data);
void Task_FollowMonSendOut(SysTask *task, void *data);
void Task_RecallPokemon(SysTask *task, void *data);
void Task_RecallFollowMon(SysTask *task, void *data);
void Task_PokemonEnterBall(SysTask *task, void *data);
void Task_TrainerEncounter(SysTask *task, void *data);
void Task_ThrowPokeball(SysTask *task, void *data);
void Task_ThrowPokeballWithFollowMon(SysTask *task, void *data);
void Task_TrainerSlideOut(SysTask *task, void *data);
void Task_TrainerSlideIn(SysTask *task, void *data);
void Task_HealthbarSlideIn(SysTask *task, void *data);
void Task_HealthbarSlideOut(SysTask *task, void *data);
void Task_PlayerSelectCommand(SysTask *task, void *data);
void Task_AISelectCommand(SysTask *task, void *data);
void Task_LinkSelectCommand(SysTask *task, void *data);
void Task_SafariSelectCommand(SysTask *task, void *data);
void Task_PalParkSelectCommand(SysTask *task, void *data);
void Task_RecordedSelectCommand(SysTask *task, void *data);
void Task_PlayerSelectMove(SysTask *task, void *data);
void Task_AISelectMove(SysTask *task, void *data);
void Task_LinkSelectMove(SysTask *task, void *data);
void Task_RecordedSelectMove(SysTask *task, void *data);
void Task_PlayerSelectTarget(SysTask *task, void *data);
void Task_AISelectTarget(SysTask *task, void *data);
void Task_LinkSelectTarget(SysTask *task, void *data);
void Task_RecordedSelectTarget(SysTask *task, void *data);
void Task_PlayerSelectItem(SysTask *task, void *data);
void Task_AISelectItem(SysTask *task, void *data);
void Task_LinkSelectItem(SysTask *task, void *data);
void Task_RecordedSelectItem(SysTask *task, void *data);
void Task_PlayerShowMonList(SysTask *task, void *data);
void Task_AIShowMonList(SysTask *task, void *data);
void Task_LinkShowMonList(SysTask *task, void *data);
void Task_RecordedShowMonList(SysTask *task, void *data);
void Task_PlayerDrawYesNoBox(SysTask *task, void *data);
void Task_AIDrawYesNoBox(SysTask *task, void *data);
void Task_LinkDrawYesNoBox(SysTask *task, void *data);
void Task_RecordedDrawYesNoBox(SysTask *task, void *data);
void Task_PlayMoveAnimation(SysTask *task, void *data);
void Task_MonFlicker(SysTask *task, void *data);
void Task_HealthbarUpdate(SysTask *task, void *data);
void Task_ExpBarUpdate(SysTask *task, void *data);
void Task_PlayFaintAnimation(SysTask *task, void *data);
void Task_FadeOutBattle(SysTask *task, void *data);
void Task_ToggleVanish(SysTask *task, void *data);
void Task_PlayExpBarFullFlash(SysTask *task, void *data);
void Task_WaitBattleMessage(SysTask *task, void *data);
void Task_PrintSelectionMessage(SysTask *task, void *data);
void Task_PlayEncounterAnimation(SysTask *task, void *data);
void Task_SelectMoveToForget(SysTask *task, void *data);
void Task_PlayMosaicAnimation(SysTask *task, void *data);
void Task_ShowPartyGauge(SysTask *task, void *data);
void Task_HidePartyGauge(SysTask *task, void *data);
void Task_SpriteToOAM(SysTask *task, void *data);
void Task_OAMToSprite(SysTask *task, void *data);
void Task_OpponentScreenFade(SysTask *task, void *data);
void OpponentData_ResetCursorPosition(OpponentData *opponentData);
Pokepic *BattleControllerOpponent_CreatePokepic(BattleSystem *battleSystem, PokepicManager *pokepicManager, PokepicTemplate *template, int x, int y, int z, int height, int shadowHeight, int shadowXOffset, int shadowSize, int battlerId, PokepicAnimScript *animScript, PokepicCallback callback);
void BattleControllerOpponent_GetReturnMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet, BattleMessage *msg);
void BattleControllerOpponent_GetSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet, BattleMessage *msg);
void BattleControllerOpponent_GetEncounterMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg);
void BattleControllerOpponent_GetFirstSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketFirstSendOutMessage *packet, BattleMessage *msg);
void BattleControllerOpponent_GetResultMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg);
void BattleControllerOpponent_GetRunAwayMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRecordData *packet, BattleMessage *msg);
void BattleControllerOpponent_GetForfeitMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg);
ManagedSprite *BattleControllerOpponent_CreateTrainerSprite(BattleSystem *battleSystem, int position, int trainerClass, int resTag, BOOL isLink, s16 x, s16 y);
void OpponentData_StartMoveAnimation(BattleSystem *battleSystem, OpponentData *opponentData, BattleAnimSystem *animSystem, MoveAnimData *animData);
void BattleController_InitBattlerPicParams(BattleSystem *battleSystem, MoveAnimData *animData, BattleAnimBattlerPicParams *params, u32 battlerId);
void MoveAnim_GetDisplayFlags(u8 *disableHpBar, u8 *updateG3, BOOL isEffect, int effect, u16 move);
BOOL OpponentTrainerEncounter_HandleAnimFrame(OpponentTrainerEncounterWork *work, ManagedSprite *unused);
void Task_FlashBrightness(SysTask *task, s32 *state);
void BattleSystem_StopRecordingDataCorrupted(BattleSystem *battleSystem);
void BattleSystem_StopRecordingTooLong(BattleSystem *battleSystem);
u8 BattleControllerOpponent_GetVersionTrainerClass(BattleSystem *battleSystem, int battlerId, u8 trainerClass);

int TrainerAI_PostKOSwitchIn(BattleSystem *battleSystem, int battler) {
    // Must keep C89-style declaration to match
    int i, j;
    u8 opponentType1, opponentType2;
    u8 monType1, monType2;
    u16 monSpecies;
    u16 move;
    int moveType;
    u8 battlersDisregarded;
    u8 score, maxScore; // BUG: Post-KO Switch-In AI Scoring Overflow (see docs/bugs_and_glitches.md)
    u8 picked = 6;
    u32 moveStatusFlags;
    Pokemon *mon;
    BattleContext *ctx = BattleSystem_GetBattleContext(battleSystem);

    u8 slot1, slot2;
    slot1 = battler;
    if ((BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_TAG)
        || (BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_MULTI)) {
        slot2 = slot1;
    } else {
        slot2 = BattleSystem_GetBattlerIdPartner(battleSystem, battler);
    }

    u8 opponent = Battler_GetRandomOpposingBattlerId(battleSystem, ctx, battler);
    int partySize = BattleSystem_GetPartySize(battleSystem, battler);
    battlersDisregarded = 0;

    // Stage 1: Loop through all the party slots and find the one with the most favorable
    // offensive type-matchup against the chosen opponent which also has a super-effective
    // move against that opponent. Choose the Pokemon with the highest such score, breaking
    // ties by party-order. If no such Pokemon exists, proceed to Stage 2.
    //
    // Mono-type Pokemon are regarded as being dual-type of the same type.
    while (battlersDisregarded != 0x3F) {
        maxScore = 0;
        picked = 6;

        for (i = 0; i < partySize; i++) {
            mon = BattleSystem_GetPartyMon(battleSystem, battler, i);
            monSpecies = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);

            if (monSpecies != SPECIES_NONE
                && monSpecies != SPECIES_EGG
                && GetMonData(mon, MON_DATA_HP, NULL)
                && (battlersDisregarded & MaskOfFlagNo(i)) == FALSE
                && ctx->selectedMonIndex[slot1] != i
                && ctx->selectedMonIndex[slot2] != i
                && i != ctx->aiSwitchedPartySlot[slot1]
                && i != ctx->aiSwitchedPartySlot[slot2]) {
                opponentType1 = GetBattlerVar(ctx, opponent, BMON_DATA_TYPE_1, NULL);
                opponentType2 = GetBattlerVar(ctx, opponent, BMON_DATA_TYPE_2, NULL);
                monType1 = GetMonData(mon, MON_DATA_TYPE_1, NULL);
                monType2 = GetMonData(mon, MON_DATA_TYPE_2, NULL);

                score = CalculateTypeEffectiveness(monType1, opponentType1, opponentType2);
                score += CalculateTypeEffectiveness(monType2, opponentType1, opponentType2);

                if (maxScore < score) {
                    maxScore = score;
                    picked = i;
                }
            } else {
                battlersDisregarded |= MaskOfFlagNo(i);
            }
        }

        if (picked != 6) {
            // Determine if this mon has any super-effective moves against the opponent
            mon = BattleSystem_GetPartyMon(battleSystem, battler, picked);

            for (i = 0; i < MAX_MON_MOVES; i++) {
                move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
                moveType = TrainerAI_GetMoveType(battleSystem, ctx, mon, move);

                if (move) {
                    moveStatusFlags = 0;
                    CalcTypeEffectivenessFlags(ctx,
                        move,
                        moveType,
                        GetMonData(mon, MON_DATA_ABILITY, NULL),
                        GetBattlerAbility(ctx, opponent),
                        GetBattlerHeldItemEffect(ctx, opponent),
                        GetBattlerVar(ctx, opponent, BMON_DATA_TYPE_1, NULL),
                        GetBattlerVar(ctx, opponent, BMON_DATA_TYPE_2, NULL),
                        &moveStatusFlags);

                    if (moveStatusFlags & MOVE_STATUS_SUPER_EFFECTIVE) {
                        break;
                    }
                }
            }

            // If this mon has no moves which would be super-effective against the
            // opponent, mark it as disregarded and move to the next in priority.
            if (i == MAX_MON_MOVES) {
                battlersDisregarded |= MaskOfFlagNo(picked);
            } else {
                return picked;
            }
        } else {
            // No valid battlers to further-evaluate, break out
            battlersDisregarded = 0x3F;
        }
    }

    maxScore = 0;
    picked = 6;

    // Stage 2: Loop through all the party slots and score them by how much damage would be done
    // by the maximum non-critical roll of each of their moves if it were used by the battler
    // which just fainted. Choose the Pokemon with the highest such score, breaking ties by
    // party-order.
    for (i = 0; i < partySize; i++) {
        mon = BattleSystem_GetPartyMon(battleSystem, battler, i);
        monSpecies = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);

        if (monSpecies != SPECIES_NONE
            && monSpecies != SPECIES_EGG
            && GetMonData(mon, MON_DATA_HP, NULL)
            && ctx->selectedMonIndex[slot1] != i
            && ctx->selectedMonIndex[slot2] != i
            && i != ctx->aiSwitchedPartySlot[slot1]
            && i != ctx->aiSwitchedPartySlot[slot2]) {
            for (j = 0; j < MAX_MON_MOVES; j++) {
                move = GetMonData(mon, MON_DATA_MOVE1 + j, NULL);
                moveType = TrainerAI_GetMoveType(battleSystem, ctx, mon, move);

                if (move && ctx->trainerAIData.moveData[move].power != 1) {
                    score = CalcMoveDamage(battleSystem,
                        ctx,
                        move,
                        ctx->fieldSideConditionFlags[BattleSystem_GetFieldSide(battleSystem, opponent)],
                        ctx->fieldCondition,
                        0,
                        0,
                        battler,
                        opponent,
                        1);

                    moveStatusFlags = 0;
                    score = BattleSystem_ApplyTypeEffectiveness(battleSystem,
                        ctx,
                        move,
                        moveType,
                        battler,
                        opponent,
                        score,
                        &moveStatusFlags);

                    if (moveStatusFlags & MOVE_STATUS_IMMUNE) {
                        score = 0;
                    }
                }

                if (maxScore < score) {
                    maxScore = score;
                    picked = i;
                }
            }
        }
    }

    return picked;
}

int TrainerAI_GetSwitchedPartySlot(BattleSystem *battleSystem, int battler) {
    BattleContext *ctx = BattleSystem_GetBattleContext(battleSystem);
    return ctx->aiSwitchedPartySlot[battler];
}

int TrainerAI_GetMoveType(BattleSystem *battleSystem, BattleContext *ctx, Pokemon *mon, int move) {
    int type;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        type = GetItemVar(ctx, GetMonData(mon, MON_DATA_HELD_ITEM, NULL), ITEMATTR_NATURAL_GIFT_TYPE);
        break;

    case MOVE_JUDGMENT:
        switch (GetItemVar(ctx, GetMonData(mon, MON_DATA_HELD_ITEM, NULL), ITEMATTR_HOLD_EFFECT)) {
        case HOLD_EFFECT_ARCEUS_FIGHTING:
            type = TYPE_FIGHTING;
            break;
        case HOLD_EFFECT_ARCEUS_FLYING:
            type = TYPE_FLYING;
            break;
        case HOLD_EFFECT_ARCEUS_POISON:
            type = TYPE_POISON;
            break;
        case HOLD_EFFECT_ARCEUS_GROUND:
            type = TYPE_GROUND;
            break;
        case HOLD_EFFECT_ARCEUS_ROCK:
            type = TYPE_ROCK;
            break;
        case HOLD_EFFECT_ARCEUS_BUG:
            type = TYPE_BUG;
            break;
        case HOLD_EFFECT_ARCEUS_GHOST:
            type = TYPE_GHOST;
            break;
        case HOLD_EFFECT_ARCEUS_STEEL:
            type = TYPE_STEEL;
            break;
        case HOLD_EFFECT_ARCEUS_FIRE:
            type = TYPE_FIRE;
            break;
        case HOLD_EFFECT_ARCEUS_WATER:
            type = TYPE_WATER;
            break;
        case HOLD_EFFECT_ARCEUS_GRASS:
            type = TYPE_GRASS;
            break;
        case HOLD_EFFECT_ARCEUS_ELECTRIC:
            type = TYPE_ELECTRIC;
            break;
        case HOLD_EFFECT_ARCEUS_PSYCHIC:
            type = TYPE_PSYCHIC;
            break;
        case HOLD_EFFECT_ARCEUS_ICE:
            type = TYPE_ICE;
            break;
        case HOLD_EFFECT_ARCEUS_DRAGON:
            type = TYPE_DRAGON;
            break;
        case HOLD_EFFECT_ARCEUS_DARK:
            type = TYPE_DARK;
            break;
        default:
            type = TYPE_NORMAL;
            break;
        }
        break;

    case MOVE_HIDDEN_POWER:
        type = ((GetMonData(mon, MON_DATA_HP_IV, NULL) & 1) >> 0)
            | ((GetMonData(mon, MON_DATA_ATK_IV, NULL) & 1) << 1)
            | ((GetMonData(mon, MON_DATA_DEF_IV, NULL) & 1) << 2)
            | ((GetMonData(mon, MON_DATA_SPEED_IV, NULL) & 1) << 3)
            | ((GetMonData(mon, MON_DATA_SPATK_IV, NULL) & 1) << 4)
            | ((GetMonData(mon, MON_DATA_SPDEF_IV, NULL) & 1) << 5);
        type = (type * 15 / 63) + 1;

        if (type >= TYPE_MYSTERY) {
            type++;
        }
        break;

    case MOVE_WEATHER_BALL:
        if (CheckAbilityActive(battleSystem, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_CLOUD_NINE) == FALSE
            && CheckAbilityActive(battleSystem, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_AIR_LOCK) == FALSE
            && (ctx->fieldCondition & FIELD_CONDITION_WEATHER)) {
            if (ctx->fieldCondition & FIELD_CONDITION_RAIN_ALL) {
                type = TYPE_WATER;
            }

            if (ctx->fieldCondition & FIELD_CONDITION_SANDSTORM_ALL) {
                type = TYPE_ROCK;
            }

            if (ctx->fieldCondition & FIELD_CONDITION_SUN_ALL) {
                type = TYPE_FIRE;
            }

            if (ctx->fieldCondition & FIELD_CONDITION_HAIL_ALL) {
                type = TYPE_ICE;
            }
        }
        break;

    default:
        type = TYPE_NORMAL;
        break;
    }

    return type;
}

OpponentData *OpponentData_New(BattleSystem *battleSystem, const u8 *data) {
    OpponentData *opponentData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentData));
    MI_CpuClearFast(opponentData, sizeof(OpponentData));
    opponentData->battlerId = data[0];
    opponentData->battlerType = data[1];
    opponentData->pokepicAnimNarc = (u32 *)NARC_New(NARC_a_1_8_0, HEAP_ID_BATTLE);
    return opponentData;
}

#pragma force_active on

void OpponentData_CreateBallData(BattleSystem *battleSystem, OpponentData *opponentData, u16 ball, int battlerId) {
    BallThrowParams ballParams;

    if ((BattleSystem_GetBattleType(battleSystem) & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_PAL_PARK)) == 0) {
        if ((opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) == 0 || (BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_TRAINER)) {
            ballParams.kind = ov12_0226D120[opponentData->battlerType];
            ballParams.heapID = HEAP_ID_BATTLE;
            ballParams.mode = 4;
            ballParams.resTagOffset = opponentData->battlerId;
            ballParams.ball = ball;
            ballParams.spriteSystem = BattleSystem_GetSpriteSystem(battleSystem);
            ballParams.paletteData = BattleSystem_GetPaletteData(battleSystem);
            ballParams.keepSubSurface = 0;
            ballParams.bgPriority = 1;
            opponentData->ballData = BallThrow_New(&ballParams);
            BallThrow_SetDrawPriority(opponentData->ballData, 100);
            BallThrow_SetPriority(opponentData->ballData, 2);
            BallThrow_SetDrawFlag(opponentData->ballData, 0);
            BallThrow_SetAnimateSprite(opponentData->ballData, 0);
        }
    }
}

#pragma force_active reset

void OpponentData_RunCommand(BattleSystem *battleSystem, OpponentData *opponentData) {
    if (OPPONENT_COMMAND_ID(opponentData) != 0) {
        opponentData->commandReceived = 0;
        sOpponentCommandHandlers[OPPONENT_COMMAND_ID(opponentData)](battleSystem, opponentData);
    }
}

void OpponentData_Free(BattleSystem *battleSystem, OpponentData *opponentData, int mode) {
    if (mode != 2) {
        BattleHpBar_FreeResources(&opponentData->hpBar);
    }
    if (opponentData->managedSprite != NULL) {
        Sprite_DeleteAndFreeResources(opponentData->managedSprite);
    }
    OpponentData_StopPokepicBobTask(opponentData);
    NARC_Delete((NARC *)opponentData->pokepicAnimNarc);
    Heap_Free(opponentData);
}

void BattleControllerOpponent_Nop(BattleSystem *battleSystem, OpponentData *opponentData) {
}

void BattleControllerOpponent_PlayEncounterAnimation(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleSystem_SetRandTemp(battleSystem, ((BattleControllerPacketEncounterAnimation *)OPPONENT_COMMAND(opponentData))->randTemp);
    OpponentData_PlayEncounterAnimation(battleSystem, opponentData);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 1);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PokemonEncounter(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PokemonEncounter(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PokemonSlideIn(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PokemonSlideIn(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PokemonSendOut(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PokemonSendOut(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_RecallPokemon(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_RecallPokemon(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PokemonEnterBall(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PokemonEnterBall(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_DeletePokemon(BattleSystem *battleSystem, OpponentData *opponentData) {
    Pokepic_Delete(opponentData->pokepic);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 7);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_TrainerEncounter(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_TrainerEncounter(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ThrowPokeball(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_ThrowPokeball(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_TrainerSlideOut(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_TrainerSlideOut(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_TrainerSlideIn(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_TrainerSlideIn(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_HealthbarSlideIn(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_HealthbarSlideIn(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_HealthbarSlideOut(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_HealthbarSlideOut(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SelectCommand(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketSelectCommand *packet = OPPONENT_COMMAND(opponentData);
    BattleSystem_SetSwitchingOrCanPickCommandMask(battleSystem, packet->switchingOrCanPickCommandMask);
    OpponentData_SelectCommand(battleSystem, opponentData, packet);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SelectMove(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_SelectMove(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SelectTarget(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_SelectTarget(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SelectItem(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_SelectItem(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ShowMonList(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_ShowMonList(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_DrawYesNoBox(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_DrawYesNoBox(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintAttackMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintAttackMessage(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintMessage(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PlayMoveAnimation(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PlayMoveAnimation(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_MonFlicker(BattleSystem *battleSystem, OpponentData *opponentData) {
    if (Pokepic_GetAttr(opponentData->pokepic, POKEPIC_VANISHED) == TRUE) {
        BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 23);
    } else {
        OpponentData_MonFlicker(battleSystem, opponentData);
    }
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_HealthbarUpdate(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_HealthbarUpdate(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ExpBarUpdate(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_ExpBarUpdate(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PlayFaintAnimation(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PlayFaintAnimation(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PlaySE(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PlaySE(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_FadeOutBattle(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_FadeOutBattle(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ToggleVanish(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_ToggleVanish(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_HealthbarStatus(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_HealthbarStatus(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintTrainerMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintTrainerMessage(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintReturnMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintReturnMessage(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintSendOutMessage(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintEncounterMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintEncounterMessage(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintFirstSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintFirstSendOutMessage(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ExpBarFullFlash(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_ExpBarFullFlash(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintSelectionMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintSelectionMessage(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_RefreshHealthbar(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_RefreshHealthbar(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

#pragma force_active on

void BattleControllerOpponent_BattleMonToPartyMonCopy(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketBattleMonToPartyMonCopy *packet = OPPONENT_COMMAND(opponentData);
    Pokemon *mon = BattleSystem_GetPartyMon(battleSystem, opponentData->battlerId, packet->selectedMonIndex);
    int i;

    if (!(packet->status2 & STATUS2_TRANSFORM)) {
        for (i = 0; i < MAX_MON_MOVES; i++) {
            if (!(MaskOfFlagNo(i) & packet->mimicedMoveIndex)) {
                SetMonData(mon, MON_DATA_MOVE1 + i, &packet->moves[i]);
                SetMonData(mon, MON_DATA_MOVE1_PP + i, &packet->movePPCur[i]);
            }
        }
    }
    if (!(MaskOfFlagNo(packet->selectedMonIndex) & packet->battlerBitKnockedOffItem)) {
        SetMonData(mon, MON_DATA_HELD_ITEM, &packet->item);
    }
    SetMonData(mon, MON_DATA_HP, &packet->hp);
    SetMonData(mon, MON_DATA_STATUS, &packet->status);
    if (packet->formChange) {
        SetMonData(mon, MON_DATA_FORM, &packet->form);
    }
    if (packet->recalcMonStats) {
        SetMonData(mon, MON_DATA_ABILITY, &packet->ability);
        CalcMonLevelAndStats(mon);
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    OpponentData_ClearCommand(opponentData);
}

#pragma force_active reset

void BattleControllerOpponent_BackgroundSlideIn(BattleSystem *battleSystem, OpponentData *opponentData) {
    u32 battleType = BattleSystem_GetBattleType(battleSystem);
    BattleInput *battleInput = BattleSystem_GetBattleInput(battleSystem);
    u32 multi;

    if (opponentData->inputMode == 0) {
        multi = battleType & BATTLE_TYPE_MULTI;
        if (multi == 0) {
            // never true; needed to match
            if (multi != 0) {
                goto done;
            }
            if (opponentData->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                goto done;
            }
        }
        BattleInput_StartMenuScrollHorizontalTask(battleInput, -0xD00, 0);
    }
done:
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, OPPONENT_COMMAND_ID(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_CancelSelection(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleHpBar_StopActiveEffect(&opponentData->hpBar);
    OpponentData_StopPokepicBobTask(opponentData);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, OPPONENT_COMMAND_ID(opponentData));
    OpponentData_ClearCommand(opponentData);
}

#pragma force_active on

void BattleControllerOpponent_PartyStatusHeal(BattleSystem *battleSystem, OpponentData *opponentData) {
    int status = 0;
    BattleControllerPacketPartyStatusHeal *packet = OPPONENT_COMMAND(opponentData);
    int partySize = BattleSystem_GetPartySize(battleSystem, opponentData->battlerId);
    int i;

    for (i = 0; i < partySize; i++) {
        Pokemon *mon = BattleSystem_GetPartyMon(battleSystem, opponentData->battlerId, i);
        int ability;
        if (packet->ability == ABILITY_MOLD_BREAKER) {
            ability = ABILITY_NONE;
        } else {
            ability = GetMonData(mon, MON_DATA_ABILITY, NULL);
        }
        if (packet->moveNo != MOVE_HEAL_BELL || (packet->moveNo == MOVE_HEAL_BELL && ability != ABILITY_SOUNDPROOF)) {
            SetMonData(mon, MON_DATA_STATUS, &status);
        }
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    OpponentData_ClearCommand(opponentData);
}

#pragma force_active reset

void BattleControllerOpponent_SelectMoveToForget(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_SelectMoveToForget(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PlayMosaicAnimation(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PlayMosaicAnimation(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ChangeForm(BattleSystem *battleSystem, OpponentData *opponentData) {
    PokepicTemplate template;
    BattleControllerPacketChangeForm *packet = OPPONENT_COMMAND(opponentData);
    int facing;
    PokepicTemplate *pokepicTemplate;
    u8 *buffer;
    int height;

    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        facing = 2;
    } else {
        facing = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&template, packet->species, packet->gender, facing, packet->shiny, packet->form, packet->personality);
    pokepicTemplate = Pokepic_GetTemplate(opponentData->pokepic);
    *pokepicTemplate = template;
    Pokepic_ScheduleReloadFromNarc(opponentData->pokepic);

    buffer = MonPicData_GetCharBuffer(BattleSystem_GetMonPicDataList(battleSystem), opponentData->battlerId);
    CharTiles_LoadMonPicAsObjFromNarc((NarcId)pokepicTemplate->narcID, pokepicTemplate->charDataID, HEAP_ID_BATTLE, buffer, packet->personality, FALSE, facing, pokepicTemplate->species);
    MonPicData_SetNarcId(BattleSystem_GetMonPicDataList(battleSystem), opponentData->battlerId, pokepicTemplate->narcID);
    MonPicData_SetPaletteId(BattleSystem_GetMonPicDataList(battleSystem), opponentData->battlerId, pokepicTemplate->palDataID);

    height = GetMonPicHeightBySpeciesGenderForm(packet->species, packet->gender, facing, packet->form, packet->personality);
    MonPicData_SetHeight(BattleSystem_GetMonPicDataList(battleSystem), opponentData->battlerId, height);
    Pokepic_SetAttr(opponentData->pokepic, POKEPIC_Y, height + ov07_02234B5C(opponentData->battlerType, 1));
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SetBattleBackground(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleSystem_SetBackground(battleSystem);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 46);
    OpponentData_ClearCommand(opponentData);
}

#pragma force_active on

void BattleControllerOpponent_EndSelection(BattleSystem *battleSystem, OpponentData *opponentData) {
    NARC *narc0;
    NARC *narc1;
    BattleInput *battleInput;
    int partner;

    if (opponentData->inputMode == 0) {
        narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
        narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
        battleInput = BattleSystem_GetBattleInput(battleSystem);
        BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
        BattleInput_SetMenuOpenDummy(battleInput, 0);
        NARC_Delete(narc0);
        NARC_Delete(narc1);
        partner = BattleSystem_GetBattlerIdPartner(battleSystem, opponentData->battlerId);
        if (partner != opponentData->battlerId) {
            BattleHpBar_MenuCloseDummy(BattleSystem_GetHpBar(battleSystem, partner));
        }
        BattleHpBar_StopActiveEffect(&opponentData->hpBar);
        BattleInput_DisableBallGauge(battleInput);
        OpponentData_StopPokepicBobTask(opponentData);
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 47);
    OpponentData_ClearCommand(opponentData);
}

#pragma force_active reset

void BattleControllerOpponent_InitStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_InitStartBallGauge(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_DeleteStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_DeleteStartBallGauge(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_InitBallGauge(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketBallGauge *packet = OPPONENT_COMMAND(opponentData);
    if (BattleSystem_GetFieldSide(battleSystem, opponentData->battlerId) != 0) {
        OpponentData_InitBallGauge(battleSystem, opponentData, packet);
    } else {
        BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 50);
    }
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_DeleteBallGauge(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketBallGauge *packet = OPPONENT_COMMAND(opponentData);
    if (BattleSystem_GetFieldSide(battleSystem, opponentData->battlerId) != 0) {
        OpponentData_DeleteBallGauge(battleSystem, opponentData, packet);
    } else {
        BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 51);
    }
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_LoadBallGfx(BattleSystem *battleSystem, OpponentData *opponentData) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleSystem);
    SpriteManager *spriteManager = BattleSystem_GetSpriteManager(battleSystem);
    PaletteData *paletteData = BattleSystem_GetPaletteData(battleSystem);
    PartyGauge_LoadGraphics(spriteSystem, spriteManager, paletteData);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 52);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_DeleteBallGfx(BattleSystem *battleSystem, OpponentData *opponentData) {
    PartyGauge_FreeGraphics(BattleSystem_GetSpriteManager(battleSystem));
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 53);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_IncrementGameStat(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketIncrementGameStat *packet = OPPONENT_COMMAND(opponentData);
    if (packet->flag == 0) {
        if (opponentData->inputMode == 0) {
            BattleSystem_GameStatIncrement(battleSystem, packet->id);
        }
    } else if (opponentData->inputMode != 0) {
        BattleSystem_GameStatIncrement(battleSystem, packet->id);
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ShowWaitMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketRecordData *packet = OPPONENT_COMMAND(opponentData);
    BattleSystem_UnpackRecordUpdate(battleSystem, packet->recordSize, packet->record);
    OpponentData_ShowWaitMessage(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_RestoreSprite(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_RestoreSprite(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SpriteToOAM(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_SpriteToOAM(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_OAMToSprite(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_OAMToSprite(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PrintResultMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PrintResultMessage(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_RunAwayMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketRecordData *packet = OPPONENT_COMMAND(opponentData);
    BattleSystem_UnpackRecordUpdate(battleSystem, packet->recordSize, packet->record);
    OpponentData_RunAwayMessage(battleSystem, opponentData, packet);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_ForfeitMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketRecordData *packet = OPPONENT_COMMAND(opponentData);
    BattleSystem_UnpackRecordUpdate(battleSystem, packet->recordSize, packet->record);
    OpponentData_ForfeitMessage(battleSystem, opponentData);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SwapToSubstituteSprite(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_SwapToSubstituteSprite(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PlayMoveSE(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PlayMoveSE(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_PlaySong(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_PlaySong(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_SetBattleResults(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleControllerPacketBattleResults *packet = OPPONENT_COMMAND(opponentData);
    u32 playerSideHp = 0;
    int i;
    u32 enemySideHp = 0;
    int j;
    Party *party;
    Pokemon *mon;

    BattleSystem_UnpackRecordUpdate(battleSystem, packet->recordSize, packet->record);
    if (BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_FRONTIER) {
        BattleSystem_SetBattleOutcomeFlags(battleSystem, (u8)packet->outcomeFlags);
    } else {
        for (i = 0; i < BattleSystem_GetMaxBattlers(battleSystem); i++) {
            party = BattleSystem_GetParty(battleSystem, i);
            for (j = 0; j < Party_GetCount(party); j++) {
                mon = Party_GetMonByIndex(party, j);
                if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE) {
                    if (BattleSystem_GetFieldSide(battleSystem, i) != 0) {
                        enemySideHp += GetMonData(mon, MON_DATA_HP, NULL);
                    } else {
                        playerSideHp += GetMonData(mon, MON_DATA_HP, NULL);
                    }
                }
            }
        }
        if (playerSideHp == 0 && enemySideHp == 0) {
            BattleSystem_SetBattleOutcomeFlags(battleSystem, BATTLE_OUTCOME_DRAW);
        } else if (playerSideHp == 0) {
            BattleSystem_SetBattleOutcomeFlags(battleSystem, BATTLE_OUTCOME_LOSE);
        } else {
            BattleSystem_SetBattleOutcomeFlags(battleSystem, BATTLE_OUTCOME_WIN);
        }
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    OpponentData_ClearCommand(opponentData);
}

void BattleControllerOpponent_BlankMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    Window *window = BattleSystem_GetWindow(battleSystem, 0);
    FillWindowPixelBuffer(window, 0xFF);
    CopyWindowPixelsToVram_TextMode(window);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 66);
    OpponentData_ClearCommand(opponentData);
}

void OpponentData_ClearCommand(OpponentData *opponentData) {
    OPPONENT_COMMAND_ID(opponentData) = 0;
}

void BattleControllerOpponent_ScreenFade(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentData_ScreenFade(battleSystem, opponentData, OPPONENT_COMMAND(opponentData));
    OpponentData_ClearCommand(opponentData);
}

void OpponentData_PlayEncounterAnimation(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentEncounterAnimWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentEncounterAnimWork));
    work->battleSystem = battleSystem;
    work->state = 0;
    work->timer = 0;
    work->fadeStep = 0;
    SysTask_CreateOnMainQueue(Task_PlayEncounterAnimation, work, 0);
}

// The NARC readers take a fourth parameter these calls never set.
#define READ_POKEPIC_SHADOW_HEIGHT(narc, ret, species)   ((void (*)(NARC *, s8 *, u16))NARC_ReadPokepicShadowHeight)(narc, ret, species)
#define READ_POKEPIC_SHADOW_X_OFFSET(narc, ret, species) ((void (*)(NARC *, s8 *, u16))NARC_ReadPokepicShadowXOffset)(narc, ret, species)
#define READ_POKEPIC_SHADOW_SIZE(narc, ret, species)     ((void (*)(NARC *, u8 *, u16))NARC_ReadPokepicShadowSize)(narc, ret, species)

void OpponentData_PokemonEncounter(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonEncounter *packet) {
    PokepicTemplate pokepicTemplate;
    OpponentPokepicAnimParams anim;
    PokepicManager *pokepicManager;
    OpponentPokemonEncounterWork *work;
    int height;
    int battlerType;
    int y;

    pokepicManager = BattleSystem_GetPokepicManager(battleSystem);
    BattleSystem_GetBattleType(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPokemonEncounterWork));
    work->state = 0;
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        work->facing = 2;
        work->terrain = BattleSystem_GetBattleTerrain(battleSystem, 1);
        ManagedSprite_SetPositionXY(work->terrain->sprite, ov07_022377F4[0][opponentData->battlerType & BATTLER_TYPE_IS_ENEMY][0], 88);
    } else {
        work->facing = 0;
        work->terrain = BattleSystem_GetBattleTerrain(battleSystem, 0);
        ManagedSprite_SetPositionXY(work->terrain->sprite, ov07_022377F4[0][opponentData->battlerType & BATTLER_TYPE_IS_ENEMY][0], 136);
    }

    GetMonSpriteCharAndPlttNarcIdsEx(&pokepicTemplate, packet->species, packet->gender, work->facing, packet->shiny != 0, packet->form, packet->personality);
    height = GetMonPicHeightBySpeciesGenderForm(packet->species, packet->gender, work->facing, packet->form, packet->personality);
    READ_POKEPIC_SHADOW_HEIGHT((NARC *)opponentData->pokepicAnimNarc, &anim.shadowHeight, packet->species);
    READ_POKEPIC_SHADOW_X_OFFSET((NARC *)opponentData->pokepicAnimNarc, &anim.shadowXOffset, packet->species);
    READ_POKEPIC_SHADOW_SIZE((NARC *)opponentData->pokepicAnimNarc, &anim.shadowSize, packet->species);
    NARC_ReadPokepicAnimScript((NARC *)opponentData->pokepicAnimNarc, anim.scripts, packet->species, opponentData->battlerType);

    battlerType = opponentData->battlerType;
    // volatile only to match: the new pokepic is stored and read back
    *(Pokepic *volatile *)&opponentData->pokepic = BattleControllerOpponent_CreatePokepic(battleSystem, pokepicManager, &pokepicTemplate, ov07_022377F4[0][battlerType][0], ov07_022377F4[0][battlerType][1], ov07_022377F4[0][battlerType][2], height, anim.shadowHeight, anim.shadowXOffset, anim.shadowSize, opponentData->battlerId, anim.scripts, NULL);
    work->pokepic = *(Pokepic *volatile *)&opponentData->pokepic;

    if (work->facing == 2) {
        Pokepic_StartPaletteFade(work->pokepic, 8, 8, 0, 0);
    }
    if (work->facing == 2 && (BattleSystem_GetBattleSpecial(battleSystem) & BATTLE_SPECIAL_GIRATINA)) {
        y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
        Pokepic_SetAttr(work->pokepic, POKEPIC_SHADOW_SIZE, 0);
        Pokepic_SetAttr(work->pokepic, POKEPIC_X, 192);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, y - 136);
        work->target = y;
    } else {
        work->target = ov07_022377DC[opponentData->battlerType][0];
    }

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->species = packet->species;
    work->form = packet->form;
    work->cryParam = packet->cryParam;
    work->battlerType = opponentData->battlerType;
    work->nature = GetNatureFromPersonality(packet->personality);
    work->shiny = packet->shiny;
    if (work->facing == 2 && (BattleSystem_GetBattleSpecial(battleSystem) & BATTLE_SPECIAL_GIRATINA)) {
        SysTask_CreateOnMainQueue(Task_GiratinaEncounter, work, 0);
    } else {
        SysTask_CreateOnMainQueue(Task_PokemonEncounter, work, 0);
    }
    sub_02005B58(1);
}

void OpponentData_PokemonSlideIn(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonSendOut *packet) {
    OpponentPokemonSendOutWork *work;
    u32 battleType;

    BattleSystem_GetBattleType(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPokemonSendOutWork));
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        work->facing = 2;
    } else {
        work->facing = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&work->pokepicTemplate, packet->species, packet->gender, work->facing, packet->shiny != 0, packet->form, packet->personality);
    work->height = GetMonPicHeightBySpeciesGenderForm(packet->species, packet->gender, work->facing, packet->form, packet->personality);
    READ_POKEPIC_SHADOW_HEIGHT((NARC *)opponentData->pokepicAnimNarc, &work->shadowHeight, packet->species);
    READ_POKEPIC_SHADOW_X_OFFSET((NARC *)opponentData->pokepicAnimNarc, &work->shadowXOffset, packet->species);
    READ_POKEPIC_SHADOW_SIZE((NARC *)opponentData->pokepicAnimNarc, &work->shadowSize, packet->species);
    OpponentData_ResetCursorPosition(opponentData);

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->state = 0;
    work->delay = 0;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->species = packet->species;
    work->form = packet->form;
    work->battlerType = opponentData->battlerType;
    work->cryParam = packet->cryParam;
    work->selectedMonIndex = packet->selectedMonIndex;
    work->nature = GetNatureFromPersonality(packet->personality);
    work->ball = packet->ball;
    work->shiny = packet->shiny;
    work->breakOut = 0;
    sub_02005B58(1);

    battleType = BattleSystem_GetBattleType(battleSystem);
    if (BattleSystem_GetFollowMonSlot(battleSystem, opponentData->battlerId) != 0xFF) {
        if ((battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI) && opponentData->battlerType > BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
            SysTask_CreateOnMainQueue(Task_PokemonSlideIn, work, 0);
            return;
        }
        SysTask_CreateOnMainQueue(Task_FollowMonSlideIn, work, 0);
        return;
    }
    SysTask_CreateOnMainQueue(Task_PokemonSlideIn, work, 0);
}

void OpponentData_PokemonSendOut(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonSendOut *packet) {
    int i;
    OpponentPokemonSendOutWork *work;

    BattleSystem_GetBattleType(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPokemonSendOutWork));
    work->state = 0;
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        work->facing = 2;
    } else {
        work->facing = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&work->pokepicTemplate, packet->species, packet->gender, work->facing, packet->shiny != 0, packet->form, packet->personality);
    work->height = GetMonPicHeightBySpeciesGenderForm(packet->species, packet->gender, work->facing, packet->form, packet->personality);
    READ_POKEPIC_SHADOW_HEIGHT((NARC *)opponentData->pokepicAnimNarc, &work->shadowHeight, packet->species);
    READ_POKEPIC_SHADOW_X_OFFSET((NARC *)opponentData->pokepicAnimNarc, &work->shadowXOffset, packet->species);
    READ_POKEPIC_SHADOW_SIZE((NARC *)opponentData->pokepicAnimNarc, &work->shadowSize, packet->species);
    OpponentData_ResetCursorPosition(opponentData);

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->species = packet->species;
    work->form = packet->form;
    work->battlerType = opponentData->battlerType;
    work->cryParam = packet->cryParam;
    work->selectedMonIndex = packet->selectedMonIndex;
    work->nature = GetNatureFromPersonality(packet->personality);
    work->ball = packet->ball;
    work->shiny = packet->shiny;
    work->breakOut = packet->breakOut;
    work->delay = 0;
    work->isSubstitute = packet->isSubstitute;

    for (i = 0; i < BATTLER_MAX; i++) {
        work->animData.species[i] = packet->allSpecies[i];
        work->animData.gender[i] = packet->allGender[i];
        work->animData.shiny[i] = packet->allShiny[i];
        work->animData.form[i] = packet->allForm[i];
        work->animData.personality[i] = packet->allPersonality[i];
    }

    i = BattleSystem_GetFollowMonSlot(battleSystem, opponentData->battlerId);
    if (i == 0xFF || i != work->selectedMonIndex) {
        SysTask_CreateOnMainQueue(Task_PokemonSendOut, work, 0);
    } else {
        SysTask_CreateOnMainQueue(Task_FollowMonSendOut, work, 0);
    }
}

void OpponentData_RecallPokemon(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRecallPokemon *packet) {
    int i;
    OpponentRecallPokemonWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentRecallPokemonWork));

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->pokepic = opponentData->pokepic;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->state = 0;
    work->height = packet->height;
    work->ball = packet->ball;
    work->isSubstitute = packet->isSubstitute;
    work->selectedMonIndex = packet->selectedMonIndex;

    for (i = 0; i < BATTLER_MAX; i++) {
        work->animData.species[i] = packet->allSpecies[i];
        work->animData.gender[i] = packet->allGender[i];
        work->animData.shiny[i] = packet->allShiny[i];
        work->animData.form[i] = packet->allForm[i];
        work->animData.personality[i] = packet->allPersonality[i];
    }

    i = BattleSystem_GetFollowMonSlot(battleSystem, opponentData->battlerId);
    if (i == 0xFF || i != work->selectedMonIndex) {
        SysTask_CreateOnMainQueue(Task_RecallPokemon, work, 0);
    } else {
        SysTask_CreateOnMainQueue(Task_RecallFollowMon, work, 0);
    }
}

void OpponentData_PokemonEnterBall(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPokemonEnterBall *packet) {
    OpponentPokepicShrinkWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPokepicShrinkWork));
    work->battleSystem = battleSystem;
    work->pokepic = opponentData->pokepic;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->state = 0;
    work->height = packet->height;
    Pokepic_StartPaletteFade(work->pokepic, 0, 16, 0, sBallColors[packet->ball]);
    Pokepic_SetAttr(work->pokepic, POKEPIC_SHADOW_AFFINE, 1);
    SysTask_CreateOnMainQueue(Task_PokemonEnterBall, work, 0);
}

#pragma force_active on
#pragma push
#pragma opt_common_subs off

void OpponentData_TrainerEncounter(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketTrainerEncounter *packet) {
    PokepicManager *pokepicManager;
    u32 battleType;
    BOOL isLink;
    s16 y;
    s16 x;
    int position;
    PokepicTemplate template;
    TrainerPicFileIds files;
    OpponentTrainerEncounterWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentTrainerEncounterWork));

    work->state = 0;
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        work->facing = 2;
        work->terrain = BattleSystem_GetBattleTerrain(battleSystem, 1);
        ManagedSprite_SetPositionXY(work->terrain->sprite, ov07_022377F4[0][opponentData->battlerType & BATTLER_TYPE_IS_ENEMY][0], 88);
    } else {
        work->facing = 0;
        work->terrain = BattleSystem_GetBattleTerrain(battleSystem, 0);
        ManagedSprite_SetPositionXY(work->terrain->sprite, ov07_022377F4[0][opponentData->battlerType & BATTLER_TYPE_IS_ENEMY][0], 136);
    }
    if ((BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_MULTI) || ((BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_TAG) && (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY))) {
        position = opponentData->battlerType;
    } else {
        position = opponentData->battlerType & BATTLER_TYPE_IS_ENEMY;
    }
    packet->trainerClass = BattleControllerOpponent_GetVersionTrainerClass(battleSystem, opponentData->battlerId, packet->trainerClass);
    battleType = BattleSystem_GetBattleType(battleSystem);
    isLink = FALSE;
    if (BattleSystem_GetFollowMonSlot(battleSystem, opponentData->battlerId) != 0xFF) {
        if ((battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI)) {
            isLink = FALSE;
        } else {
            isLink = TRUE;
        }
    }
    x = ov07_022377F4[0][position][0];
    y = ov07_022377F4[0][position][1];
    opponentData->managedSprite = BattleControllerOpponent_CreateTrainerSprite(battleSystem, position, packet->trainerClass, opponentData->battlerType, isLink, x, y);
    work->managedSprite = opponentData->managedSprite;
    if (work->facing == 0 && (BattleSystem_GetBattleType(battleSystem) == BATTLE_TYPE_NONE || BattleSystem_GetBattleType(battleSystem) == BATTLE_TYPE_SAFARI || BattleSystem_GetBattleType(battleSystem) == BATTLE_TYPE_ROAMER || BattleSystem_GetBattleType(battleSystem) == BATTLE_TYPE_PAL_PARK || BattleSystem_GetBattleType(battleSystem) == BATTLE_TYPE_TUTORIAL || BattleSystem_GetBattleType(battleSystem) == BATTLE_TYPE_BUG_CONTEST)) {
        pokepicManager = BattleSystem_GetPokepicManager(battleSystem);
        GetTrainerPicFileIdsNoLink(packet->trainerClass, work->facing, &files);
        template.narcID = files.narcId;
        template.charDataID = files.ncbr_id;
        template.palDataID = files.nclr_id;
        template.species = SPECIES_NONE;
        template.isAnimated = FALSE;
        template.personality = 0;
        work->pokepic = PokepicManager_CreatePokepic(pokepicManager, &template, x, y, ov07_022377F4[0][position][2], opponentData->battlerId, NULL, NULL);
    } else {
        work->pokepic = NULL;
    }
    work->targetX = ov07_022377DC[position][0];
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->bgScrollCount = 0;
    if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER || work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
        BgSetPosTextAndCommit(BattleSystem_GetBgConfig(battleSystem), GF_BG_LYR_MAIN_3, BG_POS_OP_SUB_X, 132);
    }
    SysTask_CreateOnMainQueue(Task_TrainerEncounter, work, 0);
}

#pragma pop

void OpponentData_ThrowPokeball(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketThrowPokeball *packet) {
    OpponentThrowPokeballWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentThrowPokeballWork));
    int backpic;

    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->throwType = packet->throwType;
    work->battlerId = opponentData->battlerId;
    work->opponentData = opponentData;
    backpic = 0;
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        work->facing = 2;
    } else {
        Trainer *trainer = BattleSystem_GetTrainer(battleSystem, opponentData->battlerId);
        work->facing = 0;
        backpic = TrainerClassToBackpicID(BattleControllerOpponent_GetVersionTrainerClass(battleSystem, opponentData->battlerId, trainer->data.trainerClass), 0);
    }
    work->backpic = backpic;
    BattleSystem_GetBattleType(battleSystem);
    if (BattleSystem_GetFollowMonSlot(battleSystem, opponentData->battlerId) != 0xFF) {
        SysTask_CreateOnMainQueue(Task_ThrowPokeballWithFollowMon, work, 0);
    } else {
        SysTask_CreateOnMainQueue(Task_ThrowPokeball, work, 0);
    }
}

#pragma force_active reset

void OpponentData_TrainerSlideOut(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentTrainerSlideOutWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentTrainerSlideOutWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->opponentData = opponentData;
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        work->facing = 2;
    } else {
        work->facing = 0;
    }
    SysTask_CreateOnMainQueue(Task_TrainerSlideOut, work, 0);
}

#pragma force_active on

void OpponentData_TrainerSlideIn(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketTrainerSlideIn *packet) {
    OpponentTrainerSlideInWork *work;

    BattleSystem_GetPokepicManager(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentTrainerSlideInWork));
    work->state = 0;
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        work->facing = 2;
    } else {
        work->facing = 0;
    }
    opponentData->managedSprite = BattleControllerOpponent_CreateTrainerSprite(battleSystem, opponentData->battlerType & BATTLER_TYPE_IS_ENEMY, packet->trainerClass, opponentData->battlerType, FALSE, ov07_022377F4[1][opponentData->battlerType & BATTLER_TYPE_IS_ENEMY][0], ov07_022377F4[1][opponentData->battlerType & BATTLER_TYPE_IS_ENEMY][1]);
    work->managedSprite = opponentData->managedSprite;
    work->targetX = ov07_022377DC[opponentData->battlerType & BATTLER_TYPE_IS_ENEMY][packet->index];
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    SysTask_CreateOnMainQueue(Task_TrainerSlideIn, work, 0);
}

void OpponentData_HealthbarSlideIn(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketHealthbarSlideIn *packet) {
    BattleHpBar *hpBar = &opponentData->hpBar;
    MIi_CpuClearFast(0, (u32 *)hpBar, 1);
    hpBar->battleSystem = battleSystem;
    hpBar->battlerId = opponentData->battlerId;
    hpBar->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(opponentData->battlerType, BattleSystem_GetBattleType(battleSystem));
    hpBar->command = packet->command;
    hpBar->hp = packet->hp;
    hpBar->maxHp = packet->maxHp;
    hpBar->level = packet->level;
    hpBar->gender = packet->gender;
    hpBar->gainedHp = 0;
    hpBar->exp = packet->exp;
    hpBar->maxExp = packet->maxExp;
    hpBar->monId = packet->selectedMonIndex;
    hpBar->status = packet->status;
    hpBar->caught = packet->caught;
    hpBar->delay = packet->delay;
    hpBar->safariBallCount = packet->safariBallCount;
    BattleHpBar_SetEnabled(hpBar, FALSE);
    BattleHpBar_Draw(hpBar, hpBar->hp, -1);
    hpBar->task = SysTask_CreateOnMainQueue(Task_HealthbarSlideIn, hpBar, 1000);
}

#pragma force_active reset

void OpponentData_HealthbarSlideOut(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleHpBar *hpBar = &opponentData->hpBar;
    MIi_CpuClearFast(0, (u32 *)hpBar, 1);
    hpBar->battleSystem = battleSystem;
    hpBar->battlerId = opponentData->battlerId;
    hpBar->command = OPPONENT_COMMAND_ID(opponentData);
    BattleHpBar_StartSlide(hpBar, TRUE);
    hpBar->task = SysTask_CreateOnMainQueue(Task_HealthbarSlideOut, hpBar, 1000);
}

#pragma force_active on

SysTask *OpponentData_SelectCommand(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectCommand *packet) {
    OpponentSelectCommandWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSelectCommandWork));
    int i;
    int j;

    MIi_CpuClearFast(0, (u32 *)work, sizeof(OpponentSelectCommandWork));
    work->state = 0;
    work->unk_0B = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->hpBar = &opponentData->hpBar;
    work->partySlot = packet->partySlot;
    work->hp = packet->hp;
    work->maxHp = packet->maxHp;
    work->ballStatusBattler = packet->ballStatusBattler;
    work->switchingOrCanPickCommandMask = packet->switchingOrCanPickCommandMask;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < PARTY_SIZE; j++) {
            work->ballStatus[i][j] = packet->ballStatus[i][j];
        }
    }
    for (i = 0; i < PARTY_SIZE; i++) {
        if (packet->ballStatus[0][i] == 2) {
            work->expPercents[i] = 0;
        } else {
            work->expPercents[i] = packet->expPercents[i];
        }
    }
    {
        int k;
        for (k = 0; k < MAX_MON_MOVES; k++) {
            work->moves[k] = packet->moves[k];
            work->movePPCur[k] = packet->movePPCur[k];
            work->movePP[k] = packet->movePP[k];
        }
    }
    return SysTask_CreateOnMainQueue((SysTaskFunc)opponentData->unk0[0], work, 0);
}

#pragma force_active reset

void OpponentData_SelectMove(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectMove *packet) {
    OpponentSelectMoveWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSelectMoveWork));
    int i = 0;

    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->hpBar = &opponentData->hpBar;
    work->selectedMonIndex = packet->selectedMonIndex;
    {
        // walks both structs two bytes at a time
        BattleControllerPacketSelectMove *src = packet;
        OpponentSelectMoveWork *dst = work;
        for (i = 0; i < MAX_MON_MOVES; i++) {
            dst->moves[0] = src->moves[0];
            work->movePPCur[i] = packet->movePPCur[i];
            work->movePP[i] = packet->movePP[i];
            src = (BattleControllerPacketSelectMove *)((u8 *)src + sizeof(u16));
            dst = (OpponentSelectMoveWork *)((u8 *)dst + sizeof(u16));
        }
    }
    work->unusableMovesMask = packet->unusableMovesMask;
    SysTask_CreateOnMainQueue((SysTaskFunc)opponentData->unk0[1], work, 0);
}

#pragma force_active on

void OpponentData_SelectTarget(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectTarget *packet) {
    OpponentSelectTargetWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSelectTargetWork));
    u8 battlerTypes[8];
    int maxBattlers;
    int i;
    OpponentSelectTargetWork *dst;

    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->moveRange = packet->moveRange;
    work->hpBar = &opponentData->hpBar;
    work->unk_32 = packet->unk_01;
    BattleSystem_GetBattlerTypes(battleSystem, battlerTypes);
    maxBattlers = BattleSystem_GetMaxBattlers(battleSystem);
    BattleSystem_GetBattleType(battleSystem);
    i = 0;
    if (maxBattlers > 0) {
        dst = work;
        do {
            // copied as halfwords, walking both structs one TargetPokemon at a time
            ((u16 *)dst->targetMons)[0] = ((u16 *)packet->targetMons)[0];
            ((u16 *)dst->targetMons)[1] = ((u16 *)packet->targetMons)[1];
            ((u16 *)dst->targetMons)[2] = ((u16 *)packet->targetMons)[2];
            ((u16 *)dst->targetMons)[3] = ((u16 *)packet->targetMons)[3];
            i++;
            packet = (BattleControllerPacketSelectTarget *)((u8 *)packet + sizeof(TargetPokemon));
            dst = (OpponentSelectTargetWork *)((u8 *)dst + sizeof(TargetPokemon));
        } while (i < maxBattlers);
    }
    SysTask_CreateOnMainQueue((SysTaskFunc)opponentData->unk0[2], work, 0);
}

SysTask *OpponentData_SelectItem(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectItem *packet) {
    OpponentSelectItemWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSelectItemWork));
    int i;
    int j;

    work->partyMenu = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSelectItemPartyMenu));
    work->partyMenu->args = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattlePartyMenuArgs));
    work->partyMenu->args->party = SaveArray_Party_Alloc(HEAP_ID_BATTLE);
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->unk_14 = packet->unk_01;
    work->targetSemiInvulnerable = packet->targetSemiInvulnerable;
    work->targetSubstitute = packet->targetSubstitute;
    for (i = 0; i < BATTLER_MAX; i++) {
        work->partyMenu->selectedMonIndex[i] = packet->selectedMonIndex[i];
        for (j = 0; j < PARTY_SIZE; j++) {
            work->partyOrder[i][j] = packet->partyOrder[i][j];
        }
        work->embargoFlag[i] = packet->embargoFlag[i];
    }
    return SysTask_CreateOnMainQueue((SysTaskFunc)opponentData->unk0[3], work, 0);
}

void OpponentData_ShowMonList(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketShowMonList *packet) {
    OpponentShowMonListWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentShowMonListWork));
    int i;
    int j;

    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = packet->battlerId;
    work->mode = packet->mode;
    work->trapped = packet->trapped;
    work->reservedMon = packet->reservedMon;
    work->item = 0;
    work->switchInFlag = packet->switchInFlag;
    for (i = 0; i < BATTLER_MAX; i++) {
        work->selectedMonIndex[i] = packet->selectedMonIndex[i];
        for (j = 0; j < PARTY_SIZE; j++) {
            work->partyOrder[i][j] = packet->partyOrder[i][j];
        }
    }
    SysTask_CreateOnMainQueue((SysTaskFunc)opponentData->unk0[4], work, 0);
}

#pragma force_active reset

void OpponentData_DrawYesNoBox(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketYesNoBox *packet) {
    OpponentYesNoWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentYesNoWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->hpBar = &opponentData->hpBar;
    work->msgId = packet->msgId;
    work->mode = packet->mode;
    work->value = packet->value;
    work->param = packet->param;
    SysTask_CreateOnMainQueue((SysTaskFunc)opponentData->unk0[5], work, 0);
}

#pragma force_active on

void OpponentData_PrintAttackMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketAttackMessage *packet) {
    MsgData *msgData = (MsgData *)BattleSystem_GetAttackMessageLoader(battleSystem);
    OpponentMessageWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    BattleMessage msg;
    u32 selectedMonIndex;
    u32 battlerId;

    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    msg.id = packet->move * 3;
    msg.tag = TAG_NICKNAME;
    selectedMonIndex = packet->selectedMonIndex;
    battlerId = opponentData->battlerId;
    msg.param[0] = battlerId | (selectedMonIndex << 8);
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

#pragma force_active reset

void OpponentData_PrintMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg) {
    MsgData *msgData = BattleSystem_GetMessageLoader(battleSystem);
    OpponentMessageWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

#pragma force_active on

void OpponentData_PlayMoveAnimation(BattleSystem *battleSystem, OpponentData *opponentData, MoveAnimData *animData) {
    OpponentMoveAnimationWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMoveAnimationWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(battleSystem);
    work->animData = *animData;
    work->pokepic = opponentData->pokepic;
    if (animData->isEffectAnim == 1 && animData->effectAnimId == 25) {
        opponentData->unk1A0 = 1;
    }
    if (animData->isEffectAnim == 1 && animData->effectAnimId == 26) {
        opponentData->unk1A0 = 0;
    }
    MoveAnim_GetDisplayFlags(&work->disableHpBar, &work->updateG3, animData->isEffectAnim, animData->effectAnimId, animData->move);
    SysTask_CreateOnMainQueue(Task_PlayMoveAnimation, work, 0);
}

#pragma force_active reset

void OpponentData_MonFlicker(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentMonFlickerWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMonFlickerWork));
    work->count = 0;
    work->battleSystem = battleSystem;
    work->pokepic = opponentData->pokepic;
    work->battlerId = opponentData->battlerId;
    work->delay = 0;
    SysTask_CreateOnMainQueue(Task_MonFlicker, work, 0);
}

void OpponentData_HealthbarUpdate(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketHealthbarUpdate *packet) {
    BattleHpBar *hpBar;

    GF_ASSERT(opponentData->hpBar.boxObj != NULL);
    hpBar = &opponentData->hpBar;
    MI_CpuFill8(&hpBar->script, 0, sizeof(hpBar->script));
    hpBar->battleSystem = battleSystem;
    hpBar->command = packet->command;
    hpBar->battlerId = opponentData->battlerId;
    hpBar->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(opponentData->battlerType, BattleSystem_GetBattleType(battleSystem));
    hpBar->hp = packet->hp;
    hpBar->maxHp = packet->maxHp;
    hpBar->gainedHp = packet->gainedHp;
    hpBar->level = packet->level;
    if (packet->gainedHp == 0x7FFF) {
        hpBar->hp = 0;
        hpBar->gainedHp = 0;
    }
    hpBar->task = SysTask_CreateOnMainQueue(Task_HealthbarUpdate, hpBar, 1000);
}

void OpponentData_ExpBarUpdate(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketExpBarUpdate *packet) {
    BattleHpBar *hpBar;

    GF_ASSERT(opponentData->hpBar.boxObj != NULL);
    hpBar = &opponentData->hpBar;
    MI_CpuFill8(&hpBar->script, 0, sizeof(hpBar->script));
    hpBar->battleSystem = battleSystem;
    hpBar->command = packet->command;
    hpBar->battlerId = opponentData->battlerId;
    hpBar->exp = packet->prevExp;
    hpBar->maxExp = packet->maxExp;
    hpBar->gainedExp = packet->exp - hpBar->exp;
    if (opponentData->battlerType == BATTLER_TYPE_SOLO_PLAYER) {
        hpBar->task = SysTask_CreateOnMainQueue(Task_ExpBarUpdate, hpBar, 1000);
    } else {
        BattleController_EmitCommandComplete(hpBar->battleSystem, hpBar->battlerId, hpBar->command);
    }
}

void OpponentData_PlayFaintAnimation(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketFaintAnimation *packet) {
    OpponentFaintWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentFaintWork));
    int i;

    work->facing = (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) ? 2 : 0;
    work->state = 0;
    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->pokepic = opponentData->pokepic;
    work->species = packet->species;
    work->gender = packet->gender;
    work->form = packet->form;
    work->personality = packet->personality;
    work->isSubstitute = packet->isSubstitute;
    work->isTransformed = packet->isTransformed;
    for (i = 0; i < BATTLER_MAX; i++) {
        work->animData.species[i] = packet->allSpecies[i];
        work->animData.gender[i] = packet->allGender[i];
        work->animData.shiny[i] = packet->allShiny[i];
        work->animData.form[i] = packet->allForm[i];
        work->animData.personality[i] = packet->allPersonality[i];
    }
    SysTask_CreateOnMainQueue(Task_PlayFaintAnimation, work, 0);
}

void OpponentData_PlaySE(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPlaySE *packet) {
    s32 pan;

    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        pan = 117;
    } else {
        pan = -117;
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    PlaySE_SetPan(packet->sndSeqNo, pan);
}

void OpponentData_FadeOutBattle(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentFadeOutWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentFadeOutWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    SysTask_CreateOnMainQueue(Task_FadeOutBattle, work, 0);
}

void OpponentData_ToggleVanish(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketToggleVanish *packet) {
    OpponentToggleVanishWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentToggleVanishWork));
    int i;

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->state = 0;
    work->vanish = packet->vanish;
    work->isSubstitute = packet->isSubstitute;
    for (i = 0; i < BATTLER_MAX; i++) {
        work->animData.species[i] = packet->allSpecies[i];
        work->animData.gender[i] = packet->allGender[i];
        work->animData.shiny[i] = packet->allShiny[i];
        work->animData.form[i] = packet->allForm[i];
        work->animData.personality[i] = packet->allPersonality[i];
    }
    SysTask_CreateOnMainQueue(Task_ToggleVanish, work, 0);
}

void OpponentData_HealthbarStatus(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketHealthbarStatus *packet) {
    GF_ASSERT(opponentData->hpBar.boxObj != NULL);
    opponentData->hpBar.status = packet->status;
    BattleHpBar_Draw(&opponentData->hpBar, opponentData->hpBar.hp, 0x100);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
}

void OpponentData_PrintTrainerMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketTrainerMessage *packet) {
    int trainerIndex = BattleSystem_GetTrainerIndex(battleSystem, opponentData->battlerId);
    OpponentMessageWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));

    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintTrainerMessage(battleSystem, trainerIndex, opponentData->battlerId, packet->msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_PrintReturnMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet) {
    BattleMessage msg;
    OpponentMessageWork *work;
    MsgData *msgData;

    BattleControllerOpponent_GetReturnMessage(battleSystem, opponentData, packet, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_PrintSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet) {
    BattleMessage msg;
    OpponentMessageWork *work;
    MsgData *msgData;

    BattleControllerOpponent_GetSendOutMessage(battleSystem, opponentData, packet, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_PrintEncounterMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleMessage msg;
    OpponentMessageWork *work;
    MsgData *msgData;

    BattleControllerOpponent_GetEncounterMessage(battleSystem, opponentData, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = 34;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_PrintFirstSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketFirstSendOutMessage *packet) {
    BattleMessage msg;
    OpponentMessageWork *work;
    MsgData *msgData;

    BattleControllerOpponent_GetFirstSendOutMessage(battleSystem, opponentData, packet, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_ExpBarFullFlash(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentExpBarFlashWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentExpBarFlashWork));
    work->battleSystem = battleSystem;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->state = 0;
    work->hpBar = &opponentData->hpBar;
    SysTask_CreateOnMainQueue(Task_PlayExpBarFullFlash, work, 0);
}

#pragma force_active on

void OpponentData_PrintSelectionMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectionMessage *packet) {
    u8 inputMode = opponentData->inputMode;
    if (inputMode == 0) {
        MsgData *msgData = BattleSystem_GetMessageLoader(battleSystem);
        OpponentSelectionMessageWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSelectionMessageWork));
        work->battleSystem = battleSystem;
        work->command = packet->command;
        work->battlerId = opponentData->battlerId;
        work->state = 0;
        work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &packet->msg, BattleSystem_GetTextFrameDelay(battleSystem));
        SysTask_CreateOnMainQueue(Task_PrintSelectionMessage, work, 0);
    } else if (inputMode == 1) {
        BattleController_ReplySelectionMessageDone(battleSystem, opponentData->battlerId);
        BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    } else {
        if (!(BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_LINK)) {
            BattleController_ReplySelectionMessageDone(battleSystem, opponentData->battlerId);
        }
        BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
    }
}

void OpponentData_RefreshHealthbar(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRefreshHealthbar *packet) {
    BattleHpBar *hpBar = &opponentData->hpBar;
    MIi_CpuClearFast(0, (u32 *)hpBar, 1);
    hpBar->battleSystem = battleSystem;
    hpBar->battlerId = opponentData->battlerId;
    hpBar->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(opponentData->battlerType, BattleSystem_GetBattleType(battleSystem));
    hpBar->command = packet->command;
    hpBar->hp = packet->hp;
    hpBar->maxHp = packet->maxHp;
    hpBar->level = packet->level;
    hpBar->gender = packet->gender;
    hpBar->gainedHp = 0;
    hpBar->exp = packet->exp;
    hpBar->maxExp = packet->maxExp;
    hpBar->monId = packet->selectedMonIndex;
    hpBar->status = packet->status;
    hpBar->caught = packet->caught;
    hpBar->safariBallCount = packet->safariBallCount;
    BattleHpBar_Draw(hpBar, hpBar->hp, ~0x20);
    BattleController_EmitCommandComplete(hpBar->battleSystem, hpBar->battlerId, hpBar->command);
}

#pragma force_active reset

void OpponentData_SelectMoveToForget(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSelectMoveToForget *packet) {
    OpponentSelectMoveToForgetWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSelectMoveToForgetWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->move = packet->move;
    work->slot = packet->slot;
    SysTask_CreateOnMainQueue(Task_SelectMoveToForget, work, 0);
}

void OpponentData_PlayMosaicAnimation(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketMosaicAnimation *packet) {
    OpponentMosaicWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMosaicWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->pokepic = opponentData->pokepic;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->mosaic = packet->mosaic;
    work->counter = 0;
    work->delay = packet->delay;
    SysTask_CreateOnMainQueue(Task_PlayMosaicAnimation, work, 0);
}

void OpponentData_InitStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet) {
    OpponentPartyGaugeWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPartyGaugeWork));
    int i;
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    for (i = 0; i < PARTY_SIZE; i++) {
        work->status[i] = packet->status[i];
    }
    work->midBattle = FALSE;
    SysTask_CreateOnMainQueue(Task_ShowPartyGauge, work, 0);
}

void OpponentData_DeleteStartBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet) {
    OpponentPartyGaugeWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPartyGaugeWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->midBattle = FALSE;
    SysTask_CreateOnMainQueue(Task_HidePartyGauge, work, 0);
}

void OpponentData_InitBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet) {
    OpponentPartyGaugeWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPartyGaugeWork));
    int i;
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    for (i = 0; i < PARTY_SIZE; i++) {
        work->status[i] = packet->status[i];
    }
    work->midBattle = TRUE;
    SysTask_CreateOnMainQueue(Task_ShowPartyGauge, work, 0);
}

void OpponentData_DeleteBallGauge(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketBallGauge *packet) {
    OpponentPartyGaugeWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentPartyGaugeWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->battlerId = opponentData->battlerId;
    work->battlerType = opponentData->battlerType;
    work->midBattle = TRUE;
    SysTask_CreateOnMainQueue(Task_HidePartyGauge, work, 0);
}

void OpponentData_ShowWaitMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleMessage msg;
    WaitingIcon *waitingIcon;
    MsgData *msgData;

    if (opponentData->inputMode == 0) {
        msgData = BattleSystem_GetMessageLoader(battleSystem);
        msg.id = 923;
        msg.tag = TAG_NONE;
        BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, 0);
        waitingIcon = WaitingIcon_New(BattleSystem_GetWindow(battleSystem, 0), 1);
        BattleSystem_SetWaitingIcon(battleSystem, waitingIcon);
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, 55);
}

void OpponentData_RestoreSprite(BattleSystem *battleSystem, OpponentData *opponentData, MoveAnimData *animData) {
    BattleAnimBattlerPicParams params;
    BattleController_InitBattlerPicParams(battleSystem, animData, &params, opponentData->battlerId);
    BattleAnimUtil_RestorePic(&params, HEAP_ID_BATTLE);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, animData->command);
}

void OpponentData_SpriteToOAM(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentSpriteOamWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSpriteOamWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->pokepic = opponentData->pokepic;
    SysTask_CreateOnMainQueue(Task_SpriteToOAM, work, 0);
}

void OpponentData_OAMToSprite(BattleSystem *battleSystem, OpponentData *opponentData) {
    OpponentSpriteOamWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentSpriteOamWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = OPPONENT_COMMAND_ID(opponentData);
    work->battlerId = opponentData->battlerId;
    work->pokepic = opponentData->pokepic;
    SysTask_CreateOnMainQueue(Task_OAMToSprite, work, 0);
}

void OpponentData_PrintResultMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleMessage msg;
    MsgData *msgData;
    OpponentMessageWork *work;

    BattleControllerOpponent_GetResultMessage(battleSystem, opponentData, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = 59;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_RunAwayMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRecordData *packet) {
    BattleMessage msg;
    MsgData *msgData;
    OpponentMessageWork *work;

    BattleControllerOpponent_GetRunAwayMessage(battleSystem, opponentData, packet, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = 60;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_ForfeitMessage(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleMessage msg;
    MsgData *msgData;
    OpponentMessageWork *work;

    BattleControllerOpponent_GetForfeitMessage(battleSystem, opponentData, &msg);
    msgData = BattleSystem_GetMessageLoader(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentMessageWork));
    work->battleSystem = battleSystem;
    work->command = 61;
    work->battlerId = opponentData->battlerId;
    work->printerId = BattleSystem_PrintBattleMessage(battleSystem, msgData, &msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(Task_WaitBattleMessage, work, 0);
}

void OpponentData_SwapToSubstituteSprite(BattleSystem *battleSystem, OpponentData *opponentData, MoveAnimData *animData) {
    BattleAnimBattlerPicParams params;
    BattleController_InitBattlerPicParams(battleSystem, animData, &params, opponentData->battlerId);
    BattleAnimUtil_LoadSubstituteIntoPic(&params, HEAP_ID_BATTLE);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, animData->command);
}

void OpponentData_PlayMoveSE(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketMoveSE *packet) {
    int pan;

    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        pan = 117;
    } else {
        pan = -117;
    }
    switch (packet->effectiveness) {
    case 0:
        PlaySE_SetPan(SEQ_SE_DP_KOUKA_M, pan);
        break;
    case 2:
        PlaySE_SetPan(SEQ_SE_DP_KOUKA_H, pan);
        break;
    case 1:
        PlaySE_SetPan(SEQ_SE_DP_KOUKA_L, pan);
        break;
    }
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
}

void OpponentData_PlaySong(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketPlaySong *packet) {
    PlayBGM(packet->song);
    BattleController_EmitCommandComplete(battleSystem, opponentData->battlerId, packet->command);
}

void OpponentData_ScreenFade(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketScreenFade *packet) {
    OpponentScreenFadeWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(OpponentScreenFadeWork));
    MI_CpuFill8(work, 0, sizeof(OpponentScreenFadeWork));
    work->state = 0;
    work->battleSystem = battleSystem;
    work->command = packet->command;
    work->mode = packet->mode;
    work->counter = 0;
    work->battlerId = opponentData->battlerId;
    SysTask_CreateOnMainQueue(Task_OpponentScreenFade, work, 0);
}

#pragma force_active on

void Task_PokemonEncounter(SysTask *task, void *data) {
    OpponentPokemonEncounterWork *work = data;
    BattleAnimSystem *animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);
    u8 state = work->state;
    BattleAnimSystem *anim = animSystem;
    s16 coords[2]; // [1] = x, [0] = y
    MoveAnimData animData;
    int targetX;

    switch (state) {
    case 0:
        work->delay = 28;
        work->state++;
    case 1:
        if (--work->delay) {
            break;
        }
        work->state++;
    case 2:
        if (work->facing == 2) {
            ManagedSprite_GetPositionXY(work->terrain->sprite, &coords[1], &coords[0]);
            if (work->battlerType == BATTLER_TYPE_SOLO_ENEMY || work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                if (coords[1] < 192) {
                    ManagedSprite_OffsetPositionXY(work->terrain->sprite, 8, 0);
                } else {
                    ManagedSprite_SetPositionXY(work->terrain->sprite, 192, 88);
                }
            }
            ManagedSprite_GetPositionXY(work->terrain->sprite, &coords[1], &coords[0]);
            if (work->battlerType == BATTLER_TYPE_SOLO_ENEMY) {
                Pokepic_SetAttr(work->pokepic, POKEPIC_X, coords[1]);
            } else if (work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                coords[1] = Pokepic_GetAttr(work->pokepic, POKEPIC_X) - coords[1];
                coords[1] -= 24;
                Pokepic_AddAttr(work->pokepic, POKEPIC_X, -coords[1]);
            } else if (work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_2) {
                coords[1] = coords[1] - Pokepic_GetAttr(work->pokepic, POKEPIC_X);
                coords[1] -= 16;
                Pokepic_AddAttr(work->pokepic, POKEPIC_X, coords[1]);
            }
            if (Pokepic_GetAttr(work->pokepic, POKEPIC_X) < work->target) {
                break;
            }
            Pokepic_SetAttr(work->pokepic, POKEPIC_SHADOW_YADJ_REQ, 0);
            Pokepic_SetAttr(work->pokepic, POKEPIC_SHADOW_AFFINE, 0);
            Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->target);
            BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
            if (work->battlerType == BATTLER_TYPE_SOLO_ENEMY || work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                ManagedSprite_SetPositionXY(work->terrain->sprite, 192, 88);
            }
            Pokepic_StartPaletteFade(work->pokepic, 8, 0, 0, 0);
            work->state++;
            return;
        } else {
            ManagedSprite_GetPositionXY(work->terrain->sprite, &coords[1], &coords[0]);
            if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER || work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                if (coords[1] > 64) {
                    ManagedSprite_OffsetPositionXY(work->terrain->sprite, -8, 0);
                } else {
                    ManagedSprite_SetPositionXY(work->terrain->sprite, 64, 136);
                }
            }
            ManagedSprite_GetPositionXY(work->terrain->sprite, &coords[1], &coords[0]);
            if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER) {
                Pokepic_SetAttr(work->pokepic, POKEPIC_X, coords[1]);
            } else if (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                coords[1] = coords[1] - Pokepic_GetAttr(work->pokepic, POKEPIC_X);
                coords[1] -= 24;
                Pokepic_AddAttr(work->pokepic, POKEPIC_X, coords[1]);
            } else if (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                coords[1] = Pokepic_GetAttr(work->pokepic, POKEPIC_X) - coords[1];
                coords[1] -= 16;
                Pokepic_AddAttr(work->pokepic, POKEPIC_X, -coords[1]);
            }
            targetX = work->target;
            if (Pokepic_GetAttr(work->pokepic, POKEPIC_X) > targetX) {
                break;
            }
            Pokepic_SetAttr(work->pokepic, POKEPIC_X, targetX);
            BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
            if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER || work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                ManagedSprite_SetPositionXY(work->terrain->sprite, 64, 136);
            }
            work->state++;
            return;
        }
    case 3:
        if (PokepicAnimManager_GetAnimState(BattleSystem_GetPokepicAnimManager(work->battleSystem), work->battlerId) == 1 && !Pokepic_IsAnimFinished(work->pokepic)) {
            if (work->shiny) {
                BattleController_InitMoveAnimData(work->battleSystem, NULL, &animData, 1, 11, work->battlerId, work->battlerId, 0);
                OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, anim, &animData);
                work->state = 4;
            } else {
                work->state = 0xFF;
            }
        }
        break;
    case 4:
        BattleAnimSystem_Execute(anim);
        if (!BattleAnimSystem_IsActive(anim)) {
            BattleAnimSystem_EndMove(anim);
            work->state = 0xFF;
        }
        break;
    default:
        sub_02005B58(0);
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_GiratinaEncounter(SysTask *task, void *data) {
    OpponentPokemonEncounterWork *work = data;
    MoveAnimData animData;
    s16 coords[2]; // [1] = x, [0] = y
    BattleAnimSystem *animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);

    switch ((int)work->state) {
    case 0:
        work->delay = 28;
        work->state++;
    case 1:
        if (--work->delay) {
            break;
        }
        work->state++;
    case 2:
        ManagedSprite_GetPositionXY(work->terrain->sprite, &coords[1], &coords[0]);
        if (work->battlerType == BATTLER_TYPE_SOLO_ENEMY || work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
            if (coords[1] < 192) {
                ManagedSprite_OffsetPositionXY(work->terrain->sprite, 8, 0);
            } else {
                ManagedSprite_SetPositionXY(work->terrain->sprite, 192, 88);
            }
        }
        ManagedSprite_GetPositionXY(work->terrain->sprite, &coords[1], &coords[0]);
        Pokepic_AddAttr(work->pokepic, POKEPIC_Y, 4);
        if (Pokepic_GetAttr(work->pokepic, POKEPIC_Y) < work->target) {
            break;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_SHADOW_YADJ_REQ, 0);
        Pokepic_SetAttr(work->pokepic, POKEPIC_SHADOW_AFFINE, 0);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->target);
        BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
        ManagedSprite_SetPositionXY(work->terrain->sprite, 192, 88);
        Pokepic_StartPaletteFade(work->pokepic, 8, 0, 0, 0);
        work->state++;
        break;
    case 3:
        if (PokepicAnimManager_GetAnimState(BattleSystem_GetPokepicAnimManager(work->battleSystem), work->battlerId) != 1) {
            break;
        }
        if (Pokepic_IsAnimFinished(work->pokepic)) {
            break;
        }
        if (work->shiny) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &animData, 1, 11, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &animData);
            work->state = 4;
        } else {
            work->state = 0xFF;
        }
        break;
    case 4:
        BattleAnimSystem_Execute(animSystem);
        if (BattleAnimSystem_IsActive(animSystem)) {
            break;
        }
        BattleAnimSystem_EndMove(animSystem);
        work->state = 0xFF;
        break;
    default:
        sub_02005B58(0);
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_PokemonSlideIn(SysTask *task, void *data) {
    OpponentPokemonSendOutWork *work = data;
    MoveAnimData animData;
    BattlePokeballOpenArgs openArgs;
    PokepicAnimScript scripts[10];

    switch (work->state) {
    case 0:
        work->delay = 0;
        work->battlerSprites = NULL;
        if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_MULTI) {
            if (!(BattleSystem_GetBattleSpecial(work->battleSystem) & BATTLE_SPECIAL_RECORDED) && work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                work->battlerSprites = BattleAnimBattlerSprites_NewAll(work->battleSystem, HEAP_ID_BATTLE);
            }
        } else if (!(BattleSystem_GetBattleSpecial(work->battleSystem) & BATTLE_SPECIAL_RECORDED)) {
            if (BattleSystem_IsServer(work->battleSystem) == 1 && work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                work->battlerSprites = BattleAnimBattlerSprites_NewAll(work->battleSystem, HEAP_ID_BATTLE);
            } else if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER) {
                work->battlerSprites = BattleAnimBattlerSprites_NewAll(work->battleSystem, HEAP_ID_BATTLE);
            }
        }
        work->state++;
        return;
    case 1:
        memset(&openArgs, 0, sizeof(openArgs));
        openArgs.battlerType = work->battlerType;
        openArgs.mon = BattleSystem_GetPartyMon(work->battleSystem, work->battlerId, work->selectedMonIndex);
        work->ballOpenEffect = BallSealEffect_New(HEAP_ID_BATTLE, &openArgs);
        BallSealEffect_LoadResources(work->ballOpenEffect);
        work->state++;
        return;
    case 2: {
        PokepicManager *pokepicManager;
        if (BallThrow_GetMode(work->opponentData->ballData) || BallSealEffect_IsLoaded(work->ballOpenEffect) != 1 || BallThrow_HasOpened(work->opponentData->ballData) != 1) {
            break;
        }
        if (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
            work->delay++;
            if (work->delay < 12) {
                break;
            }
            work->delay = 0;
        }
        pokepicManager = BattleSystem_GetPokepicManager(work->battleSystem);
        NARC_ReadPokepicAnimScript((NARC *)work->opponentData->pokepicAnimNarc, scripts, work->species, work->battlerType);
        work->opponentData->pokepic = BattleControllerOpponent_CreatePokepic(work->battleSystem, pokepicManager, &work->pokepicTemplate, ov07_022377DC[work->battlerType][0], ov07_022377F4[0][work->battlerType][1], ov07_022377F4[0][work->battlerType][2], work->height, work->shadowHeight, work->shadowXOffset, work->shadowSize, work->battlerId, scripts, NULL);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_YADJ_REQ, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_VANISHED, 1);
        Pokepic_StartPaletteFade(work->opponentData->pokepic, 16, 16, 0, sBallColors[work->ball]);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_VANISHED, 0);
        BallSealEffect_Start(work->ballOpenEffect);
        if (work->facing == 2) {
            PlaySE_SetPan(SEQ_SE_DP_BOWA2, 117);
        } else {
            PlaySE_SetPan(SEQ_SE_DP_BOWA2, -117);
        }
        if (work->battlerSprites) {
            BattleAnimBattlerSprites_Free(work->battlerSprites);
            work->battlerSprites = NULL;
        }
        work->state++;
        return;
    }
    case 3:
        if (BallThrow_IsActive(work->opponentData->ballData) != 1) {
            work->state++;
        }
    case 4:
        if (Pokepic_GetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW) == 0x100 && !BallSealEffect_IsActive(work->ballOpenEffect)) {
            if (work->facing == 2) {
                Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_AFFINE, 0);
            }
            BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->opponentData->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
            Pokepic_StartPaletteFade(work->opponentData->pokepic, 16, 0, 0, sBallColors[work->ball]);
            work->state = 5;
            return;
        }
        if (Pokepic_GetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW) >= 0x100) {
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 0x100);
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 0x100);
            if (work->facing == 2) {
                Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_AFFINE, 0);
            }
            BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->opponentData->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
            Pokepic_StartPaletteFade(work->opponentData->pokepic, 16, 0, 1, sBallColors[work->ball]);
            work->state = 5;
            return;
        } else {
            Pokepic_AddAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 32);
            Pokepic_AddAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 32);
            Pokepic_SetYOffsetForAffineHeight(work->opponentData->pokepic, work->shadowHeight);
        }
        return;
    case 5:
        if (!BallSealEffect_IsActive(work->ballOpenEffect)) {
            work->state = 6;
        }
        return;
    case 6:
        if (PokepicAnimManager_GetAnimState(BattleSystem_GetPokepicAnimManager(work->battleSystem), work->battlerId) != 1 || Pokepic_IsAnimFinished(work->opponentData->pokepic)) {
            break;
        }
        BallThrow_Delete(work->opponentData->ballData);
        work->opponentData->ballData = NULL;
        BallSealEffect_Free(work->ballOpenEffect);
        if (work->shiny) {
            work->animSystem = BattleAnimSystem_New(HEAP_ID_BATTLE);
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &animData, 1, 11, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &animData);
            work->state = 7;
        } else {
            work->state = 0xFF;
        }
        return;
    case 7:
        BattleAnimSystem_Execute(work->animSystem);
        if (BattleAnimSystem_IsActive(work->animSystem)) {
            break;
        }
        BattleAnimSystem_EndMove(work->animSystem);
        BattleAnimSystem_Delete(work->animSystem);
        work->state = 0xFF;
        return;
    default:
        sub_02005B58(0);
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_FollowMonSlideIn(SysTask *task, void *data) {
    OpponentPokemonSendOutWork *work = data;
    MoveAnimData animData;
    MoveAnimData shinyAnimData;
    PokepicAnimScript scripts[10];

    switch (work->state) {
    case 0:
        work->delay = 0;
        work->battlerSprites = NULL;
        if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_MULTI) {
            if (!(BattleSystem_GetBattleSpecial(work->battleSystem) & BATTLE_SPECIAL_RECORDED) && work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                work->battlerSprites = BattleAnimBattlerSprites_NewAll(work->battleSystem, HEAP_ID_BATTLE);
            }
        } else if (!(BattleSystem_GetBattleSpecial(work->battleSystem) & BATTLE_SPECIAL_RECORDED)) {
            if (BattleSystem_IsServer(work->battleSystem) == 1 && work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                work->battlerSprites = BattleAnimBattlerSprites_NewAll(work->battleSystem, HEAP_ID_BATTLE);
            } else if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER) {
                work->battlerSprites = BattleAnimBattlerSprites_NewAll(work->battleSystem, HEAP_ID_BATTLE);
            }
        }
        work->state++;
        return;
    case 1: {
        PokepicManager *pokepicManager;
        if (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
            work->delay++;
            if (work->delay < 12) {
                break;
            }
            work->delay = 0;
        }
        pokepicManager = BattleSystem_GetPokepicManager(work->battleSystem);
        NARC_ReadPokepicAnimScript((NARC *)work->opponentData->pokepicAnimNarc, scripts, work->species, work->battlerType);
        work->opponentData->pokepic = BattleControllerOpponent_CreatePokepic(work->battleSystem, pokepicManager, &work->pokepicTemplate, ov07_022377F4[1][work->battlerType][0], ov07_022377F4[1][work->battlerType][1], ov07_022377F4[0][work->battlerType][2], work->height, work->shadowHeight, work->shadowXOffset, work->shadowSize, work->battlerId, scripts, NULL);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 0x100);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 0x100);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_YADJ_REQ, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_VANISHED, 0);
        if (work->battlerSprites) {
            BattleAnimBattlerSprites_Free(work->battlerSprites);
            work->battlerSprites = NULL;
        }
        work->state++;
        return;
    }
    case 2:
        work->animSystem = BattleAnimSystem_New(HEAP_ID_BATTLE);
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &animData, 1, 41, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &animData);
        work->state++;
        return;
    case 3:
        BattleAnimSystem_Execute(work->animSystem);
        if (BattleAnimSystem_IsActive(work->animSystem)) {
            break;
        }
        BattleAnimSystem_EndMove(work->animSystem);
        BattleAnimSystem_Delete(work->animSystem);
        work->state++;
        return;
    case 4:
        if (work->facing == 2) {
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_AFFINE, 0);
        }
        BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->opponentData->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
        work->state = 5;
        return;
    case 5:
        if (PokepicAnimManager_GetAnimState(BattleSystem_GetPokepicAnimManager(work->battleSystem), work->battlerId) != 1 || Pokepic_IsAnimFinished(work->opponentData->pokepic)) {
            break;
        }
        BallThrow_Delete(work->opponentData->ballData);
        work->opponentData->ballData = NULL;
        if (work->shiny) {
            work->animSystem = BattleAnimSystem_New(HEAP_ID_BATTLE);
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &shinyAnimData, 1, 11, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &shinyAnimData);
            work->state = 6;
        } else {
            work->state = 0xFF;
        }
        return;
    case 6:
        BattleAnimSystem_Execute(work->animSystem);
        if (BattleAnimSystem_IsActive(work->animSystem)) {
            break;
        }
        BattleAnimSystem_EndMove(work->animSystem);
        BattleAnimSystem_Delete(work->animSystem);
        work->state = 0xFF;
        return;
    default:
        sub_02005B58(0);
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_PokemonSendOut(SysTask *task, void *data) {
    BattleAnimSystem *animSystem;
    OpponentPokemonSendOutWork *work = data;
    MoveAnimData shinyAnimData;
    MoveAnimData substituteAnimData;
    BattleAnimBattlerPicParams params;
    MoveAnimData unsubstituteAnimData;
    BallThrowParams ballParams;
    PokepicAnimScript scripts[10];
    BattlePokeballOpenArgs openArgs;
    u8 state;

    animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);
    state = work->state;
    switch (state) {
    case 0:
        ballParams.kind = ov12_0226D120[work->battlerType];
        ballParams.heapID = HEAP_ID_BATTLE;
        ballParams.resTagOffset = work->battlerId;
        ballParams.ball = work->ball;
        ballParams.spriteSystem = BattleSystem_GetSpriteSystem(work->battleSystem);
        ballParams.paletteData = BattleSystem_GetPaletteData(work->battleSystem);
        ballParams.bgPriority = 1;
        ballParams.keepSubSurface = 0;
        if (work->breakOut == 1) {
            ballParams.mode = 1;
        } else {
            ballParams.mode = 0;
        }
        work->ballData = BallThrow_New(&ballParams);
        {
            PokepicManager *pokepicManager = BattleSystem_GetPokepicManager(work->battleSystem);
            NARC_ReadPokepicAnimScript((NARC *)work->opponentData->pokepicAnimNarc, scripts, work->species, work->battlerType);
            work->opponentData->pokepic = BattleControllerOpponent_CreatePokepic(work->battleSystem, pokepicManager, &work->pokepicTemplate, ov07_022377DC[work->battlerType][0], ov07_022377F4[0][work->battlerType][1], ov07_022377F4[0][work->battlerType][2], work->height, work->shadowHeight, work->shadowXOffset, work->shadowSize, work->battlerId, scripts, NULL);
        }
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_YADJ_REQ, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_VANISHED, 1);
        work->battlerSprites = NULL;
        work->battlerSprites = BattleAnimBattlerSprites_NewAll(work->battleSystem, HEAP_ID_BATTLE);
        work->state++;
        break;
    case 1:
        memset(&openArgs, 0, sizeof(openArgs));
        openArgs.battlerType = work->battlerType;
        openArgs.mon = BattleSystem_GetPartyMon(work->battleSystem, work->battlerId, work->selectedMonIndex);
        openArgs.ball = work->ball;
        work->ballOpenEffect = BallSealEffect_New(HEAP_ID_BATTLE, &openArgs);
        BallSealEffect_LoadResources(work->ballOpenEffect);
        work->state++;
        break;
    case 2:
        if ((BallThrow_GetMode(work->ballData) == 0 || work->breakOut != 0) && BallSealEffect_IsLoaded(work->ballOpenEffect) == 1 && BallThrow_HasOpened(work->ballData) == 1) {
            if (work->battlerSprites) {
                BattleAnimBattlerSprites_Free(work->battlerSprites);
            }
            Pokepic_StartPaletteFade(work->opponentData->pokepic, 16, 16, 0, sBallColors[work->ball]);
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_VANISHED, 0);
            BallSealEffect_Start(work->ballOpenEffect);
            if (work->facing == 2) {
                PlaySE_SetPan(SEQ_SE_DP_BOWA2, 117);
            } else {
                PlaySE_SetPan(SEQ_SE_DP_BOWA2, -117);
            }
            work->state++;
        }
        break;
    case 3:
        if (BallThrow_IsActive(work->ballData) != 1) {
            work->state++;
        }
    case 4:
        if (Pokepic_GetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW) == 0x100 && BallSealEffect_IsActive(work->ballOpenEffect) == 0) {
            if (work->facing == 2) {
                Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_AFFINE, 0);
            }
            BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->opponentData->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
            Pokepic_StartPaletteFade(work->opponentData->pokepic, 16, 0, 0, sBallColors[work->ball]);
            work->state = 5;
        } else if (Pokepic_GetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW) >= 0x100) {
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 0x100);
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 0x100);
            if (work->facing == 2) {
                Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_AFFINE, 0);
            }
            BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->opponentData->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
            Pokepic_StartPaletteFade(work->opponentData->pokepic, 16, 0, 1, sBallColors[work->ball]);
            work->state = 5;
        } else {
            Pokepic_AddAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 32);
            Pokepic_AddAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 32);
            Pokepic_SetYOffsetForAffineHeight(work->opponentData->pokepic, work->shadowHeight);
        }
        break;
    case 5:
        if (BallSealEffect_IsActive(work->ballOpenEffect) == 0) {
            work->state = 6;
        }
        break;
    case 6:
        if (PokepicAnimManager_GetAnimState(BattleSystem_GetPokepicAnimManager(work->battleSystem), work->battlerId) == 1 && Pokepic_IsAnimFinished(work->opponentData->pokepic) == FALSE) {
            BallThrow_Delete(work->ballData);
            BallSealEffect_Free(work->ballOpenEffect);
            if (work->shiny) {
                BattleController_InitMoveAnimData(work->battleSystem, NULL, &shinyAnimData, 1, 11, work->battlerId, work->battlerId, 0);
                OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &shinyAnimData);
                work->state = 7;
            } else {
                work->state = 8;
            }
        }
        break;
    case 7:
    case 9:
    case 11:
        BattleAnimSystem_Execute(animSystem);
        if (BattleAnimSystem_IsActive(animSystem) == FALSE) {
            BattleAnimSystem_EndMove(animSystem);
            work->state++;
        }
        break;
    case 8:
        if (work->isSubstitute) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &substituteAnimData, 1, 15, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &substituteAnimData);
            work->state++;
        } else {
            work->state = 0xFF;
        }
        break;
    case 10:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &params, work->battlerId);
        BattleAnimUtil_LoadSubstituteIntoPic(&params, HEAP_ID_BATTLE);
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &unsubstituteAnimData, 1, 16, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &unsubstituteAnimData);
        work->opponentData->unk1A0 = 1;
        work->state++;
        break;
    default:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_FollowMonSendOut(SysTask *task, void *data) {
    BattleAnimSystem *animSystem;
    OpponentPokemonSendOutWork *work = data;
    MoveAnimData sendOutAnimData;
    MoveAnimData shinyAnimData;
    MoveAnimData substituteAnimData;
    BattleAnimBattlerPicParams params;
    MoveAnimData unsubstituteAnimData;
    PokepicAnimScript scripts[10];
    u8 state;

    animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);
    state = work->state;
    switch (state) {
    case 0: {
        PokepicManager *pokepicManager = BattleSystem_GetPokepicManager(work->battleSystem);
        NARC_ReadPokepicAnimScript((NARC *)work->opponentData->pokepicAnimNarc, scripts, work->species, work->battlerType);
        work->opponentData->pokepic = BattleControllerOpponent_CreatePokepic(work->battleSystem, pokepicManager, &work->pokepicTemplate, ov07_022377F4[1][work->battlerType][0], ov07_022377F4[1][work->battlerType][1], ov07_022377F4[0][work->battlerType][2], work->height, work->shadowHeight, work->shadowXOffset, work->shadowSize, work->battlerId, scripts, NULL);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEW, 0x100);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_AFFINEH, 0x100);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_YADJ_REQ, 0);
        Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_VANISHED, 0);
        work->state++;
    } break;
    case 1:
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &sendOutAnimData, 1, 41, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &sendOutAnimData);
        work->state++;
        break;
    case 3:
        if (work->facing == 2) {
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_SHADOW_AFFINE, 0);
        }
        BattleControllerOpponent_PlayMonAnimAndCry(work->battleSystem, work->battlerId, work->battlerType, work->opponentData->pokepic, work->opponentData->pokepicAnimNarc, work->species, work->form, work->facing, work->cryParam);
        work->state = 4;
        break;
    case 4:
        if (PokepicAnimManager_GetAnimState(BattleSystem_GetPokepicAnimManager(work->battleSystem), work->battlerId) == 1 && Pokepic_IsAnimFinished(work->opponentData->pokepic) == FALSE) {
            if (work->shiny) {
                BattleController_InitMoveAnimData(work->battleSystem, NULL, &shinyAnimData, 1, 11, work->battlerId, work->battlerId, 0);
                OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &shinyAnimData);
                work->state = 5;
            } else {
                work->state = 6;
            }
        }
        break;
    case 2:
    case 5:
    case 7:
    case 9:
        BattleAnimSystem_Execute(animSystem);
        if (BattleAnimSystem_IsActive(animSystem) == FALSE) {
            BattleAnimSystem_EndMove(animSystem);
            work->state++;
        }
        break;
    case 6:
        if (work->isSubstitute) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &substituteAnimData, 1, 15, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &substituteAnimData);
            work->state++;
        } else {
            work->state = 0xFF;
        }
        break;
    case 8:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &params, work->battlerId);
        BattleAnimUtil_LoadSubstituteIntoPic(&params, HEAP_ID_BATTLE);
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &unsubstituteAnimData, 1, 16, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &unsubstituteAnimData);
        work->opponentData->unk1A0 = 1;
        work->state++;
        break;
    default:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_RecallPokemon(SysTask *task, void *data) {
    BattleAnimSystem *animSystem;
    OpponentRecallPokemonWork *work = data;
    MoveAnimData substituteAnimData;
    BattleAnimBattlerPicParams params;
    MoveAnimData unsubstituteAnimData;
    BallThrowParams enemyBallParams;
    BallThrowParams playerBallParams;
    OpponentBallKindTable ballKinds;
    u8 state;

    animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);
    state = work->state;
    switch (state) {
    case 0:
        if (work->isSubstitute) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &substituteAnimData, 1, 15, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &substituteAnimData);
            work->state++;
        } else {
            work->state = 4;
        }
        break;
    case 2:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &params, work->battlerId);
        BattleAnimUtil_RestorePic(&params, HEAP_ID_BATTLE);
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &unsubstituteAnimData, 1, 16, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &unsubstituteAnimData);
        work->opponentData->unk1A0 = 0;
        work->state++;
        break;
    case 1:
    case 3:
        BattleAnimSystem_Execute(animSystem);
        if (BattleAnimSystem_IsActive(animSystem) == FALSE) {
            BattleAnimSystem_EndMove(animSystem);
            work->state++;
        }
        break;
    case 4: {
        int battlerType = work->battlerType;
        if (battlerType & BATTLER_TYPE_IS_ENEMY) {
            enemyBallParams.kind = ov12_0226D120[battlerType];
            enemyBallParams.heapID = HEAP_ID_BATTLE;
            enemyBallParams.mode = 5;
            enemyBallParams.resTagOffset = work->battlerId;
            enemyBallParams.ball = work->ball;
            enemyBallParams.spriteSystem = BattleSystem_GetSpriteSystem(work->battleSystem);
            enemyBallParams.paletteData = BattleSystem_GetPaletteData(work->battleSystem);
            enemyBallParams.bgPriority = 1;
            enemyBallParams.keepSubSurface = 0;
            work->ballData = BallThrow_New(&enemyBallParams);
        } else {
            ballKinds = ov12_0226D128;
            playerBallParams.kind = ballKinds.kinds[battlerType];
            playerBallParams.heapID = HEAP_ID_BATTLE;
            playerBallParams.mode = 5;
            playerBallParams.resTagOffset = work->battlerId;
            playerBallParams.ball = work->ball;
            playerBallParams.spriteSystem = BattleSystem_GetSpriteSystem(work->battleSystem);
            playerBallParams.paletteData = BattleSystem_GetPaletteData(work->battleSystem);
            playerBallParams.bgPriority = 1;
            playerBallParams.keepSubSurface = 0;
            work->ballData = BallThrow_New(&playerBallParams);
        }
        work->state = 5;
        Pokepic_StartPaletteFade(work->pokepic, 0, 16, 0, sBallColors[work->ball]);
        switch (work->battlerType) {
        case BATTLER_TYPE_SOLO_PLAYER:
        case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
        case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
            PlaySE_SetPan(SEQ_SE_DP_BOWA2, -117);
            break;
        case BATTLER_TYPE_SOLO_ENEMY:
        case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
        case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
            PlaySE_SetPan(SEQ_SE_DP_BOWA2, 117);
            break;
        }
    } break;
    case 5:
        if (Pokepic_ResumePaletteFade(work->pokepic) == FALSE) {
            work->state++;
        }
        break;
    case 6:
        Pokepic_AddAttr(work->pokepic, POKEPIC_AFFINEW, -32);
        Pokepic_AddAttr(work->pokepic, POKEPIC_AFFINEH, -32);
        Pokepic_SetYOffsetForAffineHeight(work->pokepic, work->height);
        if (Pokepic_GetAttr(work->pokepic, POKEPIC_AFFINEW) <= 0) {
            Pokepic_Delete(work->pokepic);
            work->state = 7;
        }
        break;
    case 7:
        if (BallThrow_IsActive(work->ballData) == FALSE) {
            BallThrow_Delete(work->ballData);
            work->state = 8;
        }
        break;
    case 8:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_RecallFollowMon(SysTask *task, void *data) {
    OpponentRecallPokemonWork *work = data;
    MoveAnimData substituteAnimData;
    BattleAnimBattlerPicParams params;
    MoveAnimData unsubstituteAnimData;
    MoveAnimData recallAnimData;
    BattleAnimSystem *animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);

    switch (work->state) {
    case 0:
        if (work->isSubstitute) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &substituteAnimData, 1, 15, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &substituteAnimData);
            work->state++;
        } else {
            work->state = 4;
        }
        break;
    case 2:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &params, work->battlerId);
        BattleAnimUtil_RestorePic(&params, HEAP_ID_BATTLE);
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &unsubstituteAnimData, 1, 16, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &unsubstituteAnimData);
        work->opponentData->unk1A0 = 0;
        work->state++;
        break;
    case 1:
    case 3:
    case 5:
        BattleAnimSystem_Execute(animSystem);
        if (!BattleAnimSystem_IsActive(animSystem)) {
            BattleAnimSystem_EndMove(animSystem);
            work->state++;
        }
        break;
    case 4:
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &recallAnimData, 1, 42, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &recallAnimData);
        work->state++;
        break;
    case 6:
        Pokepic_Delete(work->pokepic);
    case 7:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active reset

void Task_PokemonEnterBall(SysTask *task, void *data) {
    OpponentPokepicShrinkWork *work = data;

    switch (work->state) {
    case 0:
        Pokepic_AddAttr(work->pokepic, POKEPIC_AFFINEW, -32);
        Pokepic_AddAttr(work->pokepic, POKEPIC_AFFINEH, -32);
        if (work->height != 0) {
            work->height--;
            Pokepic_AddAttr(work->pokepic, POKEPIC_Y, -1);
        }
        if (Pokepic_GetAttr(work->pokepic, POKEPIC_AFFINEW) <= 0) {
            Pokepic_Delete(work->pokepic);
            work->state++;
        }
        break;
    case 1:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_TrainerEncounter(SysTask *task, void *data) {
    OpponentTrainerEncounterWork *work = data;
    s16 x1, y1, x2, y2;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);

    if (work->bgScrollCount < 33 && work->state >= 2 && (work->battlerType == BATTLER_TYPE_SOLO_PLAYER || work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1)) {
        BgSetPosTextAndCommit(BattleSystem_GetBgConfig(work->battleSystem), GF_BG_LYR_MAIN_3, BG_POS_OP_ADD_X, 4);
        work->bgScrollCount++;
    }

    switch (work->state) {
    case 0:
        if (ManagedSprite_GetNumFrames(work->managedSprite) > 2) {
            ManagedSprite_SetAnimationFrame(work->managedSprite, 0);
            ManagedSprite_SetAnim(work->managedSprite, 2);
        }
        work->delay = 28;
        work->state = 1;
    case 1:
        if (--work->delay) {
            break;
        }
        work->state = 2;
    case 2:
        ManagedSprite_GetPositionXY(work->terrain->sprite, &x1, &y1);
        ManagedSprite_GetPositionXY(work->managedSprite, &x2, &y2);
        if (work->facing == 2) {
            if (work->battlerType == BATTLER_TYPE_SOLO_ENEMY || work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                if (x1 < 192) {
                    ManagedSprite_OffsetPositionXY(work->terrain->sprite, 8, 0);
                } else {
                    ManagedSprite_SetPositionXY(work->terrain->sprite, 192, 88);
                }
            }
            ManagedSprite_GetPositionXY(work->terrain->sprite, &x1, &y1);
            if (work->battlerType == BATTLER_TYPE_SOLO_ENEMY || (work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1 && (battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES) || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_FRONTIER) || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_LINK)))) {
                ManagedSprite_SetPositionXY(work->managedSprite, x1, y2);
            } else if (work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                x1 = x2 - x1;
                x1 -= 24;
                ManagedSprite_OffsetPositionXY(work->managedSprite, -x1, 0);
            } else if (work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_2) {
                x1 = x1 - x2;
                x1 -= 16;
                ManagedSprite_OffsetPositionXY(work->managedSprite, x1, 0);
            }
            ManagedSprite_GetPositionXY(work->managedSprite, &x2, &y2);
            if (x2 >= work->targetX) {
                ManagedSprite_SetPositionXY(work->managedSprite, work->targetX, y2);
                if (work->battlerType == BATTLER_TYPE_SOLO_ENEMY || work->battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                    ManagedSprite_SetPositionXY(work->terrain->sprite, 192, 88);
                }
                if (ManagedSprite_GetNumFrames(work->managedSprite) > 1) {
                    ManagedSprite_SetAnimationFrame(work->managedSprite, 0);
                    ManagedSprite_SetAnim(work->managedSprite, 1);
                    work->ballFlashStarted = 0;
                    work->state = 3;
                } else {
                    work->state = 4;
                }
            }
        } else {
            if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER || work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                if (x1 > 64) {
                    ManagedSprite_OffsetPositionXY(work->terrain->sprite, -8, 0);
                } else {
                    ManagedSprite_SetPositionXY(work->terrain->sprite, 64, 136);
                }
            }
            ManagedSprite_GetPositionXY(work->terrain->sprite, &x1, &y1);
            if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER || (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1 && (battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES) || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_FRONTIER) || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TAG) || battleType == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_LINK)))) {
                ManagedSprite_SetPositionXY(work->managedSprite, x1, y2);
            } else if (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                x1 = x1 - x2;
                x1 -= 24;
                ManagedSprite_OffsetPositionXY(work->managedSprite, x1, 0);
            } else if (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                x1 = x2 - x1;
                x1 -= 16;
                ManagedSprite_OffsetPositionXY(work->managedSprite, -x1, 0);
            }
            ManagedSprite_GetPositionXY(work->managedSprite, &x2, &y2);
            if (work->pokepic != NULL) {
                Pokepic_SetAttr(work->pokepic, POKEPIC_X, x2);
            }
            if (x2 <= work->targetX) {
                if (work->pokepic != NULL) {
                    Pokepic_Delete(work->pokepic);
                }
                ManagedSprite_SetPositionXY(work->managedSprite, work->targetX, y2);
                if (work->battlerType == BATTLER_TYPE_SOLO_PLAYER || work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                    ManagedSprite_SetPositionXY(work->terrain->sprite, 64, 136);
                }
                work->state = 4;
            }
        }
        break;
    case 3:
        if (ManagedSprite_IsAnimated(work->managedSprite) == 1) {
            if (OpponentTrainerEncounter_HandleAnimFrame(work, work->managedSprite) == FALSE) {
                break;
            }
        }
        work->state = 4;
    case 4:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_ThrowPokeball(SysTask *task, void *data) {
    OpponentThrowPokeballWork *work = data;
    s16 x, y;
    int animFrame;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    BallThrowParams ball;

    switch (work->state) {
    case 0:
        if (work->throwType == 0) {
            work->state = 1;
            break;
        }
        switch (work->throwType) {
        default:
        case 1:
            ball.kind = 15;
            work->ballTargetState = 3;
            ball.ball = 5;
            break;
        case 2:
            ball.kind = 12;
            work->ballTargetState = 0;
            ball.ball = 0x401;
            break;
        case 3:
            ball.kind = 13;
            work->ballTargetState = 0;
            ball.ball = 0x402;
            break;
        case 4:
            ball.kind = 15;
            work->ballTargetState = 3;
            ball.ball = 0x400;
            break;
        }
        ball.heapID = HEAP_ID_BATTLE;
        ball.mode = 4;
        ball.resTagOffset = work->battlerId;
        ball.spriteSystem = BattleSystem_GetSpriteSystem(work->battleSystem);
        ball.paletteData = BattleSystem_GetPaletteData(work->battleSystem);
        ball.keepSubSurface = 0;
        ball.battleSystem = work->battleSystem;
        work->opponentData->ballData = BallThrow_New(&ball);
        BallThrow_SetDrawPriority(work->opponentData->ballData, 100);
        BallThrow_SetPriority(work->opponentData->ballData, 2);
        BallThrow_SetAnimateSprite(work->opponentData->ballData, 0);
        ManagedSprite_SetAnimationFrame(work->opponentData->managedSprite, 0);
        ManagedSprite_SetAnim(work->opponentData->managedSprite, 1);
        ManagedSprite_SetAnimateFlag(work->opponentData->managedSprite, TRUE);
        work->state = 3;
        break;
    case 1:
        if (work->facing == 2) {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, 5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
            if (x >= 160 && BallThrow_GetMode(work->opponentData->ballData) != 0) {
                BallThrow_SetDrawFlag(work->opponentData->ballData, 1);
                BallThrow_SetMode(work->opponentData->ballData, 0);
                if ((battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI)) {
                    OpponentData *partner = BattleSystem_GetOpponentData(work->battleSystem, BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId));
                    BallThrow_SetDrawFlag(partner->ballData, 1);
                    BallThrow_SetMode(partner->ballData, 0);
                    BallThrow_SetDelay(partner->ballData, 12);
                }
            } else if (x >= 296) {
                SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
                Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
                work->opponentData->managedSprite = NULL;
                SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
                SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
                SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
                SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
                work->state = 6;
            }
        } else {
            ManagedSprite_SetAnimationFrame(work->opponentData->managedSprite, 0);
            ManagedSprite_SetAnim(work->opponentData->managedSprite, 1);
            ManagedSprite_SetAnimateFlag(work->opponentData->managedSprite, TRUE);
            work->state = 2;
        }
        break;
    case 2:
        ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, -5, 0);
        ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
        if (work->opponentData->ballData) {
            animFrame = ManagedSprite_GetAnimationFrame(work->opponentData->managedSprite);
            if (ov12_0226D1E8[work->backpic][animFrame][0] != 0x7FFF) {
                BallThrow_SetDrawFlag(work->opponentData->ballData, 1);
                BallThrow_SetPosition(work->opponentData->ballData, x + ov12_0226D1E8[work->backpic][animFrame][0], y + ov12_0226D1EA[work->backpic][animFrame][0]);
                if (animFrame == 3 && BallThrow_GetMode(work->opponentData->ballData) != 0) {
                    BallThrow_SetMode(work->opponentData->ballData, 0);
                    BallThrow_SetAnimateSprite(work->opponentData->ballData, 1);
                    if ((battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI)) {
                        OpponentData *partner = BattleSystem_GetOpponentData(work->battleSystem, BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId));
                        BallThrow_SetPosition(partner->ballData, x + ov12_0226D1E8[work->backpic][animFrame][0], y + ov12_0226D1EA[work->backpic][animFrame][0]);
                        BallThrow_SetDrawFlag(partner->ballData, 1);
                        BallThrow_SetMode(partner->ballData, 0);
                        BallThrow_SetAnimateSprite(partner->ballData, 1);
                    }
                }
            }
        }
        if (x <= -40) {
            SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
            Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
            work->opponentData->managedSprite = NULL;
            SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
            SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
            SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
            SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
            work->state = 6;
        }
        break;
    case 3:
        ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
        animFrame = ManagedSprite_GetAnimationFrame(work->opponentData->managedSprite);
        if (animFrame == 4) {
            work->delay = 8;
            work->state = 4;
        } else if (work->opponentData->ballData && ov12_0226D1E8[work->backpic][animFrame][0] != 0x7FFF) {
            BallThrow_SetPosition(work->opponentData->ballData, x + ov12_0226D1E8[work->backpic][animFrame][0], y + ov12_0226D1EA[work->backpic][animFrame][0]);
            if (animFrame == 3 && BallThrow_GetMode(work->opponentData->ballData) != work->ballTargetState) {
                BallThrow_SetMode(work->opponentData->ballData, work->ballTargetState);
                BallThrow_SetPriority(work->opponentData->ballData, 1);
                BallThrow_SetAnimateSprite(work->opponentData->ballData, 1);
                if (work->ballTargetState != 3) {
                    PlaySE(SEQ_SE_DP_THROW);
                }
            }
        }
        break;
    case 4:
        if (--work->delay == 0) {
            if (work->ballTargetState == 3) {
                work->state = 6;
            } else {
                work->state = 5;
            }
        }
        break;
    case 5:
        if (BallThrow_IsActive(work->opponentData->ballData) != 1) {
            BallThrow_Delete(work->opponentData->ballData);
            work->opponentData->ballData = NULL;
            work->state = 6;
        }
        break;
    case 6:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_ThrowPokeballWithFollowMon(SysTask *task, void *data) {
    OpponentThrowPokeballWork *work = data;
    s16 x, y;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    switch (work->state) {
    case 0:
        if (work->facing == 2) {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, 5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
            if (x >= 296) {
                SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
                Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
                work->opponentData->managedSprite = NULL;
                SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
                SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
                SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
                SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
                work->state = 2;
            }
        } else {
            ManagedSprite_SetPriority(work->opponentData->managedSprite, 1);
            ManagedSprite_SetAnimationFrame(work->opponentData->managedSprite, 0);
            ManagedSprite_SetAnim(work->opponentData->managedSprite, 1);
            ManagedSprite_SetAnimateFlag(work->opponentData->managedSprite, TRUE);
            work->state = 1;
        }
        break;
    case 1:
        ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, -5, 0);
        ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &x, &y);
        if (work->opponentData->ballData && (battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI)) {
            int frame = ManagedSprite_GetAnimationFrame(work->opponentData->managedSprite);
            if (ov12_0226D1E8[work->backpic][frame][0] != 0x7FFF) {
                OpponentData *partner = BattleSystem_GetOpponentData(work->battleSystem, BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId));
                if (frame == 3 && BallThrow_GetMode(partner->ballData) != 0) {
                    BallThrow_SetPosition(partner->ballData, x + ov12_0226D1E8[work->backpic][frame][0], y + ov12_0226D1EA[work->backpic][frame][0]);
                    BallThrow_SetDrawFlag(partner->ballData, 1);
                    BallThrow_SetMode(partner->ballData, 0);
                    BallThrow_SetAnimateSprite(partner->ballData, 1);
                    BallThrow_SetPriority(partner->ballData, 1);
                }
            }
        }
        if (x <= -40) {
            SpriteManager *mgr = BattleSystem_GetSpriteManager(work->battleSystem);
            Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
            work->opponentData->managedSprite = NULL;
            SpriteManager_UnloadCharObjById(mgr, 20015 + work->opponentData->battlerType);
            SpriteManager_UnloadPlttObjById(mgr, 20010 + work->opponentData->battlerType);
            SpriteManager_UnloadCellObjById(mgr, 20007 + work->opponentData->battlerType);
            SpriteManager_UnloadAnimObjById(mgr, 20007 + work->opponentData->battlerType);
            work->state = 2;
        }
        break;
    case 2:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_TrainerSlideOut(SysTask *task, void *data) {
    OpponentTrainerSlideOutWork *work = data;
    s16 pos[2];
    SpriteManager *spriteManager;

    switch (work->state) {
    case 0:
        if (work->facing == 2) {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, 5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &pos[1], &pos[0]);
            if (pos[1] >= 296) {
                work->state++;
            }
        } else {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, -5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &pos[1], &pos[0]);
            if (pos[1] <= -40) {
                work->state++;
            }
        }
        break;
    case 1:
        spriteManager = BattleSystem_GetSpriteManager(work->battleSystem);
        Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
        work->opponentData->managedSprite = NULL;
        SpriteManager_UnloadCharObjById(spriteManager, work->opponentData->battlerType + 20015);
        SpriteManager_UnloadPlttObjById(spriteManager, work->opponentData->battlerType + 20010);
        SpriteManager_UnloadCellObjById(spriteManager, work->opponentData->battlerType + 20007);
        SpriteManager_UnloadAnimObjById(spriteManager, work->opponentData->battlerType + 20007);
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_TrainerSlideIn(SysTask *task, void *data) {
    OpponentTrainerSlideInWork *work = data;
    s16 pos[2];

    switch (work->state) {
    case 0:
        ManagedSprite_GetPositionXY(work->managedSprite, &pos[1], &pos[0]);
        if (work->facing == 0) {
            pos[1] += 5;
            if (pos[1] >= work->targetX) {
                pos[1] = work->targetX;
                work->state++;
            }
        } else {
            pos[1] -= 5;
            if (pos[1] <= work->targetX) {
                pos[1] = work->targetX;
                work->state++;
            }
        }
        ManagedSprite_SetPositionXY(work->managedSprite, pos[1], pos[0]);
        break;
    case 1:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_HealthbarSlideIn(SysTask *task, void *data) {
    BattleHpBar *hpBar = data;
    switch (hpBar->script) {
    case 0:
        if (hpBar->delay != 0) {
            hpBar->delay--;
            break;
        }
        BattleHpBar_StartSlide(hpBar, FALSE);
        BattleHpBar_SetEnabled(hpBar, TRUE);
        hpBar->script++;
        break;
    case 1:
        if (hpBar->slideDone == 1) {
            hpBar->script++;
        }
        break;
    default:
        BattleController_EmitCommandComplete(hpBar->battleSystem, hpBar->battlerId, hpBar->command);
        hpBar->task = NULL;
        SysTask_Destroy(task);
        break;
    }
}

void Task_HealthbarSlideOut(SysTask *task, void *data) {
    BattleHpBar *hpBar = data;
    if (hpBar->script == 0) {
        if (hpBar->slideDone == 1) {
            hpBar->script++;
        }
    } else {
        BattleController_EmitCommandComplete(hpBar->battleSystem, hpBar->battlerId, hpBar->command);
        hpBar->task = NULL;
        SysTask_Destroy(task);
        BattleHpBar_SetEnabled(hpBar, FALSE);
    }
}

void Task_PlayerSelectCommand(SysTask *task, void *data) {
    OpponentSelectCommandWork *work = data;
    BattleSystem_GetBgConfig(work->battleSystem);
    BattleInput *battleInput = BattleSystem_GetBattleInput(work->battleSystem);
    OpponentData *opponent = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    int partner = BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId);
    BattleHpBar *hpBar;

    if (partner != work->battlerId) {
        hpBar = BattleSystem_GetHpBar(work->battleSystem, partner);
    } else {
        hpBar = NULL;
    }

    switch (work->state) {
    case 0: {
        BattleInputMove moveDisplay;
        int i;
        for (i = 0; i < 4; i++) {
            moveDisplay.moveNo[i] = work->moves[i];
            moveDisplay.pp[i] = work->movePPCur[i];
            moveDisplay.ppMax[i] = work->movePP[i];
        }
        BattleInput_LoadFightMenuText(BattleSystem_GetBattleInput(work->battleSystem), work->battlerType, &moveDisplay);
        work->state++;
    }
    case 1: {
        MsgData *loader = BattleSystem_GetMessageLoader(work->battleSystem);
        BattleMessage msg;
        if (battleType & BATTLE_TYPE_TUTORIAL) {
            switch (BattleSystem_GetTutorialState(work->battleSystem)) {
            case 0:
                msg.tag = TAG_NICKNAME;
                msg.param[0] = work->battlerId | (work->partySlot << 8);
                msg.id = 921;
                BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, 0);
                work->state = 3;
                break;
            case 1:
                msg.tag = TAG_NONE;
                msg.id = 1226 + BattleSystem_GetTrainerGender(work->battleSystem, 0);
                work->printerId = BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, BattleSystem_GetTextFrameDelay(work->battleSystem));
                work->state = 2;
                break;
            }
        } else {
            if (battleType & BATTLE_TYPE_SAFARI) {
                msg.tag = TAG_TRNAME;
                msg.param[0] = BattleSystem_GetTrainerIndex(work->battleSystem, work->battlerId);
                msg.id = 922;
            } else if (battleType & BATTLE_TYPE_PAL_PARK) {
                msg.tag = TAG_TRNAME;
                msg.param[0] = BattleSystem_GetTrainerIndex(work->battleSystem, work->battlerId);
                msg.id = 1222;
            } else {
                msg.tag = TAG_NICKNAME;
                msg.param[0] = work->battlerId | (work->partySlot << 8);
                msg.id = 921;
            }
            BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, 0);
            work->state = 3;
        }
        break;
    }
    case 2:
        if (TextPrinterCheckActive(work->printerId) == FALSE) {
            work->state = 3;
        }
        break;
    case 3:
        BattleHpBar_StartActiveEffect(work->hpBar);
        OpponentData_StartPokepicBobTask(opponent, work->battleSystem);
        work->state = 4;
    case 4:
        if (BattleInput_CheckFeedbackDone(battleInput) != 0) {
            NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            BattleInputMainMenu action;
            action.battlerId = work->battlerId;
            action.battlerType = work->battlerType;
            action.selectedMon = work->partySlot;
            action.hp = work->hp;
            action.maxHp = work->maxHp;
            action.monIconStatus = work->ballStatusBattler;
            action.cancelRunDisplay = BattleSystem_GetCancelRunDisplay(work->battleSystem, work->battlerId, work->switchingOrCanPickCommandMask);
            if (opponent->unk197 != 0) {
                BattleInput_SetMenuOpenDummy(battleInput, 1);
                if (BattleSystem_GetBattleSpecial(work->battleSystem) & BATTLE_SPECIAL_FIRST_RIVAL) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 6, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_PAL_PARK) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 10, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_SAFARI) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 8, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_BUG_CONTEST) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 20, 0, (int *)&action);
                } else if (action.battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2 && !(BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_MULTI)) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 4, 0, (int *)&action);
                } else {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 3, 0, (int *)&action);
                }
                ov12_0223BB64(work->battleSystem, 1);
            } else {
                if (BattleSystem_GetBattleSpecial(work->battleSystem) & BATTLE_SPECIAL_FIRST_RIVAL) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 5, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_PAL_PARK) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 9, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_SAFARI) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 7, 0, (int *)&action);
                } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_BUG_CONTEST) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 19, 0, (int *)&action);
                } else if (action.battlerType != BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 1, 0, (int *)&action);
                } else {
                    BattleInput_ChangeMenu(narc0, narc1, battleInput, 2, 0, (int *)&action);
                }
                opponent->unk197 = 1;
            }
            BattleInput_SetPartyExpPercents(battleInput, work->expPercents);
            BattleInput_UpdateBallGaugeAnimation(battleInput, work->ballStatus[0], work->ballStatus[1]);
            BattleInput_EnableBallGauge(battleInput);
            NARC_Delete(narc0);
            NARC_Delete(narc1);
            if (hpBar != NULL) {
                BattleHpBar_MenuOpenDummy(hpBar);
            }
            work->state = 5;
        }
        break;
    case 5:
        if (gSystem.newKeys & PAD_BUTTON_START) {
            int i;
            for (i = 0; i < BattleSystem_GetMaxBattlers(work->battleSystem); i++) {
                OpponentData *other = BattleSystem_GetOpponentData(work->battleSystem, i);
                BattleHpBar_ToggleHpNumbers(&other->hpBar);
            }
        }
        work->input = BattleInput_CheckTouch(battleInput);
        if (work->input != 0xFFFFFFFF) {
            work->unk_0B = 10;
            PlaySE(SEQ_SE_DP_DECIDE);
            work->state = 6;
        }
        break;
    case 6:
        if (BattleInput_CheckFeedbackDone(battleInput) == 1 || work->input == 1) {
            switch (work->input) {
            case 1:
                if (BattleSystem_GetBattleType(work->battleSystem) & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_PAL_PARK)) {
                    work->state = 7;
                }
                break;
            case 2: {
                NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
                BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
                BattleInput_SetMenuOpenDummy(battleInput, 0);
                BattleHpBar_MenuCloseDummy(hpBar);
                BattleHpBar_StopActiveEffect(work->hpBar);
                OpponentData_StopPokepicBobTask(opponent);
                NARC_Delete(narc0);
                NARC_Delete(narc1);
                break;
            }
            case 3: {
                NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
                BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
                BattleInput_SetMenuOpenDummy(battleInput, 0);
                BattleHpBar_MenuCloseDummy(hpBar);
                BattleHpBar_StopActiveEffect(work->hpBar);
                OpponentData_StopPokepicBobTask(opponent);
                NARC_Delete(narc0);
                NARC_Delete(narc1);
                break;
            }
            case 4: {
                NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
                if (BattleInput_GetCancelRunFlag(battleInput) == 1) {
                    BattleHpBar_MenuCloseDummy(hpBar);
                    work->input = 0xFF;
                }
                BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
                NARC_Delete(narc0);
                NARC_Delete(narc1);
                break;
            }
            default:
                GF_ASSERT(FALSE);
                break;
            }
            work->state = 8;
        }
        break;
    case 7:
        if (BattleInput_CheckFeedbackDone(battleInput) == 1) {
            NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
            BattleInput_SetMenuOpenDummy(battleInput, 0);
            BattleHpBar_MenuCloseDummy(hpBar);
            BattleHpBar_StopActiveEffect(work->hpBar);
            OpponentData_StopPokepicBobTask(opponent);
            BattleInput_DisableBallGauge(battleInput);
            work->state = 8;
            NARC_Delete(narc0);
            NARC_Delete(narc1);
        }
        break;
    case 8:
        if (BattleInput_IsUnusedTaskNull(battleInput) == 1) {
            BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, work->input);
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}

void Task_AISelectCommand(SysTask *task, void *data) {
    OpponentSelectCommandWork *work = data;
    u32 input = TrainerAI_SelectCommand(work->battleSystem, work->battlerId);
    BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_LinkSelectCommand(SysTask *task, void *data) {
    OpponentSelectCommandWork *work = data;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_SafariSelectCommand(SysTask *task, void *data) {
    OpponentSelectCommandWork *work = data;
    int stage = BattleSystem_GetSafariRunAttempts(work->battleSystem);
    u16 species = GetBattlerVar(BattleSystem_GetBattleContext(work->battleSystem), work->battlerId, BMON_DATA_SPECIES, NULL);
    int fleeRate = GetMonBaseStat(species, BASE_GREAT_MARSH_RATE);
    int scaled = ov12_0226D140[stage][0] * fleeRate;
    int chance = scaled / ov12_0226D141[stage][0];
    if (BattleSystem_Random(work->battleSystem) % 0xFF <= chance) {
        BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, 4);
    } else {
        BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, 5);
    }
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_PalParkSelectCommand(SysTask *task, void *data) {
    OpponentSelectCommandWork *work = data;
    BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, 4);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_RecordedSelectCommand(SysTask *task, void *data) {
    OpponentSelectCommandWork *work = data;
    u8 input;
    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &input) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    if (input == 0 || input > 4 || input == 2) {
        BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
    }
    BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_PlayerSelectMove(SysTask *task, void *data) {
    OpponentSelectMoveWork *work = data;
    BattleSystem_GetBgConfig(work->battleSystem);
    BattleInput *battleInput = BattleSystem_GetBattleInput(work->battleSystem);
    OpponentData *opponent = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
    int partner = BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId);
    BattleHpBar *hpBar;

    if (partner != work->battlerId) {
        hpBar = BattleSystem_GetHpBar(work->battleSystem, partner);
    } else {
        hpBar = NULL;
    }

    switch (work->state) {
    case 0:
        if (BattleInput_CheckFeedbackDone(battleInput) == 0) {
            break;
        }
        {
            MsgData *loader = BattleSystem_GetMessageLoader(work->battleSystem);
            BattleMessage msg;
            NARC *narc0;
            NARC *narc1;
            BattleInputFightMenu menu;
            int i;

            msg.tag = TAG_NICKNAME;
            msg.param[0] = work->battlerId | (work->selectedMonIndex << 8);
            msg.id = 921;
            BattleSystem_PrintBattleMessage(work->battleSystem, loader, &msg, 0);
            BattleInput_EnableBallGauge(battleInput);
            narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            for (i = 0; i < 4; i++) {
                menu.moveNo[i] = work->moves[i];
                menu.pp[i] = work->movePPCur[i];
                menu.ppMax[i] = work->movePP[i];
            }
            menu.battlerType = work->battlerType;
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 11, 0, (int *)&menu);
            NARC_Delete(narc0);
            NARC_Delete(narc1);
            work->state++;
            return;
        }
    case 1:
        work->input = BattleInput_CheckTouch(battleInput);
        if (work->input == 0xFFFFFFFF) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        work->state++;
        return;
    case 2:
        if (work->input != 0xFF) {
            if (!(BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_DOUBLES)) {
                BattleInput_SetMenuOpenDummy(battleInput, 0);
                BattleHpBar_MenuCloseDummy(hpBar);
                BattleHpBar_StopActiveEffect(work->hpBar);
                OpponentData_StopPokepicBobTask(opponent);
            }
            BattleInput_DisableBallGauge(battleInput);
        }
        BattleController_ReplyMoveSelection(work->battleSystem, work->battlerId, work->input);
        work->state++;
        return;
    case 3:
        work->state++;
    default:
        if (BattleInput_IsUnusedTaskNull(battleInput) == 1) {
            BattleSystem_SetTutorialState(work->battleSystem, 1);
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}

void Task_AISelectMove(SysTask *task, void *data) {
    OpponentSelectMoveWork *work = data;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    BattleContext *ctx = BattleSystem_GetBattleContext(work->battleSystem);
    int selected;

    if ((battleType & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_ROAMER)) || (BattleSystem_GetBattleSpecial(work->battleSystem) & BATTLE_SPECIAL_FIRST_RIVAL)
        || BattleSystem_GetFieldSide(work->battleSystem, work->battlerId) == 0) {
        selected = TrainerAI_SelectMove(work->battleSystem, work->battlerId);
        if (selected == 0xFF) {
            return;
        }
        selected++;
    } else {
        int choices[4];
        int count = 0;
        int i = count;
        int *choice = choices;
        int moveTarget;

        for (; i < 4; i++) {
            if (!(MaskOfFlagNo(i) & work->unusableMovesMask)) {
                *choice++ = i + 1;
                count++;
            }
        }
        selected = choices[BattleSystem_Random(work->battleSystem) % count];
        moveTarget = BattleSystem_GetMoveTarget(work->battleSystem, ctx, work->battlerId, work->moves[selected - 1], 1, 0);
        BattleContext_Set(work->battleSystem, ctx, 11, work->battlerId, moveTarget);
    }
    BattleController_ReplyMoveSelection(work->battleSystem, work->battlerId, selected);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_LinkSelectMove(SysTask *task, void *data) {
    OpponentSelectMoveWork *work = data;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_RecordedSelectMove(SysTask *task, void *data) {
    OpponentSelectMoveWork *work = data;
    u8 input;
    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &input) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    if (input == 0 || input > 4) {
        BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
    } else {
        u16 move = GetBattlerVar(BattleSystem_GetBattleContext(work->battleSystem), work->battlerId, BMON_DATA_MOVE1 + input - 1, NULL);
        if (move == MOVE_NONE || move > NUM_MOVES) {
            BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
        }
    }
    BattleController_ReplyMoveSelection(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

#pragma force_active on

void Task_PlayerSelectTarget(SysTask *task, void *data) {
    OpponentSelectTargetWork *work = data;
    BattleSystem_GetBgConfig(work->battleSystem);
    OpponentData *opponent = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
    BattleInput *battleInput = BattleSystem_GetBattleInput(work->battleSystem);
    int partner = BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId);
    BattleHpBar *hpBar;

    if (partner != work->battlerId) {
        hpBar = BattleSystem_GetHpBar(work->battleSystem, partner);
    } else {
        hpBar = NULL;
    }

    switch (work->state) {
    case 0:
        if (BattleInput_CheckFeedbackDone(battleInput) != 0) {
            NARC *narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            BattleInputTargetMenu menu;
            int i;

            for (i = 0; i < 4; i++) {
                menu.targetMons[i] = work->targetMons[i];
            }
            menu.battlerType = work->battlerType;
            menu.selectionType = BattleInput_GetTargetSelectionType(work->moveRange, work->battlerType);
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 12, 0, (int *)&menu);
            NARC_Delete(narc0);
            NARC_Delete(narc1);
            work->state++;
        }
        break;
    case 1:
        work->input = BattleInput_CheckTouch(battleInput);
        if (work->input != -1) {
            PlaySE(SEQ_SE_DP_DECIDE);
            work->state++;
        }
        break;
    case 2:
        if (work->input != 0xFF) {
            BattleHpBar_StopActiveEffect(work->hpBar);
            OpponentData_StopPokepicBobTask(opponent);
            BattleHpBar_MenuCloseDummy(hpBar);
            if (work->unk_32 == 1) {
                BattleInput_SetMenuOpenDummy(battleInput, 0);
            }
        }
        work->state++;
        break;
    case 3:
        work->state++;
    default:
        if (BattleInput_IsUnusedTaskNull(battleInput) == 1) {
            int input = work->input;
            u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
            u8 order[6];
            if (input != 0xFF) {
                BattleSystem_GetBattlerIdsByType(work->battleSystem, order);
                if (battleType & BATTLE_TYPE_DOUBLES) {
                    input = order[work->input + 1] + 1;
                } else {
                    input = order[work->input - 1] + 1;
                }
            }
            BattleController_ReplyTargetSelection(work->battleSystem, work->battlerId, input);
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}

#pragma force_active reset

void Task_AISelectTarget(SysTask *task, void *data) {
    OpponentSelectTargetWork *work = data;
    int input = BattleContext_Get(work->battleSystem, BattleSystem_GetBattleContext(work->battleSystem), 11, work->battlerId) + 1;
    GF_ASSERT(input < 5);
    BattleController_ReplyTargetSelection(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_LinkSelectTarget(SysTask *task, void *data) {
    OpponentSelectTargetWork *work = data;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_RecordedSelectTarget(SysTask *task, void *data) {
    OpponentSelectTargetWork *work = data;
    u8 input;
    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &input) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    if (input == 0 || input > 4) {
        BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
    } else {
        int target = input - 1;
        switch (work->moveRange) {
        case RANGE_SINGLE_TARGET:
            if (work->battlerId == target) {
                BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
            }
            break;
        case RANGE_SINGLE_TARGET_USER_SIDE:
            if (BattleSystem_GetFieldSide(work->battleSystem, work->battlerId) != BattleSystem_GetFieldSide(work->battleSystem, target)) {
                BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
            }
            break;
        case RANGE_SINGLE_TARGET_SPECIAL:
        case RANGE_RANDOM_OPPONENT:
        case RANGE_ADJACENT_OPPONENTS:
        case RANGE_ALL_ADJACENT:
        case RANGE_USER:
        case RANGE_USER_SIDE:
        case RANGE_FIELD:
        case RANGE_OPPONENT_SIDE:
        case RANGE_ALLY:
        case RANGE_FRONT:
            break;
        }
    }
    BattleController_ReplyTargetSelection(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

// Can the item be used on the selected party slot (an active battler)?
#define ITEM_TARGET_ON_FIELD(work) (((work)->partyMenu->args->selectedSlot < 2 && (BattleSystem_GetBattleType((work)->battleSystem) == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES) || BattleSystem_GetBattleType((work)->battleSystem) == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TAG))) || (work)->partyMenu->args->selectedSlot < 1)

#pragma force_active on

void Task_PlayerSelectItem(SysTask *task, void *data) {
    OpponentSelectItemWork *work = data;
    MoveAnimData selfAnimData;
    MoveAnimData monAnimData;
    MoveAnimData battleItemAnimData;
    MoveAnimData restoreAnimData;
    BattleMessage usedMsg;
    BattleMessage battleItemMsg;
    BattleMessage hpMsg;
    BattleMessage statusMsg;
    BattleItemInput input;
    PaletteData *palette = BattleSystem_GetPaletteData(work->battleSystem);

    switch (work->state) {
    case 0:
        work->key = BattleInput_GetKeyPressed(BattleSystem_GetBattleInput(work->battleSystem));
        PaletteBlink_SetState(BattleSystem_GetMessageIcon(work->battleSystem), 1);
        PaletteData_BeginPaletteFade(palette, 5, 0xC00, -8, 0, 7, 0);
        PaletteData_BeginPaletteFade(palette, 10, 0xFFFF, -8, 0, 16, 0);
        work->state++;
        break;
    case 1:
        if (!PaletteData_GetSelectedBuffersBitmask(palette)) {
            BattleSystem_FreeInputForSubMenu(work->battleSystem);
            work->bagMenu = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleBagMenuArgs));
            memset(work->bagMenu, 0, sizeof(BattleBagMenuArgs));
            work->bagMenu->battleSystem = work->battleSystem;
            work->bagMenu->playerProfile = BattleSystem_GetPlayerProfile(work->battleSystem, work->battlerId);
            work->bagMenu->heapID = HEAP_ID_BATTLE;
            work->bagMenu->done = FALSE;
            work->bagMenu->bag = BattleSystem_GetBag(work->battleSystem);
            work->bagMenu->battlerId = work->battlerId;
            work->bagMenu->key = work->key;
            work->bagMenu->unk_22 = work->unk_14;
            work->bagMenu->targetSemiInvulnerable = work->targetSemiInvulnerable;
            work->bagMenu->targetSubstitute = work->targetSubstitute;
            work->bagMenu->embargoFlag = work->embargoFlag[work->battlerId];
            BattleBagMenu_Create(work->bagMenu);
            work->state = 3;
        }
        break;
    case 2:
        work->bagMenu->key = work->key;
        BattleBagMenu_Create(work->bagMenu);
        work->state++;
    case 3:
        if (work->bagMenu->done) {
            work->bagMenu->done = FALSE;
            work->key = work->bagMenu->key;
            if (work->bagMenu->item) {
                switch (work->bagMenu->pocket) {
                case BTLPOCKETLIST_HP_PP_RESTORE:
                case BTLPOCKETLIST_STATUS_RESTORE:
                    work->state = 4;
                    break;
                case BTLPOCKETLIST_BALLS:
                case BTLPOCKETLIST_BATTLE_ITEMS:
                    work->state = 6;
                    break;
                }
            } else {
                work->state = 6;
            }
        }
        break;
    case 4: {
        Party *party = BattleSystem_GetParty(work->battleSystem, work->battlerId);
        int battlerId;
        int i;
        if ((BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_DOUBLES) && !(BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_MULTI)) {
            battlerId = work->battlerId & 1;
        } else {
            battlerId = work->battlerId;
        }
        Party_InitWithMaxSize(work->partyMenu->args->party, PARTY_SIZE);
        for (i = 0; i < Party_GetCount(party); i++) {
            Party_AddMon(work->partyMenu->args->party, BattleSystem_GetPartyMon(work->battleSystem, battlerId, work->partyOrder[battlerId][i]));
            work->partyMenu->args->partyOrder[i] = work->partyOrder[battlerId][i];
        }
        work->partyMenu->args->battleSystem = work->battleSystem;
        work->partyMenu->args->heapID = HEAP_ID_BATTLE;
        work->partyMenu->args->selectedSlot = 0;
        work->partyMenu->args->done = FALSE;
        work->partyMenu->args->move = MOVE_NONE;
        work->partyMenu->args->mode = 2;
        work->partyMenu->args->item = work->bagMenu->item;
        work->partyMenu->args->pocket = work->bagMenu->pocket;
        work->partyMenu->args->battlerId = work->bagMenu->battlerId;
        work->partyMenu->args->key = work->key;
        work->partyMenu->args->selectedMonIndex = work->partyMenu->selectedMonIndex[work->battlerId];
        work->partyMenu->args->partnerSelectedMonIndex = work->partyMenu->selectedMonIndex[BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId)];
        if (work->battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
            work->partyMenu->args->embargoFlag = work->embargoFlag[BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId)];
            work->partyMenu->args->partnerEmbargoFlag = work->embargoFlag[work->battlerId];
        } else {
            work->partyMenu->args->embargoFlag = work->embargoFlag[work->battlerId];
            work->partyMenu->args->partnerEmbargoFlag = work->embargoFlag[BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId)];
        }
        BATTLE_PARTY_MENU_START(work->partyMenu->args);
        work->state++;
    } break;
    case 5:
        if (work->partyMenu->args->done) {
            work->key = work->partyMenu->args->key;
            work->partyMenu->args->done = FALSE;
            if (work->partyMenu->args->selectedSlot == PARTY_SIZE) {
                work->state = 2;
            } else {
                work->state = 6;
            }
        }
        break;
    case 6:
        BattleSystem_RestoreInputAfterSubMenu(work->battleSystem);
        BattleInput_SetKeyPressed(BattleSystem_GetBattleInput(work->battleSystem), work->key);
        PaletteData_BeginPaletteFade(palette, 5, 0xC00, -8, 7, 0, 0);
        PaletteData_BeginPaletteFade(palette, 10, 0xFFFF, -8, 16, 0, 0);
        work->state++;
        break;
    case 7:
        if (!PaletteData_GetSelectedBuffersBitmask(palette)) {
            PaletteBlink_SetState(BattleSystem_GetMessageIcon(work->battleSystem), 0);
            if (work->bagMenu->item) {
                work->state = 9;
                switch (work->bagMenu->pocket) {
                case BTLPOCKETLIST_STATUS_RESTORE:
                    // item is ITEM_REVIVE or ITEM_MAX_REVIVE (written as a u16 wrap-around add)
                    if ((u16)(work->bagMenu->item + (0x10000 - ITEM_REVIVE)) <= ITEM_MAX_REVIVE - ITEM_REVIVE) {
                        work->state = 8;
                    } else if (ITEM_TARGET_ON_FIELD(work)) {
                        if (work->bagMenu->item == ITEM_FULL_RESTORE) {
                            if (BattleSystem_AreBattleAnimationsOn(work->battleSystem) == TRUE) {
                                work->nextState = 17;
                            } else {
                                PlaySE_SetPan(SEQ_SE_DP_KAIFUKU, -117);
                                work->nextState = 21;
                            }
                        } else {
                            if (BattleSystem_AreBattleAnimationsOn(work->battleSystem) == TRUE) {
                                work->nextState = 25;
                            } else {
                                PlaySE_SetPan(SEQ_SE_DP_KAIFUKU, -117);
                                work->nextState = 29;
                            }
                        }
                    } else {
                        work->state = 8;
                    }
                    break;
                case BTLPOCKETLIST_HP_PP_RESTORE:
                    if (ITEM_TARGET_ON_FIELD(work) && GetItemAttr(work->bagMenu->item, ITEMATTR_HP_RESTORE, HEAP_ID_BATTLE)) {
                        if (BattleSystem_AreBattleAnimationsOn(work->battleSystem) == TRUE) {
                            work->nextState = 17;
                        } else {
                            PlaySE_SetPan(SEQ_SE_DP_KAIFUKU, -117);
                            work->nextState = 21;
                        }
                    } else {
                        work->state = 8;
                    }
                    break;
                case BTLPOCKETLIST_BATTLE_ITEMS:
                    // item is ITEM_POKE_DOLL or ITEM_FLUFFY_TAIL
                    if ((u16)(work->bagMenu->item + (0x10000 - ITEM_POKE_DOLL)) <= ITEM_FLUFFY_TAIL - ITEM_POKE_DOLL) {
                        work->state = 8;
                    } else if (work->bagMenu->item == ITEM_GUARD_SPEC_) {
                        if (BattleSystem_AreBattleAnimationsOn(work->battleSystem) == TRUE) {
                            work->nextState = 13;
                        } else {
                            PlaySE_SetPan(SEQ_SE_DP_KAIFUKU, -117);
                            work->nextState = 15;
                        }
                    } else {
                        if (BattleSystem_AreBattleAnimationsOn(work->battleSystem) == TRUE) {
                            work->nextState = 11;
                        } else {
                            PlaySE_SetPan(SEQ_SE_DP_KAIFUKU, -117);
                            work->nextState = 15;
                        }
                    }
                    break;
                case BTLPOCKETLIST_BALLS:
                    work->state = 8;
                    break;
                }
            } else {
                work->state = 8;
            }
            if (work->state != 8) {
                BattleSystem_SetTrainerAIOverlayLoaded(work->battleSystem, 0);
            }
        }
        break;
    case 8:
        if (work->bagMenu->item == ITEM_NONE) {
            input.item = 0xFF;
        } else {
            input.item = work->bagMenu->item;
            input.pocket = work->bagMenu->pocket;
            if (input.pocket <= BTLPOCKETLIST_STATUS_RESTORE) {
                input.target = work->partyMenu->args->partyOrder[work->partyMenu->args->selectedSlot] + 1;
            }
        }
        REPLY_ITEM_SELECTION(work->battleSystem, work->battlerId, input);
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work->partyMenu->args->party);
        Heap_Free(work->partyMenu->args);
        Heap_Free(work->partyMenu);
        Heap_Free(work->bagMenu);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    case 9: {
        MsgData *msgData;
        usedMsg.id = 1206;
        usedMsg.tag = TAG_ITEM;
        usedMsg.param[0] = work->bagMenu->item;
        msgData = BattleSystem_GetMessageLoader(work->battleSystem);
        work->printerId = BattleSystem_PrintBattleMessage(work->battleSystem, msgData, &usedMsg, BattleSystem_GetTextFrameDelay(work->battleSystem));
        work->timer = 30;
        work->state++;
    } break;
    case 10:
        if (!TextPrinterCheckActive(work->printerId)) {
            work->timer--;
            if (work->timer == 0) {
                work->state = work->nextState;
            }
        }
        break;
    case 11: {
        OpponentData *opponentData;
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &selfAnimData, 1, 9, work->battlerId, work->battlerId, 0);
        opponentData = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
        OpponentData_StartMoveAnimation(work->battleSystem, opponentData, (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem), &selfAnimData);
        work->state++;
    } break;
    case 17:
    case 25: {
        int battlerId = work->partyMenu->args->selectedSlot * 2;
        OpponentData *opponentData;
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &monAnimData, 1, 9, battlerId, battlerId, 0);
        opponentData = BattleSystem_GetOpponentData(work->battleSystem, battlerId);
        OpponentData_StartMoveAnimation(work->battleSystem, opponentData, (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem), &monAnimData);
        work->state++;
    } break;
    case 13: {
        OpponentData *opponentData;
        if (work->bagMenu->item == ITEM_GUARD_SPEC_) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &battleItemAnimData, 0, 0, work->battlerId, work->battlerId, MOVE_MIST);
            opponentData = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
            OpponentData_StartMoveAnimation(work->battleSystem, opponentData, (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem), &battleItemAnimData);
        } else if (work->bagMenu->item == ITEM_DIRE_HIT) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &battleItemAnimData, 0, 0, work->battlerId, work->battlerId, MOVE_FOCUS_ENERGY);
            opponentData = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
            OpponentData_StartMoveAnimation(work->battleSystem, opponentData, (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem), &battleItemAnimData);
        } else {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &battleItemAnimData, 1, 12, work->battlerId, work->battlerId, 0);
            opponentData = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
            OpponentData_StartMoveAnimation(work->battleSystem, opponentData, (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem), &battleItemAnimData);
        }
        work->state++;
    } break;
    case 15: {
        MsgData *msgData;
        battleItemMsg.id = 1203;
        battleItemMsg.tag = 12;
        battleItemMsg.param[0] = work->battlerId | (work->partyMenu->selectedMonIndex[work->battlerId] << 8);
        switch (work->bagMenu->item) {
        case ITEM_X_ATTACK:
            battleItemMsg.param[1] = 1;
            break;
        case ITEM_X_DEFENSE:
            battleItemMsg.param[1] = 2;
            break;
        case ITEM_X_SPEED:
            battleItemMsg.param[1] = 3;
            break;
        case ITEM_X_ACCURACY:
            battleItemMsg.param[1] = 6;
            break;
        case ITEM_X_SPECIAL:
            battleItemMsg.param[1] = 4;
            break;
        case ITEM_X_SP__DEF:
            battleItemMsg.param[1] = 5;
            break;
        case ITEM_GUARD_SPEC_:
            battleItemMsg.id = 1204;
            battleItemMsg.tag = TAG_NONE;
            break;
        case ITEM_DIRE_HIT:
            battleItemMsg.id = 1205;
            battleItemMsg.tag = TAG_NICKNAME;
            break;
        }
        msgData = BattleSystem_GetMessageLoader(work->battleSystem);
        work->printerId = BattleSystem_PrintBattleMessage(work->battleSystem, msgData, &battleItemMsg, BattleSystem_GetTextFrameDelay(work->battleSystem));
        work->timer = 30;
        work->state++;
    } break;
    case 19:
    case 27: {
        int battlerId = work->partyMenu->args->selectedSlot * 2;
        OpponentData *opponentData;
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &restoreAnimData, 1, 14, battlerId, battlerId, 0);
        opponentData = BattleSystem_GetOpponentData(work->battleSystem, battlerId);
        OpponentData_StartMoveAnimation(work->battleSystem, opponentData, (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem), &restoreAnimData);
        work->state++;
    } break;
    case 21: {
        int battlerId = work->partyMenu->args->selectedSlot * 2;
        int slot = work->partyMenu->args->partyOrder[work->partyMenu->args->selectedSlot];
        BattleHpBar *hpBar = BattleSystem_GetHpBar(work->battleSystem, battlerId);
        Pokemon *mon;
        u8 battlerType;
        MI_CpuFill8(hpBar, 0, 1);
        battlerType = BattleSystem_GetBattlerType(work->battleSystem, battlerId);
        hpBar->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(battlerType, BattleSystem_GetBattleType(work->battleSystem));
        mon = BattleSystem_GetPartyMon(work->battleSystem, battlerId, slot);
        hpBar->hp = GetMonData(mon, MON_DATA_HP, NULL) - work->partyMenu->args->hp;
        hpBar->maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
        hpBar->gainedHp = work->partyMenu->args->hp;
        if (GetMonData(mon, MON_DATA_STATUS, NULL) == 0) {
            hpBar->status = 0;
        }
        BattleHpBar_InitHpChange(hpBar, hpBar->gainedHp);
        work->state++;
    } break;
    case 22: {
        BattleHpBar *hpBar = BattleSystem_GetHpBar(work->battleSystem, work->partyMenu->args->selectedSlot * 2);
        if (BattleHpBar_UpdateHpChange(hpBar) == -1) {
            BattleHpBar_Draw(hpBar, 0, 0x100);
            work->state++;
        }
    } break;
    case 23: {
        MsgData *msgData = BattleSystem_GetMessageLoader(work->battleSystem);
        int battlerId = work->partyMenu->args->selectedSlot * 2;
        if (work->partyMenu->args->hp) {
            hpMsg.id = 1214;
            hpMsg.tag = 17;
            hpMsg.param[0] = battlerId | (work->partyMenu->selectedMonIndex[battlerId] << 8);
            hpMsg.param[1] = work->partyMenu->args->hp;
        } else {
            hpMsg.id = 1250;
            hpMsg.tag = TAG_NICKNAME;
            hpMsg.param[0] = battlerId | (work->partyMenu->selectedMonIndex[battlerId] << 8);
        }
        work->printerId = BattleSystem_PrintBattleMessage(work->battleSystem, msgData, &hpMsg, BattleSystem_GetTextFrameDelay(work->battleSystem));
        work->timer = 30;
        work->state++;
    } break;
    case 29: {
        int count = 0;
        int status;
        int battlerId = work->partyMenu->args->selectedSlot * 2;
        BattleHpBar *hpBar = BattleSystem_GetHpBar(work->battleSystem, battlerId);
        MsgData *msgData;
        if (GetMonData(BattleSystem_GetPartyMon(work->battleSystem, battlerId, work->partyMenu->args->partyOrder[work->partyMenu->args->selectedSlot]), MON_DATA_STATUS, NULL) == 0) {
            hpBar->status = 0;
        }
        BattleHpBar_Draw(hpBar, hpBar->hp, 0x100);
        statusMsg.tag = TAG_NICKNAME;
        statusMsg.param[0] = battlerId | (work->partyMenu->selectedMonIndex[battlerId] << 8);
        if (GetItemAttr(work->bagMenu->item, ITEMATTR_SLP_HEAL, HEAP_ID_BATTLE)) {
            status = 0;
            count++;
        }
        if (GetItemAttr(work->bagMenu->item, ITEMATTR_PSN_HEAL, HEAP_ID_BATTLE)) {
            status = 1;
            count++;
        }
        if (GetItemAttr(work->bagMenu->item, ITEMATTR_BRN_HEAL, HEAP_ID_BATTLE)) {
            status = 2;
            count++;
        }
        if (GetItemAttr(work->bagMenu->item, ITEMATTR_FRZ_HEAL, HEAP_ID_BATTLE)) {
            status = 3;
            count++;
        }
        if (GetItemAttr(work->bagMenu->item, ITEMATTR_PRZ_HEAL, HEAP_ID_BATTLE)) {
            status = 4;
            count++;
        }
        if (GetItemAttr(work->bagMenu->item, ITEMATTR_CFS_HEAL, HEAP_ID_BATTLE)) {
            status = 5;
            count++;
        }
        if (GetItemAttr(work->bagMenu->item, ITEMATTR_INF_HEAL, HEAP_ID_BATTLE)) {
            status = 6;
            count++;
        }
        if (count != 1) {
            statusMsg.id = 1229;
        } else {
            switch (status) {
            case 0:
                statusMsg.id = 1210;
                break;
            case 1:
                statusMsg.id = 1207;
                break;
            case 2:
                statusMsg.id = 1209;
                break;
            case 3:
                statusMsg.id = 1211;
                break;
            case 4:
                statusMsg.id = 1208;
                break;
            case 5:
                statusMsg.id = 1212;
                break;
            case 6:
                statusMsg.id = 1213;
                break;
            }
        }
        msgData = BattleSystem_GetMessageLoader(work->battleSystem);
        work->printerId = BattleSystem_PrintBattleMessage(work->battleSystem, msgData, &statusMsg, BattleSystem_GetTextFrameDelay(work->battleSystem));
        work->timer = 30;
        work->state++;
    } break;
    case 12:
    case 14:
    case 18:
    case 20:
    case 26:
    case 28:
        BattleAnimSystem_Execute((BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem));
        if (!BattleAnimSystem_IsActive((BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem))) {
            BattleAnimSystem_EndMove((BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem));
            work->state++;
        }
        break;
    case 16:
    case 24:
    case 30:
        if (!TextPrinterCheckActive(work->printerId)) {
            work->timer--;
            if (work->timer == 0) {
                BattleSystem_SetTrainerAIOverlayLoaded(work->battleSystem, 1);
                work->state = 8;
            }
        }
        break;
    }
}

#pragma force_active reset

void Task_AISelectItem(SysTask *task, void *data) {
    OpponentSelectItemWork *work = data;
    BattleItemInput input;
    input.item = 1;
    REPLY_ITEM_SELECTION(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work->partyMenu->args->party);
    Heap_Free(work->partyMenu->args);
    Heap_Free(work->partyMenu);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_LinkSelectItem(SysTask *task, void *data) {
    OpponentSelectItemWork *work = data;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work->partyMenu->args->party);
    Heap_Free(work->partyMenu->args);
    Heap_Free(work->partyMenu);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_RecordedSelectItem(SysTask *task, void *data) {
    OpponentSelectItemWork *work = data;

    struct {
        u8 value;
        u8 padding;
        BattleItemInput input;
    } recorded;

    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &recorded.value) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    recorded.input.item = recorded.value;
    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &recorded.value) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    recorded.input.item |= recorded.value << 8;
    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &recorded.value) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    recorded.input.pocket = recorded.value & 0xF;
    recorded.input.target = 0;
    REPLY_ITEM_SELECTION(work->battleSystem, work->battlerId, recorded.input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work->partyMenu->args->party);
    Heap_Free(work->partyMenu->args);
    Heap_Free(work->partyMenu);
    Heap_Free(work);
    SysTask_Destroy(task);
}

#pragma force_active on

void Task_PlayerShowMonList(SysTask *task, void *data) {
    OpponentShowMonListWork *work = data;
    PaletteData *palette = BattleSystem_GetPaletteData(work->battleSystem);
    int firstBattler;
    int secondBattler;
    int i;
    int slot;
    Pokemon *mon;
    Party *party;

    switch (work->state) {
    case 0: {
        Window *window = BattleSystem_GetWindow(work->battleSystem, 0);
        FillWindowPixelBuffer(window, 0xFF);
        CopyWindowPixelsToVram_TextMode(window);
    }
        work->key = BattleInput_GetKeyPressed(BattleSystem_GetBattleInput(work->battleSystem));
        PaletteBlink_SetState(BattleSystem_GetMessageIcon(work->battleSystem), 1);
        PaletteData_BeginPaletteFade(palette, 5, 0xC00, -8, 0, 7, 0);
        PaletteData_BeginPaletteFade(palette, 10, 0xFFFF, -8, 0, 16, 0);
        work->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(palette)) {
            break;
        }
        BattleSystem_FreeInputForSubMenu(work->battleSystem);
        work->partyMenu = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattlePartyMenuArgs));
        work->partyMenu->party = SaveArray_Party_Alloc(HEAP_ID_BATTLE);
        if ((BattleSystem_GetBattleType(work->battleSystem) & (BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI)) == (BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI) || BattleSystem_GetBattleType(work->battleSystem) == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI | BATTLE_TYPE_FRONTIER)) {
            // multi battles list both trainers' Pokemon, interleaved
            if (BattleSystem_GetBattlerType(work->battleSystem, work->battlerId) == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                firstBattler = work->battlerId;
                secondBattler = BattleSystem_GetBattlerIdPartner(work->battleSystem, firstBattler);
            } else {
                firstBattler = BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId);
                secondBattler = work->battlerId;
            }
            mon = AllocMonZeroed(HEAP_ID_BATTLE);
            for (i = 0; i < PARTY_SIZE; i++) {
                Party_AddMon(work->partyMenu->party, mon);
            }
            Heap_Free(mon);
            i = 0;
            if (BattleSystem_GetPartySize(work->battleSystem, firstBattler) > 0) {
                // walks the battler's partyOrder row through a work pointer shifted one byte per Pokemon
                OpponentShowMonListWork *row = (OpponentShowMonListWork *)((u8 *)work + PARTY_SIZE * firstBattler);
                slot = 0;
                do {
                    mon = BattleSystem_GetPartyMon(work->battleSystem, firstBattler, row->partyOrder[0][0]);
                    CopyPokemonToPokemon(mon, Party_GetMonByIndex(work->partyMenu->party, slot));
                    work->partyMenu->partyOrder[slot] = row->partyOrder[0][0];
                    row = (OpponentShowMonListWork *)((u8 *)row + 1);
                    slot += 2;
                    i++;
                } while (i < BattleSystem_GetPartySize(work->battleSystem, firstBattler));
            }
            i = 0;
            if (BattleSystem_GetPartySize(work->battleSystem, secondBattler) > 0) {
                int partySlot;
                int orderSlot;
                OpponentShowMonListWork *row = (OpponentShowMonListWork *)((u8 *)work + PARTY_SIZE * secondBattler);
                partySlot = 1;
                orderSlot = 0;
                do {
                    mon = BattleSystem_GetPartyMon(work->battleSystem, secondBattler, row->partyOrder[0][0]);
                    CopyPokemonToPokemon(mon, Party_GetMonByIndex(work->partyMenu->party, partySlot));
                    work->partyMenu->partyOrder[orderSlot + 1] = row->partyOrder[0][0];
                    partySlot += 2;
                    row = (OpponentShowMonListWork *)((u8 *)row + 1);
                    orderSlot += 2;
                    i++;
                } while (i < BattleSystem_GetPartySize(work->battleSystem, secondBattler));
            }
            if (BattleSystem_GetBattlerType(work->battleSystem, work->battlerId) == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                work->partyMenu->selectedSlot = 1;
            } else {
                work->partyMenu->selectedSlot = 0;
            }
        } else {
            if ((BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_DOUBLES) && !(BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_MULTI)) {
                firstBattler = work->battlerId & 1;
            } else {
                firstBattler = work->battlerId;
            }
            work->partyMenu->selectedSlot = BattleSystem_GetBattlerType(work->battleSystem, work->battlerId) == BATTLER_TYPE_PLAYER_SIDE_SLOT_2 ? 1 : 0;
            party = BattleSystem_GetParty(work->battleSystem, work->battlerId);
            for (i = 0; i < Party_GetCount(party); i++) {
                Party_AddMon(work->partyMenu->party, BattleSystem_GetPartyMon(work->battleSystem, firstBattler, work->partyOrder[firstBattler][i]));
                work->partyMenu->partyOrder[i] = work->partyOrder[firstBattler][i];
            }
        }
        work->partyMenu->battleSystem = work->battleSystem;
        work->partyMenu->heapID = HEAP_ID_BATTLE;
        work->partyMenu->done = FALSE;
        work->partyMenu->move = work->trapped;
        work->partyMenu->reservedMon = work->reservedMon;
        work->partyMenu->mode = work->mode;
        work->partyMenu->item = work->item;
        work->partyMenu->battlerId = work->battlerId;
        work->partyMenu->key = work->key;
        if (!(MaskOfFlagNo(work->battlerId) & work->switchInFlag)) {
            work->partyMenu->selectedMonIndex = work->selectedMonIndex[work->battlerId];
        } else {
            work->partyMenu->selectedMonIndex = PARTY_SIZE;
        }
        if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_MULTI) {
            work->partyMenu->partnerSelectedMonIndex = PARTY_SIZE;
        } else if (!(MaskOfFlagNo(BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId)) & work->switchInFlag)) {
            work->partyMenu->partnerSelectedMonIndex = work->selectedMonIndex[BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId)];
        } else {
            work->partyMenu->partnerSelectedMonIndex = PARTY_SIZE;
        }
        BATTLE_PARTY_MENU_START(work->partyMenu);
        work->state++;
        break;
    case 2:
        if (!work->partyMenu->done) {
            break;
        }
        BattleSystem_RestoreInputAfterSubMenu(work->battleSystem);
        BattleInput_SetKeyPressed(BattleSystem_GetBattleInput(work->battleSystem), work->partyMenu->key);
        PaletteData_BeginPaletteFade(palette, 5, 0xC00, -8, 7, 0, 0);
        PaletteData_BeginPaletteFade(palette, 10, 0xFFFF, -8, 16, 0, 0);
        work->state++;
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersBitmask(palette)) {
            break;
        }
        PaletteBlink_SetState(BattleSystem_GetMessageIcon(work->battleSystem), 0);
        if (work->partyMenu->selectedSlot == PARTY_SIZE) {
            BattleController_ReplyPartySelection(work->battleSystem, work->battlerId, 0xFF);
        } else {
            BattleController_ReplyPartySelection(work->battleSystem, work->battlerId, work->partyMenu->partyOrder[work->partyMenu->selectedSlot] + 1);
        }
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work->partyMenu->party);
        Heap_Free(work->partyMenu);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active reset

void Task_AIShowMonList(SysTask *task, void *data) {
    OpponentShowMonListWork *work = data;
    int battlerId;
    int selected;
    Party *party;
    int partner;
    int i;
    u32 battleType = BattleSystem_GetBattleType(work->battleSystem);
    battlerId = work->battlerId;

    if ((battleType & BATTLE_TYPE_TAG) != 0 || (battleType & BATTLE_TYPE_MULTI) != 0) {
        partner = battlerId;
    } else {
        partner = BattleSystem_GetBattlerIdPartner(work->battleSystem, battlerId);
    }

    selected = TrainerAI_GetSwitchedPartySlot(work->battleSystem, battlerId);
    if (selected == PARTY_SIZE) {
        selected = TrainerAI_PostKOSwitchIn(work->battleSystem, battlerId);
        if (selected == PARTY_SIZE) {
            party = BattleSystem_GetParty(work->battleSystem, work->battlerId);
            for (i = 0; i < Party_GetCount(party); i++) {
                Pokemon *mon = BattleSystem_GetPartyMon(work->battleSystem, work->battlerId, i);
                if (GetMonData(mon, MON_DATA_HP, NULL) != 0 && i != work->selectedMonIndex[battlerId] && i != work->selectedMonIndex[partner]) {
                    break;
                }
            }
            selected = i;
        }
    }
    BattleController_ReplyPartySelection(work->battleSystem, work->battlerId, selected + 1);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_LinkShowMonList(SysTask *task, void *data) {
    OpponentShowMonListWork *work = data;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

#pragma force_active on

void Task_RecordedShowMonList(SysTask *task, void *data) {
    OpponentShowMonListWork *work = data;
    u8 input;
    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &input) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    if (input == 0 || input > PARTY_SIZE) {
        BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
    } else {
        int slot = input - 1;
        Party *party = BattleSystem_GetParty(work->battleSystem, work->battlerId);
        if (slot == work->selectedMonIndex[work->battlerId] || slot == work->reservedMon) {
            BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
        } else {
            int count = input;
            if (count > Party_GetCount(party)) {
                BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
            } else {
                Pokemon *mon = BattleSystem_GetPartyMon(work->battleSystem, work->battlerId, count - 1);
                if (GetMonData(mon, MON_DATA_HP, NULL) == 0) {
                    BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
                }
                if (GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL) == SPECIES_EGG) {
                    BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
                }
            }
        }
    }
    BattleController_ReplyPartySelection(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_PlayerDrawYesNoBox(SysTask *task, void *data) {
    OpponentYesNoWork *work = data;
    BattleInput *battleInput;
    OpponentData *opponentData;
    BattleHpBar *partnerHpBar;
    NARC *narc0;
    NARC *narc1;
    MsgData *msgData;
    OpponentYesNoMenu menu;
    int partner;

    BattleSystem_GetBgConfig(work->battleSystem);
    battleInput = BattleSystem_GetBattleInput(work->battleSystem);
    opponentData = BattleSystem_GetOpponentData(work->battleSystem, work->battlerId);
    partner = BattleSystem_GetBattlerIdPartner(work->battleSystem, work->battlerId);
    if (partner != work->battlerId) {
        partnerHpBar = BattleSystem_GetHpBar(work->battleSystem, partner);
    } else {
        partnerHpBar = NULL;
    }
    switch (work->state) {
    case 0:
        if (BattleInput_CheckFeedbackDone(battleInput)) {
            if (work->msgId) {
                msgData = BattleSystem_GetMessageLoader(work->battleSystem);
                if (work->mode == 5) {
                    menu.msg.tag = 130;
                    menu.msg.param[0] = work->param;
                } else {
                    menu.msg.tag = TAG_NONE;
                }
                menu.msg.id = work->msgId;
                work->printerId = BattleSystem_PrintBattleMessage(work->battleSystem, msgData, &menu.msg, BattleSystem_GetTextFrameDelay(work->battleSystem));
            }
            work->state = 1;
        }
        break;
    case 1:
        if (TextPrinterCheckActive((u8)work->printerId) && work->msgId) {
            break;
        }
        narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
        narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
        BattleInput_DisableBallGauge(battleInput);
        menu.twoOption.moveNo = work->value;
        switch (work->mode) {
        case 0:
        case 5:
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 13, 0, (int *)&menu.twoOption);
            break;
        case 1:
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 14, 0, (int *)&menu.twoOption);
            break;
        case 2:
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 15, 0, (int *)&menu.twoOption);
            break;
        case 3:
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 16, 0, (int *)&menu.twoOption);
            break;
        case 4:
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 17, 0, (int *)&menu.twoOption);
            break;
        default:
            GF_ASSERT(FALSE);
            break;
        }
        work->state = 2;
        NARC_Delete(narc0);
        NARC_Delete(narc1);
        break;
    case 2:
        work->input = BattleInput_CheckTouch(battleInput);
        if (work->input != -1) {
            PlaySE(SEQ_SE_DP_DECIDE);
            work->state = 3;
        }
        break;
    case 3:
        if (BattleInput_CheckFeedbackDone(battleInput) == TRUE) {
            narc0 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            narc1 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            BattleHpBar_StopActiveEffect(work->hpBar);
            OpponentData_StopPokepicBobTask(opponentData);
            BattleHpBar_MenuCloseDummy(partnerHpBar);
            BattleInput_DisableBallGauge(battleInput);
            BattleInput_ChangeMenu(narc0, narc1, battleInput, 0, 0, NULL);
            if (work->input == 1) {
                BattleInput_SetMenuOpenDummy(battleInput, 0);
            }
            work->state = 4;
            NARC_Delete(narc0);
            NARC_Delete(narc1);
        }
        break;
    case 4:
        if (BattleInput_IsUnusedTaskNull(battleInput) == TRUE) {
            BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, work->input);
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}

#pragma force_active reset

void Task_AIDrawYesNoBox(SysTask *task, void *data) {
    OpponentYesNoWork *work = data;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_LinkDrawYesNoBox(SysTask *task, void *data) {
    OpponentYesNoWork *work = data;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_RecordedDrawYesNoBox(SysTask *task, void *data) {
    OpponentYesNoWork *work = data;
    u8 input;
    if (BattleSystem_ReadRecordedInput(work->battleSystem, work->battlerId, &input) == TRUE) {
        BattleSystem_StopRecordingTooLong(work->battleSystem);
    }
    if (input == 0 || (input != 0xFF && input != 1)) {
        BattleSystem_StopRecordingDataCorrupted(work->battleSystem);
    }
    BattleController_ReplyCommandSelection(work->battleSystem, work->battlerId, input);
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_PlayMoveAnimation(SysTask *task, void *data) {
    OpponentMoveAnimationWork *work = data;
    MoveAnimData substituteAnimData;
    BattleAnimBattlerPicParams substituteParams;
    MoveAnimData unsubstituteAnimData;
    MoveAnimData resubstituteAnimData;
    BattleAnimBattlerPicParams resubstituteParams;
    MoveAnimData restoreAnimData;
    u8 battlerId;

    switch (work->state) {
    case 0:
        if (work->animData.isSubstitute && work->animData.isEffectAnim == 0 && Pokepic_GetAttr(work->pokepic, POKEPIC_VANISHED) == 0) {
            battlerId = work->battlerId;
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &substituteAnimData, 1, 15, battlerId, battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &substituteAnimData);
            work->state = 1;
        } else {
            work->state = 4;
        }
        return;
    case 2:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &substituteParams, work->battlerId);
        BattleAnimUtil_RestorePic(&substituteParams, HEAP_ID_BATTLE);
        battlerId = work->battlerId;
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &unsubstituteAnimData, 1, 16, battlerId, battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &unsubstituteAnimData);
        work->opponentData->unk1A0 = 0;
        work->state++;
        return;
    case 4:
        BattleSystem_SetCriticalHpMusicFlag(work->battleSystem, 2);
        if (work->disableHpBar) {
            BattleSystem_SetHpBarDisabled(work->battleSystem);
        }
        if (work->updateG3) {
            PokepicManager_SetG3UpdateFlagsMask(BattleSystem_GetPokepicManager(work->battleSystem), 1);
        }
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &work->animData);
        work->state++;
        return;
    case 6:
        BattleSystem_SetCriticalHpMusicFlag(work->battleSystem, 0);
        if (work->disableHpBar) {
            BattleSystem_SetHpBarEnabled(work->battleSystem);
        }
        if (work->updateG3) {
            PokepicManager_ResetG3UpdateFlagsMask(BattleSystem_GetPokepicManager(work->battleSystem), 1);
        }
        if (work->animData.isSubstitute && work->animData.isEffectAnim == 0 && Pokepic_GetAttr(work->pokepic, POKEPIC_VANISHED) == 0) {
            battlerId = work->battlerId;
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &resubstituteAnimData, 1, 15, battlerId, battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &resubstituteAnimData);
            work->state = 7;
        } else {
            work->state = 10;
        }
        return;
    case 8:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &resubstituteParams, work->battlerId);
        BattleAnimUtil_LoadSubstituteIntoPic(&resubstituteParams, HEAP_ID_BATTLE);
        battlerId = work->battlerId;
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &restoreAnimData, 1, 16, battlerId, battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, work->animSystem, &restoreAnimData);
        work->opponentData->unk1A0 = 1;
        work->state++;
        return;
    case 10:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        return;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
        BattleAnimSystem_Execute(work->animSystem);
        if (BattleAnimSystem_IsActive(work->animSystem) == FALSE) {
            BattleAnimSystem_EndMove(work->animSystem);
            work->state++;
        }
        return;
    }
}

void Task_MonFlicker(SysTask *task, void *data) {
    OpponentMonFlickerWork *work = data;
    if (work->count < 6) {
        if (work->delay == 0) {
            work->delay = 2;
            work->count++;
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, 1 ^ Pokepic_GetAttr(work->pokepic, POKEPIC_VANISHED));
        } else {
            work->delay--;
        }
    } else {
        Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, 0);
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, 23);
        Heap_Free(work);
        SysTask_Destroy(task);
    }
}

void Task_HealthbarUpdate(SysTask *task, void *data) {
    BattleHpBar *hpBar = data;
    switch (hpBar->script) {
    case 0:
        BattleHpBar_InitHpChange(hpBar, hpBar->gainedHp);
        hpBar->script++;
    case 1:
        if (BattleHpBar_UpdateHpChange(hpBar) == -1) {
            hpBar->script++;
        }
        break;
    default:
        BattleController_EmitCommandComplete(hpBar->battleSystem, hpBar->battlerId, hpBar->command);
        hpBar->task = NULL;
        SysTask_Destroy(task);
        break;
    }
}

void Task_ExpBarUpdate(SysTask *task, void *data) {
    BattleHpBar *hpBar = data;
    switch (hpBar->script) {
    case 0:
        hpBar->unk4E = 0;
        PlaySE(SEQ_SE_DP_EXP);
        BattleHpBar_InitExpChange(hpBar, hpBar->gainedExp);
        hpBar->script++;
    case 1:
        if (hpBar->unk4E < 8) {
            hpBar->unk4E++;
        }
        if (BattleHpBar_UpdateExpChange(hpBar) == -1) {
            if (hpBar->unk4E >= 8) {
                StopSE(SEQ_SE_DP_EXP, 0);
                hpBar->script = 100;
            } else {
                hpBar->script++;
            }
        }
        break;
    case 2:
        hpBar->unk4E++;
        if (hpBar->unk4E >= 8) {
            StopSE(SEQ_SE_DP_EXP, 0);
            hpBar->script = 100;
        }
        break;
    default:
        BattleController_EmitCommandComplete(hpBar->battleSystem, hpBar->battlerId, hpBar->command);
        hpBar->task = NULL;
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active on

void Task_PlayFaintAnimation(SysTask *task, void *data) {
    OpponentFaintWork *work = data;
    MoveAnimData substituteAnimData;
    BattleAnimBattlerPicParams params;
    MoveAnimData unsubstituteAnimData;
    BattleAnimSystem *animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);
    int value;

    switch (work->state) {
    case 0:
        if (work->isSubstitute) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &substituteAnimData, 1, 15, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &substituteAnimData);
            work->state++;
        } else {
            work->state = 4;
        }
        break;
    case 2:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &params, work->battlerId);
        BattleAnimUtil_RestorePic(&params, HEAP_ID_BATTLE);
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &unsubstituteAnimData, 1, 16, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &unsubstituteAnimData);
        work->opponentData->unk1A0 = 0;
        work->state++;
        break;
    case 1:
    case 3:
        BattleAnimSystem_Execute(animSystem);
        if (!BattleAnimSystem_IsActive(animSystem)) {
            BattleAnimSystem_EndMove(animSystem);
            work->state++;
        }
        break;
    case 4:
        value = work->facing == 2 ? 117 : -117;
        PlayCryDelayedHandleChatot(BattleSystem_GetChatotVoice(work->battleSystem, work->battlerId), 5, work->species, work->form, value, 127, work->isTransformed, HEAP_ID_BATTLE, 0);
        work->state++;
    case 5:
        if (!IsCryFinished()) {
            work->state++;
        }
        break;
    case 6:
        if (work->facing == 2) {
            PlaySE_SetPan(SEQ_SE_DP_POKE_DEAD3, 117);
        } else {
            PlaySE_SetPan(SEQ_SE_DP_POKE_DEAD3, -117);
        }
        if (Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H) > 0) {
            work->state = 7;
        } else {
            work->state = 8;
        }
        break;
    case 7:
        value = Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H) - 8;
        if (value < 0) {
            value = 0;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_SHADOW_H, value);
        if (value) {
            break;
        }
        work->state++;
    case 8:
        value = GetMonPicHeightBySpeciesGenderForm(work->species, work->gender, work->facing, work->form, work->personality);
        Pokepic_SetVisible(work->pokepic, 0, 0, 80, 80 - value);
        work->state++;
        break;
    case 9:
        value = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, value + 8);
        value = Pokepic_GetAttr(work->pokepic, POKEPIC_H) - 8;
        if (value < 0) {
            value = 0;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_H, value);
        if (!value) {
            Pokepic_Delete(work->pokepic);
            work->state++;
        }
        break;
    case 10:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active reset

void Task_FadeOutBattle(SysTask *task, void *data) {
    OpponentFadeOutWork *work = data;
    PaletteData *palette = BattleSystem_GetPaletteData(work->battleSystem);
    PokepicManager *pokepicManager = BattleSystem_GetPokepicManager(work->battleSystem);

    switch (work->state) {
    case 0:
        PaletteBlink_SetState(BattleSystem_GetMessageIcon(work->battleSystem), 1);
        PaletteData_BeginPaletteFade(palette, 0xF, 0xFFFF, 1, 0, 16, 0);
        Pokepic_StartPaletteFadeAll(pokepicManager, 0, 16, 0, 0);
        GF_SndStartFadeOutBGM(0, 16);
        work->state++;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(palette) != 0) {
            return;
        }
        work->state++;
        return;
    case 2:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        return;
    default:
        return;
    }
}

#pragma force_active on

void Task_ToggleVanish(SysTask *task, void *data) {
    OpponentToggleVanishWork *work = data;
    MoveAnimData substituteAnimData;
    BattleAnimBattlerPicParams params;
    MoveAnimData restoreAnimData;
    BattleAnimSystem *animSystem = (BattleAnimSystem *)BattleSystem_GetBattleAnimSystem(work->battleSystem);

    switch (work->state) {
    case 0:
        if (work->opponentData->pokepic) {
            Pokepic_SetAttr(work->opponentData->pokepic, POKEPIC_VANISHED, work->vanish);
            if (work->vanish == TRUE) {
                work->state = 0xFF;
            } else {
                work->state++;
            }
        } else {
            work->state = 0xFF;
        }
        break;
    case 1:
        if (work->isSubstitute && work->opponentData->unk1A0 == 0) {
            BattleController_InitMoveAnimData(work->battleSystem, NULL, &substituteAnimData, 1, 15, work->battlerId, work->battlerId, 0);
            OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &substituteAnimData);
            work->state++;
        } else {
            work->state = 0xFF;
        }
        break;
    case 3:
        BattleController_InitBattlerPicParams(work->battleSystem, &work->animData, &params, work->battlerId);
        BattleAnimUtil_LoadSubstituteIntoPic(&params, HEAP_ID_BATTLE);
        BattleController_InitMoveAnimData(work->battleSystem, NULL, &restoreAnimData, 1, 16, work->battlerId, work->battlerId, 0);
        OpponentData_StartMoveAnimation(work->battleSystem, work->opponentData, animSystem, &restoreAnimData);
        work->opponentData->unk1A0 = 1;
        work->state++;
        break;
    case 2:
    case 4:
        BattleAnimSystem_Execute(animSystem);
        if (BattleAnimSystem_IsActive(animSystem) == FALSE) {
            BattleAnimSystem_EndMove(animSystem);
            work->state++;
        }
        break;
    default:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active reset

void Task_PlayExpBarFullFlash(SysTask *task, void *data) {
    OpponentExpBarFlashWork *work = data;
    switch (work->state) {
    case 0:
        BattleHpBar_BeginExpBarFullFlashEffect(work->hpBar, &work->done);
        PlaySE(SEQ_SE_DP_EXPMAX);
        work->state++;
        break;
    case 1:
        if (work->done == TRUE) {
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}

void Task_WaitBattleMessage(SysTask *task, void *data) {
    OpponentMessageWork *work = data;
    if (BattleSystem_GetWaitingIcon(work->battleSystem) != NULL) {
        WaitingIcon_Delete(BattleSystem_GetWaitingIcon(work->battleSystem));
        BattleSystem_SetWaitingIcon(work->battleSystem, NULL);
    }
    if (!TextPrinterCheckActive(work->printerId)) {
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
    }
}

void Task_PrintSelectionMessage(SysTask *task, void *data) {
    OpponentSelectionMessageWork *work = data;
    switch (work->state) {
    case 0:
        if (!TextPrinterCheckActive(work->printerId)) {
            work->state++;
            work->timer = 0;
        }
        break;
    case 1:
        work->timer++;
        if (work->timer == 40) {
            BattleController_ReplySelectionMessageDone(work->battleSystem, work->battlerId);
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}

#pragma force_active on
// The faded colours are restored from the battle system's copies of the background palettes.
#define RESTORE_BG_PALETTES()                                                                                           \
    dest = PaletteData_GetUnfadedBuf(palette, PLTTBUF_MAIN_BG);                                                         \
    MIi_CpuCopy16(BattleSystem_GetBgPaletteBackup(work->battleSystem), dest, 0xE0);                                                       \
    if (BattleSystem_GetBattleType(work->battleSystem) == (BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI)) { \
        dest = PaletteData_GetUnfadedBuf(palette, PLTTBUF_MAIN_OBJ);                                                    \
        MIi_CpuCopy16(BattleSystem_GetObjPaletteBackup(work->battleSystem), dest, 0xA0);                                                   \
    } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_DOUBLES) {                                  \
        dest = PaletteData_GetUnfadedBuf(palette, PLTTBUF_MAIN_OBJ);                                                    \
        MIi_CpuCopy16(BattleSystem_GetObjPaletteBackup(work->battleSystem), dest, 0xE0);                                                   \
    } else if (BattleSystem_GetBattleType(work->battleSystem) & BATTLE_TYPE_TRAINER) {                                  \
        dest = PaletteData_GetUnfadedBuf(palette, PLTTBUF_MAIN_OBJ);                                                    \
        MIi_CpuCopy16(BattleSystem_GetObjPaletteBackup(work->battleSystem), dest, 0xA0);                                                   \
    } else {                                                                                                            \
        dest = PaletteData_GetUnfadedBuf(palette, PLTTBUF_MAIN_OBJ);                                                    \
        MIi_CpuCopy16(BattleSystem_GetObjPaletteBackup(work->battleSystem), dest, 0x80);                                                   \
    }

void Task_PlayEncounterAnimation(SysTask *task, void *data) {
    OpponentEncounterAnimWork *work = data;
    PaletteData *palette;
    int terrain;
    u32 color;
    BattleEmitterParams emitter;
    u16 *dest;
    u16 i;

    BattleSystem_GetBgConfig(work->battleSystem);
    palette = BattleSystem_GetPaletteData(work->battleSystem);
    terrain = BattleSystem_GetTerrainId(work->battleSystem);
    color = ov12_0226D18C[BattleSystem_GetBackgroundId(work->battleSystem)];
    work->timer++;

    switch (work->state) {
    case 0:
        work->emitterManager = BattleAnimEmitterManager_New(HEAP_ID_BATTLE, 0);
        terrain = ov12_0226D350[terrain];
        emitter.index = terrain;
        emitter.unk_04 = 1;
        work->firstEmitter = BattleAnimEmitterManager_AddEmitter(work->emitterManager, &emitter);
        emitter.index = terrain + 1;
        emitter.unk_04 = 1;
        work->secondEmitter = BattleAnimEmitterManager_AddEmitter(work->emitterManager, &emitter);
        BattleAnimEmitter_Start(work->firstEmitter);
        PlaySE(SEQ_SE_DP_PASA2);
        work->state = 1;
    case 1:
        if (work->timer == 10) {
            PaletteData_BeginPaletteFade(palette, 1, 0xF3FF, 0, 0, 16, (u16)color);
            PaletteData_BeginPaletteFade(palette, 4, 0x3FFF, 0, 0, 16, (u16)color);
        }
        if (work->timer >= 10 && work->fadeStep < 16) {
            work->fadeStep++;
            if (work->fadeStep > 16) {
                work->fadeStep = 16;
            }
            {
                u16 *faded = PaletteData_GetFadedBuf(palette, PLTTBUF_SUB_BG);
                for (i = 0; i < 256; i++) {
                    u16 level = (31 * work->fadeStep) >> 4;
                    faded[i] = level | (level << 5) | (level << 10);
                }
            }
        }
        if (work->timer == 20) {
            BattleAnimEmitter_Start(work->secondEmitter);
        }
        if (work->timer == 23) {
            PlaySE(SEQ_SE_DP_PASA3);
        }
        if (work->timer == 28) {
            RESTORE_BG_PALETTES();
            PaletteData_BeginPaletteFade(palette, 1, 0xF3FF, 0, 16, 0, (u16)color);
            PaletteData_BeginPaletteFade(palette, 4, 0x3FFF, 0, 16, 0, (u16)color);
            PaletteData_BeginPaletteFade(palette, 10, 0xFFFF, 0, 16, 0, (u16)color);
        }
        if (work->timer >= 50) {
            PaletteData_BeginPaletteFade(palette, 1, 0xC00, 0, 16, 0, 0);
            work->state++;
        }
        break;
    case 2:
        if (!BattleAnimEmitter_IsActive(work->secondEmitter)) {
            BattleAnimEmitterManager_Free(work->emitterManager);
            work->state++;
        }
        break;
    case 3:
        PaletteBlink_SetState(BattleSystem_GetMessageIcon(work->battleSystem), 0);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    case 4:
        BattleAnimEmitterManager_StopAll(work->emitterManager);
        BattleAnimEmitterManager_Free(work->emitterManager);
        work->state++;
    case 5:
        RESTORE_BG_PALETTES();
        PaletteData_BeginPaletteFade(palette, 1, 0xFFFF, 0, 0, 0, 0x7FFF);
        PaletteData_BeginPaletteFade(palette, 4, 0x3FFF, 0, 0, 0, 0xFFFF);
        PaletteData_BeginPaletteFade(palette, 10, 0xFFFF, 0, 0, 0, 0x7FFF);
        work->state = 3;
        break;
    }
}

void Task_SelectMoveToForget(SysTask *task, void *data) {
    OpponentSelectMoveToForgetWork *work = data;
    PaletteData *palette = BattleSystem_GetPaletteData(work->battleSystem);
    switch (work->state) {
    case 0:
        PaletteData_BeginPaletteFade(palette, 0xA, 0xFFFF, -8, 0, 16, 0);
        work->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(palette) == 0) {
            BattleSystem_FreeInputForSubMenu(work->battleSystem);
            work->partyMenu = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattlePartyMenuArgs));
            work->partyMenu->party = BattleSystem_GetParty(work->battleSystem, work->battlerId);
            work->partyMenu->battleSystem = work->battleSystem;
            work->partyMenu->heapID = HEAP_ID_BATTLE;
            work->partyMenu->selectedSlot = work->slot;
            work->partyMenu->move = work->move;
            work->partyMenu->done = FALSE;
            work->partyMenu->reservedMon = 0;
            work->partyMenu->mode = 3;
            work->partyMenu->item = ITEM_NONE;
            work->partyMenu->battlerId = work->battlerId;
            work->partyMenu->key = 0;
            BATTLE_PARTY_MENU_START(work->partyMenu);
            work->state++;
        }
        break;
    case 2:
        if (work->partyMenu->done) {
            BattleSystem_RestoreInputAfterSubMenu(work->battleSystem);
            PaletteData_BeginPaletteFade(palette, 0xA, 0xFFFF, -8, 16, 0, 0);
            work->state++;
        }
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersBitmask(palette) == 0) {
            if (work->partyMenu->moveSlot == MAX_MON_MOVES) {
                BattleController_ReplyPartySelection(work->battleSystem, work->battlerId, 0xFF);
            } else {
                BattleController_ReplyPartySelection(work->battleSystem, work->battlerId, work->partyMenu->moveSlot + 1);
            }
            BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
            Heap_Free(work->partyMenu);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}

#pragma force_active reset

void Task_PlayMosaicAnimation(SysTask *task, void *data) {
    OpponentMosaicWork *work = data;
    int mosaic;

    switch (work->state) {
    case 0:
        if (work->counter == work->delay) {
            work->counter = 0;
            mosaic = Pokepic_GetAttr(work->pokepic, POKEPIC_MOSAIC);
            if (mosaic > work->mosaic) {
                mosaic--;
            } else if (mosaic < work->mosaic) {
                mosaic++;
            }
            Pokepic_SetAttr(work->pokepic, POKEPIC_MOSAIC, mosaic);
            if (mosaic == work->mosaic) {
                work->state++;
            }
        } else {
            work->counter++;
        }
        break;
    case 1:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_ShowPartyGauge(SysTask *task, void *data) {
    OpponentPartyGaugeWork *work = data;
    int show;
    int variant;
    int side;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PartyGauge *partyGauge;

    switch (BattleSystem_GetFieldSide(work->battleSystem, work->battlerId)) {
    case 0:
        side = 0;
        break;
    default:
        side = 1;
        break;
    }
    switch (work->state) {
    case 0:
        if (work->midBattle == FALSE) {
            show = 0;
            variant = 1;
        } else {
            show = 1;
            switch (work->battlerType) {
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
                variant = 0;
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
                variant = 2;
                break;
            default:
                variant = show;
                break;
            }
        }
        spriteSystem = BattleSystem_GetSpriteSystem(work->battleSystem);
        spriteManager = BattleSystem_GetSpriteManager(work->battleSystem);
        partyGauge = PartyGauge_NewAndShow(work->status, side, show, variant, spriteSystem, spriteManager);
        BattleSystem_SetPartyGauge(work->battleSystem, side, (u32 *)partyGauge);
        work->state++;
        break;
    case 1:
        partyGauge = (PartyGauge *)BattleSystem_GetPartyGauge(work->battleSystem, side);
        if (PartyGauge_IsArrowTaskFinished(partyGauge) == TRUE) {
            work->state++;
        }
        break;
    default:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_HidePartyGauge(SysTask *task, void *data) {
    OpponentPartyGaugeWork *work = data;
    int side;
    int hide;
    PartyGauge *partyGauge;

    switch (BattleSystem_GetFieldSide(work->battleSystem, work->battlerId)) {
    case 0:
        side = 0;
        break;
    default:
        side = 1;
        break;
    }
    partyGauge = (PartyGauge *)BattleSystem_GetPartyGauge(work->battleSystem, side);
    switch (work->state) {
    case 0:
        switch (work->midBattle) {
        case FALSE:
            hide = 0;
            break;
        default:
            hide = 1;
            break;
        }
        PartyGauge_StartHideTask(partyGauge, hide, hide);
        work->state++;
        break;
    case 1:
        if (PartyGauge_IsHideTaskFinished(partyGauge) == TRUE) {
            PartyGauge_DeleteAndFreeResources(partyGauge);
            BattleSystem_SetPartyGauge(work->battleSystem, side, NULL);
            work->state++;
        }
        break;
    default:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void Task_SpriteToOAM(SysTask *task, void *data) {
    OpponentSpriteOamWork *work = data;
    if (work->state == 0) {
        GF_ASSERT(OPPONENT_SPRITE_OAM(work->opponentData) == NULL);
        OPPONENT_SPRITE_OAM(work->opponentData) = BattleAnimBattlerSprites_New(work->battleSystem, HEAP_ID_BATTLE, work->battlerId);
        work->state++;
    } else {
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
    }
}

void Task_OAMToSprite(SysTask *task, void *data) {
    OpponentSpriteOamWork *work = data;
    GF_ASSERT(OPPONENT_SPRITE_OAM(work->opponentData) != NULL);
    if (ov07_0221FE70(OPPONENT_SPRITE_OAM(work->opponentData)) == 3 && work->state < 5) {
        work->state++;
        return;
    }
    BattleAnimBattlerSprites_Free2(OPPONENT_SPRITE_OAM(work->opponentData), work->battlerId);
    OPPONENT_SPRITE_OAM(work->opponentData) = NULL;
    BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void Task_OpponentScreenFade(SysTask *task, void *data) {
    OpponentScreenFadeWork *work = data;
    PaletteData *paletteData = BattleSystem_GetPaletteData(work->battleSystem);
    PokepicManager *pokepicManager = BattleSystem_GetPokepicManager(work->battleSystem);

    switch (work->state) {
    case 0:
        if (work->mode == 0) {
            PaletteBlink_SetState(BattleSystem_GetMessageIcon(work->battleSystem), 1);
            Pokepic_StartPaletteFadeAll(pokepicManager, 0, 16, 0, 0xFFFF);
        } else {
            GF_SndStartFadeOutBGM(0, 16);
        }
        work->state++;
    case 1:
        if (work->mode == 0) {
            PaletteData_BlendPalette(paletteData, PLTTBUF_MAIN_BG, 0, 0xA0, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, PLTTBUF_MAIN_BG, 0xC0, 0x40, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, PLTTBUF_MAIN_OBJ, 0, 0xE0, work->counter, 0xFFFF);
        } else {
            PaletteData_BlendPalette(paletteData, PLTTBUF_MAIN_BG, 0xA0, 0x20, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, PLTTBUF_SUB_BG, 0, 0x100, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, PLTTBUF_SUB_OBJ, 0, 0x100, work->counter, 0xFFFF);
        }
        if (work->counter++ == 16) {
            if (work->mode == 0) {
                ToggleBgLayer(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_OFF);
            }
            work->state++;
        }
        break;
    case 2:
        BattleController_EmitCommandComplete(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active on
// Selects the input tasks run for commands 14-19 (select command, move, target, item, party, yes/no) and the input mode.
#define SET_INPUT_TASKS(opponentData, selectCommand, selectMove, selectTarget, selectItem, showMonList, drawYesNoBox) \
    (opponentData)->unk0[0] = (u32)(selectCommand);                                                                   \
    (opponentData)->unk0[1] = (u32)(selectMove);                                                                      \
    (opponentData)->unk0[2] = (u32)(selectTarget);                                                                    \
    (opponentData)->unk0[3] = (u32)(selectItem);                                                                      \
    (opponentData)->unk0[4] = (u32)(showMonList);                                                                     \
    (opponentData)->unk0[5] = (u32)(drawYesNoBox)
#define SET_AI_INPUT_TASKS(opponentData)       SET_INPUT_TASKS(opponentData, Task_AISelectCommand, Task_AISelectMove, Task_AISelectTarget, Task_AISelectItem, Task_AIShowMonList, Task_AIDrawYesNoBox)
#define SET_RECORDED_INPUT_TASKS(opponentData) SET_INPUT_TASKS(opponentData, Task_RecordedSelectCommand, Task_RecordedSelectMove, Task_RecordedSelectTarget, Task_RecordedSelectItem, Task_RecordedShowMonList, Task_RecordedDrawYesNoBox)
#define SET_PLAYER_INPUT_TASKS(opponentData)   SET_INPUT_TASKS(opponentData, Task_PlayerSelectCommand, Task_PlayerSelectMove, Task_PlayerSelectTarget, Task_PlayerSelectItem, Task_PlayerShowMonList, Task_PlayerDrawYesNoBox)
#define SET_LINK_INPUT_TASKS(opponentData)     SET_INPUT_TASKS(opponentData, Task_LinkSelectCommand, Task_LinkSelectMove, Task_LinkSelectTarget, Task_LinkSelectItem, Task_LinkShowMonList, Task_LinkDrawYesNoBox)

void OpponentData_SetInputTasks(BattleSystem *battleSystem, OpponentData *opponentData) {
    u32 battleType = BattleSystem_GetBattleType(battleSystem);
    u32 battleSpecial = BattleSystem_GetBattleSpecial(battleSystem);

    if (battleType & BATTLE_TYPE_AI) {
        if (opponentData->battlerId) {
            SET_AI_INPUT_TASKS(opponentData);
            opponentData->inputMode = 1;
        } else if (battleSpecial & BATTLE_SPECIAL_RECORDING) {
            SET_RECORDED_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
        } else {
            SET_PLAYER_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
        }
        return;
    }
    if ((battleType & (BATTLE_TYPE_MULTI | BATTLE_TYPE_FRONTIER)) == (BATTLE_TYPE_MULTI | BATTLE_TYPE_FRONTIER)) {
        if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
            if (BattleSystem_IsServer(battleSystem)) {
                SET_AI_INPUT_TASKS(opponentData);
                opponentData->inputMode = 1;
            } else {
                SET_LINK_INPUT_TASKS(opponentData);
                opponentData->inputMode = 2;
            }
            return;
        }
        if (battleSpecial & BATTLE_SPECIAL_RECORDING) {
            SET_RECORDED_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
            return;
        }
        if (opponentData->battlerType != BattleSystem_GetBattlerType(battleSystem, BattleSystem_GetNetworkId(battleSystem) * 2)) {
            SET_LINK_INPUT_TASKS(opponentData);
            opponentData->inputMode = 2;
        } else {
            SET_PLAYER_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
        }
        return;
    }
    if (battleType & BATTLE_TYPE_MULTI) {
        if (battleSpecial & BATTLE_SPECIAL_RECORDING) {
            SET_RECORDED_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
            return;
        }
        if (opponentData->battlerType != BattleSystem_GetBattlerType(battleSystem, BattleSystem_GetNetworkId(battleSystem))) {
            SET_LINK_INPUT_TASKS(opponentData);
            opponentData->inputMode = 2;
        } else {
            SET_PLAYER_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
        }
        return;
    }
    if (battleType & BATTLE_TYPE_LINK) {
        if (battleSpecial & BATTLE_SPECIAL_RECORDING) {
            SET_RECORDED_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
            return;
        }
        if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
            SET_LINK_INPUT_TASKS(opponentData);
            opponentData->inputMode = 2;
        } else {
            SET_PLAYER_INPUT_TASKS(opponentData);
            opponentData->inputMode = 0;
        }
        return;
    }
    if (battleType & BATTLE_TYPE_PAL_PARK) {
        if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
            SET_INPUT_TASKS(opponentData, Task_PalParkSelectCommand, NULL, NULL, NULL, NULL, NULL);
            opponentData->inputMode = 1;
        } else {
            SET_INPUT_TASKS(opponentData, Task_PlayerSelectCommand, NULL, NULL, NULL, NULL, NULL);
            opponentData->inputMode = 0;
        }
        return;
    }
    if (battleType & BATTLE_TYPE_SAFARI) {
        if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
            SET_INPUT_TASKS(opponentData, Task_SafariSelectCommand, NULL, NULL, NULL, NULL, NULL);
            opponentData->inputMode = 1;
        } else {
            SET_INPUT_TASKS(opponentData, Task_PlayerSelectCommand, NULL, NULL, NULL, NULL, Task_PlayerDrawYesNoBox);
            opponentData->inputMode = 0;
        }
        return;
    }
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        SET_AI_INPUT_TASKS(opponentData);
        opponentData->inputMode = 1;
    } else if (battleSpecial & BATTLE_SPECIAL_RECORDING) {
        SET_RECORDED_INPUT_TASKS(opponentData);
        opponentData->inputMode = 0;
    } else {
        SET_PLAYER_INPUT_TASKS(opponentData);
        opponentData->inputMode = 0;
    }
}

#pragma force_active reset

u8 OpponentData_GetBattlerType(OpponentData *opponentData) {
    return opponentData->battlerType;
}

u32 OpponentData_GetInputMode(OpponentData *opponentData) {
    return opponentData->inputMode;
}

void *OpponentData_GetPokepic(OpponentData *opponentData) {
    if (opponentData->pokepic != NULL) {
        return opponentData->pokepic;
    }
    return opponentData->unk1C;
}

BattleHpBar *OpponentData_GetHpBar(OpponentData *opponentData) {
    return &opponentData->hpBar;
}

BattleCursorPosition *OpponentData_GetCursorPosition(OpponentData *opponentData) {
    return (BattleCursorPosition *)&opponentData->unk80;
}

void OpponentData_ResetCursorPosition(OpponentData *opponentData) {
    MI_CpuFill8(&opponentData->unk80, 0, 8);
}

void OpponentData_SetSpriteDrawFlag(OpponentData *opponentData, s32 drawFlag) {
    if (opponentData->managedSprite != NULL) {
        ManagedSprite_SetDrawFlag(opponentData->managedSprite, drawFlag);
    }
}

Pokepic *BattleControllerOpponent_CreatePokepic(BattleSystem *battleSystem, PokepicManager *pokepicManager, PokepicTemplate *template, int x, int y, int z, int height, int shadowHeight, int shadowXOffset, int shadowSize, int battlerId, PokepicAnimScript *animScript, PokepicCallback callback) {
    u8 *charData;
    int isEnemy;
    Pokepic *pokepic;
    int facing;
    int battlerType;

    charData = MonPicData_GetCharBuffer(BattleSystem_GetMonPicDataList(battleSystem), battlerId);
    battlerType = BattleSystem_GetBattlerType(battleSystem, battlerId);
    isEnemy = battlerType & BATTLER_TYPE_IS_ENEMY;
    if (isEnemy) {
        facing = 2;
    } else {
        facing = 0;
    }
    CharTiles_LoadMonPicAsObjFromNarc((NarcId) template->narcID, template->charDataID, HEAP_ID_BATTLE, charData, template->personality, FALSE, facing, template->species);
    MonPicData_SetNarcId(BattleSystem_GetMonPicDataList(battleSystem), battlerId, template->narcID);
    MonPicData_SetPaletteId(BattleSystem_GetMonPicDataList(battleSystem), battlerId, template->palDataID);
    MonPicData_SetHeight(BattleSystem_GetMonPicDataList(battleSystem), battlerId, height);
    pokepic = PokepicManager_CreatePokepicAt(pokepicManager, template, x, y + height, z, battlerId, battlerId, animScript, callback);
    if (isEnemy) {
        if (battlerType > BATTLER_TYPE_SOLO_ENEMY) {
            battlerType = battlerType >> 1;
        }
        Pokepic_SetAttr(pokepic, POKEPIC_SHADOW_PLTT, battlerType);
        Pokepic_SetAttr(pokepic, POKEPIC_SHADOW_SIZE, shadowSize);
        Pokepic_SetAttr(pokepic, POKEPIC_SHADOW_Y, y + 36);
        Pokepic_SetAttr(pokepic, POKEPIC_SHADOW_XOFFSET, shadowXOffset);
        Pokepic_SetAttr(pokepic, POKEPIC_SHADOW_YOFFSET, 36 - height);
        Pokepic_SetAttr(pokepic, POKEPIC_SHADOW_H, shadowHeight);
    }
    return pokepic;
}

void BattleControllerOpponent_GetReturnMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet, BattleMessage *msg) {
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        if (BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_LINK) {
            msg->id = 990;
            msg->tag = 27;
            msg->param[0] = opponentData->battlerId;
            msg->param[1] = opponentData->battlerId | packet->selectedMonIndex << 8;
        } else {
            msg->id = 989;
            msg->tag = 50;
            msg->param[0] = opponentData->battlerId;
            msg->param[1] = opponentData->battlerId;
            msg->param[2] = opponentData->battlerId | packet->selectedMonIndex << 8;
        }
    } else {
        if (!(BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_DOUBLES) && !(BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_LINK)) {
            u16 value = packet->value;
            if (value == 0) {
                msg->id = 984;
            } else if (value < 25) {
                msg->id = 985;
            } else if (value < 50) {
                msg->id = 988;
            } else if (value < 75) {
                msg->id = 986;
            } else {
                msg->id = 987;
            }
        } else {
            msg->id = 985;
        }
        msg->tag = TAG_NICKNAME;
        msg->param[0] = opponentData->battlerId | packet->selectedMonIndex << 8;
    }
}

void BattleControllerOpponent_GetSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketSwitchMessage *packet, BattleMessage *msg) {
    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        if (!(BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_LINK)) {
            msg->id = 972;
            msg->tag = 50;
            msg->param[0] = opponentData->battlerId;
            msg->param[1] = opponentData->battlerId;
            msg->param[2] = opponentData->battlerId | (packet->selectedMonIndex << 8);
        } else {
            msg->id = 974;
            msg->tag = 27;
            msg->param[0] = opponentData->battlerId;
            msg->param[1] = opponentData->battlerId | (packet->selectedMonIndex << 8);
        }
    } else {
        if (!(BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_DOUBLES) && !(BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_LINK)) {
            u16 value = packet->value;
            if (value < 100) {
                msg->id = 982;
            } else if (value < 325) {
                msg->id = 983;
            } else if (value < 550) {
                msg->id = 981;
            } else if (value < 775) {
                msg->id = 980;
            } else {
                msg->id = 979;
            }
        } else {
            msg->id = 979;
        }
        msg->tag = TAG_NICKNAME;
        msg->param[0] = opponentData->battlerId | (packet->selectedMonIndex << 8);
    }
}

void BattleControllerOpponent_GetEncounterMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg) {
    u32 battleType = BattleSystem_GetBattleType(battleSystem);
    u32 enemy1;
    u32 enemy2;

    if (battleType & BATTLE_TYPE_DOUBLES) {
        enemy1 = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_1);
        enemy2 = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_2);
    } else {
        enemy1 = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_SOLO_ENEMY);
        enemy2 = enemy1;
    }
    if (battleType & BATTLE_TYPE_LINK) {
        if (battleType & BATTLE_TYPE_FRONTIER) {
            msg->id = 992;
            msg->tag = 59;
            msg->param[0] = enemy1;
            msg->param[1] = enemy1;
            msg->param[2] = enemy2;
            msg->param[3] = enemy2;
        } else if (battleType & BATTLE_TYPE_MULTI) {
            msg->id = 971;
            msg->tag = 26;
            msg->param[0] = enemy1;
            msg->param[1] = enemy2;
        } else {
            msg->id = 970;
            msg->tag = TAG_TRNAME;
            msg->param[0] = enemy1;
        }
    } else if ((battleType & BATTLE_TYPE_TAG) || (battleType & BATTLE_TYPE_MULTI)) {
        msg->id = 992;
        msg->tag = 59;
        msg->param[0] = enemy1;
        msg->param[1] = enemy1;
        msg->param[2] = enemy2;
        msg->param[3] = enemy2;
    } else {
        msg->id = 969;
        msg->tag = 30;
        msg->param[0] = enemy1;
        msg->param[1] = enemy1;
    }
}

void BattleControllerOpponent_GetFirstSendOutMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketFirstSendOutMessage *packet, BattleMessage *msg) {
    u32 battleType = BattleSystem_GetBattleType(battleSystem);
    u32 battler;
    u32 partner;

    if (opponentData->battlerType & BATTLER_TYPE_IS_ENEMY) {
        if (battleType & BATTLE_TYPE_DOUBLES) {
            battler = opponentData->battlerId;
            partner = BattleSystem_GetBattlerIdPartner(battleSystem, battler);
        } else {
            battler = opponentData->battlerId;
            partner = battler;
        }
        if (battleType & BATTLE_TYPE_LINK) {
            if (battleType & BATTLE_TYPE_FRONTIER) {
                msg->id = 991;
                msg->tag = 60;
                msg->param[0] = battler;
                msg->param[1] = battler;
                msg->param[2] = battler | (packet->selectedMonIndex[battler] << 8);
                msg->param[3] = partner;
                msg->param[4] = partner;
                msg->param[5] = partner | (packet->selectedMonIndex[partner] << 8);
            } else if (battleType & BATTLE_TYPE_MULTI) {
                msg->id = 976;
                msg->tag = 56;
                msg->param[0] = battler;
                msg->param[1] = battler | (packet->selectedMonIndex[battler] << 8);
                msg->param[2] = partner;
                msg->param[3] = partner | (packet->selectedMonIndex[partner] << 8);
            } else if (battleType & BATTLE_TYPE_DOUBLES) {
                msg->id = 975;
                msg->tag = 49;
                msg->param[0] = battler;
                msg->param[1] = battler | (packet->selectedMonIndex[battler] << 8);
                msg->param[2] = partner | (packet->selectedMonIndex[partner] << 8);
            } else {
                msg->id = 974;
                msg->tag = 27;
                msg->param[0] = battler;
                msg->param[1] = battler | (packet->selectedMonIndex[battler] << 8);
            }
        } else if ((battleType & BATTLE_TYPE_TAG) || (battleType & BATTLE_TYPE_MULTI)) {
            msg->id = 991;
            msg->tag = 60;
            msg->param[0] = battler;
            msg->param[1] = battler;
            msg->param[2] = battler | (packet->selectedMonIndex[battler] << 8);
            msg->param[3] = partner;
            msg->param[4] = partner;
            msg->param[5] = partner | (packet->selectedMonIndex[partner] << 8);
        } else if (battleType & BATTLE_TYPE_DOUBLES) {
            msg->id = 973;
            msg->tag = 57;
            msg->param[0] = battler;
            msg->param[1] = battler;
            msg->param[2] = battler | (packet->selectedMonIndex[battler] << 8);
            msg->param[3] = partner | (packet->selectedMonIndex[partner] << 8);
        } else {
            msg->id = 972;
            msg->tag = 50;
            msg->param[0] = battler;
            msg->param[1] = battler;
            msg->param[2] = battler | (packet->selectedMonIndex[battler] << 8);
        }
        return;
    }

    if (battleType & BATTLE_TYPE_LINK) {
        u8 netId = BattleSystem_GetNetworkId(battleSystem);
        if (battleType & BATTLE_TYPE_MULTI) {
            switch (BattleSystem_GetLinkPosition(battleSystem, netId)) {
            case 0:
            case 3:
                battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_2);
                partner = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_1);
                break;
            case 1:
            case 2:
                battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_1);
                partner = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_2);
                break;
            }
        } else if (battleType & BATTLE_TYPE_DOUBLES) {
            battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_1);
            partner = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_2);
        } else {
            battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_SOLO_PLAYER);
            partner = battler;
        }
    } else if (battleType & BATTLE_TYPE_MULTI) {
        battler = BattleSystem_GetBattlerIdPartner(battleSystem, opponentData->battlerId);
        partner = opponentData->battlerId;
    } else if (battleType & BATTLE_TYPE_DOUBLES) {
        battler = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_1);
        partner = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_2);
    } else {
        battler = opponentData->battlerId;
        partner = battler;
    }

    if (battleType & BATTLE_TYPE_LINK) {
        if (battleType & BATTLE_TYPE_MULTI) {
            msg->id = 977;
            msg->tag = 49;
            msg->param[0] = battler;
            msg->param[1] = battler | (packet->selectedMonIndex[battler] << 8);
            msg->param[2] = partner | (packet->selectedMonIndex[partner] << 8);
        } else if (battleType & BATTLE_TYPE_DOUBLES) {
            msg->id = 978;
            msg->tag = 9;
            msg->param[0] = battler | (packet->selectedMonIndex[battler] << 8);
            msg->param[1] = partner | (packet->selectedMonIndex[partner] << 8);
        } else {
            msg->id = 979;
            msg->tag = TAG_NICKNAME;
            msg->param[0] = battler | (packet->selectedMonIndex[battler] << 8);
        }
    } else if (battleType & BATTLE_TYPE_MULTI) {
        msg->id = 993;
        msg->tag = 57;
        msg->param[0] = battler;
        msg->param[1] = battler;
        msg->param[2] = battler | (packet->selectedMonIndex[battler] << 8);
        msg->param[3] = partner | (packet->selectedMonIndex[partner] << 8);
    } else if (battleType & BATTLE_TYPE_DOUBLES) {
        msg->id = 978;
        msg->tag = 9;
        msg->param[0] = battler | (packet->selectedMonIndex[battler] << 8);
        msg->param[1] = partner | (packet->selectedMonIndex[partner] << 8);
    } else {
        msg->id = 979;
        msg->tag = TAG_NICKNAME;
        msg->param[0] = battler | (packet->selectedMonIndex[battler] << 8);
    }
}

void BattleControllerOpponent_GetResultMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg) {
    u32 battleType = BattleSystem_GetBattleType(battleSystem);
    u32 outcome = BattleSystem_GetBattleOutcomeFlags(battleSystem);
    u32 enemy1;
    u32 enemy2;

    if (battleType & BATTLE_TYPE_DOUBLES) {
        enemy1 = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_1);
        enemy2 = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_2);
    } else {
        enemy1 = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_SOLO_ENEMY);
        enemy2 = enemy1;
    }
    switch (outcome) {
    case BATTLE_OUTCOME_WIN:
        if (battleType & BATTLE_TYPE_MULTI) {
            msg->id = 786;
            msg->tag = 26;
            msg->param[0] = enemy1;
            msg->param[1] = enemy2;
        } else {
            msg->id = 785;
            msg->tag = TAG_TRNAME;
            msg->param[0] = enemy1;
        }
        break;
    case BATTLE_OUTCOME_LOSE:
        if (battleType & BATTLE_TYPE_MULTI) {
            msg->id = 788;
            msg->tag = 26;
            msg->param[0] = enemy1;
            msg->param[1] = enemy2;
        } else {
            msg->id = 787;
            msg->tag = TAG_TRNAME;
            msg->param[0] = enemy1;
        }
        break;
    case BATTLE_OUTCOME_DRAW:
        if (battleType & BATTLE_TYPE_MULTI) {
            msg->id = 790;
            msg->tag = 26;
            msg->param[0] = enemy1;
            msg->param[1] = enemy2;
        } else {
            msg->id = 789;
            msg->tag = TAG_TRNAME;
            msg->param[0] = enemy1;
        }
        break;
    }
}

void BattleControllerOpponent_GetRunAwayMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleControllerPacketRecordData *packet, BattleMessage *msg) {
    int i;
    int playerSide = 0;
    int enemySide = 0;

    for (i = 0; i < BattleSystem_GetMaxBattlers(battleSystem); i++) {
        if (MaskOfFlagNo(i) & packet->escapedMask) {
            if (BattleSystem_GetFieldSide(battleSystem, i) != 0) {
                enemySide++;
            } else {
                playerSide++;
            }
        }
    }
    if (playerSide && enemySide) {
        msg->id = 781;
        msg->tag = TAG_NONE;
        BattleSystem_SetBattleOutcomeFlags(battleSystem, 0xC0 | BATTLE_OUTCOME_DRAW);
        return;
    }
    if (playerSide) {
        msg->id = 781;
        msg->tag = TAG_NONE;
        BattleSystem_SetBattleOutcomeFlags(battleSystem, 0xC0 | BATTLE_OUTCOME_LOSE);
        return;
    }
    if (BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_MULTI) {
        msg->id = 792;
        msg->tag = 26;
        msg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_1);
        msg->param[1] = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_2);
    } else if (BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_DOUBLES) {
        msg->id = 791;
        msg->tag = TAG_TRNAME;
        msg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_ENEMY_SIDE_SLOT_1);
    } else {
        msg->id = 791;
        msg->tag = TAG_TRNAME;
        msg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_SOLO_ENEMY);
    }
    BattleSystem_SetBattleOutcomeFlags(battleSystem, 0xC0 | BATTLE_OUTCOME_WIN);
}

void BattleControllerOpponent_GetForfeitMessage(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg) {
    msg->id = 956;
    msg->tag = TAG_TRNAME;
    if (BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_LINK) {
        u32 position = BattleSystem_GetLinkPosition(battleSystem, BattleSystem_GetNetworkId(battleSystem));
        if (position) {
            msg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_2);
        } else {
            msg->param[0] = BattleSystem_GetBattlerFromBattlerType(battleSystem, BATTLER_TYPE_PLAYER_SIDE_SLOT_1);
        }
    } else {
        msg->param[0] = opponentData->battlerId;
    }
    BattleSystem_SetBattleOutcomeFlags(battleSystem, 0xC0 | BATTLE_OUTCOME_LOSE);
}

ManagedSprite *BattleControllerOpponent_CreateTrainerSprite(BattleSystem *battleSystem, int position, int trainerClass, int resTag, BOOL isLink, s16 x, s16 y) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *paletteData;
    int battlerPosition;

    spriteSystem = BattleSystem_GetSpriteSystem(battleSystem);
    spriteManager = BattleSystem_GetSpriteManager(battleSystem);
    paletteData = BattleSystem_GetPaletteData(battleSystem);
    if (1 & position) {
        battlerPosition = 2;
    } else {
        battlerPosition = 0;
    }
    return CreateTrainerPicSprite(spriteSystem, spriteManager, paletteData, x, y, trainerClass, battlerPosition, isLink, resTag, HEAP_ID_BATTLE);
}

void OpponentData_StartMoveAnimation(BattleSystem *battleSystem, OpponentData *opponentData, BattleAnimSystem *animSystem, MoveAnimData *animData) {
    BattleAnimSetup setup;
    u32 move;
    int i;

    if (animData->isEffectAnim == 0) {
        setup.scriptNarc = NARC_a_0_1_0;
        move = animData->move;
    } else {
        setup.scriptNarc = NARC_a_0_6_1;
        move = animData->effectAnimId;
    }
    setup.bgConfig = BattleSystem_GetBgConfig(battleSystem);
    setup.paletteData = BattleSystem_GetPaletteData(battleSystem);
    setup.spriteSystem = BattleSystem_GetSpriteSystem(battleSystem);
    for (i = 0; i < BATTLER_MAX; i++) {
        setup.picData[i] = BattleSystem_GetMonPicData(battleSystem, i);
        setup.species[i] = animData->species[i];
        setup.genders[i] = animData->gender[i];
        setup.shiny[i] = animData->shiny[i];
        setup.forms[i] = animData->form[i];
        setup.personalities[i] = animData->personality[i];
        setup.moveEffectFlags[i] = animData->moveEffectFlags[i];
    }
    BattleSystem_GetBattlerTypes(battleSystem, setup.battlerTypes);
    BattleSystem_GetPokepics(battleSystem, (void **)setup.pokepics);
    setup.battleType = BattleSystem_GetBattleType(battleSystem);
    setup.chatotCry = BattleSystem_GetChatotVoice(battleSystem, opponentData->battlerId);
    setup.battleBgTiles = BattleSystem_GetBattleBgTiles(battleSystem);
    setup.battleBgPalette = BattleSystem_GetBattleBgPalette(battleSystem);
    setup.battleBg.narcId = NARC_a_0_0_7;
    setup.battleBg.charMember = BattleSystem_GetBackgroundId(battleSystem) + 3;
    setup.battleBg.paletteMember = BattleSystem_GetBgTimeOfDay(battleSystem) + (BattleSystem_GetBackgroundId(battleSystem) * 3 + 176);
    setup.battleBg.screenMember = 2;
    setup.battleBg.palettePos = 0;
    setup.battleBg.paletteCount = 8;
    BattleAnimSystem_StartMove(animSystem, animData, move, &setup);
}

void BattleController_InitBattlerPicParams(BattleSystem *battleSystem, MoveAnimData *animData, BattleAnimBattlerPicParams *params, u32 battlerId) {
    int i;
    params->battler = battlerId;
    params->sourceBattler = battlerId;
    for (i = 0; i < BATTLER_MAX; i++) {
        params->picData[i] = BattleSystem_GetMonPicData(battleSystem, i);
        params->species[i] = animData->species[i];
        params->genders[i] = animData->gender[i];
        params->shiny[i] = animData->shiny[i];
        params->forms[i] = animData->form[i];
        params->personalities[i] = animData->personality[i];
    }
    BattleSystem_GetBattlerTypes(battleSystem, params->battlerTypes);
    BattleSystem_GetPokepics(battleSystem, (void **)params->pokepics);
}

void MoveAnim_GetDisplayFlags(u8 *disableHpBar, u8 *updateG3, BOOL isEffect, int effect, u16 move) {
    if (!isEffect) {
        const u16 moveNo = move;
        if (!(GetMoveAttr(moveNo, MOVEATTR_UNK9) & 0x40)) {
            *disableHpBar = TRUE;
        } else {
            *disableHpBar = FALSE;
        }
        if (GetMoveAttr(moveNo, MOVEATTR_UNK9) & 0x80) {
            *updateG3 = TRUE;
            return;
        }
        *updateG3 = FALSE;
        return;
    }

    switch (effect) {
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 31:
    case 32:
    case 34:
    case 35:
    case 37:
    case 39:
        *disableHpBar = TRUE;
        *updateG3 = FALSE;
        break;
    case 36:
    case 38:
        *disableHpBar = TRUE;
        *updateG3 = TRUE;
        break;
    default:
        *disableHpBar = FALSE;
        *updateG3 = FALSE;
        break;
    }
}

BOOL OpponentTrainerEncounter_HandleAnimFrame(OpponentTrainerEncounterWork *work, ManagedSprite *unused) {
    u32 attr;
    BOOL done = FALSE;

    attr = ManagedSprite_GetUserAttrForCurrentAnimFrame(work->managedSprite);
    switch (attr) {
    case 1:
        if (work->ballFlashStarted == FALSE) {
            s32 *state;
            work->ballFlashStarted = TRUE;
            state = Heap_Alloc(HEAP_ID_BATTLE, 8);
            state[0] = 0;
            state[1] = 0;
            SysTask_CreateOnMainQueue((SysTaskFunc)Task_FlashBrightness, state, 0);
        }
        break;
    case 0xFFF:
        done = TRUE;
        break;
    default:
        if ((attr & 0xF00) == 0x100) {
            u8 anim = attr;
            if (anim != 0) {
                ManagedSprite_SetAnimationFrame(work->managedSprite, 0);
                ManagedSprite_SetAnim(work->managedSprite, anim - 1);
                done = TRUE;
            }
        }
        break;
    }
    return done;
}

void Task_FlashBrightness(SysTask *task, s32 *state) {
    s32 current;

    current = *state;
    switch (current) {
    case 0:
        if (IsBrightnessTransitionActive(1) == FALSE) {
            *state = 2;
            return;
        }
        StartBrightnessTransition(4, 16, 0, (GXBlendPlaneMask)0x3D, 1);
        *state += 1;
        return;
    case 1:
        if (IsBrightnessTransitionActive(1) == TRUE) {
            StartBrightnessTransition(4, 0, 16, (GXBlendPlaneMask)0x3D, 1);
            *state += 1;
            return;
        }
        return;
    case 2:
        if (IsBrightnessTransitionActive(1) == TRUE) {
            Heap_Free(state);
            SysTask_Destroy(task);
        }
        break;
    }
}

void BattleSystem_StopRecordingDataCorrupted(BattleSystem *battleSystem) {
    BattleSystem_PauseRecording(battleSystem, 1);
    BattleController_TryEmitExitRecording(battleSystem, BattleSystem_GetBattleContext(battleSystem));
}

void BattleSystem_StopRecordingTooLong(BattleSystem *battleSystem) {
    BattleSystem_PauseRecording(battleSystem, 2);
    BattleController_TryEmitExitRecording(battleSystem, BattleSystem_GetBattleContext(battleSystem));
}

u8 BattleControllerOpponent_GetVersionTrainerClass(BattleSystem *battleSystem, int battlerId, u8 trainerClass) {
    int version;
    u8 newTrainerClass;

    newTrainerClass = trainerClass;
    if (!(BattleSystem_GetBattleType(battleSystem) & BATTLE_TYPE_LINK) || ((newTrainerClass != 0) && (newTrainerClass != 1))) {
        return newTrainerClass;
    }

    version = PlayerProfile_GetVersion(BattleSystem_GetPlayerProfile(battleSystem, battlerId));
    switch (version) {
    case 0:
        newTrainerClass += 125;
        break;
    case 12:
        newTrainerClass += 127;
        break;
    }
    return newTrainerClass;
}

// Pokepic_StartAnim takes a second argument here; pokepic.h declares it with one.
#define POKEPIC_START_ANIM(pokepic, arg) ((void (*)(Pokepic *, int))Pokepic_StartAnim)(pokepic, arg)

#pragma force_active on

// Starts the battler's pokepic animation (when battle animations are on) and plays its cry, panned towards its side.
void BattleControllerOpponent_PlayMonAnimAndCry(BattleSystem *battleSystem, int battlerId, int battlerType, Pokepic *pokepic, u32 *narc, int species, int form, int facing, int cryParam) {
    u8 cryDelay = 0;
    int pan;

    if (BattleSystem_AreBattleAnimationsOn(battleSystem) == TRUE) {
        POKEPIC_START_ANIM(pokepic, 1);
        NARC_StartPokepicAnim((NARC *)narc, BattleSystem_GetPokepicAnimManager(battleSystem), pokepic, species, facing, 0, battlerId);
        NARC_ReadPokepicCryDelay((NARC *)narc, &cryDelay, species, battlerType);
    }
    if (facing == 2) {
        pan = 117;
    } else {
        pan = -117;
    }
    if (cryDelay == 0) {
        cryDelay = 8;
    }
    PlayCryDelayedHandleChatot(BattleSystem_GetChatotVoice(battleSystem, battlerId), cryParam, species, form, pan, 127, 0, HEAP_ID_BATTLE, cryDelay);
}

#pragma force_active reset
