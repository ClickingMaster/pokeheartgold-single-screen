#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H

#include "application/vs_recorder/vs_recorder_internal.h"
#include "battle/battle_setup.h"

#include "battle_recording.h"

// VS Recorder (overlay 40): the Global Terminal screens (video lists, video search, video number keypad, delete)
// and the battle video helpers used outside the app. Shared work and framework: vs_recorder_internal.h.

// global_terminal_net.c (not in global_terminal_net.h)
BOOL GlobalTerminalNet_IsIdle(GlobalTerminalNet *net);

// battle_record.c (not in battle_recording.h)
extern BattleRecord *gBattleRecord;
void BattleRecord_GetBattlerAndPartyCount(int type, int *count, int *partySize);
int BattleRecord_WriteSlot(SaveData *save, BattleRecord *record, int index, u16 *state);
void BattleRecord_Encrypt(void *data, u32 size, u32 seed);

// VsRecorderWork.screenWork of the delete screen
typedef struct VsRecorderDeleteVideo {
    int eva; // top screen blend
    int evb;
    int fade;
    BOOL cancelled;
    Window yesNoWindows[2];
} VsRecorderDeleteVideo; // size 0x30

// one digit of the video number pad; the separators and cursors use the same layout
typedef struct VsRecorderNumberPadDigit {
    int value;    // digits: 0 none, n + 1 the digit n; separators: digit it follows; cursors: see below
    int group;    // 1-3
    BOOL enabled; // in the current group
    ManagedSprite *sprite;
    TouchscreenHitbox *hitbox; // VsRecorderNumberPad.hitboxes
    s16 dx;                    // move per frame (key cursor: column)
    s16 dy;                    // (key cursor: row)
    u8 frames;                 // frames left to move
    u8 scaleStep;
    u8 unk_1A[2];
} VsRecorderNumberPadDigit; // size 0x1C

// set up by the screen that opens the number pad
typedef struct VsRecorderNumberPadConfig {
    u32 length;          // digits
    int groupLengths[4]; // the last one repeats the third
    u32 unk_14;
    String *text; // the digits entered
    void *options;
} VsRecorderNumberPadConfig; // size 0x20

// VsRecorderNumberPad.state
#define VS_RECORDER_NUMBER_PAD_INIT   0
#define VS_RECORDER_NUMBER_PAD_INPUT  1
#define VS_RECORDER_NUMBER_PAD_SWITCH 2
#define VS_RECORDER_NUMBER_PAD_FREE   3

// VsRecorderNumberPad.action, run by the input state after a touch
#define VS_RECORDER_NUMBER_PAD_ACTION_NONE  0
#define VS_RECORDER_NUMBER_PAD_ACTION_GROUP 1 // select group actionValue (0: none, the number is complete)
#define VS_RECORDER_NUMBER_PAD_ACTION_MOVE  2 // move the digit cursor to digit actionValue
#define VS_RECORDER_NUMBER_PAD_ACTION_DONE  0xFF

// video number entry: 12 digits in three groups (2-5-5) and a keypad with 0-9, back and OK
typedef struct VsRecorderNumberPad {
    VsRecorderNumberPadDigit digits[12];     // 0x000
    VsRecorderNumberPadDigit separators[2];  // 0x150
    VsRecorderNumberPadDigit cursors[3];     // 0x188, digit cursor (value: selected digit), key cursor, key press
    s16 startX[4];                           // 0x1DC, x of the first digit, by group
    u16 groupBounds[3][2];                   // 0x1E4, first and end digit of each group
    u8 unk_1F0[4];                           // 0x1F0
    int state;                               // 0x1F4, VS_RECORDER_NUMBER_PAD_*
    u32 unk_1F8;                             // 0x1F8
    int step;                                // 0x1FC
    int timer;                               // 0x200
    int count;                               // 0x204, digits
    int group;                               // 0x208, current group (0: none)
    int prevGroup;                           // 0x20C
    int groupFirst;                          // 0x210
    int groupEnd;                            // 0x214
    int prevGroupFirst;                      // 0x218
    int prevGroupEnd;                        // 0x21C
    NARC *narc;                              // 0x220
    SpriteSystem *spriteSystem;              // 0x224
    SpriteManager *spriteManager;            // 0x228
    BgConfig *bgConfig;                      // 0x22C
    PaletteData *plttData;                   // 0x230
    TouchHitboxController *hitboxController; // 0x234
    TouchscreenHitbox hitboxes[24];          // 0x238, digits, keys 0-9, back, OK
    BOOL touchInput;                         // 0x298, TRUE: hides the key cursor
    int action;                              // 0x29C
    int actionValue;                         // 0x2A0
    int actionMode;                          // 0x2A4, ACTION_GROUP: 0 first digit, 1 last digit of the group
    VsRecorderNumberPadConfig config;        // 0x2A8
    BOOL confirmed;                          // 0x2C8, FALSE: cancelled with back on the first digit
    s64 number;                              // 0x2CC
    VsRecorderWork *owner;                   // 0x2D4
} VsRecorderNumberPad;                       // size 0x2D8

typedef BOOL (*VsRecorderNumberPadFunc)(VsRecorderNumberPad *pad);

// VsRecorderWork.screenWork of the video search screens (only the part known so far)
typedef struct VsRecorderVideoSearch {
    u8 unk_000[0x1D4];
    VsRecorderNumberPad numberPad;              // 0x1D4
    VsRecorderNumberPadConfig *numberPadConfig; // 0x4AC
    u64 videoNumber;                            // 0x4B0
} VsRecorderVideoSearch;

// vs_recorder_net.c (the Global Terminal client is embedded at VsRecorderWork.netClient)
BOOL VsRecorder_IsNetIdle(VsRecorderWork *work);

// vs_recorder_number_pad.c
void VsRecorder_PrintVideoNumber(VsRecorderVideoSearch *search, VsRecorderWork *work);
VsRecorderNumberPadConfig *VsRecorder_NewNumberPadConfig(enum HeapID heapID, u32 length, const int *groupLengths, void *options);
void VsRecorder_FreeNumberPadConfig(VsRecorderNumberPadConfig *config);
void VsRecorder_NumberPadSetGroup(VsRecorderNumberPad *pad, int group);
void VsRecorder_NumberPadInitLayout(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadSetState(VsRecorderNumberPad *pad, int state);
BOOL VsRecorder_NumberPadInit(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadFree(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadInput(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadSwitchGroup(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadMain(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadConfirm(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadBack(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadInitHitboxes(VsRecorderNumberPad *pad);
void VsRecorder_OnNumberPadTouch(u32 index, u32 event, void *arg);
void VsRecorder_NumberPadRunAction(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadClearAction(VsRecorderNumberPad *pad);
int VsRecorder_NumberPadFindGroupFirst(VsRecorderNumberPad *pad, int group);
int VsRecorder_NumberPadFindGroupLast(VsRecorderNumberPad *pad, int group);
void VsRecorder_NumberPadLoadGfx(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadShowSprites(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadFreeSprites(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadCreateDigitSprites(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadCreateCursors(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadSetCursorDrawFlag(VsRecorderNumberPad *pad, int cursor, int draw);
void VsRecorder_NumberPadMoveCursor(VsRecorderNumberPad *pad, int digit);
void VsRecorder_NumberPadPlaceKeyCursor(VsRecorderNumberPad *pad, int key);
void VsRecorder_NumberPadPlaceCursorOnKey(VsRecorderNumberPad *pad, int key, int cursor);
void VsRecorder_NumberPadUpdateCursors(VsRecorderNumberPad *pad);
int VsRecorder_GetDigitAnim(int value, BOOL enabled);
void VsRecorder_NumberPadUpdateEnabled(VsRecorderNumberPad *pad);
void VsRecorder_NumberPadLayoutDigits(VsRecorderNumberPad *pad, BOOL animate);
void VsRecorder_NumberPadUpdateHitboxes(VsRecorderNumberPad *pad);

// vs_recorder_delete.c
void VsRecorder_ShowDeleteWindow(VsRecorderWork *work, BOOL alternate);
void VsRecorder_HideDeleteWindow(VsRecorderWork *work);
void VsRecorder_PrintDeleteMessage(VsRecorderWork *work, int msgId, int slot);
void VsRecorder_ShowDeleteYesNo(VsRecorderWork *work);
void VsRecorder_HideDeleteYesNo(VsRecorderWork *work);
BOOL VsRecorder_DeleteInit(VsRecorderWork *work);
BOOL VsRecorder_DeleteEnter(VsRecorderWork *work);
BOOL VsRecorder_DeleteMain(VsRecorderWork *work);
BOOL VsRecorder_DeleteExit(VsRecorderWork *work);

// vs_recorder_battle_names.c
void VsRecorder_SanitizeBattleSetupNames(BattleRecord *record, BattleSetup *setup, enum HeapID heapID);

// vs_recorder_record.c
void VsRecorder_LoadPlaceholderTrainerName(String *str, enum HeapID heapID);
void VsRecorder_EncryptRecord(SaveData *saveData);
int VsRecorder_SaveRecordNumber(SaveData *saveData, u32 numberLow, u32 numberHigh, u16 *state, u16 *writeState);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
