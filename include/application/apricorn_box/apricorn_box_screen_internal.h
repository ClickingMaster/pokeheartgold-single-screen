#ifndef POKEHEARTGOLD_APPLICATION_APRICORN_BOX_APRICORN_BOX_SCREEN_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_APRICORN_BOX_APRICORN_BOX_SCREEN_INTERNAL_H

#include "application/apricorn_box/apricorn_box_overlay_internal.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite_system.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

enum ApricornBoxScreenState {
    APRICORN_BOX_SCREEN_STATE_FADE_IN,
    APRICORN_BOX_SCREEN_STATE_FADE_OUT,
    APRICORN_BOX_SCREEN_STATE_BLENDER_NEWS,
    APRICORN_BOX_SCREEN_STATE_VIEW,
    APRICORN_BOX_SCREEN_STATE_BLENDER,
    APRICORN_BOX_SCREEN_STATE_KURT,
    APRICORN_BOX_SCREEN_STATE_KURT_QUANTITY,
    APRICORN_BOX_SCREEN_STATE_LIFT_APRICORN,
    APRICORN_BOX_SCREEN_STATE_DRAG_APRICORN,
    APRICORN_BOX_SCREEN_STATE_PUT_BACK_APRICORN,
    APRICORN_BOX_SCREEN_STATE_BLEND_APRICORN,
    APRICORN_BOX_SCREEN_STATE_JUICE_MENU,
    APRICORN_BOX_SCREEN_STATE_WAIT_MESSAGE = 14,
    APRICORN_BOX_SCREEN_STATE_WAIT_JUICE_TASTE,
    APRICORN_BOX_SCREEN_STATE_EXIT,
};

// The Apricorn Box screen: view the apricorns, put them into the Apriblender, give them to Kurt.
typedef struct ApricornBoxScreen {
    enum HeapID heapId;                  // 0x000
    ApricornBoxApp *app;                 // 0x004
    u8 padding_08[8];                    // 0x008
    SaveApricornBox *apricornBox;        // 0x010
    u8 juiceServings;                    // 0x014, ApricornBox_GetJuiceServings
    u8 flavorTotal;                      // 0x015, ApricornJuice_GetFlavorTotal(juiceRecord)
    u8 juiceMildness;                    // 0x016, juiceRecord byte 2
    u8 topMixIndex;                      // 0x017, ApricornJuice_GetTopMixIndex(juiceRecord)
    u8 numBlending;                      // 0x018, apricorns in the blender (ApricornBox_CountQueuedApricorns, at most 5)
    u8 mixStage;                         // 0x019, ApricornBox_GetMixProgressLevel
    u8 apricornCounts[7];                // 0x01A
    u8 filler_21;                        // 0x021
    u8 juiceRecord[8];                   // 0x022, ApricornJuice (unaligned)
    ApricornBoxJuice juice;              // 0x02A
    u32 steps;                           // 0x038
    u16 subState;                        // 0x03C
    u16 animState;                       // 0x03E
    MenuInputState menuInputState;       // 0x040
    u8 mode;                             // 0x044, ApricornBoxApp.mode
    u8 textFrameDelay;                   // 0x045
    u8 frame;                            // 0x046
    u8 textPrinterId;                    // 0x047
    u8 quantityInputResult;              // 0x048
    u8 cursorArea;                       // 0x049, 0/1: apricorn columns, 2/3: buttons
    u8 cursorRow;                        // 0x04A
    u8 lastRowLeft;                      // 0x04B
    u8 lastRowRight;                     // 0x04C
    u8 selectedApricorn;                 // 0x04D
    u8 quantity;                         // 0x04E
    u8 maxQuantity;                      // 0x04F
    u8 activeTasks;                      // 0x050
    vu8 timer;                           // 0x051, volatile in AnimateHand (read twice there)
    u8 unk_52;                           // 0x052
    u8 overBlender;                      // 0x053
    BgConfig *bgConfig;                  // 0x054
    TouchscreenListMenuSpawner *spawner; // 0x058
    MsgData *msgData;                    // 0x05C
    MessageFormat *msgFormat;            // 0x060
    String *string;                      // 0x064
    String *string2;                     // 0x068
    String *closeText;                   // 0x06C, message 1 "CLOSE"
    String *unk_70;                      // 0x070
    String *numberTemplate;              // 0x074, message 25
    String *infoTexts[22];               // 0x078, messages 27-48
    String *apricornNames[7];            // 0x0D0, messages 7-13
    u8 padding_EC[0x3C];                 // 0x0EC
    Window windows[18];                  // 0x128
    YesNoPrompt *yesNoPrompt;            // 0x248
    SpriteSystem *spriteSystem;          // 0x24C
    SpriteManager *spriteManager;        // 0x250
    Sprite *sprites[15];                 // 0x254
    NARC *narc;                          // 0x290
    ListMenuItem *listMenuItems;         // 0x294
    TouchscreenListMenu *listMenu;       // 0x298
} ApricornBoxScreen;                     // size: 0x29C

void ApricornBoxScreen_DrawCloseButton(ApricornBoxScreen *screen, int sequence);
void ApricornBoxScreen_DrawConfirmButton(ApricornBoxScreen *screen, int index, int sequence);
void ApricornBoxScreen_UpdateCursor(ApricornBoxScreen *screen);
void ApricornBoxScreen_PrintQuantity(ApricornBoxScreen *screen, int value);
void ApricornBoxScreen_PrintMessage(ApricornBoxScreen *screen, int msgNo, int speed);
void ApricornBoxScreen_SetApricornsFocus(ApricornBoxScreen *screen, int mode);
u8 ApricornBoxScreen_GetCursorTarget(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleViewInput(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleBlenderInput(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleDrag(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleKurtInput(ApricornBoxScreen *screen);
int ApricornBoxScreen_HandleQuantityInput(ApricornBoxScreen *screen);

#endif // POKEHEARTGOLD_APPLICATION_APRICORN_BOX_APRICORN_BOX_SCREEN_INTERNAL_H
