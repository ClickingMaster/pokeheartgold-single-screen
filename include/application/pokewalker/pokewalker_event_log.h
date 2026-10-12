#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_EVENT_LOG_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_EVENT_LOG_H

#include "application/pokewalker/pokewalker_modes2.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "sprite_system.h"
#include "string_util.h"

// Event types of the Pokewalker event log (PokewalkerIrRecord eventType) as the event log app shows them.
// PokewalkerEventLog_BuildEntryList turns 12 into 11, 14 into 13, 28 into 12 and 29 into 14 first.
enum PokewalkerEventType {
    POKEWALKER_EVENT_NONE,
    POKEWALKER_EVENT_PEER_FIRST, // 1-10: met the Pokemon of another game's Pokewalker (5 kinds, odd/even outcome)
    POKEWALKER_EVENT_PEER_LAST = 10,
    POKEWALKER_EVENT_DOWSING_ITEM,   // found an item with the Dowsing Machine
    POKEWALKER_EVENT_GIFT_ITEM,      // an item was dropped here (28 as read from the Pokewalker)
    POKEWALKER_EVENT_RADAR_CAUGHT,   // caught a Pokemon found with the Poke Radar
    POKEWALKER_EVENT_GIFT_MON,       // met a Pokemon (29 as read from the Pokewalker)
    POKEWALKER_EVENT_RADAR_FLED,     // the Pokemon found with the Poke Radar ran away
    POKEWALKER_EVENT_RADAR_LOST,     // lost against the Pokemon found with the Poke Radar
    POKEWALKER_EVENT_MON_FOUND_ITEM, // the walking Pokemon found an item
    POKEWALKER_EVENT_HAPPY_FIRST,    // 18-22: the walking Pokemon is happy
    POKEWALKER_EVENT_HAPPY_LAST = 22,
    POKEWALKER_EVENT_MON_JOINED,    // the walking Pokemon joined the player
    POKEWALKER_EVENT_MON_WENT_HOME, // the walking Pokemon went home, too tired
    POKEWALKER_EVENT_STROLL_START,  // set out on a Stroll
    POKEWALKER_EVENT_STROLL_END,    // returned from the Stroll (added by PokewalkerEventLog_InitStrollEndRecord)
    POKEWALKER_EVENT_STEPS,         // steps walked in an hour, or the Pokemon took a nap
    POKEWALKER_EVENT_RAW_GIFT_ITEM,
    POKEWALKER_EVENT_RAW_GIFT_MON,
};

// Sprites of the event log app (PokewalkerEventLog sprites).
enum PokewalkerEventLogSprite {
    EVENT_LOG_SPRITE_PREV_PAGE,
    EVENT_LOG_SPRITE_NEXT_PAGE,
    EVENT_LOG_SPRITE_EXIT,
    EVENT_LOG_SPRITE_COURSE, // animation = course of the entry
    EVENT_LOG_SPRITE_LEFT_ICON,
    EVENT_LOG_SPRITE_RIGHT_ICON,
    EVENT_LOG_SPRITE_LEFT_EFFECT,
    EVENT_LOG_SPRITE_RIGHT_EFFECT,
    EVENT_LOG_SPRITE_CENTER_EFFECT,
    EVENT_LOG_SPRITE_MON_EFFECT,
    EVENT_LOG_SPRITE_MAX,
};

// A Pokemon walking on the event log's main screen: overworld picture loaded from the overworld models.
enum PokewalkerEventLogMonSpriteSize {
    MON_SPRITE_SMALL, // 32x32
    MON_SPRITE_LARGE, // 64x64
};

typedef struct PokewalkerEventLogMonSprite {
    BOOL isLarge;
    int species;
    ManagedSprite *sprites[2]; // small, large
    ManagedSprite *shadowSprite;
} PokewalkerEventLogMonSprite; // size=0x14

// Hop-and-run-away animation of the right-hand Pokemon (Poke Radar Pokemon that fled).
typedef struct PokewalkerEventLogFleeAnim {
    u32 active : 1;
    u32 phase : 8;
    u32 hops : 7;
    u32 frame : 16;
} PokewalkerEventLogFleeAnim;

// Poke Ball animation of a caught Poke Radar Pokemon.
typedef struct PokewalkerEventLogCatchAnim {
    u32 active : 1;
    u32 state : 7;
    u32 timer : 24;
} PokewalkerEventLogCatchAnim;

// Work of the Pokewalker event log app (heap HEAP_ID_155).
typedef struct PokewalkerEventLog {
    PokewalkerEventLogArgs *args;
    enum HeapID heapId;
    const u16 *playerName;
    u32 playerGender;
    NARC *narc; // 0x10
    BgConfig *bgConfig;
    Window windows[4];        // 0x18: date, time, message, quit button
    MessageFormat *msgFormat; // 0x58
    MsgData *msgData;
    MsgData *speciesNames;
    MsgData *itemNames;
    SpriteSystem *spriteSystem; // 0x68
    SpriteManager *spriteManager;
    ManagedSprite *sprites[EVENT_LOG_SPRITE_MAX]; // 0x70
    PokewalkerEventLogMonSprite monSprites[2];    // 0x98: left, right
    PokewalkerIrRecord *entries[24];              // 0xC0, sorted by time
    RTCDate date;                                 // 0x120, of the shown entry
    RTCTime time;                                 // 0x130
    u8 loadState;                                 // 0x13C
    u8 page;
    u8 count;
    u8 dowsingStreakPage; // page of the 4th Dowsing Machine item, 0 = none
    u8 radarStreakPage;   // page of the 4th Poke Radar catch, 0 = none
    u8 dowsingCount;
    u8 radarCount;
    u8 peerCount;
    u32 course : 8; // 0x144, course of the shown entry (0xFF = none yet)
    u32 hasGiftMon : 1;
    u32 hasGiftItem : 1;
    u32 pageTurnActive : 1;
    u32 pageTurnState : 4;
    u32 unused : 17;
    PokewalkerEventLogFleeAnim fleeAnim;   // 0x148
    PokewalkerEventLogCatchAnim catchAnim; // 0x14C
} PokewalkerEventLog;                      // size=0x150

typedef struct PokewalkerEventLogSlot {
    PokewalkerIrRecord *record;
    u32 index;
} PokewalkerEventLogSlot;

// Data sent to the Pokewalker: 16 blocks of the upload with offset, size and byte sum.
typedef struct PokewalkerUploadBlock {
    u16 offset;
    u8 size;
    u8 checksum;
} PokewalkerUploadBlock;

void PokewalkerApp_WriteUploadBlocks(PokewalkerUploadBlock *blocks);
void PokewalkerEventLog_ResetDisplay(void);
void PokewalkerEventLog_SetGxBanks(void);
int PokewalkerEventLog_GetMonModelFileId(int species, int form, int gender);
BOOL PokewalkerEventLog_CheckExit(PokewalkerEventLog *eventLog);
BOOL PokewalkerEventLog_CheckPrevPage(PokewalkerEventLog *eventLog);
BOOL PokewalkerEventLog_CheckNextPage(PokewalkerEventLog *eventLog);
u8 PokewalkerEventLog_IsMonShiny(PokewalkerIrRecord *record, BOOL isWalkingMon);
u8 PokewalkerEventLog_GetCourse(PokewalkerIrRecord *record);
int PokewalkerEventLog_CompareEntryTimes(PokewalkerEventLogSlot *a, PokewalkerEventLogSlot *b);
void PokewalkerEventLog_BuildEntryList(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_InitText(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_FreeText(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_PrintToWindowWithFont(PokewalkerEventLog *eventLog, u32 windowId, u32 msgId, u16 x, int fontId);
void PokewalkerEventLog_PrintToWindow(PokewalkerEventLog *eventLog, u32 windowId, u32 msgId, u16 x);
void PokewalkerEventLog_PrintMessage(PokewalkerEventLog *eventLog, int msgId, int msgId2);
void PokewalkerEventLog_BufferName(PokewalkerEventLog *eventLog, const u16 *name, u8 field, int length);
void PokewalkerEventLog_BufferItemName(PokewalkerEventLog *eventLog, u32 item);
void PokewalkerEventLog_BufferSpeciesName(PokewalkerEventLog *eventLog, u8 field, int species);
void PokewalkerEventLog_BufferNickname(PokewalkerEventLog *eventLog, const u16 *nickname, u8 field, int species);
void PokewalkerEventLog_BufferNumber(PokewalkerEventLog *eventLog, int field, int value, int digits, PrintingMode mode);
void PokewalkerEventLog_ConvertEntryTime(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_PrintEntry(PokewalkerEventLog *eventLog, int msgId, int msgId2);
void PokewalkerEventLog_PrintQuit(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_InitBgs(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_FreeBgs(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_LoadBgGraphics(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_LoadSky(PokewalkerEventLog *eventLog, u32 hour, int course);
void PokewalkerEventLog_PlayCourseBgm(PokewalkerEventLog *eventLog, u32 course);
void PokewalkerEventLog_InitSpriteSystem(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_FreeSprites(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_LoadSpriteResources(PokewalkerEventLog *eventLog);
ManagedSprite *PokewalkerEventLog_CreateSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 drawPriority);
ManagedSprite *PokewalkerEventLog_CreateEffectSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 drawPriority);
void PokewalkerEventLog_CreateSprites(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_SetSceneVisible(PokewalkerEventLog *eventLog, BOOL visible);
void PokewalkerEventLog_AdvanceSpriteAnim(PokewalkerEventLog *eventLog, int spriteId);
void PokewalkerEventLog_SetCourseSprite(PokewalkerEventLog *eventLog, int course);
void PokewalkerEventLog_LoadSpriteCharData(Sprite *sprite, const void *data, u32 size, u32 offset);
BOOL PokewalkerEventLog_LoadMonSpriteResources(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_SetMonSpriteVisible(PokewalkerEventLogMonSprite *monSprite, BOOL visible);
void PokewalkerEventLog_SetMonSpriteAnim(PokewalkerEventLogMonSprite *monSprite, int animation);
void PokewalkerEventLog_SetMonSpritePosition(PokewalkerEventLogMonSprite *monSprite, s16 x, s16 y, BOOL keepShadow);
void PokewalkerEventLog_CreateMonSprites(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_DeleteMonSprites(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_CreateMonSprite(PokewalkerEventLogMonSprite *monSprite, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, int side);
void PokewalkerEventLog_LoadMonSprite(PokewalkerEventLogMonSprite *monSprite, u16 species, u8 form, int gender, u8 isShiny, int heapId);
void PokewalkerEventLog_ShowMon(PokewalkerEventLog *eventLog, int side, BOOL isWalkingMon);
void PokewalkerEventLog_ShowTrainer(ManagedSprite *sprite, int gender, int pose);
void PokewalkerEventLog_ShowEffectAboveMon(PokewalkerEventLog *eventLog, int side, int spriteId, int animation);
void PokewalkerEventLog_ResetAnims(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_UpdateAnims(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_StartFleeAnim(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ResetFleeAnim(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_UpdateFleeAnim(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_StartCatchAnim(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ResetCatchAnim(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_UpdateCatchAnim(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowPeerEvent(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowDowsingItem(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowGiftItem(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowRadarCaught(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowGiftMon(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowRadarFled(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowRadarLost(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowMonFoundItem(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowHappy(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowMonJoined(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowMonWentHome(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowStrollStart(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowStrollEnd(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowSteps(PokewalkerEventLog *eventLog, u32 hour);
void PokewalkerEventLog_ShowDistantPeer(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_ShowEntry(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_StartPageTurn(PokewalkerEventLog *eventLog);
BOOL PokewalkerEventLog_UpdatePageTurn(PokewalkerEventLog *eventLog);
void PokewalkerEventLog_Setup(OverlayManager *manager);
void PokewalkerEventLog_Teardown(OverlayManager *manager);
void PokewalkerEventLog_VBlankCB(PokewalkerEventLog *eventLog);
BOOL PokewalkerEventLog_Init(OverlayManager *manager, int *state);
BOOL PokewalkerEventLog_Exit(OverlayManager *manager, int *state);
BOOL PokewalkerEventLog_Main(OverlayManager *manager, int *state);
void PokewalkerEventLog_InitStrollEndRecord(PokewalkerIrRecord *record, PokewalkerEventLogParams *params);
void PokewalkerEventLog_SetGiftMon(PokewalkerIrRecord *records, PokewalkerEventLogMon *mon);
BOOL PokewalkerEventLog_HasGiftMon(PokewalkerIrRecord *records);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_EVENT_LOG_H
