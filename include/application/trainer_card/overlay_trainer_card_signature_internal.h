#ifndef POKEHEARTGOLD_APPLICATION_TRAINER_CARD_OVERLAY_TRAINER_CARD_SIGNATURE_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_TRAINER_CARD_OVERLAY_TRAINER_CARD_SIGNATURE_INTERNAL_H

#include "bg_window.h"
#include "game_stats.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_trainer_card_signature.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"
#include "yes_no_prompt.h"

#define TRAINER_CARD_SIGNATURE_PEN_COUNT 5

// Touch samples of one pen collected during one frame.
typedef struct TrainerCardSignatureStroke {
    u8 x[8];
    u8 y[8];
    u8 style : 4;
    u8 count : 4;
} TrainerCardSignatureStroke;

// Where a pen's stroke ended in the previous frame.
typedef struct TrainerCardSignaturePenPos {
    u8 x;
    u8 y;
    u16 count;
} TrainerCardSignaturePenPos;

typedef struct TrainerCardSignature {
    BgConfig *bgConfig;
    u32 unk_0004;
    GameStats *gameStats;
    Options *options;
    MessageFormat *msgFormat;
    MsgData *msgData;
    String *penNames[TRAINER_CARD_SIGNATURE_PEN_COUNT];
    String *doneString;
    String *titleString;
    String *messageString;
    int textPrinterId;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resourceManagers[4];
    SpriteResource *resourceObjects[2][4]; // [screen][char/pltt/cell/anim]
    SpriteResourcesHeader resourceHeaders[2];
    u8 unk_01E0[0x70];
    Sprite *doneButtonSprite;
    u8 unk_0254[0x20];
    Window penNameWindows[TRAINER_CARD_SIGNATURE_PEN_COUNT];
    Window canvasWindow;
    Window messageWindow;
    Window doneButtonWindow;
    Window titleWindow;
    u8 unk_0304[8];
    int state;
    u8 unk_0310[0x4008];
    u16 paletteAngle;
    u8 penStyle;
    u8 unk_431B;
    TrainerCardSignatureStroke newStroke;
    TrainerCardSignatureStroke strokes[TRAINER_CARD_SIGNATURE_PEN_COUNT];
    TrainerCardSignaturePenPos penPos[TRAINER_CARD_SIGNATURE_PEN_COUNT];
    u8 unk_4396[0x1802];
    u8 *signature;
    u8 objTextBuffer[0x100];
    YesNoPrompt *yesNoPrompt;
    s8 prevMoveX;
    s8 prevMoveY;
    s8 moveX;
    s8 moveY;
    u32 unk_5CA4;
    int lastTouchX;
    int lastTouchY;
} TrainerCardSignature;

#endif // POKEHEARTGOLD_APPLICATION_TRAINER_CARD_OVERLAY_TRAINER_CARD_SIGNATURE_INTERNAL_H
