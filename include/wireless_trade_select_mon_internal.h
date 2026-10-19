#ifndef POKEHEARTGOLD_WIRELESS_TRADE_SELECT_MON_INTERNAL_H
#define POKEHEARTGOLD_WIRELESS_TRADE_SELECT_MON_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "field_system.h"
#include "friend_roster.h"
#include "launch_application_internal.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon_summary.h"
#include "render_window.h"
#include "save.h"
#include "save_palpad.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

// Work of the wireless trade Pokemon selection screen, shared with its link command handlers
// (trade_communication.c).

#define WIRELESS_TRADE_NUM_SLOTS   12 // 6 own party slots, then the partner's 6
#define WIRELESS_TRADE_SLOT_CANCEL 12 // the cursor position of the "Quit" button
#define WIRELESS_TRADE_NUM_WINDOWS 34

// Party data travels in chunks of this size.
#define WIRELESS_TRADE_PARTY_CHUNK_SIZE 0x590
#define WIRELESS_TRADE_MON_PIC_SIZE     0xC80

// What the screen shows of one party slot.
typedef struct WirelessTradeMonInfo {
    u16 species;  // 0: empty slot
    u16 heldItem; // 0: none, 1: item, 2: mail
    u8 ball;
    u8 isEgg;
    u8 form;
    u8 padding_7;
    u16 gender;
    u16 flip;    // BASE_FLIP of the species
    u32 capsule; // ball capsule id
} WirelessTradeMonInfo;

// A straight-line slide of a party icon to its place above the trade question.
typedef struct WirelessTradeSlide {
    fx32 startX;
    fx32 startY;
    u8 filler_08[4];
    fx32 endX;
    fx32 endY;
    u8 filler_14[4];
} WirelessTradeSlide;

// The partner's Chatot cry as received.
typedef struct WirelessTradeChatotData {
    u32 data[0xFB];
} WirelessTradeChatotData;

typedef struct WirelessTradeSelectMon WirelessTradeSelectMon;
typedef int (*WirelessTradeSelectMonFunc)(WirelessTradeSelectMon *work);

struct WirelessTradeSelectMon {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    WirelessTradeSelectMonArgs *args;
    PokemonSummaryArgs summaryArgs;
    u32 summaryOfPartner;
    OverlayManager *summaryApp;
    u32 summaryActive;
    u32 state;
    int exchangeState;
    int sentChunks;
    int commState; // written by the receive handlers
    int receivedChunks;
    u32 tradeAccepted;
    u32 answers[2]; // per player, written by the receive handlers
    u32 resultMsg;
    u8 filler_78[0x1C];
    int cursor;
    int partnerCursor;
    u16 cursorBlinkAngle;
    u8 filler_9E[0xAE];
    u32 selectResult;
    u8 filler_150[8];
    int dpadDir;
    int partnerDpadDir;
    u8 filler_160[0x18];
    u32 unk_178;
    u32 unk_17C;
    BgConfig *bgConfig;
    MessageFormat *msgFormat;
    MessageFormat *nicknameFormat;
    MessageFormat *unk_18C;
    MsgData *msgData;
    u8 filler_194[4];
    String *playerName;
    String *itemLabel;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObjs[2][4]; // char/pltt/cell/anim, main and sub screen
    SpriteResourcesHeader resHeaders[2];
    Sprite *cursorSprites[2];
    u8 filler_34C[0x30];
    Sprite *iconSprites[WIRELESS_TRADE_NUM_SLOTS];
    Sprite *itemSprites[WIRELESS_TRADE_NUM_SLOTS];
    Sprite *capsuleSprites[WIRELESS_TRADE_NUM_SLOTS];
    Sprite *detailSprites[6]; // mon pic, ball, gender; own and partner
    Sprite *tradeSprite;
    u8 filler_428[0x1C];
    Window windows[WIRELESS_TRADE_NUM_WINDOWS];
    u8 unk_664[4];
    ListMenuItem *menuItems;
    u8 filler_66C[4];
    u32 yesNoState;
    u32 unk_674;
    u8 filler_678[0x24];
    WirelessTradeMonInfo mons[WIRELESS_TRADE_NUM_SLOTS + 1]; // the last one is the "Quit" button
    u8 filler_76C[0x30];
    void *iconCharBuffers[WIRELESS_TRADE_NUM_SLOTS];
    NNSG2dCharacterData *iconCharData[WIRELESS_TRADE_NUM_SLOTS];
    u8 monPics[2][WIRELESS_TRADE_MON_PIC_SIZE];
    PokepicTemplate monPicTemplates[2];
    int pendingMonPic; // 1 + side of the pic to upload in vblank
    u8 filler_2120[0x100];
    WirelessTradeSelectMonFunc callback;
    Party *party;
    Party *partnerParty;
    u32 unk_222C;
    SavePalPad *palPad;
    SavePalPad palPadPacket;
    u8 filler_22BC[8];
    int bgScrollY;
    u32 slideStep;
    int startDelay; // written by the receive handlers
    WirelessTradeSlide slides[2];
    u8 partyChunks[2][WIRELESS_TRADE_PARTY_CHUNK_SIZE]; // receive buffers
    WirelessTradeChatotData chatot[2];
    u32 friendStates[32];
    int friendNetId;
    int chosenRosterSlot;
    MessageFormat *rosterMsgFormat;
    MsgData *rosterMsgData;
    ListMenuItem *rosterItems;
    ListMenu *rosterMenu;
    Window rosterWindow;
    FriendRoster *roster;
    u32 tradesDone;
    s32 saveState;
    int lastSentValue;
    int lastSentCommand;
    WaitingIcon *waitingIcon;
    u32 timeoutActive;
    int timeoutCounter;
    YesNoPrompt *yesNoPrompt;
    TouchscreenListMenuSpawner *touchMenuSpawner;
    TouchscreenListMenu *touchMenu;
};

#endif // POKEHEARTGOLD_WIRELESS_TRADE_SELECT_MON_INTERNAL_H
