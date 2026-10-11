#ifndef POKEHEARTGOLD_WIFI_SETUP_KEYBOARD_H
#define POKEHEARTGOLD_WIFI_SETUP_KEYBOARD_H

#include "wifi_setup_ui.h"

// Software keyboard of the access-point setup screens (ARM code in overlay 13).
//
// Keys are numbered 0-52: 0-46 are the character keys (four rows, 12 per row except the last),
// 47-50 the extra keys on the right (47 alternate page, 48 shift, 50 delete) and 51-52 the two
// buttons below the keyboard. The keyboard slides in row by row from the bottom of the touch
// screen and slides out the same way when it is closed.

#define WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS  47
#define WIFI_SETUP_KEYBOARD_NUM_EXTRA_KEYS 4
#define WIFI_SETUP_KEYBOARD_NUM_BUTTONS    2

#define WIFI_SETUP_KEY_ALT_PAGE 47
#define WIFI_SETUP_KEY_SHIFT    48
#define WIFI_SETUP_KEY_DELETE   50
#define WIFI_SETUP_KEY_BUTTON_0 51

#define WIFI_SETUP_KEYBOARD_INPUT_DELETE 128 // input code of the delete key and of the B button

typedef struct WifiSetupKeyboard {
    WifiSetupCanvas *rows[3][4];     // 0x000, text of the four key rows of each page
    GXOamAttr *keys[47];             // 0x030
    GXOamAttr *extraKeys[4];         // 0x0EC
    WifiSetupOamBlock *buttons[2];   // 0x0FC
    WifiSetupOamBlock *rowBlocks[4]; // 0x104, objects showing the current page's rows
    WifiSetupOamBlock *cursor;       // 0x114
    void *task;                      // 0x118
    u8 input;                        // 0x11C, code of the key typed this frame, 0 if none
    u8 page;                         // 0x11D, 0: normal, 1: shifted (one character), 2: alternate
    s8 pressedKey;                   // 0x11E, key the stylus went down on, -1 if none
    s8 hoveredKey;                   // 0x11F, key drawn highlighted, -1 if none
    s8 returnKey;                    // 0x120, key the cursor came from before entering a wide key
    s8 cursorKey;                    // 0x121
    u8 repeatTimer;                  // 0x122, frames the delete key has been held
    u8 canDelete;                    // 0x123
    u8 canType;                      // 0x124
    u8 deleteErrorPlayed;            // 0x125, B held with nothing to delete: error sound played once
} WifiSetupKeyboard;

extern WifiSetupKeyboard *gWifiSetupKeyboard;

// layout (positions and sizes in pixels on the touch screen)
extern const WifiSetupPoint ov13_02244FA4[47]; // character key positions
extern const WifiSetupPoint ov13_02244F60;
extern const WifiSetupPoint ov13_02244F64;    // button size
extern const WifiSetupPoint ov13_02244F68;    // character key size
extern const WifiSetupPoint ov13_02244F7C[2]; // button positions
extern const WifiSetupPoint ov13_02244F84[];  // extra key positions
extern const WifiSetupPoint ov13_02244F88[];  // = &ov13_02244F84[1]
extern const WifiSetupPoint ov13_02244F8C[];  // = &ov13_02244F84[2]
extern const WifiSetupPoint ov13_02244F94[];  // extra key sizes
extern const WifiSetupPoint ov13_02244F98[];  // = &ov13_02244F94[1]
extern const WifiSetupPoint ov13_02244F9C[];  // = &ov13_02244F94[2]
extern WifiSetupRect ov13_02245958;           // whole keyboard
extern u16 ov13_02245060[];                   // cursor x of each key (stride 2)
extern u16 ov13_02245062[];                   // cursor y of each key (stride 2)
extern const s8 ov13_02245134[][4];           // cursor moves: next key for left, up, right, down

// input codes
extern u8 *ov13_02245D68[3];      // character key codes of each page
extern u8 ov13_02245208[];        // extra key codes
extern u8 ov13_02244F58[];        // button codes
extern int ov13_02245D80[];       // sound of each extra key
extern const u8 ov13_02244F4C[4]; // key palette: normal, highlighted
extern const u8 ov13_02244F50[4]; // button palette: normal, highlighted

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
// Moves row `row` (4: the buttons) of page `page` to height y. Declared without a prototype: the
// slide-out tasks pass the row's x position as an extra argument.
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
