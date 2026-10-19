#ifndef POKEHEARTGOLD_WIFI_PLAZA_TALK_H
#define POKEHEARTGOLD_WIFI_PLAZA_TALK_H

#include "global.h"

#include "wifi_plaza/wifi_plaza_room.h"

#include "list_menu.h"

// Conversations in the Wi-Fi Plaza room: chatting with another visitor, and the staff at the general-inquiries
// counter and the reception. The two players' handlers stay in step through the lobby.

typedef struct WiFiPlazaTalkMenu {
    ListMenuTemplate template;
    u32 count;
} WiFiPlazaTalkMenu;

// Sends a value to the partner once the partner's talk status reaches the awaited one
typedef struct WiFiPlazaTalkReply {
    u16 active;
    u16 value;
    u16 awaited;
    u16 startStatus; // the partner's status when it was started; a change to another status cancels it
    u32 status;      // the partner's latest status
} WiFiPlazaTalkReply;

typedef struct WiFiPlazaTalk {
    u8 question;
    u8 myAnswer; // question * 4 + choice
    u8 partnerAnswer;
    u8 partner; // player index
    u8 myTimeOfDay;
    u8 partnerTimeOfDay;
    u8 timer;
    u8 nextStep;
    u16 me; // own player index
    u16 talking;
    u8 sameToy; // both players have the same Tap Toy
    u8 menuOpen;
    u16 menuTimer;            // frames the menu is open
    WiFiPlazaObj *partnerObj; // started by us: the partner's room object
    WiFiPlazaTalkMenu menu;
    WiFiPlazaTalkReply reply;
    u16 historyPending;
    u16 historyAvatar;
    u32 historyId;
    u32 answered;
} WiFiPlazaTalk;

BOOL WiFiPlazaTalk_Start(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 playerIndex);
BOOL WiFiPlazaTalk_Answer(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaTalk_InfoCounter(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaTalk_Reception(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
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
