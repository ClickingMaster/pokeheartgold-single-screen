#ifndef POKEHEARTGOLD_OVERLAY_TRAINER_CARD_SIGNATURE_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY_TRAINER_CARD_SIGNATURE_INTERNAL_H

#include "bg_window.h"
#include "game_stats.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_trainer_card_signature.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02009D48.h"
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
    BgConfig *bgConfig; // 0x0000
    u32 unk_0004;
    GameStats *gameStats;                               // 0x0008
    Options *options;                                   // 0x000C
    MessageFormat *msgFormat;                           // 0x0010
    MsgData *msgData;                                   // 0x0014
    String *penNames[TRAINER_CARD_SIGNATURE_PEN_COUNT]; // 0x0018
    String *doneString;                                 // 0x002C
    String *titleString;                                // 0x0030
    String *messageString;                              // 0x0034
    int textPrinterId;                                  // 0x0038
    SpriteList *spriteList;                             // 0x003C
    G2dRenderer renderer;                               // 0x0040
    GF_2DGfxResMan *resourceManagers[4];                // 0x0168
    SpriteResource *resourceObjects[2][4];              // 0x0178, [screen][char/pltt/cell/anim]
    SpriteResourcesHeader resourceHeaders[2];           // 0x0198
    u8 unk_01E0[0x70];
    Sprite *doneButtonSprite; // 0x0250
    u8 unk_0254[0x20];
    Window penNameWindows[TRAINER_CARD_SIGNATURE_PEN_COUNT]; // 0x0274
    Window canvasWindow;                                     // 0x02C4
    Window messageWindow;                                    // 0x02D4
    Window doneButtonWindow;                                 // 0x02E4
    Window titleWindow;                                      // 0x02F4
    u8 unk_0304[8];
    int state; // 0x030C
    u8 unk_0310[0x4008];
    u16 paletteAngle; // 0x4318
    u8 penStyle;      // 0x431A
    u8 unk_431B;
    TrainerCardSignatureStroke newStroke;                                 // 0x431C
    TrainerCardSignatureStroke strokes[TRAINER_CARD_SIGNATURE_PEN_COUNT]; // 0x432D
    TrainerCardSignaturePenPos penPos[TRAINER_CARD_SIGNATURE_PEN_COUNT];  // 0x4382
    u8 unk_4396[0x1802];
    u8 *signature;            // 0x5B98
    u8 objTextBuffer[0x100];  // 0x5B9C
    YesNoPrompt *yesNoPrompt; // 0x5C9C
    s8 prevMoveX;             // 0x5CA0
    s8 prevMoveY;
    s8 moveX;
    s8 moveY;
    u32 unk_5CA4;
    int lastTouchX;     // 0x5CA8
    int lastTouchY;     // 0x5CAC
} TrainerCardSignature; // size: 0x5CB0

#endif // POKEHEARTGOLD_OVERLAY_TRAINER_CARD_SIGNATURE_INTERNAL_H
