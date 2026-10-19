#ifndef POKEHEARTGOLD_UNION_CHAT_LOG_H
#define POKEHEARTGOLD_UNION_CHAT_LOG_H

#include "bg_window.h"
#include "field_system.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "sprite.h"
#include "sys_task.h"
#include "union_room_objects.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// Field bottom-screen app 8 in the Union Room: a scrolling log of the chat messages of the players
// in the room. Shown instead of the touch start menu; the "Menu" bar returns to it.

#define UNION_CHAT_LOG_NUM_ROWS         3
#define UNION_CHAT_LOG_NUM_ARROWS       2
#define UNION_CHAT_LOG_SPRITE_UP        0
#define UNION_CHAT_LOG_SPRITE_DOWN      1
#define UNION_CHAT_LOG_SPRITE_SCROLLBAR 2

enum UnionChatLogState {
    UNION_CHAT_LOG_STATE_RUNNING = 2,
    UNION_CHAT_LOG_STATE_CLOSE,
    UNION_CHAT_LOG_STATE_CLOSING,
    UNION_CHAT_LOG_STATE_FREED,
};

typedef struct UnionChatLogRow {
    Window name;
    Window message;
    Window friendOf;
    int gender;     // selects the row background
    int pressTimer; // grows while the row is held, shrinks after release
} UnionChatLogRow;

typedef struct UnionChatLogData {
    u32 state;
    UnionRoomObjects *unionRoom;
    struct UnionRoom *unionRoomData;
    FieldSystem *fieldSystem;
    PlayerProfile *profile;
    BgConfig *bgConfig;
    MessageFormat *msgFormat;
    MsgData *msgData;
    void *screenFile;
    NNSG2dScreenData *screenData;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *resObjs[4];
    SpriteResourcesHeader header;
    Sprite *sprites[3];
    u8 unk_1A4[0x20];
    int touchedButton; // button hitbox - 2, so the rows are 0-2
    UnionChatLogRow rows[UNION_CHAT_LOG_NUM_ROWS];
    UnionChatLogMessages *log;
    Window menuWindow;
    u16 count;
    u16 prevCount;
    u16 scroll;
    u16 scrollbarVisible;
    int drawnScroll;
    int pressSum;
    BOOL redraw;
    u8 touchTick;
    s8 touchTimer;
    u8 touchFirstDelay;
    u8 touchInterval;
    BOOL thumbFollowsScroll; // FALSE while the scrollbar is dragged
    u16 arrowPressed[UNION_CHAT_LOG_NUM_ARROWS];
    SysTask *task;
} UnionChatLogData;

UnionChatLogData *UnionChatLog_Init(FieldSystem *fieldSystem);
void UnionChatLog_Exit(UnionChatLogData *data);
void UnionChatLog_UpdateArrows(UnionChatLogData *data);

#endif // POKEHEARTGOLD_UNION_CHAT_LOG_H
