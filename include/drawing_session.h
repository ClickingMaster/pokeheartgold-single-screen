#ifndef POKEHEARTGOLD_DRAWING_SESSION_H
#define POKEHEARTGOLD_DRAWING_SESSION_H

#include "bg_window.h"
#include "launch_application_internal.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "touchpad_internal.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"
#include "yes_no_prompt.h"

// Union Room group drawing: up to 5 members draw on a shared canvas with 8 pen colours
// and 3 pen sizes. The leader (link id 0) collects the members' strokes and sends them to everyone;
// the link command handlers write into this work and report events through
// DrawingSession_HandleNetEvent.

#define DRAWING_SESSION_MAX_MEMBERS 5

// One frame of a member's pen: up to 2 points (first and last touch sample), sent as is.
typedef struct DrawingSessionStroke {
    u8 xs[4];
    u8 ys[4];
    u8 color : 3;
    u8 count : 3;
    u8 size : 2;
    u8 joinState; // the leader's copy carries DrawingSessionWork.joinState
} DrawingSessionStroke;

// Last point a member's stroke ended at, so the next stroke continues the line.
typedef struct DrawingSessionPoint {
    u8 x;
    u8 y;
    u16 count;
} DrawingSessionPoint;

// Profile of the member in a link slot, this frame and the previous one.
typedef struct DrawingSessionMember {
    PlayerProfile *cur;
    PlayerProfile *prev;
} DrawingSessionMember;

// One piece of the canvas picture as sent by the leader.
typedef struct DrawingSessionPictureChunk {
    u8 data[1000];
    u32 checksum; // XOR of the data words
    u8 index;
} DrawingSessionPictureChunk;

typedef struct DrawingSessionWork DrawingSessionWork;

typedef struct DrawingSessionState {
    int (*func)(DrawingSessionWork *work, int state);
    u32 runsWhileClosing; // the state also runs while the main state is 2
} DrawingSessionState;

struct DrawingSessionWork {
    BgConfig *bgConfig;
    u32 unk_004;
    DrawingSessionArgs *args;
    MessageFormat *msgFormat;
    MsgData *msgData;
    String *memberNames[DRAWING_SESSION_MAX_MEMBERS];
    String *quitText;
    String *messageText;
    u32 printerId; // when the message was printed at once
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *resources[8];
    SpriteResourcesHeader headers[2];
    Sprite *penCursors[DRAWING_SESSION_MAX_MEMBERS];
    u8 unk_1EC[0x24];
    Sprite *memberIcons[DRAWING_SESSION_MAX_MEMBERS];
    u8 unk_224[0x24];
    Sprite *buttons[12]; // 8 colours, QUIT, 3 pen sizes
    Window nameWindows[DRAWING_SESSION_MAX_MEMBERS];
    Window canvasWindow;
    Window messageWindow;
    Window quitWindow;
    u8 unk_2F8[8];
    u32 mainState; // copy of the OverlayManager state
    int subState;  // index into sDrawingSessionSubStates
    int nextSubState;
    u32 unk_30C;
    int counter;
    int memberCount;
    int connectedCount;
    u32 connectedBitmap;
    u32 joinedMember;
    u8 unk_324[0x10];
    DrawingSessionMember members[DRAWING_SESSION_MAX_MEMBERS];
    u8 unk_35C[0x4374 - 0x35C];
    u16 cursorPaletteAngle;
    u8 penColor;
    u8 penSize;
    DrawingSessionStroke ownStroke;
    DrawingSessionStroke strokes[DRAWING_SESSION_MAX_MEMBERS];
    DrawingSessionPoint lastPoints[DRAWING_SESSION_MAX_MEMBERS];
    void *unk_43C8;
    int pictureChunkIndex;
    u8 picture[0x3840]; // the canvas pixels as received from the leader
    DrawingSessionPictureChunk sendChunk;
    DrawingSessionPictureChunk recvChunks[DRAWING_SESSION_MAX_MEMBERS];
    u32 leaveConnectedCount;
    u32 leavingMask;
    u16 assignedCount;
    s16 networkCounter;
    u8 leaveRequested;
    u8 strokeSendBuffer[DRAWING_SESSION_MAX_MEMBERS * sizeof(DrawingSessionStroke)];
    YesNoPrompt *yesNoPrompt;
    u32 joinState;
    BOOL yesNoActive;
    u32 firstJoinSeen;
    u32 waitDisconnect;
    u32 newConnection;
    u32 newConnectionBit;
};

void DrawingSession_HandleNetEvent(DrawingSessionWork *work, int event, u32 member);
u32 DrawingSession_GetMemberCount(void);

#endif // POKEHEARTGOLD_DRAWING_SESSION_H
