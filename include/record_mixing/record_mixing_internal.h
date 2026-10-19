#ifndef POKEHEARTGOLD_RECORD_MIXING_INTERNAL_H
#define POKEHEARTGOLD_RECORD_MIXING_INTERNAL_H

#include "global.h"

#include "record_mixing/record_mixing.h"
#include "record_mixing/record_mixing_exchange.h"

#include "bg_window.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sys_task_api.h"
#include "yes_no_prompt.h"

// Work of the record mixing screen, shared with its link command handlers (record_mixing_link.c).

#define RECORD_MIXING_MAX_MEMBERS 5

typedef struct RecordMixingMember {
    PlayerProfile *cur;
    PlayerProfile *prev;
} RecordMixingMember;

// Trainer id of the member in a slot, compared as one 64-bit value.
typedef union RecordMixingMemberKey {
    struct {
        u32 id;
        u32 valid;
    } s;

    u64 raw;
} RecordMixingMemberKey;

typedef struct RecordMixingMemberKeys {
    RecordMixingMemberKey cur;
    RecordMixingMemberKey prev;
} RecordMixingMemberKeys;

// Palette flash of the player sprite while records are mixed.
typedef struct RecordMixingFlash {
    u32 active;
    u32 frame;
    s32 timer;
    void *plttRaw;
    NNSG2dPaletteData *pltt;
} RecordMixingFlash;

// The own records as sent to the other members.
typedef struct RecordMixingSendBuffer {
    u32 records[RECORD_MIXING_SEND_SIZE / 4];
    u32 checksum; // of the records
    u32 random;
} RecordMixingSendBuffer;

typedef struct RecordMixingApp {
    BgConfig *bgConfig;
    u32 unk_0004;
    RecordMixingArgs *args;
    RecordMixingFlash flash;
    SysTask *task;
    MessageFormat *msgFmt;
    MsgData *msgData;
    String *memberNames[RECORD_MIXING_MAX_MEMBERS];
    u32 unk_0040;
    String *messageText;
    String *titleText;
    u32 printerId;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    u8 unk_018C[0x20];
    SpriteResource *charRes;
    SpriteResource *plttRes;
    SpriteResource *cellRes;
    SpriteResource *animRes;
    u8 unk_01BC[0x48];
    SpriteResourcesHeader spriteHeader;
    Sprite *playerSprite;
    Sprite *memberSprites[RECORD_MIXING_MAX_MEMBERS];
    u8 unk_0240[0x58];
    Window memberListWindow;
    Window messageWindow;
    Window buttonWindows[2];
    Window titleWindow;
    YesNoPrompt *yesNoPrompt;
    void *avatarCharRaw[2];
    NNSG2dCharacterData *avatarChar[2];
    void *avatarPlttRaw[2];
    NNSG2dPaletteData *avatarPltt[2];
    NNSG2dScreenData *buttonScreen;
    void *buttonScreenRaw;
    MenuInputState menuInputState;
    int state;
    int nextState;
    u32 unk_0320;
    s32 counter;
    u8 unk_0328[0x10];
    RecordMixingMember members[RECORD_MIXING_MAX_MEMBERS];
    int memberStates[RECORD_MIXING_MAX_MEMBERS];
    s32 saveState;
    u16 pulseAngle;
    u8 unk_037A[2];
    u16 *avatarPalette;
    u8 unk_0380[4];
    u32 unk_0384; // cleared by the leader when a member joins
    RecordMixingSendBuffer sendBuffer;
    u8 receiveBuffers[RECORD_MIXING_MAX_MEMBERS][RECORD_MIXING_BUFFER_SIZE];
    u32 recordsReceived;
    SaveData *saveData;
    u32 leaderJoinPending;
    u8 isMixing;
    u8 yesNoActive;
    u16 unk_4A16;
    int lastMemberCount;
    int participantCount;
    int joinPending;
    u32 leavingMask;
    u8 leaveRequested;
    s8 maxEntriesMode;
    u8 startMemberCount;
    u8 startMixingSent;
    int leaveConnectedCount;
    u16 assignedCount;
    s16 leaveWaitFrames;
    s32 leaveTimeout;
    s32 buttonHighlightTimer;
    RecordMixingMemberKeys memberKeys[RECORD_MIXING_MAX_MEMBERS];
} RecordMixingApp;

void RecordMixing_StartFlash(RecordMixingApp *);
void RecordMixing_SetStateFromLink(RecordMixingApp *, int, int);
void RecordMixing_SetStateFromLinkInMenu(RecordMixingApp *, int, int);
void RecordMixingLink_RegisterCommands(RecordMixingApp *);

#endif // POKEHEARTGOLD_RECORD_MIXING_INTERNAL_H
