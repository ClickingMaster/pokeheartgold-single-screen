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

#endif // POKEHEARTGOLD_WIFI_PLAZA_TALK_H
