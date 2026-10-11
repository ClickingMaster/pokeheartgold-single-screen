#ifndef POKEHEARTGOLD_FRONTIER_LINK_LOBBY_H
#define POKEHEARTGOLD_FRONTIER_LINK_LOBBY_H

#include "field_system.h"
#include "party_menu.h"
#include "save.h"
#include "unk_02088288.h"

#define LINK_LOBBY_PARTY_SIZE 4

// What the linked players exchange in the frontier lobby before a link challenge: the facility, the streak and the
// chosen party (FrtCmd_LinkLobbyAlloc, Frontier_GetData while the lobby scripts run). Its link command handlers
// (sub_02096A34 and on) fill the partner fields and count the received messages in receiveCount.
typedef struct FrontierLinkLobby {
    SaveData *saveData;                          // 0x00
    u8 filler_04[4];                             // 0x04
    u16 sendBuffer[20];                          // 0x08, link message being sent
    u8 filler_30[0x28];                          // 0x30
    u8 partnerGender;                            // 0x58, copied to the tower's linkAllyGender by FrtCmd_LinkLobbyStartTower
    u8 partnerChoice;                            // 0x59, received from FrontierLinkLobby_Send(LINK_LOBBY_SEND_CHOICE)
    u8 filler_5A[0x10];                          // 0x5A
    u8 partySlots[LINK_LOBBY_PARTY_SIZE];        // 0x6A, chosen party slots
    u8 partnerFacility;                          // 0x6E
    u8 receiveCount;                             // 0x6F, link messages received (both players count)
    u8 filler_70;                                // 0x70
    u8 partnerValue71;                           // 0x71
    u16 partnerStreak;                           // 0x72
    u16 partnerValue74;                          // 0x74
    u16 species[LINK_LOBBY_PARTY_SIZE];          // 0x76, of the chosen party
    u16 items[LINK_LOBBY_PARTY_SIZE];            // 0x7E
    u16 partnerSpecies[LINK_LOBBY_PARTY_SIZE];   // 0x86
    u16 partnerItems[LINK_LOBBY_PARTY_SIZE];     // 0x8E
    u8 filler_96[2];                             // 0x96
    FieldSystem *fieldSystem;                    // 0x98
    u8 filler_9C;                                // 0x9C
    u8 appResult;                                // 0x9D
    u8 filler_9E;                                // 0x9E
    u8 partySlot;                                // 0x9F, cursor of the party menu / summary
    u8 facility;                                 // 0xA0
    u8 selectedOrder[2];                         // 0xA1
    u8 filler_A3[5];                             // 0xA3
    PartyMenuArgs *partyMenuArgs;                // 0xA8
    PokemonSummaryArgs *summaryArgs;             // 0xAC
    u16 *resultVar;                              // 0xB0, script variable that receives the app result
} FrontierLinkLobby; // size 0xB4

// FrontierLinkLobby_Send
#define LINK_LOBBY_SEND_FACILITY 0
#define LINK_LOBBY_SEND_CHOICE   1
#define LINK_LOBBY_SEND_PARTY    2
#define LINK_LOBBY_SEND_VALUE74  4
#define LINK_LOBBY_SEND_VALUE71  5

// FrontierLinkLobby_RunPartyApp
#define LINK_LOBBY_APP_LAUNCH_PARTY_MENU   0
#define LINK_LOBBY_APP_READ_PARTY_MENU     1
#define LINK_LOBBY_APP_LAUNCH_SUMMARY      2
#define LINK_LOBBY_APP_READ_SUMMARY        3

void FrontierLinkLobby_Free(FrontierLinkLobby *lobby);
BOOL FrontierLinkLobby_Send(FrontierLinkLobby *lobby, int kind, u16 a, u16 b);
void FrontierLinkLobby_RunPartyApp(void *frontierWork, FrontierLinkLobby *lobby, u32 kind);

extern const u8 ov80_0223D4C0[20];

#endif // POKEHEARTGOLD_FRONTIER_LINK_LOBBY_H
