#ifndef POKEHEARTGOLD_WIFI_SETUP_KEYBOARD_H
#define POKEHEARTGOLD_WIFI_SETUP_KEYBOARD_H

#include "wifi_setup/wifi_setup_ui.h"

// Software keyboard of the access-point setup screens. Keys 0-46 are the character keys, 47-50 the extra keys on
// the right (47 alternate page, 48 shift, 50 delete) and 51-52 the two buttons below the keyboard.

#define WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS  47
#define WIFI_SETUP_KEYBOARD_NUM_EXTRA_KEYS 4
#define WIFI_SETUP_KEYBOARD_NUM_BUTTONS    2

#define WIFI_SETUP_KEY_ALT_PAGE 47
#define WIFI_SETUP_KEY_SHIFT    48
#define WIFI_SETUP_KEY_DELETE   50
#define WIFI_SETUP_KEY_BUTTON_0 51

#define WIFI_SETUP_KEYBOARD_INPUT_DELETE 128 // input code of the delete key and of the B button

typedef struct WifiSetupKeyboard {
    WifiSetupCanvas *rows[3][4]; // text of the four key rows of each page
    GXOamAttr *keys[47];
    GXOamAttr *extraKeys[4];
    WifiSetupOamBlock *buttons[2];
    WifiSetupOamBlock *rowBlocks[4]; // objects showing the current page's rows
    WifiSetupOamBlock *cursor;
    void *task;
    u8 input;      // code of the key typed this frame, 0 if none
    u8 page;       // 0: normal, 1: shifted (one character), 2: alternate
    s8 pressedKey; // key the stylus went down on, -1 if none
    s8 hoveredKey; // key drawn highlighted, -1 if none
    s8 returnKey;  // key the cursor came from before entering a wide key
    s8 cursorKey;
    u8 repeatTimer; // frames the delete key has been held
    u8 canDelete;
    u8 canType;
    u8 deleteErrorPlayed; // B held with nothing to delete: error sound played once
} WifiSetupKeyboard;

extern WifiSetupKeyboard *gWifiSetupKeyboard;

// layout (positions and sizes in pixels on the touch screen)
extern const WifiSetupPoint gWifiSetupKeyboardKeyPositions[47];
extern const WifiSetupPoint ov13_02244F60;
extern const WifiSetupPoint gWifiSetupKeyboardButtonSize;
extern const WifiSetupPoint gWifiSetupKeyboardKeySize;
extern const WifiSetupPoint gWifiSetupKeyboardButtonPositions[2];
extern const WifiSetupPoint gWifiSetupKeyboardExtraKeyPositions[];
extern const WifiSetupPoint ov13_02244F88[];
extern const WifiSetupPoint ov13_02244F8C[];
extern const WifiSetupPoint gWifiSetupKeyboardExtraKeySizes[];
extern const WifiSetupPoint ov13_02244F98[];
extern const WifiSetupPoint ov13_02244F9C[];
extern WifiSetupRect gWifiSetupInputPanelRect;
extern u16 gWifiSetupKeyboardCursorX[];
extern u16 gWifiSetupKeyboardCursorY[];
extern const s8 gWifiSetupKeyboardCursorMoves[][4]; // next key for left, up, right, down

// input codes
extern u8 *gWifiSetupKeyboardPageKeyCodes[3];
extern u8 gWifiSetupKeyboardExtraKeyCodes[];
extern u8 gWifiSetupKeyboardButtonCodes[];
extern int gWifiSetupKeyboardExtraKeySounds[];
extern const u8 gWifiSetupKeyboardKeyPalettes[4];    // normal, highlighted
extern const u8 gWifiSetupKeyboardButtonPalettes[4]; // normal, highlighted

void WifiSetupKeyboard_Create(void);
void WifiSetupKeyboard_Close(void);
int WifiSetupKeyboard_GetInput(void);
void WifiSetupKeyboard_SetCanDelete(int canDelete);
void WifiSetupKeyboard_SetCanType(int canType);
BOOL WifiSetupKeyboard_Exists(void);

void WifiSetupKeyboard_SlideInRow0Task(void *task);
void WifiSetupKeyboard_SlideInRow1Task(void *task);
void WifiSetupKeyboard_SlideInRow2Task(void *task);
void WifiSetupKeyboard_SlideInRow3Task(void *task);
void WifiSetupKeyboard_SlideInButtonsTask(void *task);
void WifiSetupKeyboard_InputTask(void);
void WifiSetupKeyboard_HandleTouchPress(void);
void WifiSetupKeyboard_HandleTouchRelease(void);
void WifiSetupKeyboard_ToggleAltPage(void);
void WifiSetupKeyboard_ToggleShift(void);
void WifiSetupKeyboard_HandleTouchHold(void);
void WifiSetupKeyboard_SetHoveredKey(int key);
void WifiSetupKeyboard_HandleButtons(void);
// Moves row `row` (4: the buttons) of page `page` to height y.
void WifiSetupKeyboard_SetRowY();
void WifiSetupKeyboard_SetPage(int page);
void WifiSetupKeyboard_SetKeyHighlight(int key, int highlighted);
void WifiSetupKeyboard_UpdateCursor(void);
void WifiSetupKeyboard_MoveCursor(int direction);
void WifiSetupKeyboard_SlideOutButtonsTask(void *task);
void WifiSetupKeyboard_SlideOutRow3Task(void *task);
void WifiSetupKeyboard_SlideOutRow2Task(void *task);
void WifiSetupKeyboard_SlideOutRow1Task(void *task);
void WifiSetupKeyboard_SlideOutRow0Task(void *task);
void WifiSetupKeyboard_DestroyTask(void *task);

void WifiSetupSound_Play(int seq);

#endif // POKEHEARTGOLD_WIFI_SETUP_KEYBOARD_H
