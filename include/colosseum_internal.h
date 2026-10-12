#ifndef POKEHEARTGOLD_COLOSSEUM_INTERNAL_H
#define POKEHEARTGOLD_COLOSSEUM_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "field_system.h"
#include "list_menu_cursor.h"
#include "field_system.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_trainer_card.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "task.h"
#include "unk_02088288.h"

// Colosseum (ARM9 static 0x02058AEC-0x02059DB0): the link battle room. Once both players have taken their seats,
// each one chooses the Pokemon to enter in the party menu (message bank 14: "Please choose the Pokemon to be
// entered"). In a Mix Battle (link mode 3) the players then send each other their three Pokemon, pick one of the
// opponent's and exchange the picks. Talking to the player sitting opposite shows their Trainer Card.

typedef void (*ColosseumEntryCallback)(BOOL ready, Party *party);

// What the player did in the party menu (Colosseum_FinishPartyMenu).
enum ColosseumPartyMenuResult {
    COLOSSEUM_PARTY_MENU_CANCEL = 0,  // party menu slot 7
    COLOSSEUM_PARTY_MENU_CONFIRM = 1, // party menu slot 6
    COLOSSEUM_PARTY_MENU_SUMMARY = 2, // a Pokemon was picked: show its summary
};

// Colosseum_HandleMenuInput
enum ColosseumMenuInput {
    COLOSSEUM_MENU_NONE = 0,
    COLOSSEUM_MENU_SELECT = 1,
    COLOSSEUM_MENU_CANCEL = 2, // B, or A on the last entry (CANCEL)
};

#define COLOSSEUM_MIX_BATTLE_MONS 3
#define COLOSSEUM_NO_CHOICE       0xFF

// exchangeFlags
#define COLOSSEUM_EXCHANGE_SENT     (1 << 0)
#define COLOSSEUM_EXCHANGE_RECEIVED (1 << 1)
#define COLOSSEUM_EXCHANGE_DONE     (COLOSSEUM_EXCHANGE_SENT | COLOSSEUM_EXCHANGE_RECEIVED)

typedef struct ColosseumEntry {
    PokemonSummaryArgs *summaryArgs;     // 0x00
    PartyMenuArgs *partyMenuArgs;        // 0x04
    ColosseumEntryCallback callback;     // 0x08
    String *message;                     // 0x0C
    String *expandedMessage;             // 0x10
    Window messageWindow;                // 0x14
    FieldSystem *fieldSystem;            // 0x24
    MessageFormat *msgFormat;            // 0x28
    MsgData *msgData;                    // 0x2C
    int printerId;                       // 0x30
    int state;                           // 0x34
    int partyMenuResult;                 // 0x38: enum ColosseumPartyMenuResult
    u8 partySlot;                        // 0x3C
    u8 selectedOrder[6];                 // 0x3D
    u8 timer;                            // 0x43
    u8 delay;                            // 0x44
    u8 *receivedMons;                    // 0x48: the opponent's 3 Pokemon, then their ready flag
    u8 *sentMons;                        // 0x4C: ours, same layout
    Party *party;                        // 0x50: the Pokemon entered (Mix Battle: after the exchange)
    Window monListWindow;                // 0x54
    Window choiceWindow;                 // 0x64: SUMMARY / OK / CANCEL
    PlayerProfile *opponentProfile;      // 0x74
    struct ListMenuCursor *cursor;       // 0x78
    Window *menuWindow;                  // 0x7C
    u8 menuCount;                        // 0x80
    s8 menuCursor;                       // 0x81
    u8 ready;                            // 0x82: we confirmed our Pokemon (Mix Battle)
    u8 opponentReady;                    // 0x83
    u8 choice;                           // 0x84: index of the opponent's Pokemon we picked
    u8 opponentChoice;                   // 0x85: index of our Pokemon the opponent picked
    u16 netId;                           // 0x86
    u8 linkMode;                         // 0x88
    u8 exchangeFlags;                    // 0x89
} ColosseumEntry; // size: 0x8C

// Shown by Task_ColosseumTrainerCard; the card itself is copied into the env before the app is launched.
typedef struct ColosseumTrainerCard {
    String *strings[2];      // 0x00: message, expanded message
    Window window;           // 0x08
    MessageFormat *msgFormat; // 0x18
    MsgData *msgData;        // 0x1C
    u32 printerId;           // 0x20
    u32 netId;               // 0x24
    u32 state;               // 0x28
    TrainerCardAppArgs cardArgs; // 0x2C
} ColosseumTrainerCard; // size: 0x6A8

void Colosseum_OpenSummary(ColosseumEntry *entry, FieldSystem *fieldSystem, Party *party, int partySlot, int mode, enum HeapID heapId);
void Colosseum_OpenPartyMenu(ColosseumEntry *entry, enum HeapID heapId);
BOOL Colosseum_FinishPartyMenu(ColosseumEntry *entry, FieldSystem *fieldSystem);
BOOL Colosseum_FinishSummary(ColosseumEntry *entry, FieldSystem *fieldSystem);
BOOL Colosseum_WaitFieldResumed(ColosseumEntry *entry);
BOOL Colosseum_AnyPlayerCancelled(void);
BOOL Task_ColosseumEntry(TaskManager *taskManager);
u8 Colosseum_PrintMessage(ColosseumEntry *entry, String *message);
void Colosseum_RemoveWindows(ColosseumEntry *entry, BOOL clearFrame);
void Colosseum_StartEntry(FieldSystem *fieldSystem, ColosseumEntryCallback callback);
void Colosseum_FreeEntry(ColosseumEntry *entry);
ColosseumEntry *Colosseum_GetEntry(FieldSystem *fieldSystem);
void Colosseum_PackMixBattleMons(ColosseumEntry *entry, BOOL ready);
BOOL Colosseum_SendMixBattleMons(ColosseumEntry *entry);
BOOL Colosseum_MixBattleMonsExchanged(ColosseumEntry *entry);
BOOL Colosseum_OpponentIsReady(ColosseumEntry *entry);
BOOL Colosseum_SendMixBattleChoice(ColosseumEntry *entry);
BOOL Colosseum_MixBattleChoicesExchanged(ColosseumEntry *entry);
void Colosseum_SetOpponentParty(ColosseumEntry *entry);
void Colosseum_ExchangeMixBattleMons(ColosseumEntry *entry);
void Colosseum_ShowOpponentMons(ColosseumEntry *entry, int cursor);
void Colosseum_ShowChoiceMenu(ColosseumEntry *entry, int cursor);
int Colosseum_HandleMenuInput(ColosseumEntry *entry);
void Colosseum_CloseMenu(ColosseumEntry *entry);
void Colosseum_RecvMixBattleMons(int netId, int size, void *data, FieldSystem *fieldSystem);
u32 Colosseum_GetMixBattleMonsSize(void);
u8 *Colosseum_GetMixBattleMonsBuffer(int netId, FieldSystem *fieldSystem);
void Colosseum_RecvMixBattleChoice(int netId, int size, const u8 *data, FieldSystem *fieldSystem);
BOOL Task_ColosseumTrainerCard(TaskManager *taskManager);
void Colosseum_ShowOpponentTrainerCard(FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_COLOSSEUM_INTERNAL_H
