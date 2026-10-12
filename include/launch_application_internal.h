#ifndef POKEHEARTGOLD_FIELD_LAUNCH_APPLICATION_INTERNAL_H
#define POKEHEARTGOLD_FIELD_LAUNCH_APPLICATION_INTERNAL_H

#include "launch_application.h"
#include "naming_screen.h"
#include "friend_roster.h"

typedef enum PartyMenuMoveSelectState {
    PMMS_OPEN_PARTY_MENU,
    PMMS_WAIT_PARTY_MENU,
    PMMS_OPEN_SUMMARY,
    PMMS_WAIT_SUMMARY,
    PMMS_FREE,
} PartyMenuMoveSelectState;

typedef enum EasyChatTaskState {
    EASY_CHAT_TASK_STATE_LAUNCH,
    EASY_CHAT_TASK_STATE_WAIT,
    EASY_CHAT_TASK_STATE_FINISH,
} EasyChatTaskState;

typedef enum DressUpTaskState {
    DRESS_UP_TASK_STATE_LAUNCH,
    DRESS_UP_TASK_STATE_WAIT,
} DressUpTaskState;

typedef enum WirelessTradeState {
    WIRELESS_TRADE_STATE_0,
    WIRELESS_TRADE_STATE_1,
    WIRELESS_TRADE_STATE_2,
    WIRELESS_TRADE_STATE_3,
    WIRELESS_TRADE_STATE_4,
    WIRELESS_TRADE_STATE_5,
    WIRELESS_TRADE_STATE_6,
    WIRELESS_TRADE_STATE_7,
} WirelessTradeState;

typedef struct UnkStruct_0203E6D4 {
    enum HeapID heapID;
    PartyMenuArgs *partyMenu;
    u8 unk8[0x4];
} UnkStruct_0203E6D4;

typedef struct EasyChatTaskData {
    UnkStruct_0203E8C8 *unk0;
    u16 *unk4;
    u16 *unk8;
    EasyChatArgs *easyChat;
    int unk10;
} EasyChatTaskData;

typedef struct DressUpArgs {
    Pokemon *mon;
    SaveFashionDataSub *unk4;
    FashionCase *fashionCase;
    Options *options;
    GameStats *gameStats;
    PlayerProfile *profile;
    int *unk18;
    int unk1C;
    MenuInputStateMgr *menuInputStatePtr;
} DressUpArgs;

typedef struct DressUpTaskData {
    int state;
    u16 *unk4;
    int unk8;
    DressUpArgs *unkC;
} DressUpTaskData;

typedef struct WirelessTradeSelectMonArgs {
    FieldSystem *fieldSystem;
    PlayerProfile *profile;
    Party *party;
    SavePalPad *palPad;
    SaveData *saveData;
    SaveWiFiHistory *wifiHistory;
    Options *options;
    GameStats *gameStats;
    Pokedex *pokedex;
    int unk24;
    int unk28;
    BOOL natDexEnabled;
    int unk30;
    PlayerProfile *partnerProfile;
    Pokemon *unk38;
    Pokemon *unk3C;
} WirelessTradeSelectMonArgs;

typedef enum TradeSequenceBackground {
    TS_BG_DAY,
    TS_BG_EVENING,
    TS_BG_NIGHT,
    TS_BG_UNK_3,
} TradeSequenceBackground;

typedef struct TradeSequenceArgs {
    BoxPokemon *unk0;
    BoxPokemon *unk4;
    PlayerProfile *partnerProfile;
    TradeSequenceBackground bgType;
    int unk10;
    Options *options;
    EvolutionTaskData *evolutionTaskData;
    u8 unk1C[0x4];
} TradeSequenceArgs;

typedef struct WirelessTradeData {
    int state;
    WirelessTradeSelectMonArgs wirelessTradeSelectMon;
    TradeSequenceArgs tradeSequence;
} WirelessTradeData;

typedef struct DrawingSessionArgs {
    struct UnionRoomObjects *unk0;
    Options *options;
} DrawingSessionArgs;

typedef struct NamingScreenData {
    int state;
    int partyIdx;
    u16 *retVar;
    NamingScreenArgs *args;
    String *unk10;
} NamingScreenData;

typedef struct GtsArgs {
    int unk0;
    SysInfo *sysInfo;
    Party *party;
    PCStorage *pcStorage;
    Pokedex *pokedex;
    FriendRoster *unk14;
    SaveWiFiHistory *wifiHistory;
    PlayerProfile *profile;
    SaveData *saveData;
    Options *options;
    GameStats *gameStats;
    Bag *bag;
    BOOL natDexEnabled;
    void *unk34;
    u32 unk38;
    int unk3C;
} GtsArgs;

typedef struct UnkStruct_0203E644 {
    u8 unk0[0x26];
    u8 partySlot;
} UnkStruct_0203E644;

typedef struct PartyMenuMoveSelectData {
    enum HeapID heapID;
    UnkStruct_0203E644 *unk4;
    PokemonSummaryArgs *pokemonSummary;
} PartyMenuMoveSelectData;

#endif // POKEHEARTGOLD_FIELD_LAUNCH_APPLICATION_INTERNAL_H
