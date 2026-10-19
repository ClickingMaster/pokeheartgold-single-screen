#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_INTERNAL_H

#include "application/main_menu/mystery_gift_app.h"

#include "bg_window.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"

// The data of the Mystery Gift app, also used by the download over Nintendo Wi-Fi Connection.

struct MysteryGiftAppData {
    BgConfig *bgConfig;
    SaveData *saveData;
    Options *options;
    MessageFormat *msgFormat;
    MsgData *msgData;
    String *string; // printed instead of a message id when set
    Window messageWindow;
    Window windows[3];  // main menu, receive menu, yes/no menu
    Window titleWindow; // title of the gift found
    u32 textSpeed;
    u32 textPrinterId;
    u32 unk_0070;
    int stateAfterMessage;
    struct ListMenu *listMenu;
    ListMenuItem *listMenuItems;
    int giftCheckResult; // MysteryGift_CheckCanReceive: 0 ok, 1..5 why the gift cannot be received
    int giftCheckSource;
    u32 unk_0088;
    WonderCardInfo receivedHeader;
    WonderCard receivedCard;
    int timeout;
    int channel; // partner found by the wireless search
    int timer;
    int exitTarget; // 0: title screen, 1: Wonder Card viewer, 2: Lock Capsule card
    u8 unk_0444[0x180];
    Sprite *sprite;
    int beaconWatchState;
    int beaconWatchTimer;
    int giftBeaconSeen;
    u8 recvBuffer[0x1000]; // receivedHeader + receivedCard (receive mode 3)
    int receiveMode;       // 1/2: from a friend, 3: DS wireless, 4: Nintendo WFC
    int *mainState;
    int resultDelay;
    int wfcState; // Nintendo WFC download (MysteryGiftWfc_Run)
    u8 unk_15E4[4];
    u8 wfcConfig[0x68];       // Wi-Fi library setup (DWC_InitInetEx)
    int wfcNextState;         // after the request or cleanup it waits for
    int wfcFileCount;         // gift files on the server (1 expected)
    u8 wfcRecvBuffer[0x1000]; // receivedHeader + receivedCard (receive mode 4)
    u8 unk_2658[4];
    u32 wfcBytesReceived;
    u32 wfcBytesTotal;
    u32 wfcProgress; // percent
    BOOL wfcErrorReady;
    int wfcErrorMsgId;
    int wfcErrorCode;
    int wfcErrorType;
    BOOL wfcCancel;           // B pressed
    int (*wfcCallback)(void); // run every frame while set; 1: error
    Window wfcErrorWindow;
    u8 unk_2690[0x358];
    void (*callback)(struct MysteryGiftAppData *data); // run every frame while set
    int saveCancelled;
    struct WaitingIcon *waitingIcon;
    int animStatus; // gift animation task: 1 running, 2 may finish, 0 done
};

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_INTERNAL_H
