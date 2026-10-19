#ifndef POKEHEARTGOLD_CODE_ENTRY_INTERNAL_H
#define POKEHEARTGOLD_CODE_ENTRY_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "code_entry.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "sprite_system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

// Touch hitboxes: the 16 digit slots, the 10 digit keys, BACK, OK.
#define CODE_ENTRY_HITBOX_KEYS 16
#define CODE_ENTRY_HITBOX_BACK 26
#define CODE_ENTRY_HITBOX_OK   27
#define CODE_ENTRY_HITBOX_NUM  28

// A digit slot, a group separator, one of the three cursors or a BACK/OK button.
typedef struct CodeEntryCell {
    int value;  // digit + 1 (0: empty); separator: index of the digit before it; cursor: current anim
    int group;  // 1-based group of a digit
    int active; // digit is in the selected group (drawn large)
    ManagedSprite *sprite;
    TouchscreenHitbox *hitbox;
    s16 velocityX; // key cursor: column
    s16 velocityY; // key cursor: row
    u8 moveFrames; // button: frames since it was pressed
    u8 animStep;
    u8 padding[2];
} CodeEntryCell;

typedef struct CodeEntryRange {
    u16 start;
    u16 end;
} CodeEntryRange;

typedef struct CodeEntryButtonLabelAlloc {
    ObjCharSpaceReservation charAlloc;
} CodeEntryButtonLabelAlloc;

typedef struct CodeEntryApp {
    CodeEntryCell digits[16];
    CodeEntryCell separators[3];
    CodeEntryCell cursors[3]; // digit cursor (value: digit index), key cursor, key press flash
    CodeEntryCell buttons[2]; // BACK, OK (value: pressed state)
    s16 groupX[5];
    CodeEntryRange groupRanges[5];
    u8 padding2BE[2];
    int state;
    int unk2C4;
    int subState;
    int frame;
    int digitCount;
    int selectedGroup;
    int previousGroup;
    int selectedStart;
    int selectedEnd;
    int previousStart;
    int previousEnd;
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    TouchHitboxController *touchController;
    TouchscreenHitbox hitboxes[CODE_ENTRY_HITBOX_NUM];
    int touchActive;
    FontSystem *fontSystem;
    TextOBJ *buttonLabels[2];
    CodeEntryButtonLabelAlloc buttonLabelAllocs[2];
    Window promptWindow;
    int pendingAction;
    int pendingArg;
    int pendingToLast;
    CodeEntryArgs args;
    int separatorCount;
    int firstEditable;
} CodeEntryApp;
typedef struct CodeEntryKeyGrid {
    int cells[3][5];
} CodeEntryKeyGrid;

typedef BOOL (*CodeEntryStateFunc)(CodeEntryApp *app);

void CodeEntry_SelectGroup(CodeEntryApp *app, int group);
BOOL CodeEntry_RunState(CodeEntryApp *app);
void CodeEntry_InitTouch(CodeEntryApp *app);
void CodeEntry_InitSpriteSystem(CodeEntryApp *app);
void CodeEntry_LoadGraphics(CodeEntryApp *app);
void CodeEntry_FreeGraphics(CodeEntryApp *app);
void CodeEntry_CreateDigitSprites(CodeEntryApp *app);
void CodeEntry_CreateCursorSprites(CodeEntryApp *app);
void CodeEntry_CreateButtonSprites(CodeEntryApp *app);
void CodeEntry_SetCursorVisible(CodeEntryApp *app, int cursor, int visible);
void CodeEntry_MoveDigitCursor(CodeEntryApp *app, int digit);
void CodeEntry_MoveKeyCursor(CodeEntryApp *app, int key);
void CodeEntry_MoveCursorToKey(CodeEntryApp *app, int key, int cursor);
void CodeEntry_UpdateCursors(CodeEntryApp *app);
void CodeEntry_UpdateButtons(CodeEntryApp *app);
int CodeEntry_GetDigitAnim(int value, int active);
void CodeEntry_MarkSelectedDigits(CodeEntryApp *app);
void CodeEntry_LayoutDigits(CodeEntryApp *app, int animate);
void CodeEntry_UpdateDigitHitboxes(CodeEntryApp *app);
void CodeEntry_InitFont(CodeEntryApp *app);
void CodeEntry_CreateButtonLabels(CodeEntryApp *app);
void CodeEntry_CreatePromptWindow(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int msgNo);

#endif // POKEHEARTGOLD_CODE_ENTRY_INTERNAL_H
