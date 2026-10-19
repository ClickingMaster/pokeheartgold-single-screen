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
    enum HeapID heapId;
    ApricornBoxApp *app;
    u8 padding_08[8];
    SaveApricornBox *apricornBox;
    u8 juiceServings; // ApricornBox_GetJuiceServings
    u8 flavorTotal;   // ApricornJuice_GetFlavorTotal(juiceRecord)
    u8 juiceMildness; // juiceRecord byte 2
    u8 topMixIndex;   // ApricornJuice_GetTopMixIndex(juiceRecord)
    u8 numBlending;   // apricorns in the blender (ApricornBox_CountQueuedApricorns, at most 5)
    u8 mixStage;      // ApricornBox_GetMixProgressLevel
    u8 apricornCounts[7];
    u8 filler_21;
    u8 juiceRecord[8]; // ApricornJuice (unaligned)
    ApricornBoxJuice juice;
    u32 steps;
    u16 subState;
    u16 animState;
    MenuInputState menuInputState;
    u8 mode; // ApricornBoxApp.mode
    u8 textFrameDelay;
    u8 frame;
    u8 textPrinterId;
    u8 quantityInputResult;
    u8 cursorArea; // 0/1: apricorn columns, 2/3: buttons
    u8 cursorRow;
    u8 lastRowLeft;
    u8 lastRowRight;
    u8 selectedApricorn;
    u8 quantity;
    u8 maxQuantity;
    u8 activeTasks;
    vu8 timer;
    u8 unk_52;
    u8 overBlender;
    BgConfig *bgConfig;
    TouchscreenListMenuSpawner *spawner;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    String *string2;
    String *closeText; // message 1 "CLOSE"
    String *unk_70;
    String *numberTemplate;   // message 25
    String *infoTexts[22];    // messages 27-48
    String *apricornNames[7]; // messages 7-13
    u8 padding_EC[0x3C];
    Window windows[18];
    YesNoPrompt *yesNoPrompt;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    Sprite *sprites[15];
    NARC *narc;
    ListMenuItem *listMenuItems;
    TouchscreenListMenu *listMenu;
} ApricornBoxScreen;

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
