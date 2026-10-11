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

// The data of the Mystery Gift app (src/application/main_menu/mystery_gift_app.c), also used by the Nintendo WFC
// download (mystery_gift_wfc.c).

struct MysteryGiftAppData {
    BgConfig *bgConfig;                                // 0x0000
    SaveData *saveData;                                // 0x0004
    Options *options;                                  // 0x0008
    MessageFormat *msgFormat;                          // 0x000C
    MsgData *msgData;                                  // 0x0010
    String *string;                                    // 0x0014, printed instead of a message id when set
    Window messageWindow;                              // 0x0018
    Window windows[3];                                 // 0x0028, main menu, receive menu, yes/no menu
    Window titleWindow;                                // 0x0058, title of the gift found
    u32 textSpeed;                                     // 0x0068
    u32 textPrinterId;                                 // 0x006C
    u32 unk_0070;                                      // 0x0070
    int stateAfterMessage;                             // 0x0074
    struct ListMenu *listMenu;                         // 0x0078
    ListMenuItem *listMenuItems;                       // 0x007C
    int giftCheckResult;                               // 0x0080, MysteryGift_CheckCanReceive: 0 ok, 1..5 why the gift cannot be received
    int unk_0084;                                      // 0x0084
    u32 unk_0088;                                      // 0x0088
    UnkWonderCardSubstruct_104 receivedHeader;         // 0x008C
    WonderCard receivedCard;                           // 0x00DC
    int timeout;                                       // 0x0434
    int channel;                                       // 0x0438, partner found by the wireless search
    int timer;                                         // 0x043C
    int exitTarget;                                    // 0x0440, 0: title screen, 1: Wonder Card viewer, 2: Lock Capsule card
    u8 unk_0444[0x180];                                // 0x0444
    Sprite *sprite;                                    // 0x05C4
    int unk_05C8;                                      // 0x05C8
    int unk_05CC;                                      // 0x05CC
    int unk_05D0;                                      // 0x05D0
    u8 recvBuffer[0x1000];                             // 0x05D4, receivedHeader + receivedCard (receive mode 3)
    int receiveMode;                                   // 0x15D4, 1/2: from a friend, 3: DS wireless, 4: Nintendo WFC
    int *mainState;                                    // 0x15D8
    int unk_15DC;                                      // 0x15DC
    int wfcState;                                      // 0x15E0, Nintendo WFC download (MysteryGiftWfc_Run)
    u8 unk_15E4[4];                                    // 0x15E4
    u8 wfcConfig[0x68];                                // 0x15E8, Wi-Fi library setup (ov00_021EC3F0)
    int wfcNextState;                                  // 0x1650, after the request or cleanup it waits for
    int wfcFileCount;                                  // 0x1654, gift files on the server (1 expected)
    u8 wfcRecvBuffer[0x1000];                          // 0x1658, receivedHeader + receivedCard (receive mode 4)
    u8 unk_2658[4];                                    // 0x2658
    u32 wfcBytesReceived;                              // 0x265C
    u32 wfcBytesTotal;                                 // 0x2660
    u32 wfcProgress;                                   // 0x2664, percent
    BOOL wfcErrorReady;                                // 0x2668
    int wfcErrorMsgId;                                 // 0x266C
    int wfcErrorCode;                                  // 0x2670
    int wfcErrorType;                                  // 0x2674
    BOOL wfcCancel;                                    // 0x2678, B pressed
    int (*wfcCallback)(void);                          // 0x267C, run every frame while set; 1: error
    Window wfcErrorWindow;                             // 0x2680
    u8 unk_2690[0x358];                                // 0x2690
    void (*callback)(struct MysteryGiftAppData *data); // 0x29E8, run every frame while set
    int unk_29EC;                                      // 0x29EC
    struct WaitingIcon *waitingIcon;                   // 0x29F0
    int animStatus;                                    // 0x29F4, gift animation task: 1 running, 2 may finish, 0 done
}; // size: 0x29F8

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_INTERNAL_H
