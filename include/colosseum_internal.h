#ifndef POKEHEARTGOLD_COLOSSEUM_INTERNAL_H
#define POKEHEARTGOLD_COLOSSEUM_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "field_system.h"
#include "list_menu_cursor.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_trainer_card.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pokemon_summary.h"
#include "task.h"

// Colosseum: the link battle room. Both players choose the Pokemon to enter; in a Mix Battle they then send each
// other their three Pokemon and exchange one each. Talking to the player opposite shows their Trainer Card.

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
    PokemonSummaryArgs *summaryArgs;
    PartyMenuArgs *partyMenuArgs;
    ColosseumEntryCallback callback;
    String *message;
    String *expandedMessage;
    Window messageWindow;
    FieldSystem *fieldSystem;
    MessageFormat *msgFormat;
    MsgData *msgData;
    int printerId;
    int state;
    int partyMenuResult; // enum ColosseumPartyMenuResult
    u8 partySlot;
    u8 selectedOrder[6];
    u8 timer;
    u8 delay;
    u8 *receivedMons; // the opponent's 3 Pokemon, then their ready flag
    u8 *sentMons;     // ours, same layout
    Party *party;     // the Pokemon entered (Mix Battle: after the exchange)
    Window monListWindow;
    Window choiceWindow; // SUMMARY / OK / CANCEL
    PlayerProfile *opponentProfile;
    struct ListMenuCursor *cursor;
    Window *menuWindow;
    u8 menuCount;
    s8 menuCursor;
    u8 ready; // we confirmed our Pokemon (Mix Battle)
    u8 opponentReady;
    u8 choice;         // index of the opponent's Pokemon we picked
    u8 opponentChoice; // index of our Pokemon the opponent picked
    u16 netId;
    u8 linkMode;
    u8 exchangeFlags;
} ColosseumEntry;

// Shown by Task_ColosseumTrainerCard; the card itself is copied into the env before the app is launched.
typedef struct ColosseumTrainerCard {
    String *strings[2]; // message, expanded message
    Window window;
    MessageFormat *msgFormat;
    MsgData *msgData;
    u32 printerId;
    u32 netId;
    u32 state;
    TrainerCardAppArgs cardArgs;
} ColosseumTrainerCard;

BOOL Colosseum_FinishPartyMenu(ColosseumEntry *entry, FieldSystem *fieldSystem);
void Colosseum_StartEntry(FieldSystem *fieldSystem, ColosseumEntryCallback callback);
int Colosseum_HandleMenuInput(ColosseumEntry *entry);
void Colosseum_RecvMixBattleMons(int netId, int size, void *data, FieldSystem *fieldSystem);
u32 Colosseum_GetMixBattleMonsSize(void);
u8 *Colosseum_GetMixBattleMonsBuffer(int netId, FieldSystem *fieldSystem);
void Colosseum_RecvMixBattleChoice(int netId, int size, const u8 *data, FieldSystem *fieldSystem);
BOOL Task_ColosseumTrainerCard(TaskManager *taskManager);
void Colosseum_ShowOpponentTrainerCard(FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_COLOSSEUM_INTERNAL_H
