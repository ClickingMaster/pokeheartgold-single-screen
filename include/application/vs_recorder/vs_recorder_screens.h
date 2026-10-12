#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H

#include "application/vs_recorder/vs_recorder_internal.h"
#include "battle/battle_setup.h"

#include "battle_recording.h"

// VS Recorder (overlay 40): the Global Terminal screens (video lists, video search, video number keypad, delete)
// and the battle video helpers used outside the app. Shared work and framework: vs_recorder_internal.h.

// global_terminal_net.c (not in global_terminal_net.h)
BOOL GlobalTerminalNet_IsIdle(GlobalTerminalNet *net);

// main ARM9 (no public header)
void WirelessStatusIcon_ShowOnBothScreens(int a0, enum HeapID heapID);
BOOL sub_02087E1C(void *work);

// battle_record.c (not in battle_recording.h)
extern BattleRecord *gBattleRecord;
void BattleRecord_GetBattlerAndPartyCount(int type, int *count, int *partySize);
int BattleRecord_WriteSlot(SaveData *save, BattleRecord *record, int index, u16 *state);
void BattleRecord_Encrypt(void *data, u32 size, u32 seed);
void *BattleRecord_GetHeader(void);
void *BattleRecordHeader_New(enum HeapID heapID);
void BattleRecordHeader_Free(void *header);

// VsRecorderWork.screenWork of the delete screen
typedef struct VsRecorderDeleteVideo {
    int eva; // top screen blend
    int evb;
    int fade;
    BOOL cancelled;
    Window yesNoWindows[2];
} VsRecorderDeleteVideo; // size 0x30

// rankings received from the server (onResponse[4] / [5]); the start of the rankings screens' work
typedef struct VsRecorderRankingsDownload {
    u8 unk_000[0xE0];
    u8 rankings[0x558];    // 0x0E0
    u8 ownRankings[0xD8];  // 0x638
    u32 unk_710;           // 0x710
    void *rankingsData;    // 0x714, the response (or rankings when offline)
    void *ownRankingsData; // 0x718
    void *unk_71C;         // 0x71C
} VsRecorderRankingsDownload;

// party part of a video header: the profile, then species and forms of both sides
typedef struct VsRecorderVideoParty {
    u8 profile[0x80];
    u16 species[12];
    u8 forms[12];
} VsRecorderVideoParty;

typedef struct VsRecorderVideoListIcon {
    int resId; // also the character resource id - 100000
    ManagedSprite *sprite;
} VsRecorderVideoListIcon;

// VsRecorderVideoListScreen.listType, from VsRecorderWork.selection
#define VS_RECORDER_VIDEO_LIST_LATEST  0 // selection 300
#define VS_RECORDER_VIDEO_LIST_POPULAR 1 // selection 301
#define VS_RECORDER_VIDEO_LIST_SEARCH  2 // selection 200, the result of a video search

// VsRecorderWork.screenWork of the video list screens: the list (with party icons), a video's details (video info /
// owner's profile, play, save) and saving it to one of the three slots
typedef struct VsRecorderVideoListScreen {
    int eva; // 0x000, top screen blend
    int evb;
    int fade;
    int listType;                             // 0x00C, VS_RECORDER_VIDEO_LIST_*
    u32 showProfile;                          // 0x010, details: 0 video info, 1 owner's profile
    Window windows[8];                        // 0x014, list title
    Window buttonWindows[8];                  // 0x094, exit / details toggle label; YES, NO
    VsRecorderTextSprite buttons[4];          // 0x114, exit and back; slot menu: slots 1-3 and cancel
    u8 *downloadedVideo;                      // 0x184
    TouchHitboxController *detailHitboxes;    // 0x188, toggle, play, save, back
    TouchHitboxController *slotHitboxes;      // 0x18C, slots 1-3, cancel
    TouchHitboxController *overwriteHitboxes; // 0x190, YES, NO
    VsRecorderProfile profile;                // 0x194
    VsRecorderVideoInfo info;                 // 0x220
    u8 unk_2AC[0x2028 - 0x2AC];
    int slot;              // 0x2028, 1-3
    int buttonWindowCount; // 0x202C
    int windowCount;       // 0x2030
    int nextSubState;      // 0x2034, after the video download / the list
    BOOL downloaded;       // 0x2038
    u8 unk_203C[0x2054 - 0x203C];
    VsRecorderListConfig listConfig;     // 0x2054
    NARC *monIconNarc;                   // 0x2080
    int iconScroll;                      // 0x2084, list scroll the icons were made for
    int iconResId;                       // 0x2088, next mon icon resource
    VsRecorderVideoListIcon icons[5][6]; // 0x208C, party icons of the visible rows
} VsRecorderVideoListScreen;             // size 0x217C

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

// VsRecorderVideoSearch.condition: the search condition being edited
#define VS_RECORDER_SEARCH_FACILITY 0
#define VS_RECORDER_SEARCH_SPECIES  1
#define VS_RECORDER_SEARCH_LOCATION 2

// VsRecorderWork.screenWork of the video search screens: the search conditions (facility, species, location),
// the result list (with party icons like the video list), a found video's details and saving it; also the
// video number search (number pad)
typedef struct VsRecorderVideoSearch {
    int eva; // 0x000, top screen blend
    int evb;
    int fade;
    int nextSubState;                // 0x00C, after the result list
    VsRecorderTextSprite buttons[2]; // 0x010, exit and back
    u8 unk_048[0x80 - 0x48];
    VsRecorderProfile profile; // 0x080
    VsRecorderVideoInfo info;  // 0x10C
    u8 unk_198[0x1CC - 0x198];
    u32 showProfile; // 0x1CC, details: 0 video info, 1 owner's profile
    u8 unk_1D0[4];
    VsRecorderNumberPad numberPad;              // 0x1D4
    VsRecorderNumberPadConfig *numberPadConfig; // 0x4AC
    u64 videoNumber;                            // 0x4B0
    u8 unk_4B8[4];
    void *unk_4BC;                            // 0x4BC
    u16 species;                              // 0x4C0, 0xFFFF: any
    u8 facility;                              // 0x4C2, 0xFF: any
    u8 country;                               // 0x4C3, 0xFF: any location
    u8 region;                                // 0x4C4
    u8 condition;                             // 0x4C5, VS_RECORDER_SEARCH_*
    u8 unk_4C6[2];                            // 0x4C6
    int speciesCount;                         // 0x4C8, seen species of the chosen group
    int listSize;                             // 0x4CC, whole species list
    u16 *speciesList;                         // 0x4D0, seen species of the group first
    MsgData *speciesNames;                    // 0x4D4
    BOOL categoryChosen;                      // 0x4D8
    VsRecorderListEntry *entries;             // 0x4DC, species list window
    VsRecorderListConfig listConfig;          // 0x4E0, species list / result list
    NARC *monIconNarc;                        // 0x50C
    int iconScroll;                           // 0x510, list scroll the icons were made for
    int iconResId;                            // 0x514, next mon icon resource
    VsRecorderVideoListIcon icons[5][6];      // 0x518, party icons of the visible result rows
    TouchHitboxController *hitboxes;          // 0x608, conditions / video details
    TouchHitboxController *slotHitboxes;      // 0x60C, species groups / save slots
    TouchHitboxController *overwriteHitboxes; // 0x610, YES, NO
    Window windows[6];                        // 0x614, conditions: labels and the chosen values
    u8 unk_674[0x6D4 - 0x674];
    Window buttonWindows[9]; // 0x6D4, buttons: conditions, species groups, result list
    u8 unk_764[0x794 - 0x764];
    int windowCount;       // 0x794
    int buttonWindowCount; // 0x798
    u8 unk_79C[4];
} VsRecorderVideoSearch; // size 0x7A0

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

// vs_recorder_video_list.c
void VsRecorder_OnRankingsHeaderReceived(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnRankingsReceived(void *arg, GlobalTerminalNetError *error);
void VsRecorder_PlaceVideoListIcons(VsRecorderWork *work);
void VsRecorder_CreateVideoListButtons(VsRecorderWork *work);
void VsRecorder_PrintVideoListMessage(VsRecorderWork *work, int msgId, int slot);
BOOL VsRecorder_VideoListDownloadVideo(VsRecorderWork *work);
void VsRecorder_FreeVideoDetails(VsRecorderWork *work);
BOOL VsRecorder_VideoListOverwriteMain(VsRecorderWork *work);
BOOL VsRecorder_VideoListOverwriteNo(VsRecorderWork *work);
BOOL VsRecorder_VideoListSave(VsRecorderWork *work);
BOOL VsRecorder_VideoListConfirmOverwrite(VsRecorderWork *work);
BOOL VsRecorder_VideoListReturnToDetails(VsRecorderWork *work);
BOOL VsRecorder_VideoListSlotsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoListSlotsCancel(VsRecorderWork *work);
BOOL VsRecorder_VideoListBackToList(VsRecorderWork *work);
void VsRecorder_SetSlotButtonName(VsRecorderTextSprite *button, VsRecorderWork *work, int slot);
void VsRecorder_CreateSlotButtons(VsRecorderWork *work);
void VsRecorder_FreeSlotButtons(VsRecorderWork *work);
void VsRecorder_FreeVideoListButtons(VsRecorderWork *work);
void VsRecorder_SetVideoListButtonsMode(VsRecorderWork *work, int backOnly);
void VsRecorder_RemoveVideoListWindows(VsRecorderWork *work);
void VsRecorder_RemoveVideoListButtonWindows(VsRecorderWork *work);
BOOL VsRecorder_VideoListInit(VsRecorderWork *work);
BOOL VsRecorder_VideoListEnter(VsRecorderWork *work);
BOOL VsRecorder_VideoListDownloadList(VsRecorderWork *work);
void VsRecorder_CreateVideoListWindows(VsRecorderWork *work);
BOOL VsRecorder_VideoListShowList(VsRecorderWork *work);
BOOL VsRecorder_VideoListSelect(VsRecorderWork *work);
BOOL VsRecorder_VideoListExit(VsRecorderWork *work);
BOOL VsRecorder_VideoListShowDetails(VsRecorderWork *work);
BOOL VsRecorder_VideoListDetailsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoListToggleProfile(VsRecorderWork *work);
BOOL VsRecorder_VideoListPlay(VsRecorderWork *work);
BOOL VsRecorder_VideoListResume(VsRecorderWork *work);
void VsRecorder_ShowVideoListMessage(VsRecorderWork *work, int msgId, int unused);
BOOL VsRecorder_VideoListShowSlots(VsRecorderWork *work);
void VsRecorder_RemoveDetailsButtonWindow(VsRecorderWork *work);
void VsRecorder_PrintDetailsButtonLabel(VsRecorderWork *work, int msgId);
void VsRecorder_CreateDetailsButtonWindow(VsRecorderWork *work);
void VsRecorder_CreateOverwriteWindows(VsRecorderWork *work);
void VsRecorder_RemoveOverwriteWindows(VsRecorderWork *work);
void VsRecorder_OnVideoDetailsTouch(u32 index, u32 event, void *arg);
void VsRecorder_OnSaveSlotTouch(u32 index, u32 event, void *arg);
void VsRecorder_OnOverwriteTouch(u32 index, u32 event, void *arg);
void VsRecorder_LoadVideoDetails(VsRecorderWork *work);
void VsRecorder_CreateVideoListIcons(VsRecorderWork *work);
void VsRecorder_SetVideoListIconsDrawFlag(VsRecorderWork *work, BOOL draw);
void VsRecorder_FreeVideoListIcons(VsRecorderWork *work);

// vs_recorder_video_list_scroll.c
void VsRecorder_ScrollVideoListIcons(VsRecorderWork *work);

// vs_recorder_video_list_end.c
void VsRecorder_FreeDetailsButtons(VsRecorderWork *work);

// vs_recorder_video_search.c
void VsRecorder_PlaceSearchResultIcons(VsRecorderWork *work);
void VsRecorder_CreateSearchResultIcons(VsRecorderWork *work);
void VsRecorder_SetSearchResultIconsDrawFlag(VsRecorderWork *work, BOOL draw);
void VsRecorder_FreeSearchResultIcons(VsRecorderWork *work);
void VsRecorder_ScrollSearchResultIcons(VsRecorderWork *work);
BOOL VsRecorder_HasSearchConditions(VsRecorderWork *work);
void VsRecorder_CreateSearchConditionWindows(VsRecorderWork *work, int mode);
void VsRecorder_PrintSearchFacility(VsRecorderWork *work, u32 facility);
void VsRecorder_PrintSearchSpecies(VsRecorderWork *work, u16 species);
void VsRecorder_PrintSearchLocation(VsRecorderWork *work, u32 country, u32 region);
void VsRecorder_PrintLocationButtonLabel(VsRecorderWork *work);
void VsRecorder_CreateSearchButtonWindows(VsRecorderWork *work, int mode);
void VsRecorder_RemoveSearchWindows(VsRecorderWork *work);
void VsRecorder_RemoveSearchButtonWindows(VsRecorderWork *work);
void VsRecorder_CreateSearchResultWindows(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchShowConditions(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchConditionsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchExit(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchOpenCondition(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSelectFacility(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchClearSpecies(VsRecorderWork *work);
void VsRecorder_CreateSearchCategoryWindows(VsRecorderWork *work);
void VsRecorder_RemoveSearchCategoryWindows(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSelectCategory(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSelectSpecies(VsRecorderWork *work);
BOOL VsRecorder_SearchCategoryHasSeen(VsRecorderWork *work, int category);
void VsRecorder_BuildSearchSpeciesList(VsRecorderWork *work, int category);
void VsRecorder_FreeSearchSpeciesList(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSelectLocation(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchReturnToConditions(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchRequest(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchLeaveConditions(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchShowResults(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchResultsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchResultsBack(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchShowDetails(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchDetailsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchToggleProfile(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchPlay(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchBackToResults(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchShowSlots(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSlotsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSlotsCancel(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchReturnToDetails(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchConfirmOverwrite(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchOverwriteMain(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchOverwriteNo(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSave(VsRecorderWork *work);

// files 238-244 of the video search screens (not merged yet)
void ov40_022408AC(VsRecorderWork *work);
void ov40_02240910(VsRecorderWork *work);
void ov40_02241054(VsRecorderWork *work);
void ov40_02241114(VsRecorderWork *work);
void ov40_02241A34(VsRecorderWork *work);
void ov40_02241AB0(VsRecorderWork *work);
void ov40_02241D10(u32 index, u32 event, void *arg);
void ov40_02241E14(u32 index, u32 event, void *arg);
void ov40_02241E40(u32 index, u32 event, void *arg);
void ov40_02241ED4(u32 index, u32 event, void *arg);
void ov40_02241F3C(u32 index, u32 event, void *arg);
void ov40_022420B4(VsRecorderWork *work, int backOnly);
void ov40_02242110(VsRecorderWork *work);
void ov40_022421FC(VsRecorderWork *work);
BOOL ov40_0224222C(VsRecorderWork *work);
BOOL ov40_02242378(VsRecorderWork *work);
BOOL ov40_02242490(VsRecorderWork *work);
BOOL ov40_0224253C(VsRecorderWork *work);
BOOL ov40_022425E8(VsRecorderWork *work);
BOOL ov40_022428D4(VsRecorderWork *work);
BOOL ov40_02242AEC(VsRecorderWork *work);
BOOL ov40_02242CFC(VsRecorderWork *work);

// vs_recorder_battle_names.c
void VsRecorder_SanitizeBattleSetupNames(BattleRecord *record, BattleSetup *setup, enum HeapID heapID);

// vs_recorder_record.c
void VsRecorder_LoadPlaceholderTrainerName(String *str, enum HeapID heapID);
void VsRecorder_EncryptRecord(SaveData *saveData);
int VsRecorder_SaveRecordNumber(SaveData *saveData, u32 numberLow, u32 numberHigh, u16 *state, u16 *writeState);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
