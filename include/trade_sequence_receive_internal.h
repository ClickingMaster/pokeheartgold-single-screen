#ifndef POKEHEARTGOLD_TRADE_SEQUENCE_RECEIVE_INTERNAL_H
#define POKEHEARTGOLD_TRADE_SEQUENCE_RECEIVE_INTERNAL_H

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"

// Scene 6 of the trade animation (trade_sequence_receive.c, trade_sequence_receive_bounce.c).

typedef struct TradeSequenceReceive {
    TradeSequence *ts;                // 0x00
    u32 subState;                     // 0x04
    int timer;                        // 0x08
    BOOL fanfarePlaying;              // 0x0C
    PokepicManager *pokepicManager;   // 0x10
    Pokepic *pokepic;                 // 0x14
    PokepicAnimScript animScript[10]; // 0x18
    Sprite *sprites[2];               // 0x40
    TradeSequenceCellAnim cellAnim;   // 0x48
    BgConfig *bgConfig;               // 0x58
    Window window;                    // 0x5C
    String *msgTemplate;              // 0x6C
    String *msgString;                // 0x70
    TradeSequenceModels *models;      // 0x74
    TradeSequenceModel *ball;         // 0x78
    SysTask *brightnessTask;          // 0x7C
    SysTask *bounceTask;              // 0x80
    NARC *pokeAnimNarc;               // 0x84
} TradeSequenceReceive;               // size: 0x88

// The ball fades in, spins faster, rises, falls and bounces once, then lands while the screen brightens.
typedef struct TradeSequenceReceiveBounce {
    SysTask **task;                 // 0x00
    int state;                      // 0x04
    TradeSequenceReceive *receive;  // 0x08
    int timer;                      // 0x0C
    TradeSequenceModels *models;    // 0x10
    TradeSequenceModel *ball;       // 0x14
    fx32 alpha;                     // 0x18
    fx32 alphaStep;                 // 0x1C
    int alphaFrames;                // 0x20
    VecFx32 position;               // 0x24
    fx32 speed;                     // 0x30
    TradeSequenceRotation rotation; // 0x34
    s16 spinSpeed;                  // 0x3A
    s16 spinStep;                   // 0x3C
    s16 spinTarget;                 // 0x3E, never set
    u32 spinFrames;                 // 0x40
} TradeSequenceReceiveBounce;       // size: 0x44

void TradeSequenceReceive_StartBrightness(TradeSequenceReceive *receive, int start, int target, int frames);
BOOL TradeSequenceReceive_IsBrightnessDone(TradeSequenceReceive *receive);
void TradeSequenceReceive_StopBounce(SysTask *task);
void TradeSequenceReceive_UpdateSpin(TradeSequenceReceiveBounce *bounce);
void TradeSequenceReceive_SetSpin(TradeSequenceReceiveBounce *bounce, int target, int frames);
void TradeSequenceReceive_BounceTask(SysTask *task, void *data);

#endif // POKEHEARTGOLD_TRADE_SEQUENCE_RECEIVE_INTERNAL_H
