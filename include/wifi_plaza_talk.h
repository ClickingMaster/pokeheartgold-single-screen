#ifndef POKEHEARTGOLD_WIFI_PLAZA_TALK_H
#define POKEHEARTGOLD_WIFI_PLAZA_TALK_H

#include "global.h"

#include "list_menu.h"
#include "wifi_plaza_room.h"

// Conversations in the Wi-Fi Plaza room, run as player state handlers: chatting with another visitor (message
// bank 3 of the room, msg 766) - greetings in both players' languages, remarks on their local time of day,
// comparing and sharing Tap Toys, and asking each other questions - and the staff at the general-inquiries
// counter and the reception (bank 1, msg 777). The two players' handlers stay in step through the lobby
// (WiFiPlazaWork_AdvanceTalk sends our talk status / answer, WiFiPlazaWork_GetTalkState / WiFiPlazaWork_GetTalkPartnerMessage read the partner's).

// A list menu of the room (items set up through WiFiPlazaRoom_CreateListItems / WiFiPlazaRoom_AddListItem)
typedef struct WiFiPlazaTalkMenu {
    ListMenuTemplate template; // 0x00
    u32 count;                 // 0x20
} WiFiPlazaTalkMenu;

// Sends a value to the partner once the partner's talk status reaches the awaited one
typedef struct WiFiPlazaTalkReply {
    u16 active;      // 0x00
    u16 value;       // 0x02
    u16 awaited;     // 0x04
    u16 startStatus; // 0x06 the partner's status when it was started; a change to another status cancels it
    u32 status;      // 0x08 the partner's latest status
} WiFiPlazaTalkReply;

typedef struct WiFiPlazaTalk {
    u8 question;              // 0x00
    u8 myAnswer;              // 0x01 question * 4 + choice
    u8 partnerAnswer;         // 0x02
    u8 partner;               // 0x03 player index
    u8 myTimeOfDay;           // 0x04 WiFiPlazaTalk_GetTimeOfDay
    u8 partnerTimeOfDay;      // 0x05
    u8 timer;                 // 0x06 WiFiPlazaTalk_SetNextStep
    u8 nextStep;              // 0x07
    u16 me;                   // 0x08 own player index
    u16 talking;              // 0x0A
    u8 sameToy;               // 0x0C both players have the same Tap Toy
    u8 menuOpen;              // 0x0D
    u16 menuTimer;            // 0x0E frames the menu is open
    void *partnerObj;         // 0x10 started by us: the partner's room object
    WiFiPlazaTalkMenu menu;   // 0x14
    WiFiPlazaTalkReply reply; // 0x38
    u16 historyPending;       // 0x44 WiFiPlazaTalk_SetHistory
    u16 historyAvatar;        // 0x46
    u32 historyId;            // 0x48
    u32 answered;             // 0x4C
} WiFiPlazaTalk;

BOOL WiFiPlazaTalk_Start(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 arg);
BOOL WiFiPlazaTalk_Answer(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room);
BOOL WiFiPlazaTalk_InfoCounter(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room);
BOOL WiFiPlazaTalk_Reception(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room);
int WiFiPlazaTalk_GetTimeOfDay(WiFiPlazaPlayer *player);
String *WiFiPlazaTalk_GetMsg(WiFiPlazaRoomWork *room, u32 playerIndex, u32 msgId);
String *WiFiPlazaTalk_GetReunionMsg(WiFiPlazaRoomWork *room, u32 speaker, u32 listener);
BOOL WiFiPlazaTalk_Init(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, s32 partner);
void WiFiPlazaTalk_Free(WiFiPlazaTalk *talk);
void WiFiPlazaTalk_SetNextStep(WiFiPlazaTalk *talk, u8 frames, u8 nextStep, WiFiPlazaPlayerState *state, u8 waitStep);
void WiFiPlazaTalk_WaitTimer(WiFiPlazaTalk *talk, WiFiPlazaPlayerState *state);
void WiFiPlazaTalk_WaitMessage(WiFiPlazaTalk *talk, WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room);
void WiFiPlazaTalk_PrintGreeting(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, u32 timeOfDay, WiFiPlazaPlayer *speaker, WiFiPlazaPlayer *listener);
void WiFiPlazaTalk_BufferGreeting(WiFiPlazaRoomWork *room, u32 slot, u32 timeOfDay, WiFiPlazaPlayer *player);
BOOL WiFiPlazaTalk_PrintTimeRemark(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room);
void WiFiPlazaTalk_BufferInstrument(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, u32 slot, u32 mine);
void WiFiPlazaTalk_UpdatePartnerWait(WiFiPlazaTalk *talk, WiFiPlazaRoomWork *room);
void WiFiPlazaTalk_StartMenuTimer(WiFiPlazaTalk *talk);
void WiFiPlazaTalk_StopMenuTimer(WiFiPlazaTalk *talk);
void WiFiPlazaTalk_UpdateMenuWait(WiFiPlazaTalk *talk, WiFiPlazaRoomWork *room);
void WiFiPlazaTalk_CountMenuTimer(WiFiPlazaTalk *talk);
void WiFiPlazaTalk_SetHistory(WiFiPlazaTalk *talk, WiFiPlazaPlayer *partner);
void WiFiPlazaTalkMenu_Init(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room, u32 count, u32 firstMsg, u32 lastItem);
void WiFiPlazaTalkMenu_InitRandom(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room, u32 range, u32 count, u32 firstMsg, u32 lastItem);
void WiFiPlazaTalkMenu_InitInfoCounter(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room);
void WiFiPlazaTalkMenu_InitTimeEvents(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room);
void WiFiPlazaTalkMenu_InitPlazaGames(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room);
void WiFiPlazaTalkMenu_Free(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room);
void WiFiPlazaTalkReply_Init(WiFiPlazaTalkReply *reply);
void WiFiPlazaTalkReply_Start(WiFiPlazaTalkReply *reply, WiFiPlazaWork *lobby, u16 value, u16 awaited);
void WiFiPlazaTalkReply_Update(WiFiPlazaTalkReply *reply, WiFiPlazaWork *lobby);
BOOL WiFiPlazaTalkReply_IsDone(WiFiPlazaTalkReply *reply);
u16 WiFiPlazaTalkReply_GetStatus(WiFiPlazaTalkReply *reply);

#endif // POKEHEARTGOLD_WIFI_PLAZA_TALK_H
