#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H

#include "application/vs_recorder/vs_recorder_internal.h"
#include "battle/battle_setup.h"

#include "battle_recording.h"

// VS Recorder: the Global Terminal screens (video lists, video search, video number keypad, delete)
// and the battle video helpers used outside the app.

BOOL GlobalTerminalNet_IsIdle(GlobalTerminalNet *net);

BOOL VsRecorderApp_HasVisitedBattleFrontier(void *work);

// VsRecorderWork.screenWork of the delete screen
typedef struct VsRecorderDeleteVideo {
    int eva; // top screen blend
    int evb;
    int fade;
    BOOL cancelled;
    Window yesNoWindows[2];
} VsRecorderDeleteVideo;

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
#define VS_RECORDER_VIDEO_LIST_COLOSSEUM       0 // selection 300
#define VS_RECORDER_VIDEO_LIST_BATTLE_FRONTIER 1 // selection 301
#define VS_RECORDER_VIDEO_LIST_NEWEST_30       2 // selection 200

// VsRecorderWork.screenWork of the video list screens: the list (with party icons), a video's details (video info /
// owner's profile, play, save) and saving it to one of the three slots
typedef struct VsRecorderVideoListScreen {
    int eva; // top screen blend
    int evb;
    int fade;
    int listType;                    // VS_RECORDER_VIDEO_LIST_*
    u32 showProfile;                 // details: 0 video info, 1 owner's profile
    Window windows[8];               // list title
    Window buttonWindows[8];         // exit / details toggle label; YES, NO
    VsRecorderTextSprite buttons[4]; // exit and back; slot menu: slots 1-3 and cancel
    u8 *downloadedVideo;
    TouchHitboxController *detailHitboxes;    // toggle, play, save, back
    TouchHitboxController *slotHitboxes;      // slots 1-3, cancel
    TouchHitboxController *overwriteHitboxes; // YES, NO
    VsRecorderProfile profile;
    VsRecorderVideoInfo info;
    u8 unk_2AC[0x2028 - 0x2AC];
    int slot; // 1-3
    int buttonWindowCount;
    int windowCount;
    int nextSubState; // after the video download / the list
    BOOL downloaded;
    u8 unk_203C[0x2054 - 0x203C];
    VsRecorderListConfig listConfig;
    NARC *monIconNarc;
    int iconScroll;                      // list scroll the icons were made for
    int iconResId;                       // next mon icon resource
    VsRecorderVideoListIcon icons[5][6]; // party icons of the visible rows
} VsRecorderVideoListScreen;

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
} VsRecorderNumberPadDigit;

// set up by the screen that opens the number pad
typedef struct VsRecorderNumberPadConfig {
    u32 length;          // digits
    int groupLengths[4]; // the last one repeats the third
    u32 unk_14;
    String *text; // the digits entered
    void *options;
} VsRecorderNumberPadConfig;

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
    VsRecorderNumberPadDigit digits[12];
    VsRecorderNumberPadDigit separators[2];
    VsRecorderNumberPadDigit cursors[3]; // digit cursor (value: selected digit), key cursor, key press
    s16 startX[4];                       // x of the first digit, by group
    u16 groupBounds[3][2];               // first and end digit of each group
    u8 unk_1F0[4];
    int state; // VS_RECORDER_NUMBER_PAD_*
    u32 unk_1F8;
    int step;
    int timer;
    int count; // digits
    int group; // current group (0: none)
    int prevGroup;
    int groupFirst;
    int groupEnd;
    int prevGroupFirst;
    int prevGroupEnd;
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *plttData;
    TouchHitboxController *hitboxController;
    TouchscreenHitbox hitboxes[24]; // digits, keys 0-9, back, OK
    BOOL touchInput;                // TRUE: hides the key cursor
    int action;
    int actionValue;
    int actionMode; // ACTION_GROUP: 0 first digit, 1 last digit of the group
    VsRecorderNumberPadConfig config;
    BOOL confirmed; // FALSE: cancelled with back on the first digit
    s64 number;
    VsRecorderWork *owner;
} VsRecorderNumberPad;

typedef BOOL (*VsRecorderNumberPadFunc)(VsRecorderNumberPad *pad);

// VsRecorderVideoSearch.condition: the search condition being edited
#define VS_RECORDER_SEARCH_FACILITY 0
#define VS_RECORDER_SEARCH_SPECIES  1
#define VS_RECORDER_SEARCH_LOCATION 2

// VsRecorderWork.screenWork of the video search screens: the search conditions (facility, species, location),
// the result list (with party icons like the video list), a found video's details and saving it; also used by the
// video number search (number pad) and by sending the own video
typedef struct VsRecorderVideoSearch {
    int eva; // top screen blend
    int evb;
    int fade;
    int nextSubState;                // after the result list / the video download
    VsRecorderTextSprite buttons[4]; // exit and back; slot menu: slots 1-3 and cancel
    VsRecorderProfile profile;
    VsRecorderVideoInfo info;
    u8 unk_198[0x1C8 - 0x198];
    int slot;                 // 1-3
    u32 showProfile;          // details: 0 video info, 1 owner's profile
    FrontierCard *uploadCard; // send video
    VsRecorderNumberPad numberPad;
    VsRecorderNumberPadConfig *numberPadConfig;
    u64 videoNumber; // number search / the sent video's number
    BOOL downloaded;
    u8 *foundVideo; // the downloaded video (profile, header, battle)
    u16 species;    // any
    u8 facility;    // any
    u8 country;     // any location
    u8 region;
    u8 condition; // VS_RECORDER_SEARCH_*
    u8 unk_4C6[2];
    int speciesCount; // seen species of the chosen group
    int listSize;     // whole species list
    u16 *speciesList; // seen species of the group first
    MsgData *speciesNames;
    BOOL categoryChosen;
    VsRecorderListEntry *entries;    // species list window
    VsRecorderListConfig listConfig; // species list / result list
    NARC *monIconNarc;
    int iconScroll;                           // list scroll the icons were made for
    int iconResId;                            // next mon icon resource
    VsRecorderVideoListIcon icons[5][6];      // party icons of the visible result rows
    TouchHitboxController *hitboxes;          // conditions / video details
    TouchHitboxController *slotHitboxes;      // species groups / save slots
    TouchHitboxController *overwriteHitboxes; // YES, NO
    Window windows[6];                        // conditions: labels and the chosen values
    u8 unk_674[0x6D4 - 0x674];
    Window buttonWindows[9]; // buttons: conditions, species groups, result list
    u8 unk_764[0x794 - 0x764];
    int windowCount;
    int buttonWindowCount;
    u8 videoFlag; // VsRecorderVideoList.flags of the found video
    u8 unk_79D[3];
} VsRecorderVideoSearch;

BOOL VsRecorder_IsNetIdle(VsRecorderWork *work);

void VsRecorder_PrintVideoNumber(VsRecorderVideoSearch *search, VsRecorderWork *work);
VsRecorderNumberPadConfig *VsRecorder_NewNumberPadConfig(enum HeapID heapID, u32 length, const int *groupLengths, void *options);
void VsRecorder_FreeNumberPadConfig(VsRecorderNumberPadConfig *config);
BOOL VsRecorder_NumberPadInit(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadFree(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadInput(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadSwitchGroup(VsRecorderNumberPad *pad);
BOOL VsRecorder_NumberPadMain(VsRecorderNumberPad *pad);

BOOL VsRecorder_DeleteInit(VsRecorderWork *work);
BOOL VsRecorder_DeleteEnter(VsRecorderWork *work);
BOOL VsRecorder_DeleteMain(VsRecorderWork *work);
BOOL VsRecorder_DeleteExit(VsRecorderWork *work);

void VsRecorder_OnRankingsHeaderReceived(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnRankingsReceived(void *arg, GlobalTerminalNetError *error);
BOOL VsRecorder_VideoListDownloadVideo(VsRecorderWork *work);
BOOL VsRecorder_VideoListOverwriteMain(VsRecorderWork *work);
BOOL VsRecorder_VideoListOverwriteNo(VsRecorderWork *work);
BOOL VsRecorder_VideoListSave(VsRecorderWork *work);
BOOL VsRecorder_VideoListConfirmOverwrite(VsRecorderWork *work);
BOOL VsRecorder_VideoListReturnToDetails(VsRecorderWork *work);
BOOL VsRecorder_VideoListSlotsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoListSlotsCancel(VsRecorderWork *work);
BOOL VsRecorder_VideoListBackToList(VsRecorderWork *work);
BOOL VsRecorder_VideoListInit(VsRecorderWork *work);
BOOL VsRecorder_VideoListEnter(VsRecorderWork *work);
BOOL VsRecorder_VideoListDownloadList(VsRecorderWork *work);
BOOL VsRecorder_VideoListShowList(VsRecorderWork *work);
BOOL VsRecorder_VideoListSelect(VsRecorderWork *work);
BOOL VsRecorder_VideoListExit(VsRecorderWork *work);
BOOL VsRecorder_VideoListShowDetails(VsRecorderWork *work);
BOOL VsRecorder_VideoListDetailsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoListToggleProfile(VsRecorderWork *work);
BOOL VsRecorder_VideoListPlay(VsRecorderWork *work);
BOOL VsRecorder_VideoListResume(VsRecorderWork *work);
BOOL VsRecorder_VideoListShowSlots(VsRecorderWork *work);

BOOL VsRecorder_HasSearchConditions(VsRecorderWork *work);
void VsRecorder_PrintSearchLocation(VsRecorderWork *work, u32 country, u32 region);
void VsRecorder_PrintLocationButtonLabel(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchShowConditions(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchConditionsMain(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchExit(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchOpenCondition(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSelectFacility(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchClearSpecies(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSelectCategory(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchSelectSpecies(VsRecorderWork *work);
BOOL VsRecorder_SearchCategoryHasSeen(VsRecorderWork *work, int category);
void VsRecorder_BuildSearchSpeciesList(VsRecorderWork *work, int category);
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

BOOL VsRecorder_VideoSearchDownloadVideo(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchResume(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchInit(VsRecorderWork *work);
BOOL VsRecorder_VideoSearchEnter(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchShowPad(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchPadMain(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchClosePad(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchExit(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchDownloadVideo(VsRecorderWork *work);
void VsRecorder_LoadSearchResultDetails(VsRecorderWork *work);
void VsRecorder_FreeFoundVideoDetails(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchShowDetails(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchDetailsMain(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchToggleProfile(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchPlay(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchExitDetails(VsRecorderWork *work);
void VsRecorder_CreateSearchSlotButtons(VsRecorderWork *work);
void VsRecorder_FreeSearchSlotButtons(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchShowSlots(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchSlotsMain(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchSlotsCancel(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchReturnToDetails(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchConfirmOverwrite(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchOverwriteMain(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchOverwriteNo(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchSave(VsRecorderWork *work);
BOOL VsRecorder_NumberSearchResume(VsRecorderWork *work);
BOOL VsRecorder_SendVideoShowCursor(VsRecorderWork *work);
BOOL VsRecorder_SendVideoUpload(VsRecorderWork *work);
BOOL VsRecorder_SendVideoExit(VsRecorderWork *work);
void VsRecorder_RemoveSearchDetailsButtonWindow(VsRecorderWork *work);
void VsRecorder_CreateSearchDetailsButtonWindow(VsRecorderWork *work);
void VsRecorder_OnSearchConditionTouch(u32 index, u32 event, void *arg);
void VsRecorder_OnSpeciesGroupTouch(u32 index, u32 event, void *arg);
void VsRecorder_OnSearchDetailsTouch(u32 index, u32 event, void *arg);
void VsRecorder_OnSearchSlotTouch(u32 index, u32 event, void *arg);
void VsRecorder_OnSearchOverwriteTouch(u32 index, u32 event, void *arg);
void VsRecorder_SetSearchButtonsMode(VsRecorderWork *work, int backOnly);
void VsRecorder_CreateSearchButtons(VsRecorderWork *work);
void VsRecorder_FreeSearchButtons(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoToggleProfile(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoShowSlots(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoConfirmOverwrite(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoOverwriteNo(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoSave(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoReturnToDetails(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoSlotsCancel(VsRecorderWork *work);
BOOL VsRecorder_FoundVideoPlay(VsRecorderWork *work);
void VsRecorder_OnVideoUploaded(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnVideoDownloaded(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnVideoSaveReported(void *arg, GlobalTerminalNetError *error);

void VsRecorder_SanitizeBattleSetupNames(BattleRecord *record, BattleSetup *setup, enum HeapID heapID);

void VsRecorder_LoadPlaceholderTrainerName(String *str, enum HeapID heapID);
void VsRecorder_EncryptRecord(SaveData *saveData);
int VsRecorder_SaveRecordNumber(SaveData *saveData, u32 numberLow, u32 numberHigh, u16 *state, u16 *writeState);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
