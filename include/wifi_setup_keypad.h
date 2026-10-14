#ifndef POKEHEARTGOLD_WIFI_SETUP_KEYPAD_H
#define POKEHEARTGOLD_WIFI_SETUP_KEYPAD_H

#include "wifi_setup_ui.h"

// Numeric keypad of the access-point setup screens (ARM code in overlay 13), used to type an
// address as four fields of up to three digits.
//
// Keys are numbered 0-13: 0-9 are the digit keys (three rows of three and a last row of one),
// 10-11 the extra keys next to the last row (10 delete, 11 next field) and 12-13 the two buttons
// below the keypad. Like the software keyboard, the keypad slides in row by row from the bottom of
// the touch screen and slides out the same way when it is closed.

#define WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS 10
#define WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS 2
#define WIFI_SETUP_KEYPAD_NUM_BUTTONS    2

#define WIFI_SETUP_KEYPAD_KEY_DELETE   10
#define WIFI_SETUP_KEYPAD_KEY_NEXT     11
#define WIFI_SETUP_KEYPAD_KEY_BUTTON_0 12

// input codes besides the digits
#define WIFI_SETUP_KEYPAD_INPUT_NONE   0
#define WIFI_SETUP_KEYPAD_INPUT_DELETE 16 // delete key and B button
#define WIFI_SETUP_KEYPAD_INPUT_NEXT   17
#define WIFI_SETUP_KEYPAD_INPUT_BACK   18
#define WIFI_SETUP_KEYPAD_INPUT_OK     19

typedef struct WifiSetupKeypad {
    WifiSetupCanvas *rows[4];        // 0x00, text of the four key rows
    GXOamAttr *keys[10];             // 0x10
    GXOamAttr *extraKeys[2];         // 0x38
    WifiSetupOamBlock *buttons[2];   // 0x40
    WifiSetupOamBlock *rowBlocks[4]; // 0x48, objects showing the rows' text
    WifiSetupOamBlock *cursor;       // 0x58
    void *task;                      // 0x5C
    u8 input;                        // 0x60, code of the key typed this frame, 0 if none
    s8 pressedKey;                   // 0x61, key the stylus went down on, -1 if none
    s8 hoveredKey;                   // 0x62, key drawn highlighted, -1 if none
    s8 cursorKey;                    // 0x63
    s8 returnKey;                    // 0x64, key the cursor came from before entering a wide key
    u8 repeatTimer;                  // 0x65, frames the delete key has been held
    u8 canDelete;                    // 0x66
    u8 canType;                      // 0x67
    u8 canAdvance;                   // 0x68, the next-field key is enabled
    u8 deleteErrorPlayed;            // 0x69, B held with nothing to delete: error sound played once
} WifiSetupKeypad;

extern WifiSetupKeypad *gWifiSetupKeypad;

// layout (positions and sizes in pixels on the touch screen)
extern const WifiSetupPoint ov13_02245228;     // digit key size
extern const WifiSetupPoint ov13_02245230;     // extra key size
extern const WifiSetupPoint ov13_02245234;     // button size
extern const WifiSetupPoint gWifiSetupKeypadDigitKeyPositions[10]; // digit key positions
extern const WifiSetupPoint gWifiSetupKeypadExtraKeyPositions[2];  // extra key positions
extern const WifiSetupPoint gWifiSetupKeypadButtonPositions[2];  // button positions
extern WifiSetupRect gWifiSetupInputPanelRect;            // whole keypad (shared with the keyboard)
extern const u16 gWifiSetupKeypadKeyLabels[12];            // key labels, three per row
extern const u8 gWifiSetupKeypadExtraKeyTemplates[2];              // OAM templates of the extra keys
extern const u8 gWifiSetupKeypadButtonTemplates[2];              // OAM templates of the buttons
extern const u16 gWifiSetupKeypadCursorX[];              // cursor x of each key (stride 2)
extern const u16 gWifiSetupKeypadCursorY[];              // cursor y of each key (stride 2)
extern const s8 gWifiSetupKeypadCursorMoves[][4];            // cursor moves: next key for left, up, right, down

// input codes
extern const u8 gWifiSetupKeypadDigitKeys[10]; // digit keys
extern const u8 gWifiSetupKeypadExtraKeyCodes[2];  // extra keys
extern const u8 gWifiSetupKeypadButtonCodes[2];  // buttons
extern const u8 gWifiSetupKeypadKeyPalettes[4];  // key palette: normal, highlighted
extern const u8 gWifiSetupKeypadButtonPalettes[4];  // button palette: normal, highlighted

void WifiSetupKeypad_Create(void);
void WifiSetupKeypad_Close(void);
int WifiSetupKeypad_GetInput(void);
void WifiSetupKeypad_SetCanDelete(int canDelete);
void WifiSetupKeypad_SetCanType(int canType);
void WifiSetupKeypad_SetCanAdvance(int canAdvance);
BOOL WifiSetupKeypad_Exists(void);

void WifiSetupKeypad_SlideInRow0Task(void *task);
void WifiSetupKeypad_SlideInRow1Task(void *task);
void WifiSetupKeypad_SlideInRow2Task(void *task);
void WifiSetupKeypad_SlideInRow3Task(void *task);
void WifiSetupKeypad_SlideInButtonsTask(void *task);
void WifiSetupKeypad_InputTask(void *task);
void WifiSetupKeypad_HandleTouchPress(void);
void WifiSetupKeypad_HandleTouchRelease(void);
void WifiSetupKeypad_HandleTouchHold(void);
void WifiSetupKeypad_SetHoveredKey(int key);
void WifiSetupKeypad_HandleButtons(void);
// Moves row `row` (4: the buttons) to height y. Declared without a prototype: the slide-out tasks
// pass the row's x position as an extra argument.
void WifiSetupKeypad_SetRowY();
void WifiSetupKeypad_SetKeyHighlight(int key, int highlighted);
void WifiSetupKeypad_UpdateCursor(void);
void WifiSetupKeypad_MoveCursor(int direction);
void WifiSetupKeypad_SlideOutButtonsTask(void *task);
void WifiSetupKeypad_SlideOutRow3Task(void *task);
void WifiSetupKeypad_SlideOutRow2Task(void *task);
void WifiSetupKeypad_SlideOutRow1Task(void *task);
void WifiSetupKeypad_SlideOutRow0Task(void *task);
void WifiSetupKeypad_DestroyTask(void *task);

#endif // POKEHEARTGOLD_WIFI_SETUP_KEYPAD_H
